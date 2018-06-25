#include <iostream>

#include "include/linked_fifo.h"
#include "include/multilinked_fifo.h"
#include "include/contiguous_fifo.h"

using namespace collections;
using namespace std;


int main() {

  multilinked_fifo<int, 5> f;
  linked_fifo<int> f2;
  f.add(5);
  f.add(4);
  f.add(3);
  f.add(3);
  f2.add(7);
  f2.add(8);
  f2.add(9);
  f2.add(8);
  f2.add(10);
  f2.add(10);
  f2.add(10);
  f2.add(11);

  // auto it = f.begin();
  // auto it2 = f.end();
  //
  // cout << f.size() << endl;
  // cout << *(++it) << " " << "A" << endl;1

  auto f3 = f + f2;

  for (auto val : f3) {
    std::cout << "Print! " <<  val << " " << std::endl;
  }
  std::cout << std::endl;
  return 0;

  // auto ms = first_mismatch(f, f3);
  // std::cout << "End: " << (ms.first == f.end()) << " " << (ms.second == f3.end()) << std::endl;
  // std::cout << "Mismatch: " << *(ms.first) << " " << *(ms.second) << std::endl;

}
