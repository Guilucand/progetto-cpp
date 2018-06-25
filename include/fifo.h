#pragma once
#include <algorithm>
#include <vector>

namespace collections {
  template <class T>
  class fifo {

  protected:

    class iterator_impl {
    public:
      void (*increment)(iterator_impl &_this);
      bool (*equals)(const iterator_impl &_this, const iterator_impl &other);
      T & (*get)(iterator_impl &_this);
      struct _pos {
        size_t index;
        void *ptr;
      } pos;
      fifo<T> &reference;
    };

  private:
    template <class I>
    class iterator_base {
    public:
      I operator++();
      I operator++(int);
      bool operator !=(iterator_base const& other) const;
      bool operator ==(iterator_base const& other) const;

    protected:
      iterator_base(iterator_impl &&impl);
      iterator_impl _impl;
    };

  public:
    fifo() = default;
    fifo(const fifo& other) = default;

    virtual T& operator[](int index) = 0;
    const virtual T& operator[](int index) const = 0;

    virtual void add(const T &value) = 0;
    size_t size() const;
    size_t get_allocated() const;

    class iterator : public iterator_base<iterator> {
    public:
      T& operator *();

    private:
      iterator(iterator_impl &&impl);
      friend class fifo<T>;
    };

    class const_iterator : public iterator_base<const_iterator> {
    public:
      const T& operator *();

    private:
      const_iterator(iterator_impl &&impl);
      friend class fifo<T>;
    };

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

  protected:
    virtual iterator_impl __get_it_impl(size_t index) const = 0;
    size_t _size = 0;
    size_t _reserved = 0;
  };

  template <class T>
  std::pair<typename fifo<T>::iterator,
            typename fifo<T>::iterator> first_mismatch(fifo<T> &first,
                                                       fifo<T> &second);

#include "fifo.i.h"
}
