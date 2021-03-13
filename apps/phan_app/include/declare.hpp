#pragma once
using iter = string::iterator;


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
    State* state {nullptr};
    vector <Context*> children;
    stack <char> bracketStack;
    
    inline static string result = "";
    string variable;
    string value;
    string potential;
    string paste;
    
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
optional <string> State::declared (string const& p) {
    for (auto d = context -> declaredVariables.begin (); d != context -> declaredVariables.end(); ++d) {
        if (d -> first == p) {
            return optional{d->second};
        }
    }
    return {};
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
//    T* newstate = new T;
//    newstate -> context = context;
    
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
string& State::paste () {
    return context -> paste;
}

namespace declare
{



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

struct Paste : State
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



struct Done : Begin
{
//    using State::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

struct PasteLBracket : State
{
//    using State::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
};

struct Hashtag : State
{
//    using State::State;
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

void LParan::addResultFromChild (string const& res) {
    potential () += res;
}

void RParan::addResultFromChild (string const& res) {
    throw runtime_error ("oops");
}

void LBracket::addResultFromChild (string const& res) {
    value () += res;
}


void Hashtag::_process (iter i) {
    potential () += '{';
    
    if (*i == '{')
    {
    
        
    } else
    {
        if (hasParent())
        {
            addResultFromChild (potential ());
        } else
        {
            result () += potential ();
            potential ().clear ();
            transition <Begin> ();
        }
    }
}


void Begin::_process (iter i) {
    
    if (*i == '$')
    {
        potential () += '$';
        transition <Dollar> ();
        
    } else if (*i == '#')
    {
        potential () += '#';
        transition <Hashtag> ();
    } else
    {
        result () += *i;
    }
}
void Dollar::_process (iter i) {
    
    if (*i == '(')
    {
        potential () += *i;
        transition <LParan> ();
        
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
        
    }
        

    else if (*i == '{')
    {
        context -> bracketStack.push ('{');
        value() += *i;
    }


    else if (*i == '$')
    {
        addChildContext <Dollar> ().potential += '$';
    }
    else
    {
//        result () += *i;
        potential () += *i;
        value () += *i;
    }
}

void Done::_process (iter i) {
    Begin::_process (i);
}

//  context -> transition (Dollar);
        
//        context -> process (i);
}


//namespace paste {
//
//
//
//
//
//struct Begin : State {
//    virtual void _process (iter i);
//};
//
//struct Dollar : State {
//    using State::context;
//    virtual void _process (iter i);
//};
//
//struct LBracket : State {
//    virtual void _process (iter i);
//};
//struct PotentialNest : State {
//    virtual void _process (iter i);
//};
//struct Done : Begin {
//    virtual void _process (iter i);
//};
//
//
//
//
//
//
//
//void Done::_process (iter i) {
//    Begin::_process(i);
//}
//void Begin::_process (iter i) {
//
//    if (*i == '$'){
//        potential ().push_back ('$');
//        transition <Dollar> ();
//    }
//    else {
//        result () += *i;
//    }
//}
//void Dollar::_process (iter i) {
//
//    potential () += *i;
//
//    if (*i == '{') {
//        transition <LBracket> ();
//    }
//    else {
//        if (hasParent ()) {
//            addResultFromChild (potential ());
//            removeFromParent ();
//        } else {
//            result () += potential ();
//            potential ().clear ();
//            transition <Begin> ();
//        }
//    }
//}
//void LBracket::_process (iter i) {
//    /**
//     if never an ending bracket, must set res += variable and res += parent.potential
//     */
//
//    if (*i == '}') {
//
//        optional <string> declared = State::declared();
//
//        if (declared)
//        {
////            variable () += declared.value ();
//
//
//        } else
//        {
//            string warning = "variable \"" + variable () + "\" pasted but it has not yet been declared!";
//            cout << result () << endl;
//            throw runtime_error (warning);
//        }
//
//        if ( not hasParent ())
//        {
//            result () += declared.value();
//            potential ().clear ();
//            variable ().clear ();
//            transition <Done> ();
//
//        } else
//        {
////            cout << *i << endl;
//            addResultFromChild (variable ());
//
//            removeFromParent ();
////            cout << *i << endl;
//
//        }
//
//    }
//    else if (*i == '$') {
//        addChildContext <Dollar> ().potential.push_back ('$');
//    }
//    else {
//        variable () += *i;
//    }
//}
//void PotentialNest::_process (iter i) {
//    switch (*i) {
//        case '{':
//            potential ().clear ();
////            context -> begin_it = i;
//            transition <LBracket> ();
//            break;
//
//        default:
//            break;
//    }
//}
//
//}
