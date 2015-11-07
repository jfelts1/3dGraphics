#ifndef TOGGLE_H
#define TOGGLE_H
#include <functional>
#include <iostream>

template<typename func1,typename func2,typename ret>
class Toggle
{
public:
    Toggle()=default;
    Toggle(func1 f1,func2 f2)
    {
        m_func1 = f1;
        m_func2 = f2;
        m_curFunc = 1;
    }
    ~Toggle()=default;
    template<typename... args>
    ret toggle(args... ar)
    {
        if(m_curFunc == 1)
        {
            m_curFunc = 2;
            return m_func2(ar...);
        }
        else
        {
            m_curFunc = 1;
            return m_func1(ar...);
        }
    }

private:

    func1 m_func1;
    func2 m_func2;
    int m_curFunc;
};

#endif // TOGGLE_H
