Lua 5.3.4 Library Compiled for Tock
===================================

Lua is a powerful, efficient, lightweight, embeddable scripting language. It
supports procedural programming, object-oriented programming, functional
programming, data-driven programming, and data description.

As an extension language, Lua has no notion of a "main" program: it works
embedded in a host client, called the embedding program or simply the host.  The
host program can invoke functions to execute a piece of Lua code, can write and
read Lua variables, and can register C functions to be called by Lua code.
Through the use of C functions, Lua can be augmented to cope with a wide range
of different domains, thus creating customized programming languages sharing a
syntactical framework.

See https://www.lua.org/manual/5.3/ for documentation.

Using `lua53` in Tock
--------------------

To use `lua53`, add the following include to the application's
Makefile:

    EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/lua53



Re-compiling `lua53`
-----------------

Checkout the lua submodule in this directory

    $ git submodule init -- lua
    $ git submodule update

then run `make`

    $ make
