

const int LOOKUP_SIZE = 10;


constexpr size_t next_allocation(size_t current) {
    int log = 0;

    size_t tmp = current;
    while (tmp > 0) {
        log++;
        tmp /= 2;
    }

    return current * 2 * log;
}


constexpr std::array<size_t, LOOKUP_SIZE> lookup_table = []() constexpr
{
    std::array<size_t, LOOKUP_SIZE> arr = std::array<size_t, LOOKUP_SIZE>();
    arr[0] = 2;
    for (int i = 1; i < LOOKUP_SIZE; i++)
        arr[i] = next_allocation(arr[i - 1]);

    return arr;
}();


template <class T>
contiguous_fifo<T>::contiguous_fifo() {
}

template <class T>
contiguous_fifo<T>::~contiguous_fifo() {
    if (data) {
        for (size_t i = 0; i < this->_size; i++) {
          data[i].~T();
        }
        this->_size = 0;
        operator delete(data);
        data = nullptr;
    }
}

template <class T>
contiguous_fifo<T>::contiguous_fifo(const contiguous_fifo<T>& other) : fifo<T>(other) {
  data = reinterpret_cast<T*>(operator new(other._reserved * sizeof(T)));
  for (size_t i = 0; i < other._size; i++) {
    (*this)[i] = other[i];
  }
}

template <class T>
T& contiguous_fifo<T>::operator[](size_t index) {
  return data[index];
}

template <class T>
const T& contiguous_fifo<T>::operator[](size_t index) const {
  return data[index];
}

template <class T>
void contiguous_fifo<T>::add(const T &value) {
  if (this->_size >= this->_reserved) {
    if (this->lookup_pos >= LOOKUP_SIZE)
      this->_reserved = next_allocation(this->_reserved);
    else
      this->_reserved = lookup_table[this->lookup_pos++];

    auto tmp_ptr = reinterpret_cast<T*>(operator new(this->_reserved * sizeof(T)));
    for (size_t i = 0; i < this->_size; i++) {
      tmp_ptr[i] = std::move(data[i]);
      data[i].~T();
    }
    if (data)
        operator delete(data);
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
      _this.pos.index++;
    },
    [](const typename fifo<T>::iterator_impl &_this,
       const typename fifo<T>::iterator_impl &_other) {
      return _this.pos.index == _other.pos.index;
    },
    [](typename fifo<T>::iterator_impl &_this) -> T& {
      return reinterpret_cast<contiguous_fifo<T>&>(_this.reference).data[_this.pos.index];
    },
    {
      index,
      nullptr
    },
    *const_cast<contiguous_fifo<T>*>(this)
  };
}
