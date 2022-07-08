#include "regex_functions.h"

using namespace std;

regex_functions::regex_functions()
{

}

void regex_functions::test(){
    cout << "t";
}

string regex_functions::getSingleRegexValue(regex exp, string s, smatch m){
    string conv;
    while(regex_search(s, m, exp)){
        conv = m[1];
    s = m.suffix();
        }
    return conv;
}
