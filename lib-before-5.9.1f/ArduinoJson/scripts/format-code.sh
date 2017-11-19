cd ..
FILES=$(find include src test -regex ".*\.[ch]pp$")

clang-format -style=Google -i  $FILES

# insert newline at end of file
sed -i -e '$a\' $FILES
