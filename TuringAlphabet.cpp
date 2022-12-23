#include "TuringAlphabet.h"

namespace Turing {
    bool Alphabet::contains(QChar c) {
        return this->indexOf(c) != -1;
    }
}
