#include "main.hpp"

#include "extractor.hpp"
#include "trio.hpp"
#include <functional>
#include <algorithm>
#include <vector>
#include <optional>





using iter = string::iterator;





namespace declare_var
{
struct Context;
struct State
{
    Context* context;
    
    void process (iter i);
    virtual void _process (iter i){}
    bool hasParent () const;
    string& variable ();
    string& value ();
    string& result ();
    string& potential ();
    void removeFromParent ();
    template <class T> void transition ();
    template <class T> Context& addChildContext ();
    State* parent ();
    void declare (string const& var, string const& val);
    virtual void addResultFromChild (string const& res);
    
    
};
struct Begin : State
{
    using State::State;
    void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};

struct Dollar : State
{
    using State::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};

struct LParan : State
{
    using State::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};

struct RParan : State
{
    using State::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};

struct LBracket : State
{
    using State::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};

struct Done : State
{
    using State::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

struct Repeat : State
{
    using State::State;
    virtual void _process (iter i);
    bool done () {return false;}

};






//Begin begin;
//Dollar Dollar;
//LParan LParan;
//RParan RParan;
//LBracket LBracket;
//Repeat repeat;
//Done done;



struct Context
{
    Context* parent;
    vector <pair <string, string>>& declaredVariables;
    State* state {nullptr};
    vector <Context*> children;
    stack <char> bracketStack;
    
    inline static string result = "";
    string variable;
    string value;
    string potential;
    
