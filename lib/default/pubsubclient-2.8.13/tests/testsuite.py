#!/usr/bin/env python
import os
import os.path
import sys
import shutil
from subprocess import call
import importlib
import unittest
import re

from testcases import settings


class Workspace(object):

    def __init__(self):
        self.root_dir = os.getcwd()
        self.build_dir = os.path.join(self.root_dir, "tmpbin")
        self.log_dir = os.path.join(self.root_dir, "logs")
        self.tests_dir = os.path.join(self.root_dir, "testcases")
        self.examples_dir = os.path.join(self.root_dir, "../PubSubClient/examples")
        self.examples = []
        self.tests = []
        if not os.path.isdir("../PubSubClient"):
            raise Exception("Cannot find PubSubClient library")
        try:
            return __import__('ino')
        except ImportError:
            raise Exception("ino tool not installed")

    def init(self):
        if os.path.isdir(self.build_dir):
            shutil.rmtree(self.build_dir)
        os.mkdir(self.build_dir)
        if os.path.isdir(self.log_dir):
            shutil.rmtree(self.log_dir)
        os.mkdir(self.log_dir)

        os.chdir(self.build_dir)
        call(["ino", "init"])

        shutil.copytree("../../PubSubClient", "lib/PubSubClient")

        filenames = []
        for root, dirs, files in os.walk(self.examples_dir):
            filenames += [os.path.join(root, f) for f in files if f.endswith(".ino")]
        filenames.sort()
        for e in filenames:
            self.examples.append(Sketch(self, e))

        filenames = []
        for root, dirs, files in os.walk(self.tests_dir):
            filenames += [os.path.join(root, f) for f in files if f.endswith(".ino")]
        filenames.sort()
        for e in filenames:
            self.tests.append(Sketch(self, e))

    def clean(self):
        shutil.rmtree(self.build_dir)


class Sketch(object):
    def __init__(self, wksp, fn):
        self.w = wksp
        self.filename = fn
        self.basename = os.path.basename(self.filename)
        self.build_log = os.path.join(self.w.log_dir, "%s.log" % (os.path.basename(self.filename),))
        self.build_err_log = os.path.join(self.w.log_dir, "%s.err.log" % (os.path.basename(self.filename),))
        self.build_upload_log = os.path.join(self.w.log_dir, "%s.upload.log" % (os.path.basename(self.filename),))

    def build(self):
        sys.stdout.write(" Build:   ")
        sys.stdout.flush()

        # Copy sketch over, replacing IP addresses as necessary
        fin = open(self.filename, "r")
        lines = fin.readlines()
        fin.close()
        fout = open(os.path.join(self.w.build_dir, "src", "sketch.ino"), "w")
        for l in lines:
            if re.match(r"^byte server\[\] = {", l):
                fout.write("byte server[] = { %s };\n" % (settings.server_ip.replace(".", ", "),))
            elif re.match(r"^byte ip\[\] = {", l):
                fout.write("byte ip[] = { %s };\n" % (settings.arduino_ip.replace(".", ", "),))
            else:
                fout.write(l)
        fout.flush()
        fout.close()

        # Run build
        fout = open(self.build_log, "w")
        ferr = open(self.build_err_log, "w")
        rc = call(["ino", "build"], stdout=fout, stderr=ferr)
        fout.close()
        ferr.close()
        if rc == 0:
            sys.stdout.write("pass")
            sys.stdout.write("\n")
            return True
        else:
            sys.stdout.write("fail")
            sys.stdout.write("\n")
            with open(self.build_err_log) as f:
                for line in f:
                    print(" " + line)
            return False

    def upload(self):
        sys.stdout.write(" Upload:  ")
        sys.stdout.flush()
        fout = open(self.build_upload_log, "w")
        rc = call(["ino", "upload"], stdout=fout, stderr=fout)
        fout.close()
        if rc == 0:
            sys.stdout.write("pass")
            sys.stdout.write("\n")
            return True
        else:
            sys.stdout.write("fail")
            sys.stdout.write("\n")
            with open(self.build_upload_log) as f:
                for line in f:
                    print(" " + line)
            return False

    def test(self):
        # import the matching test case, if it exists
        try:
            basename = os.path.basename(self.filename)[:-4]
            i = importlib.import_module("testcases." + basename)
        except:
            sys.stdout.write(" Test:    no tests found")
            sys.stdout.write("\n")
            return
        c = getattr(i, basename)

        testmethods = [m for m in dir(c) if m.startswith("test_")]
        testmethods.sort()
        tests = []
        for m in testmethods:
            tests.append(c(m))

        result = unittest.TestResult()
        c.setUpClass()
        if self.upload():
            sys.stdout.write(" Test:    ")
            sys.stdout.flush()
            for t in tests:
                t.run(result)
                print(str(result.testsRun - len(result.failures) - len(result.errors)) + "/" + str(result.testsRun))
                if not result.wasSuccessful():
                    if len(result.failures) > 0:
                        for f in result.failures:
                            print("-- " + str(f[0]))
                            print(f[1])
                    if len(result.errors) > 0:
                        print(" Errors:")
                        for f in result.errors:
                            print("-- " + str(f[0]))
                            print(f[1])
        c.tearDownClass()


if __name__ == '__main__':
    run_tests = True

    w = Workspace()
    w.init()

    for e in w.examples:
        print("--------------------------------------")
        print("[" + e.basename + "]")
        if e.build() and run_tests:
            e.test()
    for e in w.tests:
        print("--------------------------------------")
        print("[" + e.basename + "]")
        if e.build() and run_tests:
            e.test()

    w.clean()
