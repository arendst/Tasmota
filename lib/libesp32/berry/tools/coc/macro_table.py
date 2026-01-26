import re

def int_safe(v):
    try:     return int(v, 0)
    except:  return 0

class macro_table:
    pat = re.compile("(?:\\n|$)\\s*#define\\s+(\\w+)[ \\t]+(\\w*)", re.MULTILINE)
    pat_query = re.compile("(!?)(\\w+)")

    def __init__(self):
        self.map = {}
    
    # def readfile(self, filename):
    #     with open(filename) as f:
    #         return f.read()

    def parse_value(self, s):
        if len(s) == 0:             return 1    # defined a macro name but no content, considered true
        if not s[0].isnumeric():    return 1
        return int_safe(s)
    
    def scan_file(self, filename):
        str = ""
        with open(filename, encoding='utf-8') as f:
           str = f.read()
        r = macro_table.pat.findall(str)
        for it in r:
            # print(f"> it0:{it[0]} it1:{it[1]}")
            self.map[it[0]] = self.parse_value(it[1])
    
    def query(self, s):
        r = macro_table.pat_query.search(s)
        value = False
        if r:
            bang = r[1]
            name = r[2]
            # print(f">query: bang={bang} name={name} value={self.map.get(name)}")
            if name in self.map:
                value = int(self.map[name]) != 0
            if bang == "!":
                value = not value
        # print(f">query: {s}:{value}")
        return value

if __name__ == '__main__':
    # from hash_map import *
    pat = re.compile("(?:\\n|$)\\s*#define\\s+(\\w+)[ \\t]+(\\w+)", re.MULTILINE)
    s = "aaa\n#define A 1  // a \n  #define B 2\n#define C \n#define D 0 \n  #define E     11 \na"
    