    void process (iter);
};


void State::declare (string const& var, string const& val) {
    auto declared = context -> declaredVariables.begin ();
    for (; declared < context -> declaredVariables.end (); ++declared) {
        if (declared -> first == var) {
            declared -> second = val;
            return;
        }
    }
    context -> declaredVariables.emplace_back (var, val);
}

State* State::parent () {
    return context -> parent -> state;
}

void Context::process(iter i) {
    state -> process (i);
}

void State::addResultFromChild (string const& res) {
    
}


void State::process (iter i) {
    if (context -> children.empty ()) {
//        context -> curr_it = i;
        _process (i);
    } else
    {
        for (auto& o : context->children)
            o -> process (i);
    }
}

bool State::hasParent () const {
    return context -> parent != nullptr;
}

void State::removeFromParent () {
    for (auto cont = context -> parent -> children.begin(); cont < context -> parent -> children.end(); ++cont) {
        if (*cont == context) {
//            cout << "removing child context from parent context" << endl;
            context -> parent -> children.erase (cont);
            break;
        }
    }
}


template <class T>
void State::transition () {
    
//    cout << "transitioning from " << typeid (*context -> state).name () << " to ";
    context -> state = new T;
    context -> state -> context = context;
//    cout << typeid (*context -> state).name () << endl;
}

template <class state>
Context& State::addChildContext () {
    State* childState = new state;
    Context* childContext = new Context {context, context->declaredVariables, childState};
    childState -> context = childContext;
    context -> children.push_back (childContext);
    return *childContext;
}
//optional <string> State::declared () {
//    for (auto d = context -> declaredVariables.begin (); d != context -> declaredVariables.end(); ++d) {
//        if (d -> first == context -> variable) {
//            return optional{d->second};
//        }
//    }
//    return {};
//}
string& State::variable () {
    return context -> variable;
}
string& State::value () {
    return context -> value;
}
string& State::result () {
    return context -> result;
}
string& State::potential () {
    return context -> potential;
}

void Begin::addResultFromChild (string const& res) {
    
}

void Dollar::addResultFromChild (string const& res) {
    
}

void LParan::addResultFromChild (string const& res) {
    potential () += res;
}

void RParan::addResultFromChild (string const& res) {
    
}

void LBracket::addResultFromChild (string const& res) {
    value () += res;
}




void Begin::_process (iter i) {
    
    if (*i == '$')
    {
        potential () += '$';
        transition <Dollar> ();
        
    } else
    {
        result () += *i;
    }
}
void Dollar::_process (iter i) {
    
    if (*i == '(')
    {
        potential () += '(';
        transition <LParan> ();
        
    } else
    {
        
        if (hasParent ())
        {
            parent () -> addResultFromChild (potential ());
            removeFromParent ();
            
        } else
        {
            result () += potential();
            result () += *i;
            potential().clear ();
            variable ().clear ();
            value ().clear ();
            transition <Begin> ();
        }
    }
}
void LParan::_process (iter i) {
    
    if (*i == ')')
    {
        variable() = string (potential().begin() + 2, potential().end());
        potential () += ')';
        transition <RParan> ();
        
    } else if (*i == '$')
    {
        addChildContext <Dollar> ().potential += '$';

    } else
    {
        potential () += *i;
    }
    
    
}
void Repeat::_process(iter i) {
//    cout << *i << endl;
    if (*i == '(')
    {
        addChildContext <LParan> ();
//        context -> transition (new LParan {context});
        
        transition <LParan> ();
//        context -> curr_it -= 2;
    }
//    else if (*i == ')')
//    {
//        context -> transition ( new RParan {context});
//
//    }
    else
    {
//        if (hasParent ())
//        {
//            chainParent (i);
//            context -> parent -> removeFromParent ();
//
//        } else
//        {
//            context -> transition (new LParan {context});
        context -> variable += *i;
        transition<LParan>();
//        }
    }
}
void RParan::_process (iter i) {
    
    
    if (*i == '{')
    {

        context -> bracketStack.push ('{');

        transition <LBracket> ();
        
    } else
    {
        
        if (hasParent ())
        {
            /**
            om parent
             **/
            parent () -> addResultFromChild (potential ());
            removeFromParent ();
        }
        else
        {
            result () += potential ();
            potential ().clear ();
            variable ().clear ();
            value ().clear ();
            transition<Begin>();
        }
        potential ().clear ();
        variable ().clear ();
    }
}

void LBracket::_process (iter i) {
    if (*i == '}')
    {
//        context -> bracketStack.pop ();
        
//        if (context -> bracketStack.empty ())
//        {
//        cout << value () << endl;
//        auto declared
        declare (variable (), value ());
            
            if (hasParent ())
            {
                parent () -> addResultFromChild (value ());
                removeFromParent ();
            }
            
//        }
            else
            {
                result () += value ();
                potential().clear();
                value().clear();
                variable().clear();
                transition <Begin> ();
    //            value() += *i;
            }
    }
        
//    }
//    else if (*i == '{')
//    {
//        context -> bracketStack.push ('{');
//        value() += *i;
//
//
    else if (*i == '$')
    {
        addChildContext <Dollar>().potential += '$';
    }
    else
    {
//        result () += *i;
        potential () += *i;
        value () += *i;
    }
}

void Done::_process (iter i) {
    
}

//  context -> transition (Dollar);
        
//        context -> process (i);
}



//$(sej$(aaa){AAA}$(bajs$(moa){kmkd}){tej$(haha){kmkm}}){kukens fitta}
//$(bajskmkd){tej$(haha){kmkd}}

namespace paste_var {

struct State;
struct Begin;
struct Dollar;
struct LBracket;

struct Context {
    State* state;
    vector <pair <string, string>>& declaredVariables;
    Context* parent {nullptr};
    inline static string res = "";
    string variable;
    iter begin_it;
    string potential;
    vector <Context*> children;
    
    void process (iter);
    
};

struct State {
    Context* context;
    
    void process (iter i);
    
protected:
    virtual void _process (iter i) {}
    
