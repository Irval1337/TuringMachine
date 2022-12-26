#ifndef TURINGALPHABET_H
#define TURINGALPHABET_H

#include "QString"

namespace Turing {
    class Alphabet : public QString {
        using base = QString;
    public:
        using base::base;

        // Constructor from base class
        Alphabet(QString s) : QString(s) {};

        // Auxiliary alphabet methods
        bool contains(QChar);
        bool validate();
    };
}

#endif // TURINGALPHABET_H
