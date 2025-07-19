# Python compatibility

python_compat = module('python_compat')

#@ solidify:python_compat
python_compat.init = def (m)
    import global
    global.True = true
    global.False = false
    global.None = nil
    return m
end
