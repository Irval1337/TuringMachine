#include "TuringExecuter.h"

namespace Turing {
    Executer::Executer() {
        this->_settings = Turing::Settings();
        this->_table = this->_settings.get_turing_table();
        this->_input_alphabet = this->_settings.get_input_alphabet();
        this->_additional_alphabet = this->_settings.get_additional_alphabet();
        this->_input_string = this->_previous_input_string = "";
        this->_current_state = this->_previous_state = 0;
        this->_string_pointer = this->_previous_string_pointer = 0;
        this->_string_offset = 0;
        this->_speed = this->_settings.get_speed();
    }

    Turing::Action Executer::next_action(int size) {
        Turing::Action act('\0', -2, Action::None);
        auto table = this->_table->get_table();
        if (table.size() <= this->_current_state || this->_current_state < 0) return act;
        QString curr_string = this->get_current_input_string();
        if (this->_string_pointer == -1) {
            this->_string_pointer = 0;
            curr_string = '_' + curr_string;
            if (this->_string_offset != 0)
                this->_string_offset++;
        }
        if (this->_string_pointer == curr_string.size()) {
            curr_string.push_back('_');
        }

        if (table.at(this->_current_state).count(curr_string[this->_string_pointer]) != 0) {
            act = table.at(this->_current_state).at(curr_string[this->_string_pointer]);
            bool cont = !this->_input_alphabet->contains(act.get_symbol()) && !this->_additional_alphabet->contains(act.get_symbol()) && act.get_symbol() != '_';;
            int fuck_you = act.get_next_state();
            bool ahahah = fuck_you >= (int)table.size();
            if (ahahah|| (act.get_symbol() != '\0' && cont)) {
                int st = act.get_next_state();
                QChar c = act.get_symbol();
                return act;
            }
            if (act.get_next_state() >= 0) this->_current_state = act.get_next_state();
            if (act.get_symbol() != '\0' && act.get_symbol() != '!') {
                curr_string[this->_string_pointer] = act.get_symbol();
                this->_input_string = curr_string;
            }
            if (act.get_move() != Turing::Action::None) {
                if (act.get_move() == Turing::Action::Right) this->_string_pointer++;
                else this->_string_pointer--;
            }
            return act;
        }
        return act;
    }
    bool Executer::check_input_string(QString s) {
        for(qsizetype i = 0; i < s.size(); ++i) {
            if (!this->_input_alphabet->contains(s[i]) && s[i] != '_') {
                return false;
            }
        }
        return true;
    }
    QString Executer::get_current_string(int size) {
        QString str = "";
        if (this->_string_offset > this->_input_string.size() - 1 || this->_string_offset + size - 1 < 0) {
            str = "";
        }
        else if (this->_string_offset >= 0) {
            str = this->_input_string.mid(this->_string_offset, qMin(size, this->_input_string.size() - this->_string_offset));
        }
        else str = QString(abs(this->_string_offset), '_') + this->_input_string.mid(0, qMin(this->_input_string.size(), size + this->_string_offset));
        str = str + QString(size - str.size(), '_');
        return str;
    }

    Turing::Table* Executer::get_table() {
        return this->_table;
    }
    Turing::Alphabet* Executer::get_input_alphabet() {
        return this->_input_alphabet;
    }
    Turing::Alphabet* Executer::get_additional_alphabet() {
        return this->_additional_alphabet;
    }
    Turing::Settings* Executer::get_settings() {
        return &this->_settings;
    }
    int Executer::get_current_state() {
        return this->_current_state;
    }
    int Executer::get_string_pointer() {
        return this->_string_pointer;
    }
    QString Executer::get_current_input_string() {
        return this->_input_string;
    }
    int Executer::get_offset() {
        return this->_string_offset;
    }

    void Executer::set_problem(QString value) {
        this->_settings.set_problem(value);
    }
    void Executer::set_input_string(QString value) {
        this->_input_string = value;
        this->_settings.set_input_string(value);
        this->_current_state = 0;
        this->_string_pointer = 0;
    }
    void Executer::set_comments(QString value) {
        this->_settings.set_comments(value);
    }
    void Executer::set_input_alphabet(Turing::Alphabet alph) {
        this->_settings.set_input_alphabet(alph);
    }
    void Executer::set_additional_alphabet(Turing::Alphabet alph) {
        this->_settings.set_additional_alphabet(alph);
    }
    void Executer::set_turing_table(Turing::Table table) {
        this->_settings.set_turing_table(table);
    }
    void Executer::set_speed(int value) {
        this->_settings.set_speed(value);
    }
    void Executer::set_current_string(QString value) {
        this->_input_string = this->_previous_input_string = value;
    }
    void Executer::update_offset(int value) {
        this->_string_offset += value;
    }
    void Executer::reset_offset() {
        this->_string_offset = 0;
    }

    bool Executer::add_action(int index, std::vector<QString>& data) {
        return this->_table->add_action(index, data);
    }
    bool Executer::change_action(int index, std::vector<QString>& data) {
        return this->_table->change_action(index, data);
    }
    bool Executer::remove_action(int index) {
        return this->_table->remove_action(index);
    }
}
