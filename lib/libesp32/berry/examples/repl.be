do
    def ismult(msg)
        import string
        return string.split(msg, -5)[1] == '\'EOS\''
    end

    def multline(src, msg)
        if !ismult(msg)
            print('syntax_error: ' + msg)
            return
        end
        while true
            try
                src += '\n' + input('>> ')
                return compile(src)
            except 'syntax_error' as e, m
                if !ismult(m)
                    print('syntax_error: ' + m)
                    return
                end
            end
        end
    end

    def parse()
        var fun, src = input('> ')
        try
            fun = compile('return (' + src + ')')
        except 'syntax_error' as e, m
            try
                fun = compile(src)
            except 'syntax_error' as e, m
                fun = multline(src, m)
            end
        end
        return fun
    end

    def run(fun)
        try
            var res = fun()
            if res print(res) end
        except .. as e, m
            import debug
            print(e .. ': ' .. m)
            debug.traceback()
        end
    end

    def repl() 
        while true
            var fun = parse()
            if fun != nil
                run(fun)
            end
        end
    end

    print("Berry Berry REPL!")
    repl()
end
