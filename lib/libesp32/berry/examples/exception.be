import debug

def test_func()
    try
        compile('def +() end')()
    except .. as e, v
        print('catch execption:', str(e) + ' >>>\n    ' + str(v))
        debug.traceback()
    end
end

test_func()
