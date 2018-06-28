


template <class T>
linked_fifo<T>::linked_fifo() {
  data = last = nullptr;
  this->_reserved = this->_size = 0;
}

template <class T>
linked_fifo<T>::~linked_fifo() {
  element *next = data;
  for (size_t i = 0; i < this->_reserved; i++) {
    element *current = next;
    next = next->next;
    delete current;
  }
  this->_reserved = this->_size = 0;
}

template <class T>
linked_fifo<T>::linked_fifo(const linked_fifo<T>& other) : fifo<T>(other) {
  data = last = nullptr;
  this->_reserved = this->_size = 0;
  for (auto el : other) {
    add(el);
  }
}

template <class T>
T& linked_fifo<T>::operator[](size_t index) {
  element *current = data;
  for (size_t i = 0; i < index; i++) current = current->next;
  return current->data;
}

template <class T>
const T& linked_fifo<T>::operator[](size_t index) const {
  element *current = data;
  for (size_t i = 0; i < index; i++) current = current->next;
  return current->data;
}

template <class T>
void linked_fifo<T>::add(const T &value) {
  if (this->_reserved == 0) {
    data = last = new element {value, nullptr};
    this->_reserved = this->_size = 1;
    return;
  }

  last->next = new element {value, nullptr};
  last = last->next;
  this->_reserved++;
  this->_size++;
}

template <class T>
linked_fifo<T> linked_fifo<T>::operator+(const fifo<T> &other) const {
  linked_fifo<T> result;
  for (T value : *this) {
    result.add(value);
  }
  for (T value : other) {
    result.add(value);
  }
  return result;
}

template <class T>
typename fifo<T>::iterator_impl linked_fifo<T>::__get_it_impl(size_t index) const {

  element* current;
  if (index == 0)
    current = data;
  else if (index == this->size())
    current = nullptr;
  else {
    current = data;
    for (size_t i = 0; i < std::min(index, this->size()); i++) {
      current = current->next;
    }
  }

  return typename fifo<T>::iterator_impl {
    [](typename fifo<T>::iterator_impl &_this) {
      _this.pos.ptr = reinterpret_cast<element*>(_this.pos.ptr)->next;
    },
    [](const typename fifo<T>::iterator_impl &_this,
       const typename fifo<T>::iterator_impl &_other) {
      return _this.pos.ptr == _other.pos.ptr;
    },
    [](typename fifo<T>::iterator_impl &_this) -> T& {
      return reinterpret_cast<element*>(_this.pos.ptr)->data;
    },
    {
      0,
      current
    },
    *const_cast<linked_fifo<T>*>(this)
  };
}
