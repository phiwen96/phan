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



#define TRANSITION(x) transition <STATE (x)> (ctx);

#define BASE_STATE State <>


using iter = string::iterator;






struct Context;
template <char...>
struct State;

template <>
struct BASE_STATE
{
//    Context* context;
    
    void process (iter i, Context& ctx);
    virtual void _process (iter i, Context& ctx){}
    bool hasParent (Context& ctx) const;
    string& variable (Context& ctx);
    string& value (Context& ctx);
    string& result (Context& ctx);
    string& potential (Context& ctx);
    string& paste (Context& ctx);
    void removeFromParent (Context& ctx);
    template <class T> void transition (Context& ctx);
    template <class T> Context& addChildContext (Context& ctx);
    State* parent (Context& ctx);
    void declare (string const& var, string const& val, Context& ctx);
    optional <string> declared (string const&, Context& ctx);
    virtual void addResultFromChild (string const& res, Context& ctx);
    void reset (Context& ctx);
    virtual void reset_hasNoParent (Context& ctx){}
    virtual void reset_hasParent (Context& ctx){}
    
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


void BASE_STATE::declare (string const& var, string const& val, Context& ctx) {
    auto declared = ctx.declaredVariables.begin ();
    for (; declared < ctx.declaredVariables.end (); ++declared) {
        if (declared -> first == var) {
            declared -> second = val;
            return;
        }
    }
    ctx.declaredVariables.emplace_back (var, val);
}
optional <string> BASE_STATE::declared (string const& p, Context& ctx) {
    for (auto d = ctx.declaredVariables.begin (); d != ctx.declaredVariables.end(); ++d) {
        if (d -> first == p) {
            return d->second;
        }
    }
    return {};
}

BASE_STATE* BASE_STATE::parent (Context& ctx) {
    return ctx.parent -> state;
}

void BASE_STATE::reset (Context& ctx){
    if (not hasParent(ctx)) {
        reset_hasNoParent(ctx);
    } else {
        reset_hasParent(ctx);
    }
}

void Context::process(iter i) {
    state -> process (i, *this);
}

void BASE_STATE::addResultFromChild (string const& res, Context& ctx) {
    
}


void BASE_STATE::process (iter i, Context& ctx) {
    if (ctx.children.empty ()) {
//        ctx.curr_it = i;
        _process (i, ctx);
    } else
    {
        for (auto& o : ctx.children)
            o -> process (i);
    }
}

bool BASE_STATE::hasParent (Context& ctx) const {
    return ctx.parent != nullptr;
}

void BASE_STATE::removeFromParent (Context& ctx) {
    for (auto cont = ctx.parent -> children.begin(); cont < ctx.parent -> children.end(); ++cont) {
        if (*cont == &ctx) {
//            cout << "removing child context from parent context" << endl;
            ctx.parent -> children.erase (cont);
            break;
        }
    }
}




template <class state>
Context& BASE_STATE::addChildContext (Context& ctx) {
    State<>* childState = new state;
    Context* childContext = new Context {&ctx, ctx.declaredVariables, childState};
//    childState -> context = childContext;
    ctx.children.push_back (childContext);
    return *childContext;
}
//optional <string> BASE_STATE::declared () {
//    for (auto d = ctx.declaredVariables.begin (); d != ctx.declaredVariables.end(); ++d) {
//        if (d -> first == ctx.variable) {
//            return optional{d->second};
//        }
//    }
//    return {};
//}
string& BASE_STATE::variable (Context& ctx) {
    return ctx.variable;
}
string& BASE_STATE::value (Context& ctx) {
    return ctx.value;
}
string& BASE_STATE::result (Context& ctx) {
    return ctx.result;
}
string& BASE_STATE::potential (Context& ctx) {
    return ctx.potential;
}
string& BASE_STATE::paste (Context& ctx) {
    return ctx.paste;
}




template <>
struct STATE ("") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        switch (*i)
        {
            case '$':
                potential (ctx) += '$';
                TRANSITION ("$")
                break;
                
            case '#':
                potential (ctx) += '#';
                TRANSITION ("#")
                break;
                
            case '@':
                potential (ctx) += '@';
                TRANSITION ("@")
                break;
                
            default:
                result (ctx) += *i;
                break;
        }

    }
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){}
    virtual void reset_hasParent (Context& ctx){}
};


