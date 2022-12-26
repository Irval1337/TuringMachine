#ifndef TURINGSETTINGS_H
#define TURINGSETTINGS_H

#include <string>
#include "TuringAlphabet.h"
#include "TuringTable.h"

namespace Turing {
    class Settings {
    private:
        QString _problem;
        QString _input_string;
        QString _comments;
        Turing::Alphabet _input_alphabet, _additional_alphabet;
        Turing::Table _turing_table;
        int _speed;
    private:
        QString _path;
    public:
        // Constructor
        Settings();

        // File methods
        bool open(QString);
        bool save();
        void save_as(QString);
        void clear();

        // Getters
        QString* get_problem();
        QString* get_input_string();
        QString* get_comments();
        Turing::Alphabet* get_input_alphabet();
        Turing::Alphabet* get_additional_alphabet();
        Turing::Table* get_turing_table();
        int* get_speed();

        // Setters
        void set_problem(QString);
        void set_input_string(QString);
        void set_comments(QString);
        void set_input_alphabet(Turing::Alphabet);
        void set_additional_alphabet(Turing::Alphabet);
        void set_turing_table(Turing::Table);
        void set_speed(int);
    };
}

#endif // TURINGSETTINGS_H
