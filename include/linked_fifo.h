#pragma once

#include "fifo.h"

#include <memory>

namespace collections {

  template <class T>
  class linked_fifo : public fifo<T> {
  public:
    linked_fifo();
    ~linked_fifo();
    linked_fifo(const linked_fifo<T>& other);
    T& operator[](int index) override;
    const T& operator[](int index) const override;
    void add(const T &value) override;
    linked_fifo<T> operator+(const fifo<T> &other) const;

  private:
    typename fifo<T>::iterator_impl __get_it_impl(size_t index) const override;


    struct element {
      T data;
      element *next;
    };

    element* data;
    element* last;
  };

  #include "linked_fifo.i.h"
}
