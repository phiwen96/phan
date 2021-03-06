#pragma once


#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <const_str/const_str.hpp>

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


//#define STR(s) \
//        STRING_TO_CHARS(100, s)

//#define S(s) STR (s)//STR (BOOST_PP_STRINGIZE (s))

#define S(s) <STR (s)>

#define STATE(x) State <STR (x)>//S(x)



#define TRANSITION(x) transition <STATE (x)> (ctx);

#define BASE_STATE State <>

#define DECLPASTE '$'

#define DECL '@'



using iter = string::iterator;






struct Context;
template <class...>
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
    virtual string trans(){
//        throw runtime_error ("");
        return "";
    }
    string transi (Context& ctx);
    void chainChildren (iter i, Context& ctx);
};

struct Context
{
    Context* parent {nullptr};
    vector <pair <string, string>>& declaredVariables;
    State<>* state {nullptr};
    vector <Context*> children;
    stack <char> bracketStack;
    
    inline static string result = "";
    string spaces {""};
    string variable;
    string value;
    string potential;
    string paste;
    string firstint{""};
    string secondint{""};
    string intvariable{""};
    string loop{""};
    
    void process (iter);
};


void BASE_STATE::declare (string const& var, string const& val, Context& ctx) {
//    throw runtime_error ("");
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
    throw runtime_error ("");
    for (auto d = ctx.declaredVariables.begin (); d != ctx.declaredVariables.end(); ++d) {
        if (d -> first == p) {
            return d->second;
        }
    }
    return {};
}

BASE_STATE* BASE_STATE::parent (Context& ctx) {
#if defined (Debug)
    if (ctx.parent == nullptr or ctx.parent -> state == nullptr)
        throw runtime_error ("");
#endif
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
#if defined (Debug)
    if (ctx.parent == nullptr || ctx.parent -> state == nullptr)
        throw runtime_error ("");
#endif
    ctx.parent -> state -> addResultFromChild (res, *ctx.parent);
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
    return ctx.parent != nullptr and ctx.parent->state != nullptr;
}

void BASE_STATE::removeFromParent (Context& ctx) {
#if defined (Debug)
    for (int i = 0; i < ctx.parent->children.size (); ++i) {
        if (ctx.parent->children[i] == &ctx) {
//            cout << "removing child context from parent context" << endl;
            ctx.parent -> children.erase (ctx.parent->children.begin() + i);
//            ctx.parent -> children.
            delete this;
            return;
        }
    }
    throw runtime_error ("");
#else
    for (auto cont = ctx.parent -> children.begin(); cont < ctx.parent -> children.end(); ++cont) {
        if (*cont == &ctx) {
//            cout << "removing child context from parent context" << endl;
            ctx.parent -> children.erase (cont);
            break;
        }
    }
#endif
    
}




template <class Q>
Context& BASE_STATE::addChildContext (Context& ctx) {
    cout << endl;
    State<>* childState = new Q;
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
string BASE_STATE::transi (Context& ctx) {
    if (hasParent (ctx)){
        return ctx.parent->state->transi(*ctx.parent) + "::" + trans();
    } else
    {
        return trans();
    }
}
void BASE_STATE::chainChildren (iter i, Context& ctx) {
#if defined (Debug)
    if (ctx.children.empty ())
        throw runtime_error ("");
#endif
    for (auto& c : ctx.children)
    {
        c->process(i);
    }
}

template <>
struct STATE ("$(x") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        potential (ctx) += *i;
        if (isnumber (*i))
        {
            ctx.firstint += *i;
//            TRANSITION ("")
        } else if (*i == ' ')
        {
            TRANSITION ("$(x ");
        } else
        {
            if (hasParent (ctx))
            {
                BASE_STATE::addResultFromChild (potential (ctx), ctx);
                potential (ctx).clear ();
                ctx.firstint.clear ();
                ctx.secondint.clear ();
                ctx.intvariable.clear ();
                TRANSITION ("begin");
            } else
            {
                result (ctx) += potential (ctx);
                potential (ctx).clear ();
                ctx.firstint.clear ();
                ctx.secondint.clear ();
                ctx.intvariable.clear ();
                TRANSITION ("begin");
            }
        }
    }
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "$(x";
    }
};

template <>
struct STATE ("$(x ") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        ctx.potential += *i;
        if (isnumber (*i))
        {
            throw runtime_error ("");
        } else
        {
            ctx.intvariable += *i;
            TRANSITION ("$(x var");
        }
    }
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "$(x ";
    }
};

