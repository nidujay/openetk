#ifndef BUFFPTR_H
#define BUFFPTR_H

#include <iterator>
#include <stdexcept>
#include <cstddef>

/**
 * \cond IGNORE_DOCS
 */
template < typename T>
struct buffptr
{
 // types:
    typedef       T                               value_type;
    typedef       T&                              reference;
    typedef const T&                              const_reference;
    typedef       T*                              iterator;
    typedef const T*                              const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t                                size_type;
    typedef ptrdiff_t                             difference_type;

    // fields:
private:
    T*        store;
    const size_type count;

    // helper functions:
    void check(size_type n)
        { if ( n >= count ) throw std::out_of_range("buffptr"); }

    // This is not a container so disable features that make it look like so
    buffptr();
    const buffptr operator=(const buffptr&);
    buffptr(const buffptr& d);

public:
    // construct and destruct:
    buffptr(T *data, size_type c)
    : store(data), count(c)
    {
    }

    ~buffptr()
    {
    }

    // iterators:
    iterator       begin()        { return store; }
    const_iterator begin()  const { return store; }
    const_iterator cbegin() const { return store; }
    iterator       end()          { return store + count; }
    const_iterator end()    const { return store + count; }
    const_iterator cend()   const { return store + count; }

    reverse_iterator       rbegin()
        { return reverse_iterator(end()); }
    const_reverse_iterator rbegin()  const
        { return reverse_iterator(end()); }
    reverse_iterator       rend()
        { return reverse_iterator(begin()); }
    const_reverse_iterator rend()    const
        { return reverse_iterator(begin()); }

    // capacity:
    size_type size()     const { return count; }
    size_type max_size() const { return count; }
    bool      empty()    const { return false; }

    // element access:
    reference       operator[](size_type n)       { return at(n); }
    const_reference operator[](size_type n) const { return at(n); }

    reference       front()       { return store[0]; }
    const_reference front() const { return store[0]; }
    reference       back()        { return store[count-1]; }
    const_reference back()  const { return store[count-1]; }

    const_reference at(size_type n) const { check(n); return store[n]; }
    reference       at(size_type n)       { check(n); return store[n]; }

    // data access:
    T*       data()       { return store; }
    const T* data() const { return store; }
};

/**
 * \endcond
 */
#endif

