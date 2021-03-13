#pragma once
using iter = string::iterator;

namespace paste {

struct State;
struct Begin;
struct Dollar;
struct LBracket;

struct Context {
    Context* parent {nullptr};
    vector <pair <string, string>>& declaredVariables;
    State* state {nullptr};
    inline static string res = "";
    string variable;
    string potential;
    vector <Context*> children;
    
    void process (iter);
    
};

struct State {
    Context* context;
    
    void process (iter i);
    
protected:
    virtual void _process (iter i) {}
    bool hasParent ();
    State* parent ();
    template <class T> void transition ();
    optional <string> declared ();
    string& variable ();
    string& result ();
    string& potential ();
    void removeFromParent ();
    virtual void addResultFromChild (string const& res) {}
    template <class state> Context& addChildContext ();
    
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
bool State::hasParent () {
    return context -> parent != nullptr;
}
State* State::parent () {
    return context -> parent -> state;
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
template <class state>
Context& State::addChildContext () {
    State* childState = new state;
    Context* childContext = new Context {context, context->declaredVariables, childState};
    childState -> context = childContext;
    context -> children.push_back (childContext);
    return *childContext;
}

template <class T>
void State::transition () {
#if defined (DO_LOUD)
    cout << "transitioning from " << typeid (*context -> state).name () << " to ";
#endif
    context -> state = new T;
    context -> state -> context = context;
#if defined (DO_LOUD)
    cout << typeid (*context -> state).name () << endl;
#endif
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
    return context -> res;
}
string& State::potential () {
    return context -> potential;
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
        potential ().push_back ('$');
        transition <Dollar> ();
    }
    else {
        result () += *i;
    }
}
void Dollar::_process (iter i) {
    
    potential () += *i;
    
    if (*i == '{') {
        transition <LBracket> ();
    }
    else {
        if (hasParent ()) {
            addResultFromChild (potential ());
            removeFromParent ();
        } else {
            result () += potential ();
            potential ().clear ();
            transition <Begin> ();
        }
    }
}
void LBracket::_process (iter i) {
    /**
     if never an ending bracket, must set res += variable and res += parent.potential
     */
    
    if (*i == '}') {
        
        optional <string> declared = State::declared();
        
        if (declared)
        {
//            variable () += declared.value ();
            
            
        } else
        {
            string warning = "variable \"" + variable () + "\" pasted but it has not yet been declared!";
            cout << result () << endl;
            throw runtime_error (warning);
        }
        
        if ( not hasParent ())
        {
            result () += declared.value();
            potential ().clear ();
            variable ().clear ();
            transition <Begin> ();
            
        } else
        {
//            cout << *i << endl;
            addResultFromChild (variable ());
            
            removeFromParent ();
//            cout << *i << endl;
            
        }
        
    }
    else if (*i == '$') {
        addChildContext <Dollar> ().potential.push_back ('$');
    }
    else {
        variable () += *i;
    }
}
void PotentialNest::_process (iter i) {
    switch (*i) {
        case '{':
            potential ().clear ();
//            context -> begin_it = i;
            transition <LBracket> ();
            break;
            
        default:
            break;
    }
}

}
