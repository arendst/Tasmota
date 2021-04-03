executable('main',
  'main.c',
  include_directories: inc,
  dependencies: [
    libmpdclient_dep,
  ])

test('t_iso8601', executable('t_iso8601',
  't_iso8601.c',
  '../src/iso8601.c',
  include_directories: inc,
  dependencies: [
    check_dep,
  ]))

test('t_commands', executable('t_commands',
  't_commands.c',
  'capture.c',
  include_directories: inc,
  dependencies: [
    libmpdclient_dep,
    check_dep,
  ]))
