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


struct extractor_state_begin : extractor_state
{
//    using extractor_state::extractor_state;
    virtual bool found (char c) override;
};

struct extractor_state_first : extractor_state
{
//    using extractor_state::extractor_state;
    virtual bool found (char c) override;
};

struct extractor_state_second : extractor_state
{
//    using extractor_state::extractor_state;
    virtual bool found (char c) override;
};


struct extractor
{
    string m_first;
    string m_second;
    int m_curr;
    int m_watched;
    int m_first_begin_found;
    int m_first_end_found;
    int m_second_found;
    
    auto found (char c) -> optional <trio <int, int, int>>
    {
        if (m_state -> found (c))
            return {trio {m_first_begin_found, m_first_end_found, m_second_found}};
        else
            return {};
    }
    
    extractor_state* m_state;
    
    extractor (string const& first, string const& second) : m_first (first), m_second (second), m_state (new extractor_state_begin), m_curr (0), m_watched (0), m_first_begin_found (-1), m_first_end_found (-1), m_second_found (-1)
    {
        m_state -> m_ext = this;
    }
    
    void change_state (extractor_state* newstate)
    {
        std::cout << "Context: Transition from " << typeid(*m_state).name() << " to "  << typeid(*newstate).name() << "\n";
        delete m_state;
        newstate -> m_ext = this;
        m_state = newstate;
    }
};






bool extractor_state_begin::found (char c)
{
    if (m_ext -> m_first [0] == c) {
        m_ext -> m_first_begin_found = m_ext -> m_watched++;
//        cout << "extractor_state_first::found(" << c << ") == true" << endl;
        ++m_ext -> m_curr;
        
        if (m_ext -> m_curr == m_ext -> m_first.size()) {
            m_ext -> m_first_end_found = m_ext -> m_watched;
            m_ext -> m_curr = 0;
            m_ext -> change_state (new extractor_state_second);
        } else
        {
            m_ext -> change_state (new extractor_state_first);
        }
        
//        ++m_ext -> m_watched;
    }
    else {
        ++m_ext -> m_watched;
//        cout << "extractor_state_first::found(" << c << ") == false" << endl;

    }
    
    
    return false;
}

bool extractor_state_first::found (char c)
{
    if (m_ext -> m_first [m_ext -> m_curr] == c) {
        m_ext -> m_first_end_found = ++m_ext -> m_watched;
//        cout << "extractor_state_first::found(" << c << ") == true" << endl;
        ++m_ext -> m_curr;
        if (m_ext -> m_curr == m_ext -> m_first.size()) {
            m_ext -> m_curr = 0;
            
            m_ext -> change_state (new extractor_state_second);
        }

        
    }
    else {
        m_ext -> m_curr = 0;
        m_ext -> change_state (new extractor_state_begin);
//        cout << "extractor_state_first::found(" << c << ") == false" << endl;
        ++m_ext -> m_watched;
    }
    return false;
}

bool extractor_state_second::found (char c)
{
    if (m_ext -> m_second [m_ext -> m_curr] == c) {
        m_ext -> m_second = m_ext -> m_watched;
//        cout << "extractor_state_second::parse(" << c << ") == true" << endl;

        ++m_ext -> m_curr;
        ++m_ext -> m_watched;
        if (m_ext -> m_curr == m_ext -> m_second.size()) {
            m_ext -> m_second_found = m_ext -> m_watched;
            return true;
        }
        
    }
    else
    {
//        cout << "extractor_state_second::parse(" << c << ") != " << m_ext -> m_second [m_ext -> m_curr] << endl;
        ++m_ext -> m_watched;
    }
    return false;
}
