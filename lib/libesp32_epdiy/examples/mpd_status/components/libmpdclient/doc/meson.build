dconf = configuration_data()
dconf.set('VERSION', meson.project_version())
dconf.set('abs_top_srcdir', meson.source_root())

if find_program('dot', required: false).found()
  dconf.set('HAVE_DOT', 'YES')
else
  dconf.set('HAVE_DOT', 'NO')
endif

doxyfile = configure_file(input: 'doxygen.conf.in',
                          output: 'doxygen.conf',
                          configuration: dconf)

datadir = join_paths(get_option('datadir'), 'doc', 'spede')

html_target = custom_target('apidocs',
                            input: doxyfile,
                            output: 'html',
                            command: [doxygen, doxyfile],
                            install: true,
                            install_dir: docdir)
