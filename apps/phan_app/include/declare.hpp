#pragma once


#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

template <unsigned int N>
constexpr char get_ch (char const (&s) [N], unsigned int i)
{
    return i >= N ? '\0' : s[i];
}


auto len = [] <size_t N> (char const (&s) [N]) constexpr {
    return N;
};

#define LEN(s) len(s) \
[] <size_t N> (char const (&s) [N]) constexpr { \
return N; \
}(s)

#define STRING_TO_CHARS_EXTRACT(z, n, data) \
        BOOST_PP_COMMA_IF(n) get_ch(data, n)

#define STRING_TO_CHARS(STRLEN, STR)  \
        BOOST_PP_REPEAT(STRLEN, STRING_TO_CHARS_EXTRACT, STR)


#define STR(s) \
        STRING_TO_CHARS(100, s)

//#define S(s) STR (s)//STR (BOOST_PP_STRINGIZE (s))

#define S(s) <STR (s)>

#define STATE(x) State S(x)

#define TRANSITION(x) transition <STATE (x)> ();



using iter = string::iterator;






struct Context;
template <char...>
struct State;

template <>
struct State <>
{
    Context* context;
    
    void process (iter i);
    virtual void _process (iter i){}
    bool hasParent () const;
    string& variable ();
    string& value ();
    string& result ();
    string& potential ();
    string& paste ();
    void removeFromParent ();
    template <class T> void transition ();
    template <class T> Context& addChildContext ();
    State* parent ();
    void declare (string const& var, string const& val);
    optional <string> declared (string const&);
    virtual void addResultFromChild (string const& res);
};

struct Context
{
    Context* parent;
    vector <pair <string, string>>& declaredVariables;
    State<>* state {nullptr};
    vector <Context*> children;
    stack <char> bracketStack;
    
    inline static string result = "";
    string variable;
    string value;
    string potential;
    string paste;
    
    void process (iter);
};


void State<>::declare (string const& var, string const& val) {
    auto declared = context -> declaredVariables.begin ();
    for (; declared < context -> declaredVariables.end (); ++declared) {
        if (declared -> first == var) {
            declared -> second = val;
            return;
        }
    }
    context -> declaredVariables.emplace_back (var, val);
}
optional <string> State<>::declared (string const& p) {
    for (auto d = context -> declaredVariables.begin (); d != context -> declaredVariables.end(); ++d) {
        if (d -> first == p) {
            return optional{d->second};
        }
    }
    return {};
}

State<>* State<>::parent () {
    return context -> parent -> state;
}

void Context::process(iter i) {
    state -> process (i);
}

void State<>::addResultFromChild (string const& res) {
    
}


void State<>::process (iter i) {
    if (context -> children.empty ()) {
//        context -> curr_it = i;
        _process (i);
    } else
    {
        for (auto& o : context->children)
            o -> process (i);
    }
}

bool State<>::hasParent () const {
    return context -> parent != nullptr;
}

void State<>::removeFromParent () {
    for (auto cont = context -> parent -> children.begin(); cont < context -> parent -> children.end(); ++cont) {
        if (*cont == context) {
//            cout << "removing child context from parent context" << endl;
            context -> parent -> children.erase (cont);
            break;
        }
    }
}


template <class T>
void State<>::transition () {
//    T* newstate = new T;
//    newstate -> context = context;
    
//    cout << "transitioning from " << typeid (*context -> state).name () << " to ";
    context -> state = new T;
    
    context -> state -> context = context;
    
//    cout << typeid (*context -> state).name () << endl;
}

template <class state>
Context& State<>::addChildContext () {
    State* childState = new state;
    Context* childContext = new Context {context, context->declaredVariables, childState};
    childState -> context = childContext;
    context -> children.push_back (childContext);
    return *childContext;
}
//optional <string> State<>::declared () {
//    for (auto d = context -> declaredVariables.begin (); d != context -> declaredVariables.end(); ++d) {
//        if (d -> first == context -> variable) {
//            return optional{d->second};
//        }
//    }
//    return {};
//}
string& State<>::variable () {
    return context -> variable;
}
string& State<>::value () {
    return context -> value;
}
string& State<>::result () {
    return context -> result;
}
string& State<>::potential () {
    return context -> potential;
}
string& State<>::paste () {
    return context -> paste;
}





struct Begin : State <>
{
    using State<>::State;
    void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};


template <>
struct STATE ("$") : State <>
{
    using State<>::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};

template <>
struct STATE ("#") : State <>
{
    using State<>::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};


template <>
struct STATE ("$(") : State <>
{
    using State<>::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};




template <>
struct STATE ("$()") : State <>
{
    using State<>::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};


template <>
struct STATE ("$(){") : State <>
{
    using State<>::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};



struct Done : Begin
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

template <>
struct STATE ("${") : State <>
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};



struct Paste_Done: Done
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

template <>
struct STATE ("#{") : State <>
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

template <>
struct STATE ("@") : State <>
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

template <>
struct STATE ("@(") : State <>
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

template <>
struct STATE ("@()") : State <>
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

template <>
struct STATE ("@(){") : State <>
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

















void Begin::addResultFromChild (string const& res) {
    throw runtime_error ("oops");
}


