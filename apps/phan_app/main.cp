#include <iostream>
#include <vector>
#include <tuple>
#include <regex>
#include <string>
#include <boost/preprocessor/seq/for_each.hpp>


using namespace std;


#define PROCC2(str, max, var) var = []()->string\
{\
return str;\
}();// = [&s]->string{return "";}();

#define PROCC(x) PROCC2 (BOOST_PP_STRINGIZE (BOOST_PP_SEQ_ELEM (0, x)), BOOST_PP_SEQ_ELEM (1, x), BOOST_PP_SEQ_ELEM (2, x))//BOOST_PP_SEQ_ELEM (2, x) = [](string s){ \



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

template <char const*>


struct parse
{
    
};


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
        else
        {
            break;
        }
        a1 = str.find ("${");
    }
    
//    string res;
    
//    for (auto const& i : str)
//        res += i;
    
    return str;
}

string readFileIntoString(ifstream& input_file) {
      
//      if (!input_file.is_open()) {
//            cerr << "Could not open the file - '"
//            << path << "'" << endl;
//            exit(EXIT_FAILURE);
//      }
      return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}



auto main(int argc,  char** argv) -> int
{
    ifstream infile;
    infile.open (argv[1]);
    
    
    
    ofstream outfile;
    outfile.open (argv[2]);
    
    
    
    
    
    string str = PH(
               template <>
               struct gpdu <${0 i 3}>
               {
                int count fp${i}
            };
               
               );
//    cout << str << endl;
    
    auto first_parser = [](string str) -> string {
        
        // cannot return same!!!!
        str.erase(str.begin(), str.begin() + 2);
//        str.pop_front();
//        str.erase(1);
//        str.pop_back();
        str.pop_back();
//        str = "kuk";
        
//        cout << str << endl;
        return str;
    };
    
    string outtext = readFileIntoString(infile);
    outtext = first_signature (outtext, first_parser);
    outfile << outtext;

    
   
//    cout << first_signature (str, first_parser) << endl;
    
    

    PROCC ((
        template <int>
        struct gpu;
        
        $(1)
        {
a       # = comment # ?(name)?{explanation}
        
        € = _function (_anonymous/_non-anonymous) # ?[?_scope_var = @(_var)?]?(0 ?i? 0)?{}??(_function_name)?
        $ = _variable (_non-anonymous){_code} # (_name){}
            @ = _paste
            
            template <>
            struct gpu <${0 i 10}> # {_public change all in scope}
            {
    
                $(0 i 4) # {everyting refering to i will clone for times}
//                # int i${i} -> int i0123
@(i)<1 10>
@(i)<1 10> -> {@} 12345678910
@<0 2> -> {ph}  phph
  
                

                     2
@(hej) <-> ${0#}     02  02
                
                    -1
@(hej#) <-> ${0#}    -10  0-1
                
                     2
@(#hej) <-> ${0#}     022  02

                          2
${0#} -> @(#hej)     002  02

kuk @<1 3> &{ph&}               kuk ph1ph2ph3
fitta @<1 3> -> {ph&} kuk       fitta ph123 kuk
@<1 3> -> &{ph}                 phphph
    

                
                

                
                
@(hej) <-> {0}      0 0
@(hej)              ()  0
@(hej) <- {1}       1   ()
@(hej) -> {2@}      ()  21
@(hej)              1   ()
@(hej) <-> {3@}     3   31
                
@(hej@) <-> {kuk}   "snopphejkuk"
                
@(hej) {philip@} philipkuk

                
  @{i} -> {int i} kommer bli int i int i int i int i
  @{i} -> {int@ i@}  int0 i0 int1 i1 int2 i2 int3 i3
 ${i}    0123
                
                $(str){philip}    "philip"
                ${str} -> {int i@} -> {kuk @ hora}   "kuk int iphilip hora"
$(hej){kukens fitta}   kukens fitta
${hej}       kukens fitta
        
                int i${0 10} = 0; #{_private}
                int j€(0 5)(k) = 3; #{public}
                
                €[j = @(i)](0 i 3){int i@(i + 1) = @(j);}(myFun)
                
//                €[j = @(i)](0 i 3){int i@(i) = @(j);} anonymous
//            0 i 3 prio före 0 i 10 -> därför sätter vi om namnet till j
//            0 i 3 is internal dvs endast inne i {} och i refererar inget utanför
                $(_stat_int){static $(con){constexpr} int} fitta = GPU_COUNT;
            ERROR-> $(_stat){kiss}
                @(_stat_int) count = GPU_COUNT;
                
                 ${static} constexpr uint32_t max_image_dimension_1D = GPU_${i}_MAX_IMAGE_DIMENSION_1D;
                 @(st) int i = 3;
            };
        }
           )(0)(string s));
    
    cout << s << endl;

//    PROCC ((
//        template <int>
//        struct gpu;
//
//        $(1)
//        {
//            € = inline
//            $ = declare variable, only visible to the current scope
//                and to those below
//
//            template <>
//            struct gpu <$(0 i 10)>
//            {
//                int $(0 j 10) = 0;
//
//                €[j = @(i)](0 i 3){int i@(i) = @(j);}(myFun)
//            0 i 3 prio före 0 i 10 -> därför sätter vi om namnet till j
//            0 i 3 is internal dvs endast inne i {} och i refererar inget utanför
//                $(stat){static $(con){constexpr} int} fitta = GPU_COUNT;
//            ERROR-> $(stat){kiss}
//                 @(stat) count = GPU_COUNT;
//                 ${static} constexpr uint32_t max_image_dimension_1D = GPU_${i}_MAX_IMAGE_DIMENSION_1D;
//                 @(st) int i = 3;
//            };
//        }
//           )(0)(string s));
    
//    cout << s << endl;
        
        
        
        
    
    std::vector<double> input = {1.2, 2.3, 3.4, 4.5};
    
    cout << "hello world" << endl;
    return 0;
}


