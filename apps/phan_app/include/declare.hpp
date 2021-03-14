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

#define BASE_STATE State <>


using iter = string::iterator;






struct Context;
template <char...>
struct State;

template <>
struct BASE_STATE
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
    void reset ();
    virtual void reset_hasNoParent (){}
    virtual void reset_hasParent (){}
    
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


void BASE_STATE::declare (string const& var, string const& val) {
    auto declared = context -> declaredVariables.begin ();
    for (; declared < context -> declaredVariables.end (); ++declared) {
        if (declared -> first == var) {
            declared -> second = val;
            return;
        }
    }
    context -> declaredVariables.emplace_back (var, val);
}
optional <string> BASE_STATE::declared (string const& p) {
    for (auto d = context -> declaredVariables.begin (); d != context -> declaredVariables.end(); ++d) {
        if (d -> first == p) {
            return optional{d->second};
        }
    }
    return {};
}

BASE_STATE* BASE_STATE::parent () {
    return context -> parent -> state;
}

void BASE_STATE::reset (){
    if (not hasParent()) {
        reset_hasNoParent();
    } else {
        reset_hasParent();
    }
}

void Context::process(iter i) {
    state -> process (i);
}

void BASE_STATE::addResultFromChild (string const& res) {
    
}


void BASE_STATE::process (iter i) {
    if (context -> children.empty ()) {
//        context -> curr_it = i;
        _process (i);
    } else
    {
        for (auto& o : context->children)
            o -> process (i);
    }
}

bool BASE_STATE::hasParent () const {
    return context -> parent != nullptr;
}

void BASE_STATE::removeFromParent () {
    for (auto cont = context -> parent -> children.begin(); cont < context -> parent -> children.end(); ++cont) {
        if (*cont == context) {
//            cout << "removing child context from parent context" << endl;
            context -> parent -> children.erase (cont);
            break;
        }
    }
}



template <class T>
void BASE_STATE::transition () {
//    T* newstate = new T;
//    newstate -> context = context;
    
//    cout << "transitioning from " << typeid (*context -> state).name () << " to ";
    context -> state = new T;
    
    context -> state -> context = context;
    
//    cout << typeid (*context -> state).name () << endl;
}

template <class state>
Context& BASE_STATE::addChildContext () {
    State* childState = new state;
    Context* childContext = new Context {context, context->declaredVariables, childState};
    childState -> context = childContext;
    context -> children.push_back (childContext);
    return *childContext;
}
//optional <string> BASE_STATE::declared () {
//    for (auto d = context -> declaredVariables.begin (); d != context -> declaredVariables.end(); ++d) {
//        if (d -> first == context -> variable) {
//            return optional{d->second};
//        }
//    }
//    return {};
//}
string& BASE_STATE::variable () {
    return context -> variable;
}
string& BASE_STATE::value () {
    return context -> value;
}
string& BASE_STATE::result () {
    return context -> result;
}
string& BASE_STATE::potential () {
    return context -> potential;
}
string& BASE_STATE::paste () {
    return context -> paste;
}




template <>
struct STATE ("") : BASE_STATE
{
    void _process (iter i){
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
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (){}
    virtual void reset_hasParent (){}
};


template <>
struct STATE ("$") : BASE_STATE
{
    virtual void _process (iter i){
        
        potential () += *i;
        
        switch (*i)
        {
            case '(':
                TRANSITION ("$(")
                break;
                
            case '{':
                TRANSITION ("${")
                break;
                
            default:
                reset ();
                break;
        }
    
    }
    void addResultFromChild (string const& res){
        potential() += res;
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (){
        result () += potential();
        potential().clear ();
        variable ().clear ();
        value ().clear ();
        TRANSITION ("done")
    }
    virtual void reset_hasParent (){
        parent () -> addResultFromChild (potential ());
        removeFromParent ();
    }
};

template <>
struct STATE ("#") : BASE_STATE
{
    virtual void _process (iter i){
        potential () += *i ;
        
        switch (*i)
        {
            case '{':
                TRANSITION ("#{")
                break;
                
            default:
                reset ();
                break;
        }
    }
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (){
        result () += potential ();
        potential ().clear ();
        TRANSITION ("")
    }
    virtual void reset_hasParent (){
        addResultFromChild (potential ());
        removeFromParent ();
    }
};


template <>
struct STATE ("$(") : BASE_STATE
{
    virtual void _process (iter i){
        
        switch (*i)
        {
            case ')':
                variable() = string (potential().begin() + 2, potential().end());
                potential () += ')';
                TRANSITION ("$()")
                break;
                
            case '$':
                addChildContext <STATE ("$")> ().potential += '$';
                break;
                
            default:
                potential () += *i;
                break;
        }
    }
    void addResultFromChild (string const& res){
        potential () += res;
    }
    
