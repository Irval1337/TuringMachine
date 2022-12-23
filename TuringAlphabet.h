#ifndef TURINGALPHABET_H
#define TURINGALPHABET_H

#include "QString"

namespace Turing {
    class Alphabet : public QString {
        using base = QString;
    public:
        using base::base;

        // Auxiliary alphabet methods
        bool contains(QChar);
        bool validate();
    };
}

#endif // TURINGALPHABET_H
