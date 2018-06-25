


template <class T, size_t _BucketSize>
multilinked_fifo<T, _BucketSize>::multilinked_fifo() {
  data = last = nullptr;
  this->_reserved = this->_size = 0;
}

template <class T, size_t _BucketSize>
multilinked_fifo<T, _BucketSize>::~multilinked_fifo() {
  element *next = data;
  for (size_t i = 0; i < this->_reserved; i+=_BucketSize) {
    element *current = next;
    next = next->next;
    delete current;
  }
  this->_reserved = this->_size = 0;
}

template <class T, size_t _BucketSize>
multilinked_fifo<T, _BucketSize>::multilinked_fifo(const multilinked_fifo<T, _BucketSize>& other) : fifo<T>(other) {
  data = last = nullptr;
  this->_reserved = this->_size = 0;
  for (auto el : other) {
    add(el);
  }
}

template <class T, size_t _BucketSize>
T& multilinked_fifo<T, _BucketSize>::operator[](int index) {
  element *current = data;

  for (int i = 0; i < index; i+= _BucketSize)
    current = current->next;

  return current->data[index % _BucketSize];
}

template <class T, size_t _BucketSize>
const T& multilinked_fifo<T, _BucketSize>::operator[](int index) const {
  element *current = data;

  for (int i = 0; i < index; i+= _BucketSize)
    current = current->next;

  return current->data[index % _BucketSize];
}

template <class T, size_t _BucketSize>
void multilinked_fifo<T, _BucketSize>::add(const T &value) {

  if (this->_reserved == 0) {
    data = last = new element { {value}, nullptr };
    this->_reserved = this->_size = 1;
    return;
  }

  if (this->_size % _BucketSize == 0) {
    last->next = new element { {value}, nullptr };
    last = last->next;
  }

  last->data[this->_size++ % _BucketSize] = value;
  this->_reserved = this->_size;
}

template <class T, size_t _BucketSize>
multilinked_fifo<T, _BucketSize> multilinked_fifo<T, _BucketSize>::operator+(const fifo<T> &other) const {
  multilinked_fifo<T, _BucketSize> result;
  for (T value : *this) {
    result.add(value);
  }
  for (T value : other) {
    result.add(value);
  }
  return result;
}

template <class T, size_t _BucketSize>
typename fifo<T>::iterator_impl multilinked_fifo<T, _BucketSize>::__get_it_impl(size_t index) const {

  element* current;
  if (index == 0) {
    current = data;
  }
  else if (index == this->size()) {
    current = nullptr;
  }
  else {
    current = data;
    for (size_t i = 0; i < std::min(index, this->size()); i+=_BucketSize) {
      current = current->next;
    }
  }

  return typename fifo<T>::iterator_impl {
    [](typename fifo<T>::iterator_impl &_this) {
      _this.pos.index++;
      if (_this.pos.index >= _this.reference.size()) {
        _this.pos.ptr = nullptr;
        _this.pos.index = _this.reference.size();
      }
      else if (_this.pos.index % _BucketSize == 0) {
        _this.pos.ptr = reinterpret_cast<element*>(_this.pos.ptr)->next;
      }
    },
    [](const typename fifo<T>::iterator_impl &_this,
       const typename fifo<T>::iterator_impl &_other) {
      return (_this.pos.ptr == _other.pos.ptr) && (_this.pos.index == _other.pos.index);
    },
    [](typename fifo<T>::iterator_impl &_this) -> T& {
      return reinterpret_cast<element*>(_this.pos.ptr)->data[_this.pos.index % _BucketSize];
    },
    {
      std::min(index, this->size()),
      current,
    },
    *const_cast<multilinked_fifo<T, _BucketSize>*>(this)
  };
}
