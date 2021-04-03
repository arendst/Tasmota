set(LIBMPDCLIENT_MAJOR_VERSION 1)
set(LIBMPDCLIENT_MINOR_VERSION 18)
set(LIBMPDCLIENT_PATCH_VERSION 0)
set(SRC src)

set(app_sources 
    "${SRC}/async.c"
    "${SRC}/audio_format.c"
    "${SRC}/capabilities.c"
    "${SRC}/cmessage.c"
    "${SRC}/cmount.c"
    "${SRC}/cneighbor.c"
    "${SRC}/connection.c"
    "${SRC}/coutput.c"
    "${SRC}/cpartition.c"
    "${SRC}/cplaylist.c"
    "${SRC}/cstats.c"
    "${SRC}/cstatus.c"
    "${SRC}/database.c"
    "${SRC}/directory.c"
    "${SRC}/entity.c"
    "${SRC}/error.c"
    "${SRC}/example.c"
    "${SRC}/fd_util.c"
    "${SRC}/fingerprint.c"
    "${SRC}/idle.c"
    "${SRC}/ierror.c"
    "${SRC}/iso8601.c"
    "${SRC}/kvlist.c"
    "${SRC}/list.c"
    "${SRC}/message.c"
    "${SRC}/mixer.c"
    "${SRC}/mount.c"
    "${SRC}/neighbor.c"
    "${SRC}/output.c"
    "${SRC}/parser.c"
    "${SRC}/partition.c"
    "${SRC}/password.c"
    "${SRC}/player.c"
    "${SRC}/playlist.c"
    "${SRC}/queue.c"
    "${SRC}/quote.c"
    "${SRC}/rdirectory.c"
    "${SRC}/recv.c"
    "${SRC}/resolver.c"
    "${SRC}/response.c"
    "${SRC}/rplaylist.c"
    "${SRC}/run.c"
    "${SRC}/search.c"
    "${SRC}/send.c"
    "${SRC}/settings.c"
    "${SRC}/socket.c"
    "${SRC}/song.c"
    "${SRC}/stats.c"
    "${SRC}/status.c"
    "${SRC}/sticker.c"
    "${SRC}/sync.c"
    "${SRC}/tag.c")


idf_component_register(SRCS ${app_sources} INCLUDE_DIRS "include" ${CMAKE_CURRENT_BINARY_DIR} REQUIRES newlib)
configure_file(include/mpd/version.h.in version.h)
# use gnu stdlib stndup
# there is no PF_UNIX, but we'll fake it to compile
add_definitions(
    -DHAVE_STRNDUP
    -DENABLE_TCP=true
    -DPF_UNIX=PF_INET
    -DDEFAULT_HOST="localhost"
    -DDEFAULT_PORT=6600)
