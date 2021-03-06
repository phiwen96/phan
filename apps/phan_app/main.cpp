#include <iostream>
#include <vector>
#include <tuple>
#include <regex>
#include <string>

using namespace std;


//#include <boost/preprocessor/
#define STR(...) BOOST_


auto parse = [](string str){
    str = regex_replace (str, regex ("(.*)(\\[)"), R"($1)");
    str = regex_replace (str, regex ("(.*)(\\])"), R"($1)");
    str = regex_replace (str, regex ("(.*)(\\.)"), R"()");
    str = regex_replace (str, regex ("([a-z])([A-Z])"), R"($1_$2)");
    str = regex_replace (str, regex ("([[:digit:]]+)"), R"(_$1)");
    for (auto & c: str) c = toupper(c);str.insert (0, "_");
    return str;
};



#define PH(...) BOOST_PP_STRINGIZE (__VA_ARGS__)



string first_signature (string str, auto&& fun)
{
    auto a1 = str.find("${");
    
    while (a1 != string::npos)
    {
        if (int a2 = str.find ("}"); a2 != string::npos)
        {
//            for (int i = 0; i < max; ++i)
//            {
//
            string replac = fun (string (str.begin() + a1, str.begin() + a2 + 1));
                str.replace (str.begin() + a1, str.begin() + a2 + 1, replac);
//            }
        }
        a1 = str.find ("${");
    }
    
//    string res;
    
//    for (auto const& i : str)
//        res += i;
    
    return str;
}

int main() {
    
    
    
    
    
    string str = PH(
               template <>
               struct gpdu <${0 i 3}>
               {
                int count fp${i}
            };
               
               );
    cout << str << endl;
    
    auto first_parser = [](string const& str) -> string {
        // cannot return same!!!!
        
        cout << str << endl;
        return str;
    };
    
   
    cout << first_signature (str, first_parser) << endl;
    
    
    
    
    std::vector<double> input = {1.2, 2.3, 3.4, 4.5};
    
    cout << "hello world" << endl;
    return 0;
}


