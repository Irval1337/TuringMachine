#ifndef TURINGTABLE_H
#define TURINGTABLE_H

#include <string.h>
#include <map>
#include <vector>
#include <TuringAlphabet.h>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

namespace Turing {
    class Action {
    public:
        enum MoveType { Left, Right, None };
    private:
        QChar _symbol;
        int _next_state; // -1 = don`t change, -2 = stop machine
        MoveType _move;
    public:
        // Constructor
        Action(QChar, int, MoveType);

        // Getters
        QChar get_symbol();
        int get_next_state();
        MoveType get_move();
    };

    class Table {
    private:
        std::vector<std::map<QChar, Turing::Action>> _actions;
        Turing::Alphabet _input_alphabet, _additional_alphabet;
        QChar _empty;
    public:
        // Default constructor
        Table();

        // Access methods
        Turing::Action get_next_action(QChar, int);
        bool add_action(int, std::vector<QString>&);
        bool change_action(int, std::vector<QString>&);
        void remove_action(int);
        int validate(QString);
        Turing::Action parse_action(QString);

        // Getters
        Turing::Alphabet get_input_alphabet();
        Turing::Alphabet get_additional_alphabet();

        // Setters
        void set_input_alphabet(Turing::Alphabet);
        void set_additional_alphabet(Turing::Alphabet);
    };
}

#endif // TURINGTABLE_H
