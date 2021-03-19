#pragma once


template <bool b, class T>
struct option
{
    inline static constexpr bool value = false;
    
};

template <class T>
struct option <true, T>
{
    inline static constexpr bool value = true;
    using type = T;
};
template <bool b, class T>
using option_t = typename option <b, T>::type;

template <bool b, class T>
constexpr bool option_v = option <b, T>::value;



template <bool b, class c, class d>
struct If
{
    using type = c;
};


template <class c, class d>
struct If <false, c, d>
{
    using type = d;
};


template <bool b, class c, class d>
using if_t = typename If <b, c, d>::type;





template <class T, T const& t>
struct Test
{
    
};


template <class Fun, class T, class... Rest>
struct For_eac
{
    using type = if_t <Fun::template visit <T> (), option <true, T>, typename For_eac <Fun, Rest...>::type>;
};

template <class Fun, class T, class... Rest>
using For_each = typename For_eac <Fun, T, Rest...>::type;



template <class Fun, class T>
struct For_eac <Fun, T>
{
    using type = option <false, T>;
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




