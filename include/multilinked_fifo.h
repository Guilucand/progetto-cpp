#pragma once

#include "fifo.h"

#include <memory>

namespace collections {

  /// Classe che rappresenta una forward linked-list con piu' elementi
  /// per ciascun nodo della lista
  template <class T, size_t _BucketSize>
  class multilinked_fifo : public fifo<T> {
  public:
    multilinked_fifo();
    ~multilinked_fifo() override;
    multilinked_fifo(const multilinked_fifo<T, _BucketSize>& other);
    T& operator[](size_t index) override;
    const T& operator[](size_t index) const override;
    void add(const T &value) override;
    multilinked_fifo<T, _BucketSize> operator+(const fifo<T> &other) const;

  private:
    typename fifo<T>::iterator_impl __get_it_impl(size_t index) const override;

    struct element {
      std::array<T, _BucketSize> data;
      element *next;
    };

    element* data;
    element* last;
  };

  #include "impl/multilinked_fifo.i.h"
}
