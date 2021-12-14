#! ./berry
import os

os.system('lcov', '-q -c -i -d . -o init.info')

var exec = './berry'
var path = 'tests'
var testcases = os.listdir(path)
var total = 0, failed = 0

for i : testcases
    if os.path.splitext(i)[1] == '.be'
        print('\033[0;36mrun testcase: ' + i + '\033[0m')
        var ret = os.system(exec, os.path.join(path, i))
        if ret != 0
            print('\033[0;31mreturn code:', ret, '\033[0m')
            failed += 1
        end
        total += 1
    end
end

print('\033[0;32mtest results: ' +
    str(total) + ' total, ' + str(failed) + ' failed' +
    (failed ? '' : ' (all tests passed)') +
    '.\033[0m')

if failed != 0
    os.exit(-1)
end

var cmds = [
    'lcov -q -c -d ./ -o cover.info',
    'lcov -q -a init.info -a cover.info -o total.info',
    'lcov --remove total.info */usr/include/* -o final.info',
    'genhtml -q -o test_report --legend --title "lcov" --prefix=./ final.info',
    'rm -f init.info cover.info total.info final.info'
]

for cmd : cmds
    if os.system(cmd)
        os.exit(-1)
    end
end
