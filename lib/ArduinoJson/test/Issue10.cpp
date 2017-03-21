// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

struct Person {
  int id;
  char name[32];
};

class Issue10 : public testing::Test {
 protected:
  virtual void SetUp() {
    Person boss;
    boss.id = 1;
    strcpy(boss.name, "Jeff");
    Person employee;
    employee.id = 2;
    strcpy(employee.name, "John");
    persons[0] = boss;
    persons[1] = employee;
  }

  template <typename T>
  void checkJsonString(const T &p) {
    char buffer[256];
    p.printTo(buffer, sizeof(buffer));

    EXPECT_STREQ("[{\"id\":1,\"name\":\"Jeff\"},{\"id\":2,\"name\":\"John\"}]",
                 buffer);
  }

  StaticJsonBuffer<JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(2)> json;
  Person persons[2];
};

TEST_F(Issue10, PopulateArrayByAddingAnObject) {
  JsonArray &array = json.createArray();

  for (int i = 0; i < 2; i++) {
    JsonObject &object = json.createObject();

    object["id"] = persons[i].id;
    object["name"] = persons[i].name;

    array.add(object);
  }

  checkJsonString(array);
}

TEST_F(Issue10, PopulateArrayByCreatingNestedObjects) {
  JsonArray &array = json.createArray();

  for (int i = 0; i < 2; i++) {
    JsonObject &object = array.createNestedObject();

    object["id"] = persons[i].id;
    object["name"] = persons[i].name;
  }

  checkJsonString(array);
}
