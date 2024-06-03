import json

class bytes_build:
    def __init__(self, map):
        self.map = map.copy()

    def build(self, path):
        prefix = path + "/be_const_bytes"
        self.writefile(prefix + "_def.h", self.build_bytes_def())
        self.writefile(prefix + ".h", self.build_bytes_ext())
    
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
    
    def build_bytes_def(self):
        ostr = ""
        ostr += "/* binary arrays */\n"
        ostr += "be_define_const_bytes(,);\n"
        for k in self.map:
            ostr += "be_define_const_bytes("
            ostr += k + ", " + ", ".join( [ "0x" + k[i:i+2] for i in range(0, len(k), 2)] )
            ostr += ");\n"

        return ostr

    def build_bytes_ext(self):
        ostr = ""
        ostr += "/* extern binary arrays */\n"
        ostr += "extern const uint8_t be_const_bin_[];\n"
        for k in self.map:
            ostr += "extern const uint8_t be_const_bin_" + k + "[];\n"

        return ostr