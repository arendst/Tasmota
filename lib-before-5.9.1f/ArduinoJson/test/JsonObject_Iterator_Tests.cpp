// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonObject_Iterator_Test : public testing::Test {
 public:
  JsonObject_Iterator_Test() : _object(_buffer.createObject()) {
    _object["ab"] = 12;
    _object["cd"] = 34;
  }

 protected:
  StaticJsonBuffer<JSON_OBJECT_SIZE(2)> _buffer;
  JsonObject& _object;
};

TEST_F(JsonObject_Iterator_Test, NonConstIterator) {
  JsonObject::iterator it = _object.begin();
  ASSERT_NE(_object.end(), it);
  EXPECT_STREQ("ab", it->key);
  EXPECT_EQ(12, it->value);
  it->key = "a.b";
  it->value = 1.2;
  ++it;
  ASSERT_NE(_object.end(), it);
  EXPECT_STREQ("cd", it->key);
  EXPECT_EQ(34, it->value);
  it->key = "c.d";
  it->value = 3.4;
  ++it;
  ASSERT_EQ(_object.end(), it);

  ASSERT_EQ(2, _object.size());
  EXPECT_EQ(1.2, _object["a.b"]);
  EXPECT_EQ(3.4, _object["c.d"]);
}

TEST_F(JsonObject_Iterator_Test, ConstIterator) {
  const JsonObject& const_object = _object;
  JsonObject::const_iterator it = const_object.begin();

  ASSERT_NE(const_object.end(), it);
  EXPECT_STREQ("ab", it->key);
  EXPECT_EQ(12, it->value);
  ++it;
  ASSERT_NE(const_object.end(), it);
  EXPECT_STREQ("cd", it->key);
  EXPECT_EQ(34, it->value);
  ++it;
  ASSERT_EQ(const_object.end(), it);
}
