libmpdclient
============

libmpdclient is a C library which implements the `Music Player Daemon
<http://www.musicpd.org/>`__ protocol.

The API documentation is generated during the build if you have
doxygen.  You may browse it online:

http://www.musicpd.org/doc/libmpdclient/

libmpdclient is released under the revised BSD License.
For the full license, see `COPYING <COPYING>`__.


Installing from source
----------------------

You need:

- a C99 compliant compiler (e.g. gcc)
- `Meson 0.37 <http://mesonbuild.com/>`__ and `Ninja <https://ninja-build.org/>`__

Run ``meson``:

 meson . output

Compile and install::

 ninja -C output
 ninja -C output install


Links
-----

- `Home page and download <https://www.musicpd.org/libs/libmpdclient/>`__
- `git repository <https://github.com/MusicPlayerDaemon/libmpdclient/>`__
- `Bug tracker <https://github.com/MusicPlayerDaemon/libmpdclient/issues>`__
