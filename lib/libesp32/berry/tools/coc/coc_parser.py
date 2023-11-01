import re
from coc_string import unescape_operator

class data_value:
    def __init__(self):
        self.value = None
        self.depend = None

class object_block:
    def __init__(self):
        self.type = None
        self.name = None
        self.attr = {}
        self.data = {}
        self.data_ordered = []

class coc_parser:
    """Parser for Berry"""

    def __init__(self, text):
        """Parse text file"""
        self.objects = []
        self.strtab = set()
        self.strtab_weak = set()
        self.text = text
        self.parsers = {
            "@const_object_info_begin": self.parse_object,
            "be_const_str_": self.parse_string,
            "be_const_key(": self.parse_string,
            "be_nested_str(": self.parse_string,
            "be_const_key_weak(": self.parse_string_weak,
            "be_nested_str_weak(": self.parse_string_weak,
            "be_str_weak(": self.parse_string_weak,
        }

        while len(self.text) > 0:
            pattern = "|".join(self.parsers.keys())
            pattern = re.sub("\\(", "\\(", pattern)
            r = re.search(pattern, self.text)
            if not r: break

            self.text = self.text[r.end(0):]        # keep only after pattern
            func = self.parsers[r[0]]                # retrieve function for matched
            func()                                  # call function
    
    # def scan_const_string(self):
    #     r = re.match(r"\w*", self.text)
    #     if r:
    #         self.text = self.text[r.end(0)]
    #         self.strtab.append(r[0])
    
    def skip_space(self):
        r = re.match(r"\s+", self.text)
        if r:
            self.text = self.text[r.end(0):]
    
    def parse_char_base(self, c, necessary):
        res = self.text[0] == c
        if not res and necessary:   print(self.text); raise "error"
        if res: self.text = self.text[1:]
        return res
    
    def parse_char(self, c, necessary = False):
        self.skip_space()
        return self.parse_char_base(c, necessary)
    
    def skip_char(self, c):
        self.parse_char(c, True)

    def parse_char_continue(self, c, necessary = False):
        ch = self.text[0]
        while ch == ' ' or ch == "\t":  self.text = self.text[1:]
        return self.parse_char_base(c, necessary)

    def parse_word(self):
        self.skip_space()
        r = re.match(r"\w+", self.text)
        self.text = self.text[r.end(0):]
        return r[0]

    # parse until the next comma or space (trim preceding spaces before)
    # does not skip the comma
    def parse_tocomma(self):
        self.skip_space()
        r = re.match(r"[^,\s]*", self.text)
        self.text = self.text[r.end(0):]
        return r[0]

    # parse until the next closing parenthesis or a single token if no parenthesis (trim preceding spaces before)
    # matches:
    #  'int'
    #  'func(aa)'
    #  'mapped_func(aa,"ee", "aa")
    def parse_value(self):
        self.skip_space()
        r = re.match(r"(\S+\(.*?\))|([^,\s]*)", self.text)
        self.text = self.text[r.end(0):]
        return r[0]

    def parse_tonewline(self):
        self.skip_space()
        r = re.match(r"[^\r\n]*", self.text)
        self.text = self.text[r.end(0):]
        return r[0]

    def parse_object(self):
        self.text = re.sub(r"\s+//.*?$", "", self.text, flags=re.MULTILINE)      # remove trailing comments
        while True:
            obj = self.parse_block()
            self.objects.append(obj)
            if self.parse_char("@"): break
        
        end_text = "const_object_info_end"
        if not str.startswith(self.text, end_text): raise "error"
        self.text = self.text[len(end_text):]
        # print("END: @const_object_info_end test={self.text}")

    def parse_string(self):
        if not self.text[0].isalnum() and self.text[0] != '_': return      # do not proceed, maybe false positive in solidify
        ident = self.parse_word()
        literal = unescape_operator(ident)
        if not literal in self.strtab:
            self.strtab.add(literal)
            # print(f"str '{ident}' -> {literal}")

    def parse_string_weak(self):
        if not self.text[0].isalnum() and self.text[0] != '_': return      # do not proceed, maybe false positive in solidify
        ident = self.parse_word()
        literal = unescape_operator(ident)
        if not literal in self.strtab:
            self.strtab_weak.add(literal)
            # print(f"str '{ident}' -> {literal}")

    #################################################################################
    # Parse a block of definition like module, class...
    #################################################################################
    def parse_block(self):
        obj = object_block()
        obj.type = self.parse_word()
        obj.name = self.parse_word()
        # print(f"parse_block: type={obj.type} name={obj.name}")
        # # ex: 'parse_block: type=module name=gpio'
        self.parse_attr(obj)
        self.parse_body(obj)
        return obj

    def parse_attr(self, obj):
        self.skip_char("(")
        self.parse_attr_pair(obj)
        while self.parse_char(","):
            self.parse_attr_pair(obj)
        self.skip_char(")")

    def parse_attr_pair(self, obj):
        key = self.parse_word()
        self.skip_char(":")
        value = self.parse_word()
        obj.attr[key] = value
    
    #################################################################################
    # Parse the body definition of a class, module...
    #################################################################################
    def parse_body(self, obj):
        self.skip_char("{")
        if not self.parse_char("}"):
            while True:
                self.parse_body_item(obj)
                if self.parse_char("}"): break
    
    #################################################################################
    # Parse each line item in the module/class/vartab
    #################################################################################
    def parse_body_item(self, obj):
        value = data_value()
        key = self.parse_tocomma()
        # print(f"Key={key}")
        self.parse_char_continue(",", True)     # skip the ',' after the key
        value.value = self.parse_value()
        # print(f"value.value={value.value}")
        if self.parse_char_continue(","):
            value.depend = self.parse_tonewline()
        obj.data[key] = value
        obj.data_ordered.append(key)
