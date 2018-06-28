#include "multilinked_fifo.h"

namespace collections {
    template class multilinked_fifo<char, 16>;
    template class multilinked_fifo<int, 16>;
    template class multilinked_fifo<float, 16>;
    template class multilinked_fifo<double, 16>;
}
