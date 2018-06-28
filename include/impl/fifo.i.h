
#include <type_traits>

template <class T>
size_t fifo<T>::size() const {
  return _size;
}

template <class T>
size_t fifo<T>::get_allocated() const {
  return _reserved;
}

template <class T>
std::pair<typename fifo<T>::iterator,
          typename fifo<T>::iterator> first_mismatch(fifo<T> &first, fifo<T> &second) {

  typename fifo<T>::iterator fit = first.begin();
  typename fifo<T>::iterator sit = second.begin();

  for (; fit != first.end() && sit != second.end(); ++fit, ++sit) {
    if (*fit != *sit) return std::make_pair(fit, sit);
  }
  return std::make_pair(fit, sit);
}


template <class T>
template <class I>
I fifo<T>::iterator_base<I>::operator++() {
  this->_impl.increment(this->_impl);
  return *reinterpret_cast<I*>(this);
}

template <class T>
template <class I>
I fifo<T>::iterator_base<I>::operator++(int) {
  auto result = *reinterpret_cast<I*>(this);
  ++(*this);
  return result;
}

template <class T>
template <class I>
bool fifo<T>::iterator_base<I>::operator !=(iterator_base const& other) const {
  return !this->_impl.equals(this->_impl, other._impl);
}

template <class T>
template <class I>
bool fifo<T>::iterator_base<I>::operator ==(iterator_base const& other) const {
  return !(*this != other);
}

template <class T>
template <class I>
fifo<T>::iterator_base<I>::iterator_base(typename fifo<T>::iterator_impl &&impl)
                                        : _impl(impl) {
}

template <class T>
fifo<T>::iterator::iterator(iterator_impl &&impl) :
                            iterator_base<iterator>(std::move(impl)) {
}

template <class T>
fifo<T>::const_iterator::const_iterator(iterator_impl &&impl) :
                            iterator_base<const_iterator>(
                                           std::move(impl)) {
}

template <class T>
T& fifo<T>::iterator::operator *() {
  return this->_impl.get(this->_impl);
}

template <class T>
const T& fifo<T>::const_iterator::operator *() {
  return this->_impl.get(this->_impl);
}

template <class T>
typename fifo<T>::iterator fifo<T>::begin() {
  return fifo<T>::iterator(__get_it_impl(0));
}

template <class T>
typename fifo<T>::const_iterator fifo<T>::begin() const {
  return fifo<T>::const_iterator(__get_it_impl(0));
}

template <class T>
typename fifo<T>::iterator fifo<T>::end() {
  return fifo<T>::iterator(__get_it_impl(size()));
}

template <class T>
typename fifo<T>::const_iterator fifo<T>::end() const {
  return fifo<T>::const_iterator(__get_it_impl(size()));
}
