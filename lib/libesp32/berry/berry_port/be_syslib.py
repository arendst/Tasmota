"""
Berry sys module — Python port of src/be_syslib.c

Provides the ``sys`` module with a single function ``path()`` that returns
the current module search path as a Berry list.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/


def m_path(vm):
    """Return the module search path as a Berry list.

    Mirrors the C implementation:

    // static int m_path(bvm *vm)
    // {
    //     be_getbuiltin(vm, "list");
    //     be_module_path(vm);
    //     be_call(vm, 1);
    //     be_pop(vm, 1);
    //     be_return(vm);
    // }
    """
    from berry_port.be_api import (
        be_getbuiltin, be_call, be_pop, be_returnvalue,
    )
    from berry_port.be_module import be_module_path

    be_getbuiltin(vm, "list")
    be_module_path(vm)
    be_call(vm, 1)
    be_pop(vm, 1)
    return be_returnvalue(vm)


def be_sys_module_table():
    """Build the sys module attribute table.

    Mirrors the C module definition:

    // be_native_module_attr_table(sys){
    //     be_native_module_function("path", m_path)
    // };
    //
    // be_define_native_module(sys, NULL);
    """
    return [
        ("path", m_path),
    ]
