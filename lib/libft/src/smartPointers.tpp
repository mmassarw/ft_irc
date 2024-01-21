namespace ft {
    template <class T>
    uniquePtr<T>::uniquePtr(T *ptr): ptr(ptr) {}

    template <class T>
    uniquePtr<T>::~uniquePtr() throw()
    {
        delete ptr;
    }

    template <class T>
    T *uniquePtr<T>::release() throw()
    {
        T *r = ptr;
        ptr = NULL;
        return r;
    }

    template <class T>
    void uniquePtr<T>::reset(T *nptr) throw()
    {
        delete ptr;
        ptr = nptr;
    }

    template <class T>
    T *uniquePtr<T>::get() const
    {
        return ptr;
    }

    template <class T>
    T &uniquePtr<T>::operator*()
    {
        return *ptr;
    }

    template <class T>
    T *uniquePtr<T>::operator->()
    {
        return ptr;
    }

    template <class T>
    uniquePtr<T>::operator bool() const
    {
        return (ptr);
    }

    //shared pointer c++ 98
    template <class T>
    ControlBlock<T>::ControlBlock(T *ptr): shared_nb(1), weak_nb(0), ptr(ptr) {}

    template <class T>
    sharedPtr<T>::sharedPtr(T *ptr)
    {
        shared = new ControlBlock<T>(ptr);
    }

    template <class T>
    sharedPtr<T>::sharedPtr(const sharedPtr<T> &old)
    {
        shared = old.shared;
        ++shared->shared_nb;
    }

    template <class T>
    sharedPtr<T>::sharedPtr(const weakPtr<T> &old)
    {
        shared = old.shared;
        ++shared->shared_nb;
    }

    template <class T>
    sharedPtr<T>::~sharedPtr() throw()
    {
        reset();
    }

    template <class T>
    void sharedPtr<T>::reset() throw()
    {
        if (!--shared->shared_nb)
        {
            delete shared->ptr;
            if (!shared->weak_nb)
                delete shared;
        }
        shared = NULL;
    }

    template <class T>
    void sharedPtr<T>::reset(T *nptr)
    {
        reset();
        if (nptr)
            shared = new ControlBlock<T>(nptr);
    }

    template <class T>
    T *sharedPtr<T>::get() const
    {
        return shared ? shared->ptr : NULL;
    }

    template <class T>
    T &sharedPtr<T>::operator*()
    {
        return *get();
    }

    template <class T>
    T *sharedPtr<T>::operator->()
    {
        return get();
    }

    template <class T>
    sharedPtr<T>::operator bool() const
    {
        return get();
    }

    //weak pointer c++ 98
    template <class T>
    weakPtr<T>::weakPtr(const sharedPtr<T> &old)
    {
        shared = old.shared;
        ++shared->weak_nb;
    }

    template <class T>
    weakPtr<T>::weakPtr(const weakPtr<T> &old)
    {
        shared = old.shared;
        ++shared->weak_nb;
    }

    template <class T>
    weakPtr<T>::~weakPtr() throw()
    {
        reset();
    }

    template <class T>
    sharedPtr<T> weakPtr<T>::lock() const throw()
    {
        return shared->shared_nb ? sharedPtr<T>(*this) : sharedPtr<T>(NULL);
    }

    template <class T>
    void weakPtr<T>::reset() throw()
    {
        if (!--shared->weak_nb && !shared->shared_nb)
            delete shared;
    }
}