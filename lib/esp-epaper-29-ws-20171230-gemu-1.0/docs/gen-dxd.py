# gen-dxd.py - Generate Doxygen Directives
#
# This code is in the Public Domain (or CC0 licensed, at your option.)
# Unless required by applicable law or agreed to in writing, this
# software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, either express or implied.
#

import sys
import os
import re

# Script configuration
header_file_path_prefix = "../components/"
"""string: path prefix for header files.
"""
doxyfile_path = "Doxyfile"
"""string: path to a file containing header files to processs.
"""
xml_directory_path = "xml"
"""string: path to directory with XML files by Doxygen.
"""
inc_directory_path = "_build/inc"
"""string: path prefix for header files.
"""
all_kinds = [
    ("function", "Functions"),
    ("union", "Unions"),
    ("struct", "Structures"),
    ("define", "Macros"),
    ("typedef", "Type Definitions"),
    ("enum", "Enumerations")
    ]
"""list of items that will be generated for a single API file
"""


def get_doxyfile_input():
    """Get contents of Doxyfile's INPUT statement.

    Returns:
        Contents of Doxyfile's INPUT.

    """
    if not os.path.isfile(doxyfile_path):
        print "Doxyfile '%s' does not exist!" % doxyfile_path
        sys.exit()

    print "Getting Doxyfile's INPUT"

    input_file = open(doxyfile_path, "r")

    line = input_file.readline()
    # read contents of Doxyfile until 'INPUT' statement
    while line:
        if line.find("INPUT") == 0:
            break
        line = input_file.readline()

    doxyfile_INPUT = ""
    line = input_file.readline()
    # skip input_file contents until end of 'INPUT' statement
    while line:
        if line.isspace():
            # we have reached the end of 'INPUT' statement
            break
        # process only lines that are not comments
        if line.find("#") == -1:
            # extract header file path inside components folder
            m = re.search(header_file_path_prefix + "(.*\.h)", line)
            header_file_path = m.group(1)
            doxyfile_INPUT += header_file_path + "\n"
        # proceed reading next line
        line = input_file.readline()

    input_file.close()
    return doxyfile_INPUT


def get_api_name(header_file_path):
    """Get name of API from header file path.

    Args:
        header_file_path: path to the header file.

    Returns:
        The name of API.

    """
    api_name = ""
    regex = r".*/(.*)\.h"
    m = re.search(regex, header_file_path)
    if m:
        api_name = m.group(1)

    return api_name


def get_rst_header(header_name):
    """Get rst formatted code with a header.

    Args:
        header_name: name of header.

    Returns:
        Formatted rst code with the header.

    """

    rst_output = ""
    rst_output += header_name + "\n"
    rst_output += "^" * len(header_name) + "\n"
    rst_output += "\n"

    return rst_output


def select_unions(innerclass_list):
    """Select unions from innerclass list.

    Args:
        innerclass_list: raw list with unions and structures
                         extracted from Dogygen's xml file.

    Returns:
        Doxygen directives with unions selected from the list.

    """

    rst_output = ""
    for line in innerclass_list.splitlines():
        # union is denoted by "union" at the beginning of line
        if line.find("union") == 0:
            union_id, union_name = re.split(r"\t+", line)
            rst_output += ".. doxygenunion:: "
            rst_output += union_name
            rst_output += "\n"

    return rst_output


def select_structs(innerclass_list):
    """Select structures from innerclass list.

    Args:
        innerclass_list: raw list with unions and structures
                         extracted from Dogygen's xml file.

    Returns:
        Doxygen directives with structures selected from the list.
        Note: some structures are excluded as described on code below.

    """

    rst_output = ""
    for line in innerclass_list.splitlines():
        # structure is denoted by "struct" at the beginning of line
        if line.find("struct") == 0:
            # skip structures that are part of union
            # they are documented by 'doxygenunion' directive
            if line.find("::") > 0:
                continue
            struct_id, struct_name = re.split(r"\t+", line)
            rst_output += ".. doxygenstruct:: "
            rst_output += struct_name
            rst_output += "\n"
            rst_output += "    :members:\n"
            rst_output += "\n"

    return rst_output


