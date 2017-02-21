// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

TEST(Issue214, IsBool) {
  char json[] = "{\"ota\": {\"enabled\": true}}";
  StaticJsonBuffer<JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(1)> jsonBuffer;
  JsonObject& parsedJson = jsonBuffer.parseObject(json);
  ASSERT_TRUE(parsedJson["ota"]["enabled"].is<bool>());
}
