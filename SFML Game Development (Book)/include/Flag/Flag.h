#pragma once

#include <type_traits>

namespace ezg {

    template<typename T>
    class Flag
    {
    public:
        //find out the type of enum members
        using underT = typename std::underlying_type<T>::type;

        explicit Flag(T _flag = T()) noexcept
            : m_flag(_flag)
        {
            static_assert(std::is_enum<T>::value, "is not enum type");
        }

        //type conversion operator to enum<T>
        explicit operator T() { return m_flag; }
        //type conversion operator to bool
        //bug:
        //    It allowed a meaningless expression: if(flag1 | flag2) {...}
        explicit operator bool() { return (static_cast<underT>(m_flag) != 0); }

    public:

        Flag<T> set(T _rhs);
        T       get() { return m_flag; }
        Flag<T> unset(T _rhs);

        bool test(T _rhs);

        Flag<T>& operator = (T _rhs);

        bool operator == (Flag<T> _rhs);
        bool operator == (T _rhs);
        bool operator != (Flag<T> _rhs);
        bool operator != (T _rhs);

        Flag<T>  operator |  (Flag<T> _rhs);
        Flag<T>& operator |= (T _rhs);
        Flag<T>& operator |= (Flag<T> _rhs);

        Flag<T>  operator &  (Flag<T> _rhs);
        Flag<T>& operator &= (T _rhs);
        Flag<T>& operator &= (Flag<T> _rhs);


    private:

        T m_flag;
    };

    template<typename T>
    Flag<T> operator | (T _lhs, T _rhs);
    template<typename T>
    Flag<T> operator | (Flag<T> _lhs, T _rhs);
    template<typename T>
    Flag<T> operator | (T _lhs, Flag<T> _rhs);


    template<typename T>
    Flag<T> operator & (T _lhs, T _rhs);
    template<typename T>
    Flag<T> operator & (Flag<T> _lhs, T _rhs);
    template<typename T>
    Flag<T> operator & (T _lhs, Flag<T> _rhs);

}

#include "Flag.inl"
