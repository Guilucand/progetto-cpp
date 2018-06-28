
#include <stdio.h>

#include "linked_fifo.h"
#include "multilinked_fifo.h"
#include "contiguous_fifo.h"


#define CATCH_CONFIG_RUNNER
#   include <catch.hpp>
#undef CATCH_CONFIG_RUNNER
#include <iomanip>
#include <iostream>
#include <vector>
#include <limits>
#include <random>

using namespace collections;
using namespace std;
using namespace Catch::clara;

static int fifo_size = 0;


/// Riempie uno std::vector e una fifo con gli stessi elementi
/// per permettere il controllo della correttezza della fifo
template <class T>
void fill_fifo_vector(vector<T> &vec, fifo<T> &fifo, int dim) {
    std::default_random_engine generator;
    std::uniform_int_distribution<T> distribution(std::numeric_limits<T>::min(),
                                                  std::numeric_limits<T>::max());

    for (int i = 0; i < dim; i++) {
        int val = distribution(generator);
        vec.push_back(val);
        fifo.add(val);
        REQUIRE(vec[i] == fifo[i]);
    }
}

template <class T>
void check_iterators(vector<T> &vec, fifo<T> &fifo) {
    auto it1 = vec.begin();
    auto it2 = fifo.begin();

    while (it1 != vec.end()) {
        REQUIRE(*it1 == *it2);
        ++it1;
        ++it2;
    }

    REQUIRE(it2 == fifo.end());
}

template<class T>
void check_concat_equivalence(vector<T> &veca, vector<T> &vecb, fifo<T> &&concat) {
    auto it = concat.begin();

    for (T const& v : veca) {
        REQUIRE(v == *(it++));
    }

    for (T const& v : vecb) {
        REQUIRE(v == *(it++));
    }

    REQUIRE(it == concat.end());
}


TEST_CASE("Contiguous fifo Test", "[ContiguousFifoTest]") {

  contiguous_fifo<int> fifo;
  vector<int> vec;

  fill_fifo_vector(vec, fifo, fifo_size);
  check_iterators(vec, fifo);
}

TEST_CASE("Linked fifo Test", "[LinkedFifoTest]") {

  linked_fifo<int> fifo;
  vector<int> vec;

  fill_fifo_vector(vec, fifo, fifo_size);
  check_iterators(vec, fifo);
}

TEST_CASE("Multilinked fifo Test", "[MultilinkedFifoTest]") {

  multilinked_fifo<int, 16> fifo;
  vector<int> vec;

  fill_fifo_vector(vec, fifo, fifo_size);
  check_iterators(vec, fifo);
}

TEST_CASE("Concat fifo Test", "[ConcatFifoTest]") {

  contiguous_fifo<int>        fifo1;
  linked_fifo<int>            fifo2;
  multilinked_fifo<int, 16>   fifo3;
  vector<int> vec1, vec2, vec3;

  fill_fifo_vector(vec1, fifo1, fifo_size);
  fill_fifo_vector(vec2, fifo2, fifo_size);
  fill_fifo_vector(vec3, fifo3, fifo_size);

  check_concat_equivalence(vec1, vec2, fifo1 + fifo2);
  check_concat_equivalence(vec2, vec1, fifo2 + fifo1);

  check_concat_equivalence(vec2, vec3, fifo2 + fifo3);
  check_concat_equivalence(vec3, vec2, fifo3 + fifo2);

  check_concat_equivalence(vec1, vec3, fifo1 + fifo3);
  check_concat_equivalence(vec3, vec1, fifo3 + fifo1);

}


int main(int argc, char* argv[]) {
    Catch::Session session;

    auto cli = session.cli()
        | Opt(fifo_size, "Fifo size")
             ["-s"]["--fifo-size"]
             ("Fifo test size");

    session.cli(cli);

    auto ret = session.applyCommandLine( argc, argv );
    if (ret) {
        return ret;
    }

    return session.run();
}
