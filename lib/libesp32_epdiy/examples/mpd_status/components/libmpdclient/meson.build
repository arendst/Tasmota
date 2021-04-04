project('libmpdclient', 'c', version: '2.18',
  default_options: [
    'c_std=c99',
  ],
  license: 'BSD',
)

cc = meson.get_compiler('c')

conf = configuration_data()
conf.set_quoted('PACKAGE', meson.project_name())
conf.set_quoted('VERSION', meson.project_version())

if host_machine.system() != 'windows'
  conf.set_quoted('DEFAULT_SOCKET', get_option('default_socket'))
endif

conf.set_quoted('DEFAULT_HOST', get_option('default_host'))
conf.set('DEFAULT_PORT', get_option('default_port'))

conf.set('HAVE_STRNDUP', cc.has_function('strndup', prefix: '#define _GNU_SOURCE\n#include <string.h>'))

platform_deps = []
if host_machine.system() == 'haiku'
  platform_deps = [cc.find_library('network')]
endif

if get_option('tcp')
  conf.set('ENABLE_TCP', true)
  conf.set('HAVE_GETADDRINFO', cc.has_function('getaddrinfo', dependencies: platform_deps))
endif

configure_file(output: 'config.h', configuration: conf)

version_conf = configuration_data()
splitted_version = meson.project_version().split('.')
version_conf.set('MAJOR_VERSION', splitted_version[0])
version_conf.set('MINOR_VERSION', splitted_version[1])
if splitted_version.length() >= 3
  version_conf.set('PATCH_VERSION', splitted_version[2])
else
  version_conf.set('PATCH_VERSION', '0')
endif
configure_file(input: 'include/mpd/version.h.in', output: 'version.h', configuration: version_conf)

common_cflags = [
  # for strdup() with glibc
  '-D_GNU_SOURCE',
]

test_cflags = [
  '-Wall',
  '-Wextra',
  '-Wno-deprecated-declarations',
  '-Wmissing-prototypes',
  '-Wshadow',
  '-Wpointer-arith',
  '-Wstrict-prototypes',
  '-Wcast-qual',
  '-Wwrite-strings',
]

foreach f: test_cflags
  if cc.has_argument(f)
    common_cflags += [ f ]
  endif
endforeach

add_global_arguments(common_cflags, language: 'c')

common_ldflags = []

test_ldflags = [
]

if host_machine.system() == 'linux'
  test_ldflags += [ '-Wl,--version-script=' + join_paths(meson.source_root(), 'libmpdclient.ld') ]
endif

if meson.version().version_compare('>=0.46.0')
  # compiler.has_link_argument() was added in Meson 0.46.0
  foreach f: test_ldflags
    if cc.has_link_argument(f)
      common_ldflags += [ f ]
    endif
  endforeach
else
  # Meson too old: assume the linker supports all flags
  common_ldflags += test_ldflags
endif

if host_machine.system() == 'windows'
  platform_deps = [cc.find_library('ws2_32')]
endif

inc = include_directories(
  'src',
  'include',

  # for the generated config.h
  '.',
)

libmpdclient = library('mpdclient',
  'src/async.c',
  'src/audio_format.c',
  'src/ierror.c',
  'src/resolver.c',
  'src/capabilities.c',
  'src/connection.c',
  'src/database.c',
  'src/directory.c',
  'src/rdirectory.c',
  'src/error.c',
  'src/fd_util.c',
  'src/fingerprint.c',
  'src/output.c',
  'src/coutput.c',
  'src/entity.c',
  'src/idle.c',
  'src/iso8601.c',
  'src/kvlist.c',
  'src/list.c',
  'src/mixer.c',
  'src/mount.c', 'src/cmount.c',
  'src/neighbor.c',
  'src/cneighbor.c',
  'src/parser.c',
  'src/password.c',
  'src/player.c',
  'src/playlist.c',
  'src/player.c',
  'src/rplaylist.c',
  'src/cplaylist.c',
  'src/queue.c',
  'src/quote.c',
  'src/recv.c',
  'src/response.c',
  'src/run.c',
  'src/search.c',
  'src/send.c',
  'src/socket.c',
  'src/song.c',
  'src/status.c',
  'src/cstatus.c',
  'src/stats.c',
  'src/cstats.c',
  'src/sync.c',
  'src/tag.c',
  'src/sticker.c',
  'src/settings.c',
  'src/message.c',
  'src/cmessage.c',
  'src/partition.c',
  'src/cpartition.c',
  link_depends: [
    'libmpdclient.ld'
  ],
  include_directories: inc,
  dependencies: [
    platform_deps,
  ],
  link_args: common_ldflags,
  version: meson.project_version(),
  soversion: splitted_version[0],
  install: true
)
libmpdclient_dep = declare_dependency(link_with: libmpdclient)

executable('example',
  'src/example.c',
  include_directories: inc,
  dependencies: [
    libmpdclient_dep,
  ])

install_headers(
  'include/mpd/async.h',
  'include/mpd/audio_format.h',
  'include/mpd/client.h',
  'include/mpd/capabilities.h',
  'include/mpd/compiler.h',
  'include/mpd/connection.h',
  'include/mpd/database.h',
  'include/mpd/directory.h',
  'include/mpd/entity.h',
  'include/mpd/error.h',
  'include/mpd/fingerprint.h',
  'include/mpd/idle.h',
  'include/mpd/list.h',
  'include/mpd/mixer.h',
  'include/mpd/mount.h',
  'include/mpd/neighbor.h',
  'include/mpd/parser.h',
  'include/mpd/partition.h',
  'include/mpd/password.h',
  'include/mpd/player.h',
  'include/mpd/playlist.h',
  'include/mpd/protocol.h',
  'include/mpd/queue.h',
  'include/mpd/recv.h',
  'include/mpd/response.h',
  'include/mpd/send.h',
  'include/mpd/status.h',
  'include/mpd/stats.h',
  'include/mpd/tag.h',
  'include/mpd/output.h',
  'include/mpd/pair.h',
  'include/mpd/search.h',
  'include/mpd/socket.h',
  'include/mpd/song.h',
  'include/mpd/sticker.h',
  'include/mpd/settings.h',
  'include/mpd/message.h',
  join_paths(meson.build_root(), 'version.h'),
  subdir: 'mpd')

docdir = join_paths(get_option('datadir'), 'doc', meson.project_name())
install_data('AUTHORS', 'COPYING', 'NEWS', 'README.rst',
  install_dir: docdir)

install_data('vapi/libmpdclient.vapi',
  install_dir : join_paths(get_option('datadir'), 'vala', 'vapi'))

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  libraries: libmpdclient,
  version: meson.project_version(),
  name: 'libmpdclient',
  description: 'Music Player Daemon client library',
)

if get_option('documentation')
  doxygen = find_program('doxygen', required: false)
  if doxygen.found()
    subdir('doc')
  endif
endif

if get_option('test')
  check_dep = dependency('check')
  subdir('test')
endif
