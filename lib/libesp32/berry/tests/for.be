var global

global = 0
for i : 0 .. 10
    global += i
end
assert(global == 55)

global = 0
for i : 0 .. 20
    if i > 10
        break
    end
    global += i
end
assert(global == 55)

global = 0
for i : 0 .. 20
    if i > 10
        continue
    end
    global += i
end
assert(global == 55)

assert(def ()
        for i : 0 .. 20
            if i > 10
                return i
            end
        end
    end() == 11)

# test for "stop_iteration" exception as recurrence
def for_rec(depth)
    for i : 0 .. 10
        if i == 4 && depth < 200
            for_rec(depth + 1)
        end
    end
end

for_rec(0)