template <>
struct STATE ("$") : BASE_STATE
{
    static STATE ("$") instance;
    virtual void _process (iter i, Context& ctx){
        
        potential (ctx) += *i;
        
        if (*i == '(')
        {
            TRANSITION ("$(")

        } else if (*i == '{')
        {
            
            TRANSITION ("${")

        } else
        {
            reset (ctx);
        }
        
   
    
    }
    virtual void addResultFromChild (string const& res, Context& ctx){
        potential(ctx) += res;
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        result (ctx) += potential(ctx);
        potential(ctx).clear ();
        variable (ctx).clear ();
        value (ctx).clear ();
        TRANSITION ("done")
    }
    virtual void reset_hasParent (Context& ctx){
        parent (ctx) -> addResultFromChild (potential (ctx), ctx);
        removeFromParent (ctx);
    }
};

template <>
struct STATE ("#") : BASE_STATE
{
    static STATE ("#") instance;
    virtual void _process (iter i, Context& ctx){
        potential (ctx) += *i ;
        if (*i == '{')
        {
            TRANSITION ("#{")

        } else
        {
            reset (ctx);

        }
        
    }
    virtual void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        result (ctx) += potential (ctx);
        potential (ctx).clear ();
        TRANSITION ("")
    }
    virtual void reset_hasParent (Context& ctx){
        addResultFromChild (potential (ctx));
        removeFromParent (ctx);
    }
};


template <>
struct STATE ("$(") : BASE_STATE
{
    static STATE ("$(") instance;
    virtual void _process (iter i, Context& ctx){
        potential (ctx) += *i;
        if (*i == ')')
        {
//            variable (ctx) = string (potential(ctx).begin() + 2, potential(ctx).end());
//            potential (ctx) += ')';
            TRANSITION ("$()")
            
        } else if (*i == '$')
        {
            addChildContext <STATE ("$")> (ctx).potential = '$';

        } else
        {
            variable (ctx) += *i;
//            potential (ctx) += *i;

        }
       
    }
    virtual void addResultFromChild (string const& res, Context& ctx){
        variable (ctx) += res;
    }
    
    virtual void reset_hasNoParent (Context& ctx){}
    virtual void reset_hasParent (Context& ctx){}
};




template <>
struct STATE ("$()") : BASE_STATE
{
    static STATE ("$()") instance;
    virtual void _process (iter i, Context& ctx){
        
        if (*i == '{')
        {
            ctx.bracketStack.push ('{');
            TRANSITION ("$(){")
            
        } else if (*i == '$')
        {
            addChildContext <STATE ("$")> (ctx).potential = '$';
            
        } else
        {
            reset (ctx);

        }
    }
    virtual void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        result (ctx) += potential (ctx);
        potential (ctx).clear ();
        variable (ctx).clear ();
        value (ctx).clear ();
        TRANSITION ("")
        
        potential (ctx).clear ();
        variable (ctx).clear ();
    }
    virtual void reset_hasParent (Context& ctx){
        parent (ctx) -> addResultFromChild (potential (ctx), ctx);
        removeFromParent (ctx);
        
        potential (ctx).clear ();
        variable (ctx).clear ();
    }
};


template <>
struct STATE ("$(){") : BASE_STATE
{
   
    virtual void _process (iter i, Context& ctx){
        potential (ctx) += *i;
        if (*i == '}')
        {
//            ctx.bracketStack.pop ();
//
//            if (ctx.bracketStack.empty ())
//            {
            bool found = false;
            cout << "var: " << variable(ctx) << endl << "val: " << value(ctx) << endl;
            for (auto j = ctx.declaredVariables.begin(); j < ctx.declaredVariables.end(); ++j){
                if (j->first == variable(ctx)){
//                    value(ctx) = j->second;
                    j->second = value (ctx);
                    cout << "found:" << j->first << " = " << j->second << endl;
                    found = true;
                    break;
                }
            }
            if (not found){
                cout << "found:" << variable(ctx) << " = " << value(ctx) << endl;
                ctx.declaredVariables.emplace_back(variable(ctx), value(ctx));
                cout << "v::" << value(ctx) << endl;
            }
            if (hasParent(ctx))
            {
                addResultFromChild(value(ctx), ctx);
                removeFromParent(ctx);
            } else {
                result (ctx) += value (ctx);
                variable (ctx).clear();
                value (ctx).clear();
                potential (ctx).clear();
                TRANSITION ("done")
            }
//                declare (variable (ctx), value (ctx), ctx);
//                reset(ctx);
//            } else
//            {
//                value (ctx) += '}';
//            }
            
        }
//        else if (*i == '{')
//        {
//            ctx.bracketStack.push ('{');
//            value(ctx) += *i;
//
//        }
        else if (*i == '$')
        {
            addChildContext <STATE ("$")> (ctx).potential += '$';

        } else
        {
            value (ctx) += *i;
        }
        
    }
    virtual void addResultFromChild (string const& res, Context& ctx){
        value (ctx) += res;
    }
    void finish () {
        
    }
    virtual void reset_hasNoParent (Context& ctx){
        result (ctx) += value (ctx);
        potential(ctx).clear();
        value(ctx).clear();
        variable(ctx).clear();
        TRANSITION ("")
    }
    virtual void reset_hasParent (Context& ctx){
        parent (ctx) -> addResultFromChild (value (ctx), ctx);
        removeFromParent (ctx);
    }
};

