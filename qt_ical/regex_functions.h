#ifndef REGEX_FUNCTIONS_H
#define REGEX_FUNCTIONS_H
#include <regex>
#include <iostream>


using namespace std;

class regex_functions
{
public:
    regex_functions();
    static void test();
    static string getSingleRegexValue(regex exp, string s, smatch m);
};

#endif // REGEX_FUNCTIONS_H