void STATE ("$")::addResultFromChild (string const& res) {
    potential() += res;
    throw runtime_error ("oops");
}

void STATE ("#")::addResultFromChild (string const& res) {
    throw runtime_error ("oops");
}

void STATE ("$(")::addResultFromChild (string const& res) {
    potential () += res;
}

void STATE ("$()")::addResultFromChild (string const& res) {
    throw runtime_error ("oops");
}

void STATE ("$(){")::addResultFromChild (string const& res) {
    value () += res;
}



void Paste_Done::_process (iter i) {
    if (*i == '\n')
    {
        
    } else
    {
        Done::_process (i);
    }
}

void STATE ("#{")::_process (iter i) {
    potential() += *i;
    if (*i == '}') {
        if (hasParent()) {
            removeFromParent ();
        } else
        {
            potential().clear();
            transition<Paste_Done>();
        }
    }
}

void STATE ("#")::_process (iter i) {
    potential () += *i ;
    
    if (*i == '{')
    {
        transition <STATE ("#{")> ();
        
    } else
    {
        if (hasParent())
        {
            addResultFromChild (potential ());
            removeFromParent ();
        } else
        {
            result () += potential ();
            potential ().clear ();
            transition <Begin> ();
        }
    }
}






void STATE ("@")::_process (iter i) {
    potential() += *i;
    
    switch (*i)
    {
        case '(':
            transition <STATE ("@(")> ();
            break;
            
        default:
            if (hasParent())
            {
                addResultFromChild (potential ());
                removeFromParent ();
            } else
            {
                result () += potential ();
                potential ().clear ();
                transition <Begin> ();
            }
            break;
    }
}

void STATE ("@(")::_process (iter i) {
    potential() += *i;
    
    switch (*i)
    {
        case ')':
            transition<STATE ("@()")>();
            break;
            
        default:
            if (hasParent())
            {
                addResultFromChild (potential ());
                removeFromParent ();
            } else
            {
                result () += potential ();
                potential ().clear ();
                transition <Begin> ();
            }
            break;
    }
}

void STATE ("@()")::_process (iter i) {
    
}

void STATE ("@(){")::_process (iter i) {
    
}

void Begin::_process (iter i) {
    switch (*i)
    {
        case '$':
            potential () += '$';
            TRANSITION ("$")
            break;
            
        case '#':
            potential () += '#';
            TRANSITION ("#")
            break;
            
        case '@':
            potential () += '@';
            TRANSITION ("@")
            break;
            
        default:
            result () += *i;
            break;
    }

}
void STATE ("$")::_process (iter i) {
    
    if (*i == '(')
    {
        potential () += *i;
        TRANSITION ("$(")
        
    } else if (*i == '{')
    {
//        addChildContext<STATE ("${")>();
        potential() += '{';
        TRANSITION ("${")
        // so that parent can push bracket to it's bracketstack
    } else
    {
        potential () += *i;
        if (hasParent ())
        {
            parent () -> addResultFromChild (potential ());
            removeFromParent ();
            
        } else
        {
            result () += potential();
            potential().clear ();
            variable ().clear ();
            value ().clear ();
            transition <Done> ();
        }
    }
}
void STATE ("${")::_process (iter i) {
    
    potential() += *i;
    
    if (*i == '}')
    {
//        cout << paste () << endl << paste () << endl;
        optional <string> decl = declared (paste ());
        if (decl)
        {
//            cout << paste () << endl << paste () << endl;
            if (hasParent ())
            {
//                cout << paste () << endl << paste () << endl;
                parent () -> addResultFromChild (decl.value ());
                removeFromParent ();
            } else
            {
//                cout << paste () << endl << paste () << endl;
                result() += decl.value ();
                potential().clear();
                value().clear();
                variable().clear();
                paste().clear();
                transition<Done>();
            }
            
        } else
        {
            string warning = "variable \"" + paste () + "\" pasted but it has not yet been declared!";
            //            cout << result () << endl;
            throw runtime_error (warning);
        }
    } else
    {
        paste () += *i;
//        potential() += *i;
    }
}
void STATE ("$(")::_process (iter i) {
    
    if (*i == ')')
    {
        variable() = string (potential().begin() + 2, potential().end());
        potential () += ')';
        TRANSITION ("$()")
        
    } else if (*i == '$')
    {
        addChildContext <STATE ("$")> ().potential += '$';

    } else
    {
        potential () += *i;
    }
    
    
}

void STATE ("$()")::_process (iter i) {
    
    
    if (*i == '{')
    {

        context -> bracketStack.push ('{');

        TRANSITION ("$(){")
        
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

void STATE ("$(){")::_process (iter i) {
    
    switch (*i)
    {
        case '}':
            context -> bracketStack.pop ();
            
            if (context -> bracketStack.empty ())
            {
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
            } else
            {
                value () += '}';
            }
            break;
            
        case '{':
            context -> bracketStack.push ('{');
            value() += *i;
            break;
            
        case '$':
            addChildContext <STATE ("$")> ().potential += '$';
            break;
            
        default:
            potential () += *i;
            value () += *i;
            break;
    }
}

void Done::_process (iter i) {
    Begin::_process (i);
}