//STATE ("$(){")* STATE ("$(){")::a = new STATE ("$(){");









template <>
struct STATE ("done") : STATE ("")
{
    virtual void _process (iter i, Context& ctx){
        STATE ("")::_process (i, ctx);
    }
    
    virtual void reset_hasNoParent (Context& ctx){}
    virtual void reset_hasParent (Context& ctx){}
};

template <>
struct STATE ("${") : BASE_STATE
{
    virtual void _process (iter i, Context& ctx){
        
        potential(ctx) += *i;
        
        
        
        if (*i == '}')
        {
//            optional <string> decl = declared (variable (ctx), ctx);
            for (auto d = ctx.declaredVariables.begin (); d != ctx.declaredVariables.end(); ++d) {
                if (d -> first == variable(ctx)) {
//                    return d->second;
                    cout << "::" << d->second << endl;
                    if (hasParent(ctx))
                    {
    //                    cout << "kuk" << endl;
                        ctx.state -> addResultFromChild (d->second, ctx);
                        removeFromParent(ctx);
                    } else {
                        result(ctx) += d->second;
                        cout << result(ctx) << endl;
                        potential(ctx).clear();
                        value(ctx).clear();
                        variable(ctx).clear();
                        paste(ctx).clear();
                        TRANSITION ("${} done")
                    }
                    return;
                    
                }
            }
           
                string warning = "variable \"" + variable (ctx) + "\" pasted but it has not yet been declared!";
                //            cout << result (ctx) << endl;
                throw runtime_error (warning);
            
        } else if (*i == '$')
        {
            addChildContext<STATE ("$")>(ctx).potential = '$';
            
        } else
        {
            variable (ctx) += *i;
            cout << *i << endl;
        }
    }
    
    virtual void addResultFromChild (string const& res, Context& ctx){
        variable (ctx) += res;
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        result(ctx) += value (ctx);
        potential(ctx).clear();
        value(ctx).clear();
        variable(ctx).clear();
        paste(ctx).clear();
        TRANSITION ("${} done")
    }
    virtual void reset_hasParent (Context& ctx){
        parent (ctx) -> addResultFromChild (value (ctx), ctx);
        removeFromParent (ctx);
    }
 
};


template <>
struct STATE ("${} done"): STATE ("done")
{
    virtual void _process (iter i, Context& ctx){
        if (*i != '\n')
        {
            STATE ("done")::_process (i, ctx);
        } 
    }
    
    virtual void reset_hasNoParent (Context& ctx){}
    virtual void reset_hasParent (Context& ctx){}
 
};

template <>
struct STATE ("#{") : BASE_STATE
{
    virtual void _process (iter i, Context& ctx){
        potential(ctx) += *i;
        if (*i == '}') {
            reset (ctx);
        }
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        potential(ctx).clear();
        transition<STATE ("#{} done")>(ctx);
    }
    virtual void reset_hasParent (Context& ctx){
        removeFromParent (ctx);
    }
 
};

template <>
struct STATE ("#{} done") : STATE ("done")
{
    virtual void _process (iter i, Context& ctx) {
        if (*i != '\n')
            STATE ("done")::_process (i, ctx);
    }
};

template <>
struct STATE ("@") : BASE_STATE
{
    virtual void _process (iter i, Context& ctx){
        potential(ctx) += *i;
        
        switch (*i)
        {
            case '(':
                TRANSITION ("@(")
                break;
                
            default:
                reset (ctx);
                break;
        }
    }
    virtual void reset_hasNoParent(Context& ctx){
        result (ctx) += potential (ctx);
        potential (ctx).clear ();
        TRANSITION ("")
    }
    virtual void reset_hasParent(Context& ctx){
        addResultFromChild (potential (ctx), ctx);
        removeFromParent (ctx);
    }
 
};

