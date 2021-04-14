# Reference from https://github.com/BerryMathDevelopmentTeam/BerryMath/blob/master/testscript/BinaryTree.bm

class node
    var v, l, r
    def init(v, l, r)
        self.v = v
        self.l = l
        self.r = r
    end
    def insert(v)
        if v < self.v
            if self.l
                self.l.insert(v)
            else
                self.l = node(v)
            end
        else
            if self.r
                self.r.insert(v)
            else
                self.r = node (v)
            end
        end
    end
    def sort(l)
        if (self.l) self.l.sort(l) end
        l.push(self.v)
        if (self.r) self.r.sort(l) end
    end
end

class btree
    var root
    def insert(v)
        if self.root
            self.root.insert(v)
        else
            self.root = node(v)
        end
    end
    def sort()
        var l = []
        if self.root
            self.root.sort(l)
        end
        return l
    end
end

var tree = btree()
tree.insert(-100)
tree.insert(5);
tree.insert(3);
tree.insert(9);
tree.insert(10);
tree.insert(10000000);
tree.insert(1);
tree.insert(-1);
tree.insert(-10);
print(tree.sort());
