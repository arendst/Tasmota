# anonymous function and closure
def count(x)
    var arr = []
    for i : 0 .. x
        arr.push(
            def (n) # loop variable cannot be used directly as free variable
                return def ()
                    return n * n
                end
            end (i) # define and call anonymous function
        )
    end
    return arr
end

for xx : count(6)
    print(xx()) # 0, 1, 4 ... n * n
end

return count