template <>
struct STATE ("$(x var") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        ctx.potential += *i;
        if (*i == ' ')
        {
            TRANSITION ("$(x var ");
            
        } else
        {
            ctx.intvariable += *i;
        }
    }
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "$(x var";
    }
};

template <>
struct STATE ("$(x var ") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        ctx.potential += *i;
        if (isnumber (*i))
        {
            ctx.secondint += *i;
            TRANSITION ("$(x var y")
        } else
        {
            if (hasParent (ctx))
            {
                BASE_STATE::addResultFromChild (potential (ctx), ctx);
                potential (ctx).clear ();
                ctx.firstint.clear ();
                ctx.secondint.clear ();
                ctx.intvariable.clear ();
                TRANSITION ("begin");
            } else
            {
                result (ctx) += potential (ctx);
                potential (ctx).clear ();
                ctx.firstint.clear ();
                ctx.secondint.clear ();
                ctx.intvariable.clear ();
                TRANSITION ("begin");
            }
        }
    }
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "$(x var y";
    }
};

template <>
struct STATE ("$(x var y") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        ctx.potential += *i;
        if (isnumber (*i))
        {
            ctx.secondint += *i;
        } else if (*i == ')')
        {
            TRANSITION ("$(x var y)")
        } else
        {
            if (hasParent (ctx))
            {
                BASE_STATE::addResultFromChild (potential (ctx), ctx);
                potential (ctx).clear ();
                ctx.firstint.clear ();
                ctx.secondint.clear ();
                ctx.intvariable.clear ();
                TRANSITION ("begin");
            } else
            {
                result (ctx) += potential (ctx);
                potential (ctx).clear ();
                ctx.firstint.clear ();
                ctx.secondint.clear ();
                ctx.intvariable.clear ();
                TRANSITION ("begin");
            }
        }
    }
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "$(x var y";
    }
};

template <>
struct STATE ("$(x var y)") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        ctx.potential += *i;
        if (*i == '{')
        {
            ctx.bracketStack.push ('{');
            TRANSITION ("$(x var y){")
        } else
        {
            if (hasParent (ctx))
            {
                BASE_STATE::addResultFromChild (potential (ctx), ctx);
                potential (ctx).clear ();
                ctx.firstint.clear ();
                ctx.secondint.clear ();
                ctx.intvariable.clear ();
                TRANSITION ("begin");
//                removeFromParent (ctx);
            } else
            {
                result (ctx) += potential (ctx);
                potential (ctx).clear ();
                ctx.firstint.clear ();
                ctx.secondint.clear ();
                ctx.intvariable.clear ();
                TRANSITION ("begin");
            }
        }
    }
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "$(x var y)";
    }
};




template <>
struct STATE ("$(x var y){") : BASE_STATE
{
    void _process (iter i, Context& ctx)
    {
        ctx.potential += *i;
        
        if (*i == '}')
        {
            ctx.bracketStack.pop ();
            
            if (ctx.bracketStack.empty ())
            {
                int i = stoi (ctx.firstint);
                int end = stoi (ctx.secondint);
                
                for (; i < end; ++i)
                {
                    declare (ctx.intvariable, to_string (i), ctx);
//                    cout << ctx.intvariable << to_string (i) << endl;
//                    cout << "bajs::" << ctx.value << endl;
//                    addChildContext <STATE ("begin")> (ctx);
//                    string temp = ctx.value;
//                    ctx.value.clear ();
                    auto* childstate = new BASE_STATE;
                    Context* childctx = new Context {&ctx, ctx.declaredVariables, childstate};
                    childstate -> transition <STATE ("begin")> (*childctx);
//                    ctx.children.push_back (childctx);
    //                addChildContext <STATE ("begin")> (ctx);
                    for (iter j = ctx.value.begin (); j < ctx.value.end (); ++j)
                    {
                        
                        childctx -> process (j);
//                        chainChildren (j, ctx);
                    }
                    
//                    cout << "kuk" << endl;
                    delete childstate;
                    delete childctx;
                }
                
                if (hasParent (ctx))
                {
                    BASE_STATE::addResultFromChild (ctx.loop, ctx);
                    potential (ctx).clear ();
                    ctx.variable.clear ();
                    ctx.value.clear ();
                    ctx.firstint.clear ();
                    ctx.secondint.clear ();
                    ctx.intvariable.clear ();
                    ctx.loop.clear ();
                    ctx.bracketStack = stack <char> {};
                    TRANSITION ("begin")
//                    removeFromParent (ctx);
                } else
                {
                    result (ctx) += ctx.loop;
                    potential (ctx).clear ();
                    ctx.variable.clear ();
                    ctx.value.clear ();
                    ctx.firstint.clear ();
                    ctx.secondint.clear ();
                    ctx.intvariable.clear ();
                    ctx.loop.clear ();
                    ctx.bracketStack = stack <char> {};
                    TRANSITION ("begin");
                }
            }
            else
            {
                ctx.value += *i;
            }
            
            
//            cout << "DONE" << endl;
        }
        else if (*i == '{')
        {
            ctx.value += *i;
            ctx.bracketStack.push ('{');
        }
//        else if (*i == ' ')
//        {
////            ctx.spaces += ' ';
//            cout << "tab" << endl;
//        }
//        else if (*i == '$')
//        {
//
//
//        }
        else
        {
            ctx.value += *i;
        }
            
    }
    