    virtual void reset_hasNoParent (){}
    virtual void reset_hasParent (){}
};




template <>
struct STATE ("$()") : BASE_STATE
{
    virtual void _process (iter i){
        
        switch (*i)
        {
            case '{':
                context -> bracketStack.push ('{');
                TRANSITION ("$(){")
                break;
                
            default:
                reset ();
                break;
        }
    }
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (){
        result () += potential ();
        potential ().clear ();
        variable ().clear ();
        value ().clear ();
        TRANSITION ("")
        
        potential ().clear ();
        variable ().clear ();
    }
    virtual void reset_hasParent (){
        parent () -> addResultFromChild (potential ());
        removeFromParent ();
        
        potential ().clear ();
        variable ().clear ();
    }
};


template <>
struct STATE ("$(){") : BASE_STATE
{
    virtual void _process (iter i){
        
        switch (*i)
        {
            case '}':
                context -> bracketStack.pop ();
                
                if (context -> bracketStack.empty ())
                {
                    declare (variable (), value ());
                    reset();
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
    void addResultFromChild (string const& res){
        value () += res;
    }
    void finish () {
        
    }
    virtual void reset_hasNoParent (){
        result () += value ();
        potential().clear();
        value().clear();
        variable().clear();
        TRANSITION ("")
    }
    virtual void reset_hasParent (){
        parent () -> addResultFromChild (value ());
        removeFromParent ();
    }
};










template <>
struct STATE ("done") : STATE ("")
{
    virtual void _process (iter i){
        STATE ("")::_process (i);
    }
    
    virtual void reset_hasNoParent (){}
    virtual void reset_hasParent (){}
};

template <>
struct STATE ("${") : BASE_STATE
{
    virtual void _process (iter i){
        
        potential() += *i;
        
        
        
        if (*i == '}')
        {
            optional <string> decl = declared (paste ());
            
            if (decl)
            {
                value() = decl.value ();
                reset ();
                
            } else
            {
                string warning = "variable \"" + paste () + "\" pasted but it has not yet been declared!";
                //            cout << result () << endl;
                throw runtime_error (warning);
            }
        } else
        {
            paste () += *i;
        }
    }
    
    virtual void reset_hasNoParent (){
        result() += value ();
        potential().clear();
        value().clear();
        variable().clear();
        paste().clear();
        TRANSITION ("done")
    }
    virtual void reset_hasParent (){
        parent () -> addResultFromChild (value ());
        removeFromParent ();
    }
 
};


template <>
struct STATE ("${} done"): STATE ("done")
{
    virtual void _process (iter i){
        if (*i == '\n')
        {
            
        } else
        {
            STATE ("done")::_process (i);
        }
    }
    
    virtual void reset_hasNoParent (){}
    virtual void reset_hasParent (){}
 
};

template <>
struct STATE ("#{") : BASE_STATE
{
    virtual void _process (iter i){
        potential() += *i;
        if (*i == '}') {
            reset ();
        }
    }
    
    virtual void reset_hasNoParent (){
        potential().clear();
        transition<STATE ("${} done")>();
    }
    virtual void reset_hasParent (){
        removeFromParent ();
    }
 
};

template <>
struct STATE ("@") : BASE_STATE
{
    virtual void _process (iter i){
        potential() += *i;
        
        switch (*i)
        {
            case '(':
                TRANSITION ("@(")
                break;
                
            default:
                reset ();
                break;
        }
    }
    virtual void reset_hasNoParent(){
        result () += potential ();
        potential ().clear ();
        TRANSITION ("")
    }
    virtual void reset_hasParent(){
        addResultFromChild (potential ());
        removeFromParent ();
    }
 
};

template <>
struct STATE ("@(") : BASE_STATE
{
    virtual void _process (iter i){
        
        potential() += *i;
        
        if (*i == ')')
        {
            TRANSITION ("@()")
            
        } else if (*i == '$')
        {
            addChildContext<STATE ("$")>().potential = '$';

        } else
        {
            variable () += *i;
        }
        
    }
    void addResultFromChild (string const& res){
        variable () += res;
    }
    virtual void reset_hasNoParent (){}
    virtual void reset_hasParent (){}
 
};

template <>
struct STATE ("@()") : BASE_STATE
{
    virtual void _process (iter i){
        potential() += *i;
        
        if (*i == '{')
        {
            TRANSITION ("@(){")
        } else
        {
            reset ();
        }
    }
    
    virtual void reset_hasNoParent (){
        result() += potential();
        potential().clear();
        TRANSITION ("")
    }
    virtual void reset_hasParent (){
        addResultFromChild (potential ());
        removeFromParent ();
    }
 
};

template <>
struct STATE ("@(){") : BASE_STATE
{
    virtual void _process (iter i){
        
        potential () += *i;
        
        if (*i == '}')
        {
            declare (variable (), value ());
            reset ();
            
        } else if (*i == '$')
        {
            addChildContext <STATE ("$")>().potential = '$';
        } else
        {
            value () += *i;
        }
    }
    
    void addResultFromChild (string const& res){
        value () += res;
    }
    
    void finish () {
        
    }
    
    virtual void reset_hasNoParent (){
        potential().clear();
        variable().clear();
        value().clear();
        TRANSITION ("@(){} done")
    }
    virtual void reset_hasParent (){
        removeFromParent ();
    }
};

template <>
struct STATE ("@(){} done") : STATE ("done")
{
    virtual void _process (iter i) {
        if (*i != '\n')
            STATE ("done")::_process (i);
    }
};












