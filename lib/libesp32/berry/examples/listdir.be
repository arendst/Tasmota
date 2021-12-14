import os

def scandir(path)
    print('path: ' + path)
    for name : os.listdir(path)
        var fullname = os.path.join(path, name)
        if os.path.isfile(fullname)
            print('file: ' + fullname)
        else
            print('path: ' + fullname)
            scandir(fullname)
        end
    end
end

scandir('.')