    void addResultFromChild (string const& res, Context& ctx) {
        ctx.loop += res;
//        ctx.value += res;
//        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "$(x var y){";
    }
};

template <>
struct STATE ("${") : BASE_STATE
{
    virtual void _process (iter i, Context& ctx){
//        cout << *i << endl;
        if (*i == '}')
        {
            
//            optional <string> decl = declared (variable (ctx), ctx);
            for (auto& d : ctx.declaredVariables)
            {
                if (d.first == variable (ctx)) {
//                    return d->second;
//                    cout << "::" << d->second << endl;
                    if (hasParent(ctx))
                    {
//                        cout << "kuk::${" << endl;
                        BASE_STATE::addResultFromChild (d.second, ctx);
//                        cout << "adding result \"" << d.second << "\" from ${ to parent" << endl;
                        potential(ctx).clear();
                        value(ctx).clear();
                        variable(ctx).clear();
                        paste(ctx).clear();
//                        removeFromParent (ctx);
                        TRANSITION ("begin")
                    } else {
                        
                        result(ctx) += d.second;
//                        cout << "adding result \"" << d.second << "\" from ${ to parent" << endl;

//                        cout << result(ctx) << endl;
                        potential(ctx).clear();
                        value(ctx).clear();
                        variable(ctx).clear();
                        paste(ctx).clear();
                        TRANSITION ("done")
                    }
                    return;
                }
            }
           
                string warning = "variable \"" + variable (ctx) + "\" pasted but it has not yet been declared!";
                //            cout << result (ctx) << endl;
                throw runtime_error (warning);
            
        } else if (*i == DECLPASTE)
        {
            addChildContext<STATE ("$")>(ctx).potential = *i;
            
        } else if (*i == '@')
        {
            addChildContext<STATE ("@")>(ctx).potential = '@';
            
        } else if (*i == '#')
        {
            addChildContext<STATE ("#")>(ctx).potential = '#';
            
        } else
        {
            variable (ctx) += *i;
            potential(ctx) += *i;
//            cout << *i << endl;
        }
    }
    
    virtual void addResultFromChild (string const& res, Context& ctx){
//        cout << "getting result \"" << res << "\" to ${" << endl;
        variable (ctx) += res;
        potential (ctx) += res;
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
        BASE_STATE::addResultFromChild (value (ctx), ctx);
        removeFromParent (ctx);
    }
    virtual string trans (){
        return "${";
    }
 
};



template <>
struct STATE ("begin") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        
        if (*i == DECLPASTE)
        {
            potential (ctx) += *i;
            TRANSITION ("$")
            
        } else if (*i == '#')
        {
            potential (ctx) += '#';
            TRANSITION ("#")
            
        } else if (*i == '@')
        {
            potential (ctx) += '@';
            TRANSITION ("@")
            
        } else
        {
            if (hasParent (ctx))
            {
                BASE_STATE::addResultFromChild (string {*i}, ctx);
                
            } else
            {
                result (ctx) += *i;
            }
        }
       

    }
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "\"\"";
    }
};


template <>
struct STATE ("$") : BASE_STATE
{

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
        TRANSITION ("begin")
    }
    virtual void reset_hasParent (Context& ctx){
        BASE_STATE::addResultFromChild (potential (ctx), ctx);
        TRANSITION ("begin")
//        removeFromParent (ctx);
    }
    virtual string trans (){
        return "$";
    }
};




template <>
struct STATE ("$(") : BASE_STATE
{

