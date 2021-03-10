#pragma once
using namespace std;
#include "trio.hpp"


struct extractor;

struct extractor_state
{
    
    extractor* m_ext;
//    int m_curr;
    
    
    virtual bool found (char){}
    
};


struct extractor_state_first_begin : extractor_state
{
//    using extractor_state::extractor_state;
    virtual bool found (char c) override;
};

struct extractor_state_first_end : extractor_state
{
//    using extractor_state::extractor_state;
    virtual bool found (char c) override;
};

struct extractor_state_second_begin : extractor_state
{
//    using extractor_state::extractor_state;
    virtual bool found (char c) override;
};

struct extractor_state_second_end : extractor_state
{
//    using extractor_state::extractor_state;
    virtual bool found (char c) override;
};


struct extractor
{
    string const& m_first;
    string const& m_second;
    int m_curr;
    int m_watched;
    int m_first_begin_found;
    int m_first_end_found;
    int m_second_begin_found;
    int m_second_end_found;
    
    void reset ()
    {
        m_curr = 0;
        m_watched = 0;
        m_first_begin_found = -1;
        m_first_end_found = -1;
        m_second_begin_found = -1;
        m_second_end_found = -1;
        delete m_state;
        m_state = new extractor_state_first_begin;
        m_state -> m_ext = this;
    }
    
    auto found (char c) -> optional <tuple <int, int, int, int>>
    {
        if (m_state -> found (c))
            return {tuple {m_first_begin_found, m_first_end_found, m_second_begin_found, m_second_end_found}};
        else
            return {};
    }
    
    extractor_state* m_state;
    
    extractor (string const& first, string const& second) : m_first (first), m_second (second), m_state (new extractor_state_first_begin), m_curr (0), m_watched (0), m_first_begin_found (-1), m_first_end_found (-1), m_second_begin_found (-1)
    {
        m_state -> m_ext = this;
    }
    
    void change_state (extractor_state* newstate)
    {
//        std::cout << "Context: Transition from " << typeid(*m_state).name() << " to "  << typeid(*newstate).name() << "\n";
        delete m_state;
        newstate -> m_ext = this;
        m_state = newstate;
    }
};






bool extractor_state_first_begin::found (char c)
{
    ++m_ext -> m_watched;
    
    if (m_ext -> m_first [0] == c) {
        m_ext -> m_first_begin_found = m_ext -> m_watched - 1;
//        cout << "extractor_state_first_end::found(" << c << ") == true" << endl;
        ++m_ext -> m_curr;
        
        if (m_ext -> m_curr == m_ext -> m_first.size()) {
            m_ext -> m_first_end_found = m_ext -> m_watched;
            m_ext -> m_curr = 0;
            m_ext -> change_state (new extractor_state_second_begin);
        } else
        {
            m_ext -> change_state (new extractor_state_first_end);
        }
        
//        ++m_ext -> m_watched;
    }
    else {
//        cout << "extractor_state_first_end::found(" << c << ") == false" << endl;

    }
    
    
    return false;
}

bool extractor_state_first_end::found (char c)
{
    ++m_ext -> m_watched;
    
    if (m_ext -> m_first [m_ext -> m_curr] == c)
    {
        m_ext -> m_first_end_found = m_ext -> m_watched;
//        cout << "extractor_state_first_end::found(" << c << ") == true" << endl;
        ++m_ext -> m_curr;
        if (m_ext -> m_curr == m_ext -> m_first.size())
        {
            m_ext -> m_curr = 0;
            
            m_ext -> change_state (new extractor_state_second_begin);
        }
    }
    else
    {
        m_ext -> m_curr = 0;
        m_ext -> change_state (new extractor_state_first_begin);
//        cout << "extractor_state_first_end::found(" << c << ") == false" << endl;
    }
    return false;
}

bool extractor_state_second_begin::found (char c)
{
    ++m_ext -> m_watched;
//    cout << "second_begin if " << c << " == " << m_ext -> m_second [m_ext -> m_curr] << endl;
    if (m_ext -> m_second [m_ext -> m_curr] == c)
    {
//        m_ext -> m_second = m_ext -> m_watched;
//        cout << "extractor_state_second_begin::parse(" << c << ") == true" << endl;

        ++m_ext -> m_curr;
//        cout << m_ext -> m_second[m_ext -> m_watched-1] << endl;
        m_ext -> m_second_begin_found = m_ext -> m_watched;
        
        if (m_ext -> m_curr == m_ext -> m_second.size())
        {
//            cout << "if " << m_ext -> m_curr << " == " << m_ext -> m_second.size() << endl;
            m_ext -> m_second_end_found = m_ext -> m_watched;
            return true;
            
        } else
        {
            m_ext -> change_state (new extractor_state_second_end);
        }
        
    } else
    {
        
//        cout << "extractor_state_second_begin::parse(" << c << ") != " << m_ext -> m_second [m_ext -> m_curr] << endl;
    }
    return false;
}


bool extractor_state_second_end::found (char c)
{
//    cout << "second_end if " << c << " == " << m_ext -> m_second [m_ext -> m_curr] << endl;
    ++m_ext -> m_watched;
    
    if (m_ext -> m_second [m_ext -> m_curr] == c) {
//        m_ext -> m_second = m_ext -> m_watched;
//        cout << "extractor_state_second_begin::parse(" << c << ") == true" << endl;

        

        if (m_ext -> m_curr + 1 == m_ext -> m_second.size()) {
            m_ext -> m_second_end_found = m_ext -> m_watched;
            return true;
        }
        ++m_ext -> m_curr;
    }
    else
    {
        
//        cout << "extractor_state_second_begin::parse(" << c << ") != " << m_ext -> m_second [m_ext -> m_curr] << endl;
        m_ext -> change_state (new extractor_state_first_begin);
    }
    return false;
}
