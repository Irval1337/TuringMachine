#include "TuringAlphabet.h"
#include <map>

namespace Turing {
    bool Alphabet::contains(QChar c) {
        return this->indexOf(c) != -1;
    }
    bool Alphabet::validate() {
        std::map<QChar, int> chars_count;
        for (qsizetype i = 0; i < this->size(); ++i) {
            if (this->at(i) == '_') return false;
            ++chars_count[this->at(i)];
            if (chars_count[this->at(i)] > 1) {
                return false;
            }
        }
        return true;
    }
}