    virtual void _process (iter i, Context& ctx){
        
        if (*i == ')')
        {
            potential (ctx) += *i;
//            variable (ctx) = string (potential(ctx).begin() + 2, potential(ctx).end());
//            potential (ctx) += ')';
            TRANSITION ("$()")
            
        } else if (*i == DECLPASTE)
        {
            addChildContext <STATE ("$")> (ctx).potential = *i;

        } else if (isnumber (*i))
        {
            ctx.potential += *i;
            ctx.firstint = *i;
            TRANSITION ("$(x")
            
        } else
        {
            variable (ctx) += *i;
            potential (ctx) += *i;
//            potential (ctx) += *i;

        }
       
    }
    virtual void addResultFromChild (string const& res, Context& ctx){
        variable (ctx) += res;
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "$(";
    }
};




template <>
struct STATE ("$()") : BASE_STATE
{

    virtual void _process (iter i, Context& ctx){
        
        if (*i == '{')
        {
            potential (ctx) += '{';
            ctx.bracketStack.push ('{');
            TRANSITION ("$(){")
            
        } else if (*i == DECLPASTE)
        {
            addChildContext <STATE ("$")> (ctx).potential = *i;
            
        } else
        {
            potential (ctx) += *i;
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
        ctx.bracketStack = stack <char> {};
        TRANSITION ("begin")
        
        
    }
    virtual void reset_hasParent (Context& ctx){
        BASE_STATE::addResultFromChild (potential (ctx), ctx);
        potential (ctx).clear ();
        variable (ctx).clear ();
        value (ctx).clear ();
        ctx.bracketStack = stack <char> {};
        TRANSITION ("begin")
//        removeFromParent (ctx);
    }
    virtual string trans (){
        return "$()";
    }
};



template <>
struct STATE ("$(){") : BASE_STATE
{
   
    virtual void _process (iter i, Context& ctx){
        
       
        
        if (*i == '}')
        {
//            ctx.bracketStack.pop ();
//
//            if (ctx.bracketStack.empty ())
//            {
            bool found = false;
//            cout << "var: " << variable(ctx) << endl << "val: " << value(ctx) << endl;
            for (auto& j : ctx.declaredVariables){
                if (j.first == variable(ctx)){
//                    value(ctx) = j->second;
                    j.second = value (ctx);
//                    cout << "found:" << j->first << " = " << j->second << endl;
                    found = true;
                    break;
                }
            }
//            optional <string> found = declared (ctx.variable, ctx);
            if (not found){
//                cout << "found:" << variable(ctx) << " = " << value(ctx) << endl;
                ctx.declaredVariables.emplace_back(variable(ctx), value(ctx));
//                cout << "v::" << value(ctx) << endl;
            }
            if (hasParent(ctx))
            {
//                BASE_STATE::addResultFromChild(value(ctx), ctx);
                BASE_STATE::addResultFromChild (value(ctx), ctx);
                variable (ctx).clear();
                value (ctx).clear();
                potential (ctx).clear();
                ctx.bracketStack = stack <char> {};
//                removeFromParent(ctx);
//                TRANSITION ("begin")
            } else {
                result (ctx) += value (ctx);
                variable (ctx).clear();
                value (ctx).clear();
                potential (ctx).clear();
                ctx.bracketStack = stack <char> {};
//                TRANSITION ("done")
            }
            TRANSITION ("done")
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
        else if (*i == DECLPASTE)
        {
            addChildContext <STATE ("$")> (ctx).potential = *i;

        } else if (*i == '@')
        {
            addChildContext <STATE ("@")> (ctx).potential = '@';

        } else if (*i == '#')
        {
            addChildContext <STATE ("#")> (ctx).potential = '#';

        } else
        {
            value (ctx) += *i;
            potential (ctx) += *i;
        }
        
    }
    virtual void addResultFromChild (string const& res, Context& ctx){
        value (ctx) += res;
//        cout << "kuk::$(){" << endl;
//        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        result (ctx) += value (ctx);
        potential(ctx).clear();
        value(ctx).clear();
        variable(ctx).clear();
        TRANSITION ("begin")
        throw runtime_error ("oops");
    }
    virtual void reset_hasParent (Context& ctx){
        BASE_STATE::addResultFromChild (value (ctx), ctx);
        removeFromParent (ctx);
        throw runtime_error ("oops");
    }
    virtual string trans (){
        return "$(){";
    }
};

//STATE ("$(){")* STATE ("$(){")::a = new STATE ("$(){");









template <>
struct STATE ("done") : STATE ("begin")
{
    virtual void _process (iter i, Context& ctx){
        STATE ("begin")::_process (i, ctx);
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void addResultFromChild (string const& res, Context& ctx){
        throw runtime_error ("");
    }
};




template <>
struct STATE ("${} done"): STATE ("done")
{
    virtual void _process (iter i, Context& ctx){
//        if (*i != '\n')
//        {
            STATE ("done")::_process (i, ctx);
//        }
    }
    virtual void addResultFromChild (string const& res, Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "${} done";
    }
 
};


template <>
struct STATE ("#") : BASE_STATE
{
    virtual void _process (iter i, Context& ctx){
        
        potential (ctx) += *i ;
        
        if (*i == '{')
        {
            ctx.bracketStack.push ('{');
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
        TRANSITION ("begin")
    }
    virtual void reset_hasParent (Context& ctx){
        BASE_STATE::addResultFromChild (potential (ctx), ctx);
        removeFromParent (ctx);
    }
    virtual string trans (){
        return "#";
    }
};



template <>
struct STATE ("#{") : BASE_STATE
{
    virtual void _process (iter i, Context& ctx){
        potential(ctx) += *i;
        if (*i == '}') {
            ctx.bracketStack.pop ();
            if (ctx.bracketStack.empty ())
            {
                reset (ctx);
            }

        } else if (*i == '{')
        {
            ctx.bracketStack.push ('{');
        }
    }
    
