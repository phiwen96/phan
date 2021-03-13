#pragma once
using iter = string::iterator;


namespace declare
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



struct Done : Begin
{
//    using State::State;
    virtual void _process (iter i);
    bool done () {return true;}
 
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
    throw runtime_error ("oops");
}

void Dollar::addResultFromChild (string const& res) {
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
        potential () += *i;
        transition <LParan> ();
        
    } else if (*i == '{' and hasParent ())
    {
        parent () -> addResultFromChild (potential ());
        removeFromParent ();
        parent () -> process (i);
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
