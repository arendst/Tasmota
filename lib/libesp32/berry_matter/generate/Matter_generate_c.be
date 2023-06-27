# ../../berry/berry -s -g Matter_generate_c.be
#
# generate C headers

import matter_clusters
var c = matter_clusters


#################################################################################
# Simple insertion sort - sorts the list in place, and returns the list
#################################################################################
def sort(l) for i:1..size(l)-1 var k = l[i] var j = i while (j > 0) && (l[j-1] > k) l[j] = l[j-1] j -= 1 end l[j] = k end return l end

# keys to llist
def k2l(m) var r=[] for k:m.keys() r.push(k) end return sort(r) end

var cl_keys = {}
for cl:c
  for k:cl.keys()
    cl_keys[k] = true
  end
end

var attr_ids = {}
var attr_keys = {}
var attr_types = {}
for cl:c
  for attr:cl['attributes']
    attr_ids[attr['attributeId']] = true
    for k:attr.keys()
      attr_keys[k] = true
    end
    attr_types[attr['type']] = true
  end
end

import string

# prepare file
f=open("be_matter_clusters.h", "w")
def fprint(*l)
  f.write(l.concat(" "))
  f.write("\n")
end

# fprint("Cluster keys:",k2l(cl_keys))
# fprint("Attribute ids:",k2l(attr_ids))
# fprint("Attribute keys:",k2l(attr_keys))
# fprint("Attribute types:",k2l(attr_types))

var cl_id_name = {}
for cl:c
  cl_id_name[cl['clusterId']] = cl['clusterName']
end
var cl_ids = k2l(cl_id_name)

fprint("/*********************************************************************************\\")
fprint("* Compact form for attributes and clusters")
fprint("*")
fprint("* Generated content, do not edit")
fprint("\\*********************************************************************************/")
fprint("#include <stddef.h>")
fprint("#include <stdint.h>")
fprint()
fprint("typedef struct {")
fprint("  uint16_t id;")
fprint("  uint8_t  type;")
fprint("  uint8_t  flags;")
fprint("  const char* name;")
fprint("} matter_attribute_t;")
fprint()
fprint("typedef struct {")
fprint("  uint16_t id;")
fprint("  const char* name;")
fprint("} matter_command_t;")
fprint()
fprint("typedef struct {")
fprint("  uint16_t id;")
fprint("  const char* name;")
fprint("  const matter_attribute_t* attributes;")
fprint("  const matter_command_t* commands;")
fprint("} matter_cluster_t;")
fprint()
for cl:cl_ids
  fprint(f"const matter_attribute_t matter_Attributes_{cl:04X}[] = {")
  var attributes = c[cl_id_name[cl]]['attributes']
  var attr_id_name = {}
  for attr:attributes
    attr_id_name[attr['attributeId']] = attr['attributeName']
  end
  var attr_ids_local = k2l(attr_id_name)

  for attr_id:attr_ids_local
    var reportable = attributes[attr_id].find('reportable', false)
    var writable = attributes[attr_id].find('writable', false)
    var flags = (writable ? 0x01 : 0x00) | (reportable ? 0x02 : 0x00)
    fprint(format('  { 0x%04X, %i, 0x%02X, "%s" },', attr_id, 0, flags, attributes[attr_id]['attributeName']))
  end
  fprint('  { 0xFFFF, 0, 0x00, NULL },')
  fprint("};")
  fprint()
  # commands
  fprint(f"const matter_command_t matter_Commands_{cl:04X}[] = {")
  var commands = c[cl_id_name[cl]]['commands']
  var cmd_id_name = {}
  for cmd:commands
    cmd_id_name[cmd['commandId']] = cmd['commandName']
  end
  var cmd_ids_local = k2l(cmd_id_name)

  for cmd_id:cmd_ids_local
    fprint(format('  { 0x%04X, "%s" },', cmd_id, commands[cmd_id]['commandName']))
  end
  fprint('  { 0xFFFF, NULL },')
  fprint("};")
  fprint()
end

fprint("const matter_cluster_t matterAllClusters[] = {")
for cl:cl_ids
  fprint(format('  { 0x%04X, "%s", matter_Attributes_%04X, matter_Commands_%04X },', cl, cl_id_name[cl], cl, cl))
end
  fprint('  { 0xFFFF, NULL, NULL },')
fprint("};")

f.close()
