#pragma once

#include "fifo.h"

#include <memory>
#include <cmath>

namespace collections {

  template <class T>
  class contiguous_fifo : public fifo<T> {
  public:
    contiguous_fifo();
    contiguous_fifo(const contiguous_fifo<T>& other);
    T& operator[](int index) override;
    const T& operator[](int index) const override;
    void add(const T &value) override;
    contiguous_fifo<T> operator+(const fifo<T> &other) const;


  private:
    typename fifo<T>::iterator_impl __get_it_impl(size_t index) const override;

    std::unique_ptr<T[]> data;
  };

  #include "contiguous_fifo.i.h"
}
