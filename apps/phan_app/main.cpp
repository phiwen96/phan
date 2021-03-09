#include <iostream>
#include <vector>
#include <tuple>
#include <regex>
#include <string>
#include <boost/preprocessor/seq/for_each.hpp>
#include <optional>
#define DEBUGGING


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

//bool is_number (string const& s)
//{
//      return regex_match (s, regex ("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
//}

template <class T0, class T1, class T2>
struct trio
{
    T0 t0;
    T1 t1;
    T2 t2;
    
    auto first () const -> T0 const& {
        return t0;
    }
    
    auto second () const -> T1 const& {
        return t1;
    }
    
    auto third () const -> T2 const& {
        return t2;
    }
    
    auto first () -> T0& {
        return t0;
    }
    
    auto second () -> T1& {
        return t1;
    }
    
    auto third () -> T2& {
        return t2;
    }
};

template <class T0, class T1, class T2>
trio (T0, T1, T2) -> trio <T0, T1, T2>;






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

struct extractor;

struct extractor_state
{
    
    extractor* m_ext;
//    int m_curr;
    
    
    virtual bool found (char){}
    
};


struct extractor_state_begin : extractor_state
{
//    using extractor_state::extractor_state;
    virtual bool found (char c) override;
};

struct extractor_state_first : extractor_state
{
//    using extractor_state::extractor_state;
    virtual bool found (char c) override;
};

struct extractor_state_second : extractor_state
{
//    using extractor_state::extractor_state;
    virtual bool found (char c) override;
};


struct extractor
{
    string m_first;
    string m_second;
    int m_curr;
    int m_watched;
    int m_first_begin_found;
    int m_first_end_found;
    int m_second_found;
    
    auto found (char c) -> optional <trio <int, int, int>>
    {
        if (m_state -> found (c))
            return {trio {m_first_begin_found, m_first_end_found, m_second_found}};
        else
            return {};
    }
    
    extractor_state* m_state;
    
    extractor (string const& first, string const& second) : m_first (first), m_second (second), m_state (new extractor_state_begin), m_curr (0), m_watched (0), m_first_begin_found (-1), m_first_end_found (-1), m_second_found (-1)
    {
        m_state -> m_ext = this;
    }
    
    void change_state (extractor_state* newstate)
    {
        std::cout << "Context: Transition from " << typeid(*m_state).name() << " to "  << typeid(*newstate).name() << "\n";
        delete m_state;
        newstate -> m_ext = this;
        m_state = newstate;
    }
};

bool extractor_state_begin::found (char c)
{
    if (m_ext -> m_first [0] == c) {
        m_ext -> m_first_begin_found = m_ext -> m_watched++;
//        cout << "extractor_state_first::found(" << c << ") == true" << endl;
        ++m_ext -> m_curr;
        
        if (m_ext -> m_curr == m_ext -> m_first.size()) {
            m_ext -> m_first_end_found = m_ext -> m_watched;
            m_ext -> m_curr = 0;
            m_ext -> change_state (new extractor_state_second);
        } else
        {
            m_ext -> change_state (new extractor_state_first);
        }
        
//        ++m_ext -> m_watched;
    }
    else {
        ++m_ext -> m_watched;
//        cout << "extractor_state_first::found(" << c << ") == false" << endl;

    }
    
    
    return false;
}

bool extractor_state_first::found (char c)
{
    if (m_ext -> m_first [m_ext -> m_curr] == c) {
        m_ext -> m_first_end_found = ++m_ext -> m_watched;
//        cout << "extractor_state_first::found(" << c << ") == true" << endl;
        ++m_ext -> m_curr;
        if (m_ext -> m_curr == m_ext -> m_first.size()) {
            m_ext -> m_curr = 0;
            
            m_ext -> change_state (new extractor_state_second);
        }

        
    }
    else {
//        cout << "extractor_state_first::found(" << c << ") == false" << endl;
        ++m_ext -> m_watched;
    }
    return false;
}

bool extractor_state_second::found (char c)
{
    if (m_ext -> m_second [m_ext -> m_curr] == c) {
        m_ext -> m_second = m_ext -> m_watched;
//        cout << "extractor_state_second::parse(" << c << ") == true" << endl;

        ++m_ext -> m_curr;
        ++m_ext -> m_watched;
        if (m_ext -> m_curr == m_ext -> m_second.size()) {
            m_ext -> m_second_found = m_ext -> m_watched;
            return true;
        }
        
    }
    else
    {
//        cout << "extractor_state_second::parse(" << c << ") != " << m_ext -> m_second [m_ext -> m_curr] << endl;
        ++m_ext -> m_watched;
    }
    return false;
}






string find_between (string const& str, vector<pair<string, string>> const& finders)
{
    
//    auto a1 = str.find(first);
//
//    while (a1 != string::npos)
//    {
//        if (int a2 = str.find (second); a2 != string::npos)
//        {
//
//            string replac = fun (string (str.begin() + a1, str.begin() + a2 + 1));
//                str.replace (str.begin() + a1, str.begin() + a2 + 1, replac);
//        }
//        else
//        {
//            break;
//        }
//        a1 = str.find ("${");
//    }
}

#define PH(...) BOOST_PP_STRINGIZE (__VA_ARGS__)

#include <const_str/const_str.hpp>


constexpr char const* a1 = "hej";








string first_signature (string const& first, string const& second, string str, auto&& fun)
{
    auto a1 = str.find(first);
    
    while (a1 != string::npos)
    {
        if (int a2 = str.find (second); a2 != string::npos)
        {

            string replac = fun (string (str.begin() + a1, str.begin() + a2 + 1));
                str.replace (str.begin() + a1, str.begin() + a2 + 1, replac);
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

using namespace placeholders;

auto main(int argc,  char** argv) -> int
{
    
#ifdef DEBUGGING
    ifstream infile;
    infile.open ("/Users/philipwenkel/GitHub/phan/tests/test_phan_app/testFiles_pre/test0.hpp");
    
    ofstream outfile;
    outfile.open ("/Users/philipwenkel/GitHub/phan/tests/test_phan_app/testFiles_post/test0.hpp");
    
#else
    ifstream infile;
    infile.open (argv[1]);
    
    ofstream outfile;
    outfile.open (argv[2]);
#endif
    
    auto remove_beginning_spaces = bind (remove_beginning_chars, _1, ' ');
    auto remove_beginning_newlines = bind (remove_beginning_chars, _1, '\n');
    
    
//    cout << str << endl;
    
    auto first_parser = [&](string str) -> string {
        
        // cannot return same!!!!
        str.erase(str.begin(), str.begin() + 2);
        str.pop_back();
        
//        remove_beginning_newlines (str);
    
        
        if (str[0] >= '0' and str[0] <= '9') {
            str += " is number";
        } else
        {
            str += " is var";
        }
//        str.pop_front();
//        str.erase(1);
//        str.pop_back();
        
//        str = "kuk";
        
//        cout << str << endl;
        return str;
    };
    
   
    
    string outtext = readFileIntoString(infile);
    extractor ext1 {"${", "}"};
    
//    bool found = false;
    for (auto i : outtext) {
        auto found = ext1.found(i);
        if (found) {
            auto [t0, t1, t2] = found.value();
            cout << "yaay" << endl;
            cout << t0 << " : " << t1 << " : " << t2 << endl;
            break;
        }
    }
//    if (found) {
//        outfile << "YES\n";
//    }
//    else {
//        outfile << "NO\n";
//    }
//    outtext = first_signature ("${", "}", outtext, first_parser);
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