    template <class T> void transition ();
    optional <string> declared ();
    string& variable ();
    string& result ();
    string& potential ();
    void removeFromParent ();
    virtual void addResultFromChild (string const& res) {}
    
};

struct Begin : State {
    void _process (iter i);
};

struct Dollar : State {
    using State::context;
    void _process (iter i);
};

struct LBracket : State {
    void _process (iter i);
};
struct PotentialNest : State {
    void _process (iter i);
};

void Context::process (iter i) {
    state -> process (i);
}
void State::process (iter i) {
    if (auto& children = context -> children;
        not children.empty ()) {
        for (auto& child : children)
            child -> process (i);
    }
    else {
        _process (i);
    }
}
template <class T>
void State::transition () {
    context -> state = new T;
    context -> state -> context = context;
}
optional <string> State::declared () {
    for (auto d = context -> declaredVariables.begin (); d != context -> declaredVariables.end(); ++d) {
        if (d -> first == context -> variable) {
            return optional{d->second};
        }
    }
    return {};
}
string& State::variable () {
    return context -> variable;
}
string& State::result () {
    return result ();
}
string& State::potential () {
    return potential();
}
void State::removeFromParent () {
    for (auto cont = context -> parent -> children.begin(); cont < context -> parent -> children.end(); ++cont) {
        if (*cont == context) {
//            cout << "removing child context from parent context" << endl;
            context -> parent -> children.erase (cont);
            break;
        }
    }
}

void Begin::_process (iter i) {
    if (*i == '$'){
        context -> begin_it = i;
        transition <Dollar> ();
//        context -> state = new Dollar {};
//        context -> state -> context = context;
    }
    else {
        result () += *i;
    }
}
void Dollar::_process (iter i) {
    if (*i == '{') {
        transition <LBracket> ();
    }
    else {
        result () += *i;
        transition <Begin> ();
    }
}
void LBracket::_process (iter i) {
    /**
     if never an ending bracket, must set res += variable and res += parent.potential
     */
    if (*i == '}') {
        
        
        
        optional <string> declared = State::declared();
        
        if (auto* parent = context -> parent;
            parent == nullptr)
        {
            if (declared)
            {
                result () += declared.value();
                
            } else
            {
                string warning = "variable \"" + variable () + "\" pasted but it has not yet been declared!";
                cout << result () << endl;
                throw runtime_error (warning);
            }
            
            transition <Begin> ();
            
        } else
        {
            context -> parent -> variable += variable ();
            removeFromParent ();
        }
        
        
        
    
        variable ().clear ();
        

        
    }
    else if (*i == '$') {
        State* childState = new Dollar;
        Context* childContext = new Context {childState, context -> declaredVariables, context};
//        childContext -> begin_it = i;
        childState -> context = childContext;
        
        context -> children.push_back (childContext);
    }
    else {
        variable () += *i;
    }
}
void PotentialNest::_process (iter i) {
    switch (*i) {
        case '{':
            potential ().clear ();
            context -> begin_it = i;
            transition <LBracket> ();
            break;
            
        default:
            break;
    }
}

}


struct Process
{
    vector <pair <string, string>> declaredVariables;
    declare_var::Context declVar;
    paste_var::Context pasteVar;
    string str;
    
    Process (string const& str) : pasteVar {new paste_var::Begin{}, declaredVariables}, str (str), declVar {nullptr, declaredVariables, new declare_var::Begin}
    {
        declVar.state -> context = &declVar;
        pasteVar.state -> context = &pasteVar;
    }
    
