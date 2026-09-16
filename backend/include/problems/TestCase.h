#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <string>

struct TestCase {
    int testCaseId;
    std::string input;
    std::string expectedOutput;
    bool isSample;
};

#endif
