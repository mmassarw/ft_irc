#pragma once

namespace ft
{
    template<class T>
    class uniquePtr;

    template<class T>
    class sharedPtr;

    template<class T>
    class weakPtr;

    template <class T>
    class uniquePtr
    {
    public:
        uniquePtr(T *ptr = NULL);
        ~uniquePtr() throw();

        T *release() throw();
        void reset(T *nptr = NULL) throw();

        T *get() const;
        T &operator*();
        T *operator->();
        operator bool() const;

    private:
        T *ptr;
        uniquePtr<T> &operator=(const uniquePtr<T> &);
        uniquePtr(const uniquePtr<T> &);
    };

    template <class T>
    struct ControlBlock
    {
        size_t shared_nb;
        size_t weak_nb;
        T *ptr;
        ControlBlock(T *ptr = NULL);
    };

    template <class T>
    class sharedPtr
    {
    public:
        sharedPtr(T *ptr = NULL);
        sharedPtr(const sharedPtr<T> &old);
        sharedPtr(const weakPtr<T> &old);
        ~sharedPtr() throw();

        void reset() throw();
        void reset(T *nptr);

        T *get() const;
        T &operator*();
        T *operator->();
        operator bool() const;

    private:
        ControlBlock<T> *shared;
        sharedPtr &operator=(const sharedPtr<T> &);

        friend class weakPtr<T>;
    };

    template <class T>
    class weakPtr
    {
    public:
        weakPtr(const sharedPtr<T> &old);
        weakPtr(const weakPtr<T> &old);
        ~weakPtr() throw();

        sharedPtr<T> lock() const throw();
        void reset() throw();

    private:
        ControlBlock<T> *shared;
        weakPtr &operator=(const weakPtr<T> &);

        friend class sharedPtr<T>;
    };
}
