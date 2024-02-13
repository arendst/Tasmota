import json
from coc_string import *

# from https://stackoverflow.com/questions/14945095/how-to-escape-string-for-generated-c (simplified)
def escape_c(s, encoding='ascii'):
    result = ''
    for c in s:
        if not (32 <= ord(c) < 127):
            result += '\\%03o' % ord(c)
        elif c == '\\':
            result += "\\\\"
        elif c == '"':
            result += "\\\""
        else:
            result += c
    return '"' + result + '"'

class str_info:
    def __init__(self):
        self.hash = 0
        self.str = ""
        self.extra = 0

class str_build:
    def __init__(self, map, map_weak, map_long):
        self.map = map.copy()
        self.str_weak = []
        self.str_long = []

        size = int(len(self.map) / 2)         # voluntarily reduce hash size to half
        if size < 4: size = 4
        self.buckets = []
        for i in range(size):
            self.buckets.append([])
        
        self.keywords()     # add keywords to self.map

        self.make_ceil("", 0)   # add empty string as it is always useful
        self.count = len(self.map) + 1       # TODO it is not actually accurate since keywords are not counted
        for k in sorted(self.map.keys()):
            self.make_ceil(k, self.map[k])

        # handle weak strings
        for k in sorted(map_weak.keys()):
            if not k in self.map:
                self.str_weak.append(k)

        # handle long strings
        for k in sorted(map_long.keys()):
            if not k in self.map:
                self.str_long.append(k)

    def build(self, path):
        prefix = path + "/be_const_strtab"
        self.writefile(prefix + "_def.h", self.build_table_def())
        self.writefile(prefix + ".h", self.build_table_ext())
    
    def get_count(self):            # compute the total size by adding sizes of each bucket
        size = 0
        for bucket in self.buckets:
            size += len(bucket)
        return size
    
    def keywords(self):
        opif = 50
        tab = {
            "if": opif,             "elif": opif + 1 ,
            "else": opif + 2 ,      "while": opif + 3 ,
            "for": opif + 4 ,       "def": opif + 5 ,
            "end": opif + 6 ,       "class": opif + 7 ,
            "break": opif + 8 ,     "continue": opif + 9 ,
            "return": opif + 10 ,   "true": opif + 11 ,
            "false": opif + 12 ,    "nil": opif + 13 ,
            "var": opif + 14 ,      "do": opif + 15 ,
            "import": opif + 16 ,   "as": opif + 17 ,
            "try": opif + 18 ,      "except": opif + 19 ,
            "raise": opif + 20 ,    "static": opif + 21,
        }
        for key, v in tab.items():
            self.map[key] = v
    
    def make_ceil(self, name, extra):
        info = str_info()
        info.hash = hashcode(name)
        info.str = name
        info.extra = extra
        self.buckets[info.hash % len(self.buckets)].append(info)

    def writefile(self, filename, text):
        buf = ""
        try:
            with open(filename) as f:
                buf = f.read()
        except FileNotFoundError:
            pass
        if buf != text:
            with open(filename, "w") as f:
                f.write(text)
    
    def build_table_def(self):
        strings = {}
        for bucket in self.buckets:
            # print(f"> Bucket= {[x.str for x in bucket]}")
            size = len(bucket)
            for i in range(size):
                info = bucket[i]
                node = escape_operator(info.str)
                istr = ""
                if i < size - 1:
                    next = "&be_const_str_" + escape_operator(bucket[i + 1].str)
                else:
                    next = "NULL"
                istr += "be_define_const_str("
                istr += node + ", " + escape_c(info.str) + ", "
                istr += str(info.hash) + "u, " + str(info.extra) + ", "
                istr += str(len(info.str)) + ", " + next + ");\n"
                strings[info.str] = istr
        
        ostr = ""
        for s in sorted(strings.keys()):
            ostr += strings[s]
        ostr += "\n"

        ostr += "\n/* weak strings */\n"
        for k in self.str_weak:
            ostr += "be_define_const_str("
            ostr += escape_operator(k) + ", " + escape_c(k) + ", "
            ostr += "0u, 0, " + str(len(k)) + ", NULL);\n"

        for k in self.str_long:
            ostr += "be_define_const_str_long("
            ostr += escape_operator(k) + ", " + escape_c(k) + ", " + str(len(k)) + ");\n"

        ostr += "\n"
        ostr += "static const bstring* const m_string_table[] = {\n"


        size = len(self.buckets)
        for i in range(size):
            bucket = self.buckets[i]
            if len(bucket) > 0:
                ostr += "    (const bstring *)&be_const_str_" + escape_operator(bucket[0].str)
            else:
                ostr += "    NULL"
            if i < size - 1: ostr += ","
            ostr += "\n"
        ostr += "};\n\n"
        ostr += "static const struct bconststrtab m_const_string_table = {\n"
        ostr += "    .size = " + str(size) + ",\n"
        ostr += "    .count = " + str(self.get_count()) + ",\n"
        ostr += "    .table = m_string_table\n"
        ostr += "};\n"
        return ostr

    def build_table_ext(self):
        ostr = ""
        # put all in a sorted sed
        all = set()
        for bucket in self.buckets:
            for info in bucket:
                all.add(escape_operator(info.str))
        for s in sorted(all):
            ostr += "extern const bcstring be_const_str_" + s + ";\n"
            # ostr += "#define BE_CONST_STR_" + s + "\n"
        # weak strings
        ostr += "\n/* weak strings */\n"
        for s in self.str_weak:
            ostr += "extern const bcstring be_const_str_" + escape_operator(s) + ";\n"
        for s in self.str_long:
            ostr += "extern const bclstring be_const_str_" + escape_operator(s) + ";\n"
        return ostr
