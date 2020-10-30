import sys
from distutils import unixccompiler
from distutils import ccompiler


def register():
    sys.modules['distutils.crossunixccompiler'] = sys.modules[__name__]
    ccompiler.compiler_class['crossunix'] = (__name__,
                                             'CrossUnixCCompiler',
                                             'UNIX-style compiler for cross compilation')


def try_remove_all(lst, starts):
    lst[:] = [x for x in lst if not x.startswith(starts)]


class CrossUnixCCompiler(unixccompiler.UnixCCompiler):
    def _compile(self, obj, src, ext, cc_args, extra_postargs, pp_opts):
        try_remove_all(self.compiler_so, ('-m64', '-fstack-protector-strong', '-mtune=generic'))
        try_remove_all(cc_args, '-I/usr')
        try_remove_all(pp_opts, '-I/usr')
        return unixccompiler.UnixCCompiler._compile(self, obj, src, ext, cc_args, extra_postargs, pp_opts)

    def link(self, target_desc, objects,
             output_filename, output_dir=None, libraries=None,
             library_dirs=None, runtime_library_dirs=None,
             export_symbols=None, debug=0, extra_preargs=None,
             extra_postargs=None, build_temp=None, target_lang=None):
        try_remove_all(self.library_dirs, ('/usr'))
        return unixccompiler.UnixCCompiler.link(self, target_desc, objects, output_filename, output_dir, libraries,
                                                library_dirs, runtime_library_dirs, export_symbols, debug,
                                                extra_preargs, extra_postargs, build_temp, target_lang)

    def _fix_lib_args(self, libraries, library_dirs, runtime_library_dirs):
        self.__class__ = unixccompiler.UnixCCompiler
        ret = unixccompiler.UnixCCompiler._fix_lib_args(self, libraries, library_dirs, runtime_library_dirs)
        self.__class__ = CrossUnixCCompiler
        return ret
