#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>

struct Problem {
    int problem_id;
    std::string title;
    std::string description;
    std::string difficulty;
    std::string constraints;
};

#endif