#include "BDDTest.h"
#include "trace.h"
#include <sstream>
#include <iostream>
#include <string>
#include <list>

int testCount = 0;
int testPasses = 0;
const char* testDescription;

std::list<std::string> failureList;

void bddtest_suite(const char* name) {
    LOG(name << "\n");
}

int bddtest_test(const char* file, int line, const char* assertion, int result) {
    if (!result) {
        LOG("✗\n");
        std::ostringstream os;
        os << "   ! "<<testDescription<<"\n      " <<file << ":" <<line<<" : "<<assertion<<" ["<<result<<"]";
        failureList.push_back(os.str());
    }
    return result;
}

void bddtest_start(const char* description) {
    LOG(" - "<<description<<" ");
    testDescription = description;
    testCount ++;
}
void bddtest_end() {
    LOG("✓\n");
    testPasses ++;
}

int bddtest_summary() {
    for (std::list<std::string>::iterator it = failureList.begin(); it != failureList.end(); it++) {
        LOG("\n");
        LOG(*it);
        LOG("\n");
    }

    LOG(std::dec << testPasses << "/" << testCount << " tests passed\n\n");
    if (testPasses == testCount) {
        return 0;
    }
    return 1;
}
