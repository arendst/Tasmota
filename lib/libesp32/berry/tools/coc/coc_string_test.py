import unittest
from coc_string import *

class Test_coc_string(unittest.TestCase):

    def test_hash(self):
        self.assertEqual(hashcode("add"), 993596020)
        self.assertEqual(hashcode("real"), 3604983901)

    def test_escape(self):
        self.assertEqual(escape_operator("foo"), "foo")
        self.assertEqual(escape_operator(".."), "_opt_connect")
        self.assertEqual(escape_operator("foo.bar"), "foo_dot_bar")
        self.assertEqual(escape_operator("foo._bar"), "foo_dot___bar")
        self.assertEqual(escape_operator("foo. bar"), "foo_dot__20bar")

if __name__ == '__main__':
    unittest.main()