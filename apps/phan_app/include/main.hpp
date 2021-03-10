#pragma once
#define DEBUGGING
#include <const_str/const_str.hpp>



#define PROCC2(str, max, var) var = []()->string\
{\
return str;\
}();// = [&s]->string{return "";}();

#define PROCC(x) PROCC2 (BOOST_PP_STRINGIZE (BOOST_PP_SEQ_ELEM (0, x)), BOOST_PP_SEQ_ELEM (1, x), BOOST_PP_SEQ_ELEM (2, x))//BOOST_PP_SEQ_ELEM (2, x) = [](string s){ \

using namespace placeholders;
using namespace std;

string readFileIntoString(ifstream& input_file) {
      return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}






void remove_all_chars (string& str, char c)
{
    std::string::iterator end_pos = std::remove(str.begin(), str.end(), c);
    str.erase(end_pos, str.end());
}

void remove_beginning_chars (string& str, char c)
{
    while (str.front() == c) {
        str.erase (str.begin(), str.begin() + 1);
    }
}