    string process ()
    {
        
        for (auto i = str.begin(); i < str.end(); ++i)
        {
            declVar.process (i);
        }
        
//        for (auto& i : declaredVariables)
//            cout << i.first << " : " << i.second << endl;
        
        
    
        
        str = declVar.result;
        
        cout << str << endl;
//
        return str;
//        cout << str << endl;
        
        
//        cout << endl << "-------------------" << endl;
        for (auto i = str.begin(); i < str.end(); ++i)
        {
            pasteVar.process (i);
//            if (declVar.state->done()) {
////                cout << "done" << endl;
////                cout << string (declVar.begin_it, declVar.end_it) << endl;
////                declVar.print ();
//                declVar.state = new declare_var::Begin {&declVar};
//            }
                
        }
        cout << endl << "-------------------" << endl;
        str = pasteVar.res;
        cout << str << endl;
        return str;
        
//        cout << pasteVar.res << endl;
        
//        cout << declVar.res << endl;
        
//        cout << endl << "-------------------" << endl;
//        cout << declVar.res << endl;
    }
};





void app (string const& inputPath, string const& outputPath) {
 
    ofstream outputFile (outputPath);
    
    if (!outputFile.is_open ())
        throw runtime_error ("could not open file " + outputPath);
    
    outputFile << Process {readFileIntoString (inputPath)}.process ();
    outputFile.close ();
}



auto main(int argc,  char** argv) -> int
{

    
#ifdef Debug
    
    #define ANTAL TEST_FILE_COUNT
    
    string warning = "";
    
    #define TEST_SINGEL_FILE 2.hpp
    
    #ifdef TEST_SINGEL_FILE
        string inputPath =  string (TEST_FILES_PRE_PATH) + string (BOOST_PP_STRINGIZE (TEST_SINGEL_FILE));
        string outputPath = string (TEST_FILES_POST_PATH) + string (BOOST_PP_STRINGIZE (TEST_SINGEL_FILE));
        string facitPath = string (TEST_FILES_FACIT_PATH) + string (BOOST_PP_STRINGIZE (TEST_SINGEL_FILE));
    
        app (inputPath, outputPath);
        string result = readFileIntoString (outputPath);
        string facit = readFileIntoString (facitPath);
        
        if (result != facit)
        {
            warning += "\n\n\t" + outputPath + "\n\t != " + "\n\t" + facitPath + "\n\n\n";
        }
    
    #else
        array <string, TEST_FILE_COUNT> test_files_pre;
        array <string, TEST_FILE_COUNT> test_files_post;
        array <string, TEST_FILE_COUNT> test_files_facit;


        #define PRE(z, n, text) test_files_pre [n] = BOOST_PP_CAT (text, n);
        #define POST(z, n, text) test_files_post [n] = BOOST_PP_CAT (text, n);
        #define FACIT(z, n, text) test_files_facit [n] = BOOST_PP_CAT (text, n);
    
      
    
        BOOST_PP_REPEAT (TEST_FILE_COUNT, PRE, TEST_FILE_PRE_)
        BOOST_PP_REPEAT (TEST_FILE_COUNT, POST, TEST_FILE_POST_)
        BOOST_PP_REPEAT (TEST_FILE_COUNT, FACIT, TEST_FILE_FACIT_)
    
        for (int i = 0; i < ANTAL; ++i)
        {
            string inputPath = test_files_pre [i];
            string outputPath = test_files_post [i];
            string facitPath = test_files_facit [i];
            
            app (inputPath, outputPath);
            
            string result = readFileIntoString (outputPath);
            string facit = readFileIntoString (facitPath);
        
    //        string post = readFileIntoString (test_files_post[i]);
    //        string facit = readFileIntoString (test_files_facit[i]);
     
            if (result != facit)
            {
                warning += "\n\n\t" + test_files_post[i] + "\n\t != " + "\n\t" + test_files_facit[i] + "\n\n\n";
            }
        }
    
    #endif
    
    if (warning != "") {
//        throw runtime_error (warning);
        cout << warning << endl;
    }
    return 0;
    
#endif
    
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
    
    
    string outtext = readFileIntoString(infile);
//    auto extractors = array <extractor, 1> {extractor{"${", "}"}};
//    auto extractors = array <extractor, 1> {extractor{"${", "}"}};
    
    auto stringVariableDeclerationExtractor = extractor {"$(", ")"};
    auto stringValueDeclerationExtractor = extractor {"{", "}"};
    auto stringVariablePasterExtractor = extractor {"${", "}"};
//    auto stringVariablePasteExtractor = extractor {"${", "}"};
    vector <pair <string, string>> declaredVariables;
    
    
    [&outtext]{
        vector <pair <string, string>> declaredVariables;
        auto* stringVariableDeclerationExtractor = new extractor {"$((", "))"};
        auto* stringValueDeclerationExtractor = new extractor {"{{", "}}"};
        auto* stringVariablePasterExtractor = new extractor {"${{", "}}"};
        for (auto it = outtext.begin(); it != outtext.end(); ++it) {
            auto declVar = stringVariableDeclerationExtractor -> found (*it);
            
            if (declVar) {
                auto [var0, var1, var2, var3] = declVar.value();

                auto [_begin, _end] = pair {it - (var3 - var0 - 1), it + 1};
                auto varstring = string (_begin + (var1-var0), _end - (var3 - var2) - 1);

                for (auto it2 = it + 1; it2 != outtext.end(); ++it2)
                {
                    auto declVal = stringValueDeclerationExtractor -> found (*it2);
                    if (declVal) {
                        auto [val0, val1, val2, val3] = declVal.value();
                        auto [_begin2, _end2] = pair {it2 - (val3 - val0 - 1), it2 + 1};

                        auto valstring = string (_begin2 + (val1-val0), _end2 - (val3 - val2) - 1);
                        
                        auto declared = declaredVariables.begin();
                        for (; declared != declaredVariables.end(); ++declared)
                        {
                            if (declared -> first == varstring) {
                                break;
                            }
                        }
                        
                        if (declared == declaredVariables.end())
                        {
                            declaredVariables.emplace_back (varstring, valstring);
                        } else
                        {
                            declared -> second = valstring;
                        }
                        
                        cout << varstring << " = " << valstring << endl;
                        
                        delete stringVariableDeclerationExtractor;
                        stringVariableDeclerationExtractor = new extractor {"$((", "))"};
                        
                        delete stringValueDeclerationExtractor;
                        stringValueDeclerationExtractor = new extractor {"{{", "}}"};
                        
                        delete stringVariablePasterExtractor;
                        stringVariablePasterExtractor = new extractor {"${{", "}}"};
                        
    //                    cout << *it2 << endl;
                        outtext.replace (it - (var3 - var0) + 1, it2 + 1, valstring);
    //                    cout << valstring << endl;
    //                    cout << (var3 - var2) + (var1 - var0) << endl;
    //                    cout << *(it - (var3 - var2) - (var1 - var0) - 1) << endl;
                        it = it - (var3 - var2) - (var1 - var0) - 1;
                        
                        break;
                    }
                }
                
                continue;
            }
            
            auto pasteVar = stringVariablePasterExtractor -> found (*it);
            if (pasteVar)
            {
                auto [var0, var1, var2, var3] = pasteVar.value();
                auto [_begin2, _end2] = pair {it - (var3 - var0 - 1), it + 1};
                auto varstring = string (_begin2 + (var1-var0), _end2 - (var3 - var2) - 1);
    //            cout << varstring << endl;
                
                auto declared = declaredVariables.begin();
                for (; declared != declaredVariables.end(); ++declared)
                {
                    if (declared -> first == varstring) {
                        break;
                    }
                }
                
                if (declared == declaredVariables.end())
                {
                    throw runtime_error ("variable pasted but not yet declared");
                } else
                {
    //                cout << *it << endl;
                    outtext.replace (_begin2, _end2, declared -> second);
                    it = it - (var3 - var2) - (var1 - var0) - 1;
    //                cout << *(it) << endl;
                }
                
                delete stringVariablePasterExtractor;
                stringVariablePasterExtractor = new extractor {"${{", "}}"};
            }
            
        }
    };
    
        
    
    
    {
//        Process p;
//        p.process (outtext);
    }
    

    
    
    
    

    
    for(auto& i : declaredVariables)
    {
//        cout << i.first << endl << i.second << endl;
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
    
//    cout << s << endl;

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
    
//    cout << "hello world" << endl;
    return 0;
}


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
