import os

def strfind(str, char)
    var len = size(str)
    for i : 0 .. len - 1
        if str[i] == char
            return true
        end
    end
    return false
end

def checkfile(path)
    var subname = os.path.splitext(path)[1]
    if (subname == '.c' || subname == '.h' ||
        subname == '.cpp' || subname == '.be' || subname == '.json')
        var f = open(path)
        assert(!strfind(f.read(), '\t'), 'file \'' + path + '\' has tab character')
        f.close()
    end
end

def findpath(path)
    var ls = os.listdir(path)
    for name : ls
        var fullname = os.path.join(path, name)
        if os.path.isfile(fullname)
            checkfile(fullname)
        elif fullname != '.' && fullname != '..'
            findpath(fullname)
        end
    end
end

findpath('.')
