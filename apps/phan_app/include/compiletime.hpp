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



template <class Fun>
struct _Find <Fun>
{
    using type = option <false>;
};


template <int N, class T, class... Rest>
struct For
{
    
    static int Do (auto a)
    {
        if (a (T{}))
            return N;
        return For <N - 1, Rest...>::Do (a);
        cout << "hi" << endl;
    }
};

template <int N, class T>
struct For <N, T>
{
    
    static int Do (auto a)
    {
        if (a (T{}))
            return N;
        return -1;
    }
    
    
};