template <>
struct STATE ("@(") : BASE_STATE
{
    virtual void _process (iter i, Context& ctx){
        
        potential(ctx) += *i;
        
        if (*i == ')')
        {
            TRANSITION ("@()")
            
        } else if (*i == '$')
        {
            addChildContext<STATE ("$")>(ctx).potential = '$';

        } else
        {
            variable (ctx) += *i;
        }
        
    }
    virtual void addResultFromChild (string const& res, Context& ctx){
        variable (ctx) += res;
    }
    virtual void reset_hasNoParent (Context& ctx){}
    virtual void reset_hasParent (Context& ctx){}
 
};

template <>
struct STATE ("@()") : BASE_STATE
{
    virtual void _process (iter i, Context& ctx){
        potential(ctx) += *i;
        
        if (*i == '{')
        {
            TRANSITION ("@(){")
        } else
        {
            reset (ctx);
        }
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        result(ctx) += potential(ctx);
        potential(ctx).clear();
        TRANSITION ("")
    }
    virtual void reset_hasParent (Context& ctx){
        addResultFromChild (potential (ctx), ctx);
        removeFromParent (ctx);
    }
 
};

template <>
struct STATE ("@(){") : BASE_STATE
{
    virtual void _process (iter i, Context& ctx){
        
        potential (ctx) += *i;
        
        if (*i == '}')
        {
            declare (variable (ctx), value (ctx), ctx);
            reset (ctx);
            
        } else if (*i == '$')
        {
            addChildContext <STATE ("$")>(ctx).potential = '$';
        } else
        {
            value (ctx) += *i;
        }
    }
    
    virtual void addResultFromChild (string const& res, Context& ctx){
        value (ctx) += res;
    }
    
    void finish () {
        
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        potential(ctx).clear();
        variable(ctx).clear();
        value(ctx).clear();
        TRANSITION ("@(){} done")
    }
    virtual void reset_hasParent (Context& ctx){
        removeFromParent (ctx);
    }
};

template <>
struct STATE ("@(){} done") : STATE ("done")
{
    virtual void _process (iter i, Context& ctx) {
        if (*i != '\n')
            STATE ("done")::_process (i, ctx);
    }
};







//STATE ("") a0;
//STATE ("#") a1;
//STATE ("#{") a2;
//STATE ("$") a3;
//STATE ("${") a4;
//STATE ("${} done") a5;
//STATE ("done") a6;
//STATE ("@(){} done") a7;
//STATE ("@") a8;
//STATE ("@(") a9;
//STATE ("@()") a10;
//STATE ("@(){") a11;
//STATE ("$(") a12;
//STATE ("$()") a13;
//STATE ("$(){") a14;





template <class T>
void BASE_STATE::transition (Context& ctx) {
//    T* newstate = new T;
//    newstate -> context = context;
    
    cout << "transitioning from " << typeid (*ctx.state).name () << " to ";
//    auto a = get<T>(states);
//    if constexpr (is_same_v<T, decltype (a0)>)
//        context -> state = &a0;
//    if constexpr (is_same_v<T, decltype (a1)>)
//        context -> state = &a1;
//    if constexpr (is_same_v<T, decltype (a2)>)
//        context -> state = &a2;
//    if constexpr (is_same_v<T, decltype (a3)>)
//        context -> state = &a3;
//    if constexpr (is_same_v<T, decltype (a4)>)
//        context -> state = &a4;
//    if constexpr (is_same_v<T, decltype (a5)>)
//        context -> state = &a5;
//    if constexpr (is_same_v<T, decltype (a6)>)
//        context -> state = &a6;
//    if constexpr (is_same_v<T, decltype (a7)>)
//        context -> state = &a7;
//    if constexpr (is_same_v<T, decltype (a8)>)
//        context -> state = &a8;
//    if constexpr (is_same_v<T, decltype (a9)>)
//        context -> state = &a9;
//    if constexpr (is_same_v<T, decltype (a10)>)
//        context -> state = &a10;
//    if constexpr (is_same_v<T, decltype (a11)>)
//        context -> state = &a11;
//    if constexpr (is_same_v<T, decltype (a12)>)
//        context -> state = &a11;
//    if constexpr (is_same_v<T, decltype (a13)>)
//        context -> state = &a13;
//    if constexpr (is_same_v<T, decltype (a14)>)
//        context -> state = &a14;
    
    ctx.state = new T;
    
//    ctx.state -> context = context;
    
    cout << typeid (*ctx.state).name () << endl;
}


