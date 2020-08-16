namespace ezg {

    template<typename T>
    Flag<T> Flag<T>::set(T _rhs)
    {
        m_flag = static_cast<T>(static_cast<underT>(m_flag) | static_cast<underT>(_rhs));
        return *this;
    }

    template<typename T>
    Flag<T> Flag<T>::unset(T _rhs)
    {
        m_flag = static_cast<T>(static_cast<underT>(m_flag) ^ static_cast<underT>(_rhs));
        return *this;
    }

    template<typename T>
    bool Flag<T>::test(T _rhs)
    {
        return static_cast<underT>(m_flag) & static_cast<underT>(_rhs);
    }

    template<typename T>
    Flag<T>& Flag<T>::operator = (T _rhs)
    {
        m_flag = _rhs;
        return *this;
    }

    template<typename T>
    bool Flag<T>::operator == (Flag<T> _rhs)
    {
        return m_flag == _rhs.get();
    }

    template<typename T>
    bool Flag<T>::operator == (T _rhs)
    {
        return m_flag == _rhs;
    }

    template<typename T>
    bool Flag<T>::operator != (Flag<T> _rhs)
    {
        return !(*this == _rhs);
    }

    template<typename T>
    bool Flag<T>::operator != (T _rhs)
    {
        return !(*this == _rhs);
    }

    template<typename T>
    Flag<T> Flag<T>::operator | (Flag<T> _rhs)
    {
        return m_flag | _rhs.get();
    }

    template<typename T>
    Flag<T>& Flag<T>::operator |= (T _rhs)
    {
        set(_rhs);
        return *this;
    }

    template<typename T>
    Flag<T>& Flag<T>::operator |= (Flag<T> _rhs)
    {
        *this = *this | _rhs;
        return *this;
    }

    template<typename T>
    Flag<T> Flag<T>::operator & (Flag<T> _rhs)
    {
        return m_flag & _rhs.get();
    }

    template<typename T>
    Flag<T>& Flag<T>::operator &= (T _rhs)
    {
        *this = *this & _rhs;
        return *this;
    }

    template<typename T>
    Flag<T>& Flag<T>::operator &= (Flag<T> _rhs)
    {
        *this = *this & _rhs;
        return *this;
    }

    ///////////////////////////////////////////////////////////////

    template<typename T>
    Flag<T> operator | (T _lhs, T _rhs) {
        return Flag<T>(static_cast<T>(static_cast<typename Flag<T>::underT>(_lhs) | static_cast<typename Flag<T>::underT>(_rhs)));
    }
    template<typename T>
    Flag<T> operator | (Flag<T> _lhs, T _rhs) {
        return Flag<T>(static_cast<T>(static_cast<typename Flag<T>::underT>(_lhs.get()) | static_cast<typename Flag<T>::underT>(_rhs)));
    }
    template<typename T>
    Flag<T> operator | (T _lhs, Flag<T> _rhs) {
        return Flag<T>(static_cast<T>(static_cast<typename Flag<T>::underT>(_lhs) | static_cast<typename Flag<T>::underT>(_rhs.get())));
    }


    template<typename T>
    Flag<T> operator & (T _lhs, T _rhs) {
        return Flag<T>(static_cast<T>(static_cast<typename Flag<T>::underT>(_lhs) & static_cast<typename Flag<T>::underT>(_rhs)));
    }
    template<typename T>
    Flag<T> operator & (Flag<T> _lhs, T _rhs) {
        return Flag<T>(static_cast<T>(static_cast<typename Flag<T>::underT>(_lhs.get()) & static_cast<typename Flag<T>::underT>(_rhs)));
    }
    template<typename T>
    Flag<T> operator & (T _lhs, Flag<T> _rhs) {
        return Flag<T>(static_cast<T>(static_cast<typename Flag<T>::underT>(_lhs) & static_cast<typename Flag<T>::underT>(_rhs.get())));
    }
}