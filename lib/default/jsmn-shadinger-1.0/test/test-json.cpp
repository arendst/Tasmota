#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../src/JsonParser.h"


static char test_simple[] = "{\"Device\":\"0x9C33\",\"Illuminance\":42,\"Occupancy\":1,\"Endpoint\":1,\"LinkQuality\":59}";
static char test_moderate[] = "{\"ZbReceived\":{\"Prez\":{\"Device\":\"0x9C33\",\"Illuminance\":42,\"Occupancy\":1,\"Endpoint\":1,\"LinkQuality\":59}}}";
static char test_complex[] = "{\"ZbStatus3\":[{\"Device\":\"0x7869\",\"INT\":-3,\"Name\":\"Tilt\",\"IEEEAddr\":\"0x00158D00031310F4\",\"ModelId\":\"lumi.vibration.aq1\",\"Manufacturer\":\"LUMI\",\"Endpoints\":{\"0x01\":{\"ProfileId\":\"0x0104\",\"ClustersIn\":[\"0x0000\",\"0x0003\",\"0x0019\",\"0x0101\"],\"ClustersOut\":[\"0x0000\",\"0x0004\",\"0x0003\",\"0x0005\",\"0x0019\",\"0x0101\"]},\"0x02\":{\"ProfileId\":\"0x0000\\ta\",\"ClustersIn\":[2],\"ClustersOut\":[-3,0.4,5.8]}}}]}";

int main(int argc, char* argv[]) {
  printf("Starting... sizeof = %lu / %lu\n", sizeof(jsmntok_t), sizeof(JsonParserToken));

  // char * json_str = test_complex;
  char * json_str = test_simple;

  // JsonParser parser(64);      // size for 64 tokens

  int r = parser.parse(json_str);

  printf("r = %d\n", r);

  for (uint32_t i=0; i<r; i++) {
    JsonParserToken token = parser[i];
    uint32_t start = token.t->start;
    uint32_t len = token.t->len;
    printf("Tok[%2d]= type=%s, start=%d, len=%d, size=%d, str ='%s'\n", i, JSMNTypeName(token.t->type), start, len, token.t->size, (token.t->type >= JSMN_STRING || 1) ? &json_str[start] : "");
  }
  printf("==================\n");
  JsonParserObject root = parser.getRootObject();
  
  for (const auto key : root) {
    // printf("Index = %ld\n", parser.index(key));
    JsonParserToken value = key.getValue();
    printf("Key = %s, Val type = %s\n", parser.getStr(key), JSMNTypeName(value.t->type));
    if (value.isArray()) {
      for (const auto arr_val : JsonParserArray(value)) {
        printf("Array = %s, type = %s\n", parser.getStr(arr_val), JSMNTypeName(arr_val.t->type));
      }
    } else {
      printf("Value = %s\n", parser.getStr(value));
    }
  }

  // root.nextOne();
  // printf("Index = %ld\n", parser.index(root));
  // root.skipObject();
  // printf("Index = %ld\n", parser.index(root));

  JsonParserToken oc = parser.GetCaseInsensitive(root, "occupancy");
  printf("Looking for 'Occupancy': %s, %d\n", parser.getStr(oc), parser.getInt(oc));
  JsonParserToken oc2 = parser.GetCaseInsensitive(root, "occupanc");
  printf("Looking for 'Occupanc': %s, %d\n", parser.getStr(oc2), parser.getInt(oc2));
}
