#include "regex_functions.h"

using namespace std;

regex_functions::regex_functions()
{

}

string regex_functions::getSingleRegexValue(regex exp, string s, smatch m){
    string conv;
    while(regex_search(s, m, exp)){
        conv = m[1];
    s = m.suffix();
        }
    return conv;
}

bool regex_functions::checkDayCheckBox(regex exp, string s, smatch m){

    if(regex_search(s, m, exp)){
        return true;
    }
    else{
        return false;
    }
}
