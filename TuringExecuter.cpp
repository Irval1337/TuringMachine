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
        this->_speed = this->_settings.get_speed();
    }

    bool Executer::check_input_string(QString s) {
        for(qsizetype i = 0; i < s.size(); ++i) {
            if (!this->_input_alphabet->contains(s[i])) {
                return false;
            }
        }
        return true;
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
    QString Executer::get_current_string() {
        return this->_input_string;
    }

    void Executer::set_problem(QString value) {
        this->_settings.set_problem(value);
    }
    void Executer::set_input_string(QString value) {
        this->_input_string = value;
        this->_settings.set_input_string(value);
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
