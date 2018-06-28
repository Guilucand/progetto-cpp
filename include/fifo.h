#pragma once
#include <algorithm>
#include <vector>
#include <stdio.h>

namespace collections {

  template <class T>


  /// Classe che rappresenta una generica struttura fifo
  class fifo {

  protected:

    /// Classe che definisce le operazioni basi di un iteratore
    /// Sono definite tramite puntatori a funzione per permettere
    /// l'utilizzo della classe come valore di ritorno di una funzione
    /// cosa non possibile con l'utilizzo di una classe astratta
    class iterator_impl {
    public:
      /// Puntatore alla funzione di incremento
      void (*increment)(iterator_impl &_this);

      /// Puntatore alla funzione di uguaglianza
      bool (*equals)(const iterator_impl &_this, const iterator_impl &other);

      /// Puntatore alla funzione di ottenimento del valore
      T & (*get)(iterator_impl &_this);

      /// Struttura che rappresenta in modo universale
      /// la posizione di un iteratore
      struct _pos {
        size_t index;
        void *ptr;
      } pos;

      /// Referenza alla coda di appartenenza
      fifo<T> &reference;
    };

  private:

    /// Classe base per un iteratore
    /// da essa derivano l'iteratore normale e il const_iterator
    template <class I>
    class iterator_base {
    public:

      I operator++();
      I operator++(int);
      bool operator !=(iterator_base const& other) const;
      bool operator ==(iterator_base const& other) const;

    protected:
      /// Costruttore che accetta un'implementazione
      /// di un iteratore
      iterator_base(iterator_impl &&impl);
      iterator_base(iterator_base const& copy) = default;
      iterator_impl _impl;
    };

  public:
    fifo() = default;
    fifo(const fifo& other) = default;
    virtual ~fifo() = default;

    /*
     * Operatori virtuali per l'accesso agli elementi della coda
     */

    virtual T& operator[](size_t index) = 0;
    const virtual T& operator[](size_t index) const = 0;

    /// Funzione virtuale per l'aggiunta di un elemento alla coda
    virtual void add(const T &value) = 0;

    /// Ritorna la dimensione della coda
    size_t size() const;

    /// RItorna la dimensione riservata per la coda
    size_t get_allocated() const;

    /// Classe iteratore generica per ogni tipo di coda
    class iterator : public iterator_base<iterator> {
    public:
      iterator(iterator const& copy) = default;
      T& operator *();

    private:
      iterator(iterator_impl &&impl);
      friend class fifo<T>;
    };

    /// Classe iteratore costante generica
    class const_iterator : public iterator_base<const_iterator> {
    public:
      const_iterator(const_iterator const& copy) = default;
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

    /// Metodo virtuale per l'ottenimento dell'implementazione dell'iteratore
    virtual iterator_impl __get_it_impl(size_t index) const = 0;
    size_t _size = 0;
    size_t _reserved = 0;
  };

  /// Funzione per trovare il primo mismatch tra due generiche code
  /// Ritorna due iteratori corrispondenti agli elementi che differiscono
  template <class T>
  std::pair<typename fifo<T>::iterator,
            typename fifo<T>::iterator> first_mismatch(fifo<T> &first,
                                                       fifo<T> &second);

#include "impl/fifo.i.h"
}
