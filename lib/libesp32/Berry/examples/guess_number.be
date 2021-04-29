import time
import math

math.srand(time.time())
res = math.rand() % 100
max_test = 7
test = -1
idx = 1
print('Guess a number between 0 and 99. You have', max_test, 'chances.')
while test != res && idx <= max_test
    test = number(input(str(idx) + ': enter the number you guessed: '))
    if type(test) != 'int'
        print('This is not an integer. Continue!')
        continue
    elif test > res
        print('This number is too large.')
    elif test < res
        print('This number is too small.')
    end
    idx = idx + 1
end
if test == res
    print('You win!')
else
    print('You failed, the correct answer is', res)
end
