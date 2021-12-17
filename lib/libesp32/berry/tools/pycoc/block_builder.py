import copy
from hash_map import *

class block:
    def __init__(self):
        self.type = ""
        self.name = ""
        self.attr = {}
        self.data = {}
        self.data_ordered = []

def depend(obj, macro):
    if "depend" in obj.attr:
        return macro.query(obj.attr["depend"])
    else:
        return True

class block_builder:
    """Output an object"""

    def __init__(self, obj, macro):
        self.block = block()
        self.strtab = []

        self.block.name = obj.name
        if depend(obj, macro):
            self.block.type = obj.type
            self.block.attr = obj.attr
            
            if "name" in obj.attr:
                self.strtab.append(obj.attr["name"])
            
            for key in obj.data_ordered:
                second = obj.data[key]
                if second.depend == None or macro.query(second.depend):
                    self.block.data[key] = second.value
                    self.strtab.append(key)
                    self.block.data_ordered.append(key)
    
    def block_tostring(self, block):
        ostr = ""
        if block.type == "map":
            ostr += self.map_tostring(block, block.name)
        elif block.type == "class":
            ostr += self.class_tostring(block)
        elif block.type == "vartab":
            ostr += self.vartab_tostring(block)
        elif block.type == "module":
            ostr += self.module_tostring(block)
        return ostr

    def class_tostring(self, block):
        ostr = ""
        hmap = hash_map(block.data)
        map_name = block.name + "_map"
        if len(block.data) > 0:
            ostr += self.map_tostring(block, map_name, True) + "\n"
        
        ostr += self.scope(block) + " be_define_const_class(\n    "
        ostr += block.name + ",\n    "
        ostr += str(hmap.var_count()) + ",\n    "
        ostr += self.get_super(block) + ",\n    "
        ostr += self.name(block) + "\n);\n"
        return ostr
    
    def map_tostring(self, block, name, local):
        hmap = hash_map(block.data)
        entlist = hmap.entry_list()
        ostr = "static be_define_const_map_slots(" + name + ") {\n"
        for ent in entlist:
            ostr += "    { be_const_key(" + ent.key + ", "
            ostr += str(ent.next) + "), " + ent.value + " },\n"
        ostr += "};\n\n"

        if local:
            ostr += "static"
        else:
            ostr += self.scope(block)
        ostr += " be_define_const_map(\n    "
        ostr += name + ",\n    "
        ostr += str(len(entlist)) + "\n);\n"
        return ostr
    
    def vartab_tostring(self, block):
        ostr = ""
        varvec = []
        idxblk = copy.deepcopy(block)
        index = 0
        idxblk.data = {}
        for key in block.data_ordered:
            varvec.append(block.data[key])
            idxblk.data[key] = "int(" + str(index) + ")"
            index += 1
        
        ostr += self.map_tostring(idxblk, block.name + "_map", True) + "\n"
        ostr += "static const bvalue __vlist_array[] = {\n";
        for it in varvec:
            ostr += "    be_const_" + it + ",\n"
        ostr += "};\n\n"

        ostr += "static be_define_const_vector(\n    "
        ostr += block.name + "_vector,\n    __vlist_array,\n    "
        ostr += str(len(varvec)) + "\n);\n"
        return ostr
    
    def module_tostring(self, block):
        ostr = ""
        name = "m_lib" + block.name
        map_name = name + "_map"

        ostr += self.map_tostring(block, map_name, True) + "\n"
        ostr += "static be_define_const_module(\n    "
        ostr += name + ",\n    "
        ostr += "\"" + block.name + "\"\n);\n"
        scp = self.scope(block)
        if scp != "static":
            ostr += "\n" + scp
            ostr += " be_define_const_native_module("
            ostr += block.name + ");\n"
        return ostr

    def scope(self, block):
        if "local" in block.attr:
            return "static"
        else:
            return "BE_EXPORT_VARIABLE"

    def get_super(self, block):
        if "super" in block.attr:
            return "(bclass *)&" + block.attr["super"]
        else:
            return "NULL"
    
    def name(self, block):
        if "name" in block.attr:
            return block.attr["name"]
        else:
            return block.name
    
    def writefile(self, filename, text):
        otext = "#include \"be_constobj.h\"\n\n" + text
        with open(filename, "w") as f:
            f.write(otext)

    def dumpfile(self, path):
        s = self.block_tostring(self.block)
        if "file" in self.block.attr:
            name = self.block.attr["file"]
        else:
            name = self.block.name
        self.writefile(path + "/be_fixed_" + name + ".h", s)
        