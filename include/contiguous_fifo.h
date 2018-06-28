#pragma once

#include "fifo.h"

#include <memory>
#include <cmath>

namespace collections {

  /// Classe che rappresenta una vettore contiguo di elementi in memoria
  template <class T>
  class contiguous_fifo : public fifo<T> {
  public:
    contiguous_fifo();
    ~contiguous_fifo() override;

    contiguous_fifo(const contiguous_fifo<T>& other);
    T& operator[](size_t index) override;
    const T& operator[](size_t index) const override;
    void add(const T &value) override;
    contiguous_fifo<T> operator+(const fifo<T> &other) const;


  private:
    typename fifo<T>::iterator_impl __get_it_impl(size_t index) const override;

    T *data = nullptr;
    int lookup_pos = 0;
  };

  #include "impl/contiguous_fifo.i.h"
} // namespace collections
