#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <string>

struct TestCase{
    int testCaseId;
    std::string input , expectedOutput;
    bool isSample;
};

#endif
