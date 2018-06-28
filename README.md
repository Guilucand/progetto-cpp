
# Collections
Questo progetto implementa 3 strutture FIFO, derivate da una classe comune:
* *contiguous_fifo&lt;T&gt;*  
    Lista di elementi contigui in memoria (simile a std::vector).

* *linked_fifo&lt;T&gt;*  
    Lista di elementi non contigui in memoria, che formano una forward-linked-list (simile a std::forward_list).

* *multilinked_fifo&lt;T, _BucketSize&gt;*  
    Lista di elementi parzialmente contigui in memoria, che formano una forward-linked-list nella quale ogni nodo contiene *_BucketSize* elementi.

Tutte e 3 derivano dalla classe astratta *fifo&lt;T&gt;*, che e' accessibile utilizzando l'operatore [] o degli iteratori generici, compatibili con tutte e 3 le liste derivate.

Cio' permette di poter utilizzare la sintassi del for aggiunta nel c++11:

    for (auto &el : fifo){
        ...
    }

La funzione *first_mismatch(fifo&lt;T&gt;, fifo&lt;T&gt;)* permette di trovare gli iteratori ai primi elementi delle due code che differiscono. Nel caso in cui non ce ne siano oppure si raggiunga la fine di una coda un iteratore alla fine del contenitore e' ritornato. 


## Istruzioni per l'utilizzo:

### Compilazione
cd build
cmake ..
make

### Installazione
make install (default bin)

### Test
make test

### Documentazione
make doc

### Stats
make stats

### Complessita' ciclomatica
make cyclo

### Linter
make tidy



Autore: Andrea Cracco
Matricola: VR397650