def get_directives(tree, kind):
    """Get directives for specific 'kind'.

    Args:
        tree: the ElementTree 'tree' of XML by Doxygen
        kind: name of API "kind" to be generated

    Returns:
        Doxygen directives for selected 'kind'.
        Note: the header with "kind" name is included.

    """

    rst_output = ""
    if kind in ["union", "struct"]:
        innerclass_list = ""
        for elem in tree.iterfind('compounddef/innerclass'):
            innerclass_list += elem.attrib["refid"] + "\t" + elem.text + "\n"
        if kind == "union":
            rst_output += select_unions(innerclass_list)
        else:
            rst_output += select_structs(innerclass_list)
    else:
        for elem in tree.iterfind(
                'compounddef/sectiondef/memberdef[@kind="%s"]' % kind):
            name = elem.find('name')
            rst_output += ".. doxygen%s:: " % kind
            rst_output += name.text + "\n"
    if rst_output:
        all_kinds_dict = dict(all_kinds)
        rst_output = get_rst_header(all_kinds_dict[kind]) + rst_output + "\n"

    return rst_output


def generate_directives(header_file_path):
    """Generate API reference with Doxygen directives for a header file.

    Args:
        header_file_path: a path to the header file with API.

    Returns:
        Doxygen directives for the header file.

    """

    api_name = get_api_name(header_file_path)

    # in XLT file name each "_" in the api name is expanded by Doxygen to "__"
    xlt_api_name = api_name.replace("_", "__")
    xml_file_path = "%s/%s_8h.xml" % (xml_directory_path, xlt_api_name)

    rst_output = ""
    rst_output = ".. File automatically generated by 'gen-dxd.py'\n"
    rst_output += "\n"
    rst_output += get_rst_header("Header File")
    rst_output += "* :component_file:`" + header_file_path + "`\n"
    rst_output += "\n"

    try:
        import xml.etree.cElementTree as ET
    except ImportError:
        import xml.etree.ElementTree as ET

    tree = ET.ElementTree(file=xml_file_path)
    for i in range(len(all_kinds)):
        kind = all_kinds[i][0]
        rst_output += get_directives(tree, kind)

    return rst_output


def generate_api_inc_files():
    """Generate header_file.inc files
    with API reference made of doxygen directives
    for each header file
    specified in the 'INPUT' statement of Doxyfile.

    """

    if not os.path.isdir(xml_directory_path):
        print "Directory %s does not exist!" % xml_directory_path
        sys.exit()

    if not os.path.exists(inc_directory_path):
        os.makedirs(inc_directory_path)

    list_to_generate = get_doxyfile_input()
    print "Generating 'api_name.inc' files with Doxygen directives"
    for header_file_path in list_to_generate.splitlines():
        api_name = get_api_name(header_file_path)
        inc_file_path = inc_directory_path + "/" + api_name + ".inc"
        rst_output = generate_directives(header_file_path)
        inc_file = open(inc_file_path, "w")
        inc_file.write(rst_output)
        inc_file.close()


if __name__ == "__main__":
    """The main script that generates
    Doxygen directives.

    """

    # Process command line arguments, if any
    if len(sys.argv) > 1:
        if not os.path.isdir(xml_directory_path):
            print "Directory %s does not exist!" % xml_directory_path
            sys.exit()
        header_file_path = sys.argv[1]
        api_name = get_api_name(header_file_path)
        if api_name:
            rst_output = generate_directives(header_file_path)
            print "Doxygen directives for '%s'" % header_file_path
            print
            print rst_output
        else:
            print "Options to execute 'gen-dxd.py' application:"
            print "1: $ python gen-dxd.py"
            print "   Generate API 'header_file.inc' files for headers defined in '%s'" % doxyfile_path
            print "2: $ python gen-dxd.py header_file_path"
            print "   Print out Doxygen directives for a single header file"
            print "   example: $ python gen-dxd.py mdns/include/mdns.h"
            print "   NOTE: Run Doxygen first to get XML files for the header file"

        sys.exit()

    # No command line arguments given
    generate_api_inc_files()
