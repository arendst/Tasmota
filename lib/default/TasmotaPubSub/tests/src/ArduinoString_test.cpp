#include <string>

#include "doctest.h"
#include "ArduinoString.h"
#include <LList.h>

TEST_CASE("host Arduino String grows without truncating assigned values") {
    const std::string fixture(4096, 'x');
    String value(fixture.c_str());

    CHECK(value.length() == fixture.length());
    CHECK(std::string(value.c_str()) == fixture);
    CHECK(value == fixture.c_str());
    CHECK(fixture.c_str() == value);
}

TEST_CASE("host Arduino String copies, assigns, compares, and clears") {
    String original("complete value");
    String copied(original);
    String assigned;
    assigned = original;

    original.clear();
    CHECK(original.isEmpty());
    CHECK(original.length() == 0);
    CHECK(original == "");
    CHECK(copied == "complete value");
    CHECK(assigned == copied);

    assigned = static_cast<const char*>(nullptr);
    CHECK(assigned.isEmpty());
    CHECK(std::string(assigned.c_str()).empty());
}

TEST_CASE("host harness resolves the real Tasmota LList header") {
    LList<int> values;
    values.addToLast() = 7;
    values.addToLast() = 11;

    REQUIRE(values.length() == 2);
    CHECK(*values.at(0) == 7);
    CHECK(*values.at(1) == 11);
}
