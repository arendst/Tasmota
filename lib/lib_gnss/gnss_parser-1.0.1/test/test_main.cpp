#include <unity.h>
#include "test_nmea.h"
#include "test_dumps.h"

void process()
{
    UNITY_BEGIN();

    // Register all test suites
    register_nmea_tests();
    register_dump_tests();

    UNITY_END();
}

#ifdef ARDUINO
void setup()
{
    delay(2000);
    process();
}

void loop()
{
    delay(1000);
}
#else

int main(int argc, char **argv)
{
    process();
    return 0;
}

#endif