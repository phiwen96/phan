#pragma once
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

namespace declare
{



struct Begin : State <>
{
    using State<>::State;
    void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};



struct Dollar : State <>
{
    using State<>::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};

struct Paste : State <>
{
    using State<>::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};


struct DeclPaste_LParan : State <>
{
    using State<>::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};



struct DeclPaste_RParan : State <>
{
    using State<>::State;
    virtual void _process (iter i);
    bool done () {return false;}
    void addResultFromChild (string const& res);
};



struct DeclPaste_LBracket : State <>
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

struct PasteLBracket : State <>
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

struct Hashtag : State <>
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

struct Paste_LBracket : State <>
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

struct At : State <>
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

struct Declare_LParan : State <>
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

struct Declare_RParan : State <>
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

struct Declare_LBracket : State <>
{
//    using State<>::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

















void Begin::addResultFromChild (string const& res) {
    throw runtime_error ("oops");
}

void Dollar::addResultFromChild (string const& res) {
    potential() += res;
    throw runtime_error ("oops");
}

void DeclPaste_LParan::addResultFromChild (string const& res) {
    potential () += res;
}

void DeclPaste_RParan::addResultFromChild (string const& res) {
    throw runtime_error ("oops");
}

void DeclPaste_LBracket::addResultFromChild (string const& res) {
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

void Paste_LBracket::_process (iter i) {
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

void Hashtag::_process (iter i) {
    potential () += *i ;
    
    if (*i == '{')
    {
        transition <Paste_LBracket> ();
        
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






void At::_process (iter i) {
    potential() += *i;
    
    switch (*i)
    {
        case '{':
            
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

void Declare_LParan::_process (iter i) {
    
}

void Declare_RParan::_process (iter i) {
    
}

void Declare_LBracket::_process (iter i) {
    
}

void Begin::_process (iter i) {
    switch (*i)
    {
        case '$':
            potential () += '$';
            transition <Dollar> ();
            break;
            
        case '#':
            potential () += '#';
            transition <Hashtag> ();
            break;
            
        case '@':
            potential () += '@';
            transition <At> ();
            break;
            
        default:
            result () += *i;
            break;
    }

}
void Dollar::_process (iter i) {
    
    if (*i == '(')
    {
        potential () += *i;
        transition <DeclPaste_LParan> ();
        
    } else if (*i == '{')
    {
//        addChildContext<PasteLBracket>();
        potential() += '{';
        transition <PasteLBracket> ();
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
void PasteLBracket::_process (iter i) {
    
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
void DeclPaste_LParan::_process (iter i) {
    
    if (*i == ')')
    {
        variable() = string (potential().begin() + 2, potential().end());
        potential () += ')';
        transition <DeclPaste_RParan> ();
        
    } else if (*i == '$')
    {
        addChildContext <Dollar> ().potential += '$';

    } else
    {
        potential () += *i;
    }
    
    
}

void DeclPaste_RParan::_process (iter i) {
    
    
    if (*i == '{')
    {

        context -> bracketStack.push ('{');

        transition <DeclPaste_LBracket> ();
        
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

void DeclPaste_LBracket::_process (iter i) {
    
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
            addChildContext <Dollar> ().potential += '$';
            break;
            
        default:
            potential () += *i;
            value () += *i;
            break;
    }
//    if (*i == '}')
//    {
//        context -> bracketStack.pop ();
//
//        if (context -> bracketStack.empty ())
//        {
//            declare (variable (), value ());
//
//            if (hasParent ())
//            {
//                parent () -> addResultFromChild (value ());
//                removeFromParent ();
//            }
//
////        }
//            else
//            {
//                result () += value ();
//                potential().clear();
//                value().clear();
//                variable().clear();
//                transition <Begin> ();
//    //            value() += *i;
//            }
//        } else
//        {
//            value () += '}';
//        }
//
//    }
        

//    else if (*i == '{')
//    {
//        context -> bracketStack.push ('{');
//        value() += *i;
//    }


//    else if (*i == '$')
//    {
//        addChildContext <Dollar> ().potential += '$';
//    }
//    else
//    {
////        result () += *i;
//        potential () += *i;
//        value () += *i;
//    }
}

void Done::_process (iter i) {
    Begin::_process (i);
}

}

