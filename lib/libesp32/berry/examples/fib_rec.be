import time

def fib(x)
    if x <= 2
        return 1
    end
    return fib(x - 1) + fib(x - 2)
end

c = time.clock()
print("fib:", fib(38)) # minimum stack size: 78!!
print("time:", time.clock() - c, 's')
