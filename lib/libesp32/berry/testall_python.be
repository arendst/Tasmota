#! python3 -m berry_port
# Test runner for the Python port of Berry (berry_port).
# Iterates over tests/*.be and runs each with `python3 -m berry_port -s -g`.
# No lcov/gcov coverage — that only makes sense for the C build.

import os

var exec = 'python3 -m berry_port '
var path = 'tests'
var testcases = os.listdir(path)
var total = 0, failed = 0

for i : testcases
    if os.path.splitext(i)[1] == '.be'
        print('\033[0;36mrun testcase: ' + i + '\033[0m')
        var ret = os.system(exec, os.path.join(path, i))
        if ret != 0
            print('\033[0;31mreturn code:', ret, '\033[0m')
            failed += 1
        end
        total += 1
    end
end

print('\033[0;32mtest results: ' +
    str(total) + ' total, ' + str(failed) + ' failed' +
    (failed ? '' : ' (all tests passed)') +
    '.\033[0m')

if failed != 0
    os.exit(-1)
end
