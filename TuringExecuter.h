#ifndef TURINGEXECUTER_H
#define TURINGEXECUTER_H

#endif // TURINGEXECUTER_H

#include "TuringSettings.h"
#include "TuringAlphabet.h"
#include "TuringTable.h"

namespace Turing {
    class Executer {
    private:
        Turing::Table* _table;
        Turing::Alphabet* _input_alphabet, *_additional_alphabet;
        Turing::Settings _settings;
        QString _input_string, _previous_input_string;
        int _current_state, _previous_state;
        int _string_offset;
        int _string_pointer, _previous_string_pointer;
        int* _speed;
    public:
        // Default constructor
        Executer();

        // Executer methods
        Turing::Action next_action(int);
        Turing::Action recover_previous_action();
        bool check_input_string(QString);
        QString get_current_string(int);

        // Getters
        Turing::Table* get_table();
        Turing::Alphabet* get_input_alphabet();
        Turing::Alphabet* get_additional_alphabet();
        Turing::Settings* get_settings();
        int get_current_state();
        int get_string_pointer();
        QString get_current_input_string();
        int get_offset();

        // Setters
        void set_problem(QString);
        void set_input_string(QString);
        void set_comments(QString);
        void set_input_alphabet(Turing::Alphabet);
        void set_additional_alphabet(Turing::Alphabet);
        void set_turing_table(Turing::Table);
        void set_speed(int);
        void set_current_string(QString);
        void update_offset(int);
        void reset_offset();

        // Table access methods
        bool add_action(int, std::vector<QString>&);
        bool change_action(int, std::vector<QString>&);
        bool change_action(int, int, QString);
        bool remove_action(int);
    };
}
