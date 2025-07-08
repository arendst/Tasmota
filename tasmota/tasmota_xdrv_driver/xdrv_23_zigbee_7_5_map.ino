/*
  xdrv_23_zigbee_7_5_map.ino - zigbee support for Tasmota

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_ZIGBEE

class Z_Mapper_Edge {
public:

  // enum Edge_Type : uint8_t {
  //   Unknown = 0x00,
  //   Parent = 0x01,          // node_1 is parent of node_2
  //   Child = 0x02,           // node_1 is child of node_2
  //   Sibling = 0x03,         // both nodes are siblings
  // };

  Z_Mapper_Edge(void) :
    node_1(BAD_SHORTADDR),
    node_2(BAD_SHORTADDR),
    lqi(0x00)
    // edge_type(Unknown)
    {}

  // Z_Mapper_Edge(uint16_t node_a, uint16_t node_b, uint8_t _lqi, Edge_Type _type) :
  Z_Mapper_Edge(uint16_t node_a, uint16_t node_b, uint8_t _lqi) :
    node_1(BAD_SHORTADDR),
    node_2(BAD_SHORTADDR),
    lqi(_lqi)
    // edge_type(_type)
    {
      setEdges(node_a, node_b);
    }

  void setEdges(uint16_t node_a, uint16_t node_b);

  bool sameEdge(const Z_Mapper_Edge & edge2) const;

  // Edge_Type Z_Mapper_Edge_Type_Reverse(Edge_Type _type) {
  //   switch (_type) {
  //     case Parent:  return Child;
  //     case Child:   return Parent;
  //     default: return _type;
  //   }
  // }

  // we always orientate the edge from with shortaddresses in ascending order
  // invariant: node_1 < node_2
  uint16_t    node_1;
  uint16_t    node_2;

  uint8_t     lqi;
  // Edge_Type   edge_type;
};

//
// Handles the mapping of Zigbee devices
//
class Z_Mapper {
public:
  Z_Mapper(void) :
    edges()
    {}

  void reset(void) { edges.reset(); zigbee_devices.clearDeviceRouterInfo(); }

  Z_Mapper_Edge & findEdge(const Z_Mapper_Edge & edge2);
  bool addEdge(const Z_Mapper_Edge & edge2);

  void dumpInternals(void) const;

  LList<Z_Mapper_Edge>    edges;

};

// global
Z_Mapper zigbee_mapper;

/*********************************************************************************************\
 * Implementation for Z_Mapper_Edge
\*********************************************************************************************/
void Z_Mapper_Edge::setEdges(uint16_t node_a, uint16_t node_b) {
  if (node_a < node_b) {
    node_1 = node_a;
    node_2 = node_b;
  } else if (node_a > node_b) {
    node_1 = node_b;
    node_2 = node_a;
  } else {
    // do nothing
  }
}

bool Z_Mapper_Edge::sameEdge(const Z_Mapper_Edge & edge2) const {
  return (node_1 == edge2.node_1) && (node_2 == edge2.node_2);
}

/*********************************************************************************************\
 * Implementation for Z_Mapper
\*********************************************************************************************/
Z_Mapper_Edge & Z_Mapper::findEdge(const Z_Mapper_Edge & edge2) {
  if ((edge2.node_1 == BAD_SHORTADDR) || (edge2.node_2 == BAD_SHORTADDR)) { return *(Z_Mapper_Edge*)nullptr; }
  for (auto & edge : edges) {
    if (edge2.sameEdge(edge)) {
      return edge;
    }
  }
  return *(Z_Mapper_Edge*)nullptr;
}

bool Z_Mapper::addEdge(const Z_Mapper_Edge & edge2) {
  if ((edge2.node_1 == BAD_SHORTADDR) || (edge2.node_2 == BAD_SHORTADDR)) { return false; }
  Z_Mapper_Edge & cur_edge = findEdge(edge2);
  if (&cur_edge == nullptr) {
    edges.addHead(edge2);
  } else {
    //upgrade fields
    if (edge2.lqi > cur_edge.lqi) {
      cur_edge.lqi = edge2.lqi;
    }
    // if (cur_edge.edge_type == Z_Mapper_Edge::Unknown) {
    //   cur_edge.edge_type = edge2.edge_type;
    // } else if ((edge2.edge_type == Z_Mapper_Edge::Parent) || (edge2.edge_type == Z_Mapper_Edge::Child)) {
    //   // Parent or Child always has priority over Sibling
    //   cur_edge.edge_type = edge2.edge_type;
    // }
  }
  return true;
}

String EscapeHTMLString(const char *s_P) {
  String s((const __FlashStringHelper*) s_P);
  s.replace(F("&"), F("&amp;"));
  s.replace(F("\""), F("&quot;"));
  s.replace(F("<"), F("&lt;"));
  s.replace(F(">"), F("&gt;"));
  return s;
}

void Z_Mapper::dumpInternals(void) const {
  WSContentSend_P(PSTR("nodes:[" "{id:\"0x0000\",label:\"Coordinator\",group:\"o\",title:\"0x0000\"}"));
  for (const auto & device : zigbee_devices.getDevices()) {
    WSContentSend_P(PSTR(",{id:\"0x%04X\",group:\"%c\",title:\"0x%04X\",label:\""),
                    device.shortaddr, device.isRouter() ? 'r' : 'e', device.shortaddr);

    const char *fname = device.friendlyName;
    if (fname != nullptr) {
      WSContentSendRaw_P( EscapeJSONString(fname).c_str());
    } else {
      WSContentSend_P(PSTR("0x%04X"), device.shortaddr);
    }
    WSContentSend_P("\"}");
  }
  WSContentSend_P(PSTR("],"));

  WSContentSend_P(PSTR("edges:["));
  for (auto & edge : edges) {
    uint32_t lqi_color = 0x000;
    // if (edge.lqi >= 192) {
    //   lqi_color = 0x364;
    // } else if (edge.lqi >= 128) {
    //   lqi_color = 0x346;
    // } else if (edge.lqi > 0) {
    //   lqi_color = 0xd56;
    // }
    char hex[8];
    snprintf(hex, sizeof(hex), PSTR("%d"), edge.lqi);

    WSContentSend_P(PSTR("{from:\"0x%04X\",to:\"0x%04X\",label:\"%s\",color:\"#%03X\"},"),
                edge.node_1, edge.node_2, (edge.lqi > 0) ? hex : "", lqi_color);
  }
  WSContentSend_P(PSTR("],"));
}

#endif // USE_ZIGBEE
