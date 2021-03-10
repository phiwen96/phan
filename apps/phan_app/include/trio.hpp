#pragma once

template <class T0, class T1, class T2>
struct trio
{
    T0 t0;
    T1 t1;
    T2 t2;
    
    auto first () const -> T0 const& {
        return t0;
    }
    
    auto second () const -> T1 const& {
        return t1;
    }
    
    auto third () const -> T2 const& {
        return t2;
    }
    
    auto first () -> T0& {
        return t0;
    }
    
    auto second () -> T1& {
        return t1;
    }
    
    auto third () -> T2& {
        return t2;
    }
};

template <class T0, class T1, class T2>
trio (T0, T1, T2) -> trio <T0, T1, T2>;
