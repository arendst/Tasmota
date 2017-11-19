// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class GbathreeBug : public testing::Test {
 public:
  GbathreeBug() : _object(_buffer.parseObject(getJson())) {}

 protected:
  char _json[1024];
  DynamicJsonBuffer _buffer;
  const JsonObject& _object;

 private:
  char* getJson() {
    strcpy(_json,
           "{\"protocol_name\":\"fluorescence\",\"repeats\":1,\"wait\":0,"
           "\"averages\":1,\"measurements\":3,\"meas2_light\":15,\"meas1_"
           "baseline\":0,\"act_light\":20,\"pulsesize\":25,\"pulsedistance\":"
           "10000,\"actintensity1\":50,\"actintensity2\":255,\"measintensity\":"
           "255,\"calintensity\":255,\"pulses\":[50,50,50],\"act\":[2,1,2,2],"
           "\"red\":[2,2,2,2],\"detectors\":[[34,34,34,34],[34,34,34,34],[34,"
           "34,34,34],[34,34,34,34]],\"alta\":[2,2,2,2],\"altb\":[2,2,2,2],"
           "\"measlights\":[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,"
           "15,15]],\"measlights2\":[[15,15,15,15],[15,15,15,15],[15,15,15,15],"
           "[15,15,15,15]],\"altc\":[2,2,2,2],\"altd\":[2,2,2,2]}");
    return _json;
  }
};

TEST_F(GbathreeBug, Success) {
  EXPECT_TRUE(_object.success());
}

TEST_F(GbathreeBug, ProtocolName) {
  EXPECT_STREQ("fluorescence", _object["protocol_name"]);
}

TEST_F(GbathreeBug, Repeats) {
  EXPECT_EQ(1, _object["repeats"]);
}

TEST_F(GbathreeBug, Wait) {
  EXPECT_EQ(0, _object["wait"]);
}

TEST_F(GbathreeBug, Measurements) {
  EXPECT_EQ(3, _object["measurements"]);
}

TEST_F(GbathreeBug, Meas2_Light) {
  EXPECT_EQ(15, _object["meas2_light"]);
}

TEST_F(GbathreeBug, Meas1_Baseline) {
  EXPECT_EQ(0, _object["meas1_baseline"]);
}

TEST_F(GbathreeBug, Act_Light) {
  EXPECT_EQ(20, _object["act_light"]);
}

TEST_F(GbathreeBug, Pulsesize) {
  EXPECT_EQ(25, _object["pulsesize"]);
}

TEST_F(GbathreeBug, Pulsedistance) {
  EXPECT_EQ(10000, _object["pulsedistance"]);
}

TEST_F(GbathreeBug, Actintensity1) {
  EXPECT_EQ(50, _object["actintensity1"]);
}

TEST_F(GbathreeBug, Actintensity2) {
  EXPECT_EQ(255, _object["actintensity2"]);
}

TEST_F(GbathreeBug, Measintensity) {
  EXPECT_EQ(255, _object["measintensity"]);
}

TEST_F(GbathreeBug, Calintensity) {
  EXPECT_EQ(255, _object["calintensity"]);
}

TEST_F(GbathreeBug, Pulses) {
  // "pulses":[50,50,50]

  JsonArray& array = _object["pulses"];
  EXPECT_TRUE(array.success());

  EXPECT_EQ(3, array.size());

  for (int i = 0; i < 3; i++) {
    EXPECT_EQ(50, array[i]);
  }
}

TEST_F(GbathreeBug, Act) {
  // "act":[2,1,2,2]

  JsonArray& array = _object["act"];
  EXPECT_TRUE(array.success());

  EXPECT_EQ(4, array.size());
  EXPECT_EQ(2, array[0]);
  EXPECT_EQ(1, array[1]);
  EXPECT_EQ(2, array[2]);
  EXPECT_EQ(2, array[3]);
}

TEST_F(GbathreeBug, Detectors) {
  // "detectors":[[34,34,34,34],[34,34,34,34],[34,34,34,34],[34,34,34,34]]

  JsonArray& array = _object["detectors"];
  EXPECT_TRUE(array.success());
  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    JsonArray& nestedArray = array[i];
    EXPECT_EQ(4, nestedArray.size());

    for (int j = 0; j < 4; j++) EXPECT_EQ(34, nestedArray[j]);
  }
}

TEST_F(GbathreeBug, Alta) {
  // alta:[2,2,2,2]

  JsonArray& array = _object["alta"];
  EXPECT_TRUE(array.success());

  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(2, array[i]);
  }
}

TEST_F(GbathreeBug, Altb) {
  // altb:[2,2,2,2]

  JsonArray& array = _object["altb"];
  EXPECT_TRUE(array.success());

  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(2, array[i]);
  }
}

TEST_F(GbathreeBug, Measlights) {
  // "measlights":[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,15,15]]

  JsonArray& array = _object["measlights"];
  EXPECT_TRUE(array.success());
  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    JsonArray& nestedArray = array[i];

    EXPECT_EQ(4, nestedArray.size());

    for (int j = 0; j < 4; j++) EXPECT_EQ(15, nestedArray[j]);
  }
}

TEST_F(GbathreeBug, Measlights2) {
  // "measlights2":[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,15,15]]

  JsonArray& array = _object["measlights2"];
  EXPECT_TRUE(array.success());
  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    JsonArray& nestedArray = array[i];
    EXPECT_EQ(4, nestedArray.size());

    for (int j = 0; j < 4; j++) EXPECT_EQ(15, nestedArray[j]);
  }
}

TEST_F(GbathreeBug, Altc) {
  // altc:[2,2,2,2]

  JsonArray& array = _object["altc"];
  EXPECT_TRUE(array.success());

  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(2, array[i]);
  }
}

TEST_F(GbathreeBug, Altd) {
  // altd:[2,2,2,2]

  JsonArray& array = _object["altd"];
  EXPECT_TRUE(array.success());

  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(2, array[i]);
  }
}
