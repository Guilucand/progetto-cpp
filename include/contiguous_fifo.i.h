

template <class T>
contiguous_fifo<T>::contiguous_fifo() {
}

template <class T>
contiguous_fifo<T>::contiguous_fifo(const contiguous_fifo<T>& other) : fifo<T>(other) {
  data = std::unique_ptr<T[]>((T*)operator new(other._reserved * sizeof(T)));
  for (int i = 0; i < other._size; i++) {
    (*this)[i] = other[i];
  }
}

template <class T>
T& contiguous_fifo<T>::operator[](int index) {
  return data[index];
}

template <class T>
const T& contiguous_fifo<T>::operator[](int index) const {
  return data[index];
}

template <class T>
void contiguous_fifo<T>::add(const T &value) {
  if (this->_size >= this->_reserved) {
    if (this->_reserved)
      this->_reserved *= 2;
    else
      this->_reserved = 1;
    auto tmp_ptr = std::unique_ptr<T[]>((T*)operator new(this->_reserved * sizeof(T)));
    for (int i = 0; i < this->_size; i++) {
      tmp_ptr[i] = data[i];
    }
    data = std::move(tmp_ptr);
  }
  data[this->_size++] = value;
}

template <class T>
contiguous_fifo<T> contiguous_fifo<T>::operator+(const fifo<T> &other) const {
  contiguous_fifo<T> result;
  for (T value : *this) {
    result.add(value);
  }
  for (T value : other) {
    result.add(value);
  }
  return result;
}

template <class T>
typename fifo<T>::iterator_impl contiguous_fifo<T>::__get_it_impl(size_t index) const {
  return typename fifo<T>::iterator_impl {
    [](typename fifo<T>::iterator_impl &_this) {
      _this.index++;
    },
    [](const typename fifo<T>::iterator_impl &_this,
       const typename fifo<T>::iterator_impl &_other) {
      return _this.index == _other.index;
    },
    [](typename fifo<T>::iterator_impl &_this) -> T& {
      return reinterpret_cast<contiguous_fifo<T>&>(_this.reference).data[_this.index];
    },
    {
      index,
      nullptr
    },
    *const_cast<contiguous_fifo<T>*>(this)
  };
}
