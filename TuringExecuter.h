#ifndef TURINGEXECUTER_H
#define TURINGEXECUTER_H

#endif // TURINGEXECUTER_H

#include "TuringSettings.h"
#include "TuringAlphabet.h"
#include "TuringTable.h"

namespace Turing {
    class Executer {
    private:
        Turing::Table _table;
        Turing::Alphabet _input_alphabet, _additional_alphabet;
        Turing::Settings _settings;
        QString _input_string, _previous_input_string;
        int _current_state, _previous_state;
        int _string_pointer, _previous_string_pointer;
        bool _changed;
    public:
        // Default constructor
        Executer();

        // Executer methods
        Turing::Action next_action();
        Turing::Action recover_previous_action();

        // Setters
        void set_problem(QString);
        void set_input_string(QString);
        void set_comments(QString);
        void set_input_alphabet(Turing::Alphabet);
        void set_additional_alphabet(Turing::Alphabet);
        void set_turing_table(Turing::Table);
        void set_speed(int);

        // Table access methods
        bool add_action(int, std::vector<QString>&);
        bool change_action(int, std::vector<QString>&);
        void remove_action(int);
    };
}
