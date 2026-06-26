# Test: mutating a list during iteration should not segfault
# This reproduces the bug where pushing to a list during a for loop
# caused a stale pointer dereference due to list reallocation.

var entities = []

class Entity
    var x, y
    def init(x, y)
        self.x = x
        self.y = y
    end
    def update()
        entities.push(Entity(10, 10))
    end
end

entities.push(Entity(1, 2))

var count = 0
for e : entities
    assert(type(e) == 'instance')
    assert(classname(e) == 'Entity')
    e.update()
    count += 1
    if count > 10
        break
    end
end

assert(count > 1)
# The list should have grown from the pushes during iteration
assert(entities.size() > 1)
