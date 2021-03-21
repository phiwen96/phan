#pragma once

template <bool, class...>
struct option;

template <bool b, class T>
struct option <b, T>
{
    inline static constexpr bool value = false;
    
};

template <class T>
struct option <true, T>
{
    inline static constexpr bool value = true;
    using type = T;
};

template <>
struct option <false>
{
    inline static constexpr bool value = false;
};


template <bool b, class T>
using option_t = typename option <b, T>::type;

template <bool b, class T>
constexpr bool option_v = option <b, T>::value;

template <bool b, class c, class d>
struct IIf;

template <class c, class d>
struct IIf <true, c, d>
{
    using type = c;
};


template <class c, class d>
struct IIf <false, c, d>
{
    using type = d;
};


template <bool b, class c, class d>
using if_t = typename IIf <b, c, d>::type;





template <class T, T const& t>
struct Test
{
    
};

template <class... Rest>
struct _Find;

template <class Fun, class T, class... Rest>
struct _Find <Fun, T, Rest...>
{
    using type = if_t <Fun::template accept <T> (), option <true, T>, typename _Find <Fun, Rest...>::type>;
//    using type =  __builtin_choose_expr (Fun::template accept <T> (), option <true, T>, typename _Find <Fun, Rest...>::type);
};

template <class Fun, class T, class... Rest>
using Find = typename _Find <Fun, T, Rest...>::type;
/**
 example
 
 struct K
 {
     template <class T>
     static consteval bool accept ()
     {
         return is_same_v <T, E>;
 //        return true;
     }
 };
 
 cout << Find <K, E, A>::value << endl;
 */



template <class Fun>
struct _Find <Fun>
{
    using type = option <false>;
};


template <int, int, class...>
struct _For;


template <int I, int N, class T, class... Rest>
struct _For <I, N, T, Rest...>
{
    
    static constexpr int Do (auto a)
    {
        if (a (T{}))
            return N;
        return _For <I + 1, N, Rest...>::Do (a);
        cout << "hi" << endl;
    }
};

template <int I, int N>
struct _For <I, N>
{
    
    static constexpr int Do (auto a)
    {
        return -1;
    }
    
    
};

template <int N, class... Rest>
using For = _For <0, N, Rest...>;









namespace {
template <size_t N, size_t M> consteval bool same_strings (char const(&lhs)[N], char const(&rhs)[M]){
    if (N != M)
        return false;
    for (int i = 0; i < N; ++i)
    {
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}
#define SAME_TOKEN(x, y) __builtin_choose_expr (same_strings (BOOST_PP_STRINGIZE (x), BOOST_PP_STRINGIZE (y)), 1, 0)
#define macro(x) __builtin_choose_expr (SAME_TOKEN (x, hej), kuk, "jo")
}
namespace {
consteval auto my_function_name (char const* s = __builtin_FUNCTION ()) {
    return s;
}
#define MY_FUNCTION_NAME my_function_name ()
}
