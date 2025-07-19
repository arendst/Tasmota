import time

c = time.clock()
do
    i = 0
    while i < 100000000
        i += 1
    end
end
print('while iteration 100000000 times', time.clock() - c, 's')

c = time.clock()
for i : 1 .. 100000000
end
print('for   iteration 100000000  times', time.clock() - c, 's')