    virtual void addResultFromChild (string const& res, Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasNoParent (Context& ctx){
        potential(ctx).clear();
        transition<STATE ("#{} done")>(ctx);
    }
    virtual void reset_hasParent (Context& ctx){
        removeFromParent (ctx);
    }
    virtual string trans (){
        return "#{";
    }
 
};

template <>
struct STATE ("#{} done") : STATE ("done")
{
    virtual void _process (iter i, Context& ctx) {
        if (*i != '\n')
            STATE ("done")::_process (i, ctx);
    }
    virtual void addResultFromChild (string const& res, Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "#{} done";
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
        TRANSITION ("begin")
    }
    virtual void reset_hasParent(Context& ctx){
        BASE_STATE::addResultFromChild (potential (ctx), ctx);
        potential (ctx).clear ();
        TRANSITION ("begin")
//        removeFromParent (ctx);
    }
    virtual string trans (){
        return "@";
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
            
        } else if (*i == DECLPASTE)
        {
            addChildContext<STATE ("$")>(ctx).potential = DECLPASTE;

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
    virtual string trans (){
        return "@(";
    }
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
        TRANSITION ("begin")
    }
    virtual void reset_hasParent (Context& ctx){
        BASE_STATE::addResultFromChild (potential (ctx), ctx);
        potential (ctx).clear ();
        TRANSITION ("begin")
//        removeFromParent (ctx);
    }
    virtual string trans (){
        return "@()";
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
            
        } else if (*i == DECLPASTE)
        {
            addChildContext <STATE ("$")>(ctx).potential = *i;
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
        TRANSITION ("done")
    }
    virtual void reset_hasParent (Context& ctx){
        potential (ctx).clear ();
        variable(ctx).clear();
        value(ctx).clear();
        TRANSITION ("done")
//        removeFromParent (ctx);
    }
    virtual string trans (){
        return "@(){";
    }
};

template <>
struct STATE ("@(){} done") : STATE ("done")
{
    virtual void _process (iter i, Context& ctx) {
        if (*i != '\n')
        {
            STATE ("done")::_process (i, ctx);
            TRANSITION ("done")
        }
        else
        {
            TRANSITION ("done")
        }
            
        
    }
    virtual string trans (){
        return "@(){} done";
    }
};







template <class T>
void BASE_STATE::transition (Context& ctx) {
    
//    cout << transi(ctx) << " -> ";

    
    ctx.state = new T;
    
    
//    cout << ctx.state->transi(ctx) << endl;
//    cout << ctx.result << endl;
}




/**
 
 ${fornamn${fornamn}}
 
 




try
   {
       int i = std::stoi(s);
       std::cout << i << '\n';
   }
   catch (std::invalid_argument const &e)
   {
       std::cout << "Bad input: std::invalid_argument thrown" << '\n';
   }
   catch (std::out_of_range const &e)
   {
       std::cout << "Integer overflow: std::out_of_range thrown" << '\n';
   }




${hej}
$(fornamn){Philip}
${fornamn}
$(kiss){bajs}
$(snopp){${fornamn}}
$(namn){${fornamn}$(efternamn){Wenkel}}



*/
