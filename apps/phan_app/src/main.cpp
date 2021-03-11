#include "main.hpp"

#include "extractor.hpp"
#include "trio.hpp"
#include <functional>
#include <algorithm>
#include <vector>





using iter = string::iterator;





namespace declare_var
{
struct Context;
struct State
{
    void process (iter i);
    virtual void _process (iter i){}
    Context* context;
    int chainParent (iter i);
    bool hasParent () const;
    State (Context* context) : context (context) {}
    virtual bool done () const;
    
};
struct Begin : State
{
    using State::State;
    void _process (iter i);
};

struct DollarFound : State
{
    using State::State;
    virtual void _process (iter i);
};

struct lParanthesisFound : State
{
    using State::State;
    virtual void _process (iter i);
};

struct rParanthesisFound : State
{
    using State::State;
    virtual void _process (iter i);
};

struct lCurlyBracketFound : State
{
    using State::State;
    virtual void _process (iter i);
};

struct Done : State
{
    using State::State;
    virtual void _process (iter i);
    bool done () const {return true;}
};

struct Repeat : State
{
    using State::State;
    virtual void _process (iter i);
};






//Begin begin;
//DollarFound dollarFound;
//lParanthesisFound lparanthesisFound;
//rParanthesisFound rparanthesisFound;
//lCurlyBracketFound lcurlyBracketFound;
//Repeat repeat;
//Done done;



struct Context
{
    bool root;
    Context* parent;
    vector <pair <string, string>>& declaredVariables;
    State* state;
    iter begin_it;
    iter end_it;
    iter curr_it;
    vector <Context*> others;
    stack <char> bracketStack;
    
//    Context (Context&& other) : declaredVariables (other.declaredVariables)
//    {
//        swap (*this, other);
//    }
//    Context (Context const& other) : root (other.root), parent (other.parent), declaredVariables (other.declaredVariables), state (other.state), begin_it (other.begin_it), end_it (other.end_it), others (other.others), bracketStack (other.bracketStack)
//    {
//
//    }
//    friend void swap (Context& lhs, Context& rhs) {
//        using std::swap;
//        swap (lhs.root, rhs.root);
//        swap (lhs.parent, rhs.parent);
//        swap (lhs.state, rhs.state);
//        swap (lhs.begin_it, rhs.begin_it);
//        swap (lhs.end_it, rhs.end_it);
//        swap (lhs.others, rhs.others);
//        swap (lhs.bracketStack, rhs.bracketStack);
//    }
    
    
    Context (vector <pair <string, string>>& declaredVariables, State* state, Context* parent = nullptr, bool root = true) : state (state), root (root), parent (parent), declaredVariables (declaredVariables) {
//        state -> context = this;
    }
    void transition (State* newstate) {
//        cout << "transition from " << typeid (*state).name() << " to " << typeid (*newstate).name() << endl;

//        state = newstate;
//        state -> context = this;
    }
    void process (iter i) {
//        if (state == nullptr)
//            cout << "state is null" << endl;
        state -> process (i);
    }
    void removeFromParent () {
        if (parent == nullptr)
        {
//            transition (&begin);
            return;
        }
        
        
        
//        erase_if (parent->others, [&](Context& c){return &c == this;});
//        std::stable_partition(vec.begin(), vec.end(), [](int* pi){ return *pi % 2 != 0; });
//        remove (parent->others.begin(), parent->others.end(), this);
        for (auto i = parent->others.begin(); i != parent->others.end(); ++i) {
            if ((*i) == this)
            {
//                cout << "oj" << endl;
//                remove (i, i+1);
                i = parent->others.erase(i);
                break;
            }
        }
    }
    void print () const
    {
        cout << string (begin_it, end_it) << endl;
    }
};

void State::process (iter i) {
    if (context -> others.empty ()) {
        context -> curr_it = i;
        _process (i);
    } else
    {
        for (auto& o : context->others)
            o -> process (i);
    }
}

int State::chainParent (iter i) {
    if (hasParent ())
    {
        context -> parent -> process (i);
    }
    else
    {
//        _process (i);
    }
}
bool State::hasParent () const {
    return context -> parent != nullptr;
}

bool State::done () const {
//    cout << string (context -> begin_it, context -> end_it) << endl;
    return false;
}


void Begin::_process (iter i) {
    
    if (*i == '$')
    {
        context -> begin_it = i;
//        context -> transition (new DollarFound {context});
        context -> state = new DollarFound {context};
        context -> state -> context = context;
        context -> transition (context -> state);
        
    } else
    {
//        chainParent(i);
    }
}
void DollarFound::_process (iter i) {
    if (*i == '(')
    {
//        context -> transition (new lParanthesisFound {context});
        context -> state = new lParanthesisFound {context};
        context -> state -> context = context;
        context -> transition (context -> state);
        
    } else
    {
        if (hasParent ())
        {
            chainParent (i);
            context -> removeFromParent ();
            
        } else
        {
//            context -> transition (new Begin {context});
            context -> state = new Begin {context};
            context -> state -> context = context;
            context -> transition (context -> state);
        }
    }
}
void lParanthesisFound::_process (iter i) {
    if (*i == ')')
    {
//        context -> end_it = i + 1;
//        context -> transition (new rParanthesisFound {context});
        context -> state = new rParanthesisFound {context};
        context -> state -> context = context;
        context -> transition (context -> state);
        
    } else if (*i == '$')
    {
        
//        Context* pushed = context -> others.back ();
//        pushed->transition (&dollarFound);
//        context -> transition (new Repeat {context});
        context -> state = new Repeat {context};
        context -> state -> context = context;
        context -> transition (context -> state);
        
    } else
    {
//        cout << *i << endl;
//        cout << "hi" << endl;
//        chainParent (i);
    }
}
void Repeat::_process(iter i) {
//    cout << *i << endl;
    if (*i == '(')
    {
        context -> others.push_back (new Context {context -> declaredVariables, /*state*/nullptr, /*parent*/context, /*root*/false});
        context -> others.back () -> state = new lParanthesisFound {context -> others.back ()};
        context -> others.back () -> begin_it = i - 1;
//        context -> transition (new lParanthesisFound {context});
        context -> state = new lParanthesisFound {context};
        context -> state -> context = context;
        context -> transition (context -> state);
//        context -> curr_it -= 2;
    }
//    else if (*i == ')')
//    {
//        context -> transition ( new rParanthesisFound {context});
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
//            context -> transition (new lParanthesisFound {context});
        context -> state = new lParanthesisFound {context};
        context -> state -> context = context;
        context -> transition (context -> state);
//        }
    }
}
void rParanthesisFound::_process (iter i) {
    
//    context -> others.clear();
    
    if (*i == '{')
    {
//        context -> end_it = i + 1;
        context -> bracketStack.push ('{');
//        context -> transition (new lCurlyBracketFound {context});
        context -> state = new lCurlyBracketFound {context};
        context -> state -> context = context;
        context -> transition (context -> state);
        
        
    } else
    {
        if (hasParent ())
        {
            chainParent (i);
            context -> removeFromParent ();
        }
        else
        {
//            context -> transition (new Begin {context});
            context -> state = new Begin {context};
            context -> state -> context = context;
            context -> transition (context -> state);
        }
    
        
    }
}
void lCurlyBracketFound::_process (iter i) {
    if (*i == '}')
    {
        
        context -> bracketStack.pop ();
        
        if (context -> bracketStack.empty ())
        {
            context -> end_it = i + 1;
            
            if (hasParent ())
            {
//                cout << "yay" << endl;
//                cout << *context -> parent -> curr_it << endl;
                context -> print ();
                
                //parent ->
                
//                cout << "oooo" << endl;
//                cout << string (context -> parent -> begin_it, context -> end_it) << endl;
                context -> removeFromParent ();
//                delete this;
                
            } else
            {

                context -> state = new Done {context};
                context -> state -> context = context;
                context -> transition (context -> state);
                context -> print ();
            }
        }
        
    } else if (*i == '{')
    {
        context -> bracketStack.push ('{');
    }
}

void Done::_process (iter i) {
    
}

//  context -> transition (dollarFound);
        
//        context -> process (i);
}








struct Process
{
    vector <pair <string, string>>& declaredVariables;
    declare_var::Context declVar;
    
    Process (vector <pair <string, string>>& declaredVariables) : declaredVariables (declaredVariables), declVar (declaredVariables, new declare_var::Begin {nullptr})
    {
        declVar.state -> context = &declVar;
    }
    
    void process (string& str)
    {
        
        for (auto i = str.begin(); i < str.end(); ++i)
        {
            declVar.process (i);
            if (declVar.state->done()) {
//                cout << "done" << endl;
//                cout << string (declVar.begin_it, declVar.end_it) << endl;
//                declVar.print ();
                declVar.state = new declare_var::Begin {&declVar};
            }
                
        }
    }
};








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
        Process p (declaredVariables);
        p.process (outtext);
    }
    
    
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
