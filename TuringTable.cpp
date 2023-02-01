#include "TuringTable.h"

namespace Turing {
    Action::Action(QChar symbol, int next_state, Turing::Action::MoveType move) {
        this->_symbol = symbol;
        this->_next_state = next_state;
        this->_move = move;
    }

    QChar Action::get_symbol() {
        return this->_symbol;
    }
    int Action::get_next_state() {
        return this->_next_state;
    }
    Turing::Action::MoveType Turing::Action::get_move() {
        return this->_move;
    }

    Table::Table() {
        this->_actions = std::vector<std::map<QChar, Turing::Action>>();
        this->_input_alphabet = this->_additional_alphabet = "";
        this->_empty = '_';
    }

    int Turing::Table::validate(QString s) {
        QRegularExpression rx_default("^.?[><]?Q\\d+$"), rx_no_state("^.?[><]?$"), rx_break("^.?[><]?!$");
        if (rx_default.match(s).hasMatch()) return 1;
        if (rx_no_state.match(s).hasMatch()) return 2;
        if (rx_break.match(s).hasMatch()) return 3;
        return 0;
    }
    Turing::Action Turing::Table::parse_action(QString s) {
        int validate_value = this->validate(s);
        if (s.size() == 0) {
            return Turing::Action('\0', -2, Turing::Action::MoveType::None);
        } else if (validate_value == 0) {
            throw 1;
        }

        Turing::Action::MoveType move = Turing::Action::MoveType::None;
        if (s.contains('>') && s.contains('<')) {
            move = s.lastIndexOf('>') > s.lastIndexOf('<') ? Turing::Action::MoveType::Right : Turing::Action::MoveType::Left;
        }
        else if (s.contains('>')) {
            move = Turing::Action::MoveType::Right;
        } else if (s.contains('<')) {
            move = Turing::Action::MoveType::Left;
        }
        switch (validate_value) {
        case 1: {
            QChar symbol = '\0';
            if (!(s[0] == '>' && s.count('>') == 1) && !(s[0] == '<' && s.count('<') == 1) && !(s[0] == 'Q' && s.count('Q') == 1)) {
                symbol = s[0];
            }
            int next_state = s.mid(s.lastIndexOf('Q') + 1).toInt();
            return Turing::Action(symbol, next_state, move);
        }
        case 2: {
            QChar symbol = '\0';
            if (!(s[0] == '>' && s.count('>') == 1) && !(s[0] == '<' && s.count('<') == 1)) {
                symbol = s[0];
            }
            int next_state = -1;
            return Turing::Action(symbol, next_state, move);
        }
        case 3: {
            QChar symbol = '\0';
            if (!(s[0] == '>' && s.count('>') == 1) && !(s[0] == '<' && s.count('<') == 1) && !(s[0] == '!' && s.count('!') == 1)) {
                symbol = s[0];
            }
            int next_state = -2;
            return Turing::Action(symbol, next_state, move);
        }
        };
        return Turing::Action('\0', -2, Turing::Action::MoveType::None);
    }
    Turing::Action Table::get_next_action(QChar current_symbol, int state) {
        return this->_actions[state].at(current_symbol);
    }
    bool Table::add_action(int index, std::vector<QString>& row) {
        this->_actions.insert(this->_actions.begin() + index, std::map<QChar, Turing::Action>());
        return this->change_action(index, row);
    }
    bool Table::change_action(int index, std::vector<QString>& row) {
        try {
            if ((std::size_t)index >= this->_actions.size()) return true;

            std::map<QChar, Turing::Action> mp;
            qsizetype i = 0;
            for (; i < this->_input_alphabet.size(); ++i) { // default alphabet
                mp.insert(std::make_pair(this->_input_alphabet[i], this->parse_action(row[i])));
            }
            mp.insert(std::make_pair(this->_empty, this->parse_action(row[i]))); // empty simbol
            for (i = 0; i < this->_additional_alphabet.size(); ++i) { // additional alphabet
                mp.insert(std::make_pair(this->_additional_alphabet[i], this->parse_action(row[this->_input_alphabet.size() + i + 1])));
            }
            this->_actions[index] = mp;
            return true;
        }
        catch (...) {
            return false;
        }
    }
    bool Table::remove_action(int index) {
        if (index < 0 || index >= (int)this->_actions.size())
            return false;
        this->_actions.erase(this->_actions.begin() + index);
        return true;
    }

    Turing::Alphabet Table::get_input_alphabet() {
        return this->_input_alphabet;
    }
    Turing::Alphabet Table::get_additional_alphabet() {
        return this->_additional_alphabet;
    }
    std::vector<std::map<QChar, Turing::Action>> Table::get_table() {
        return this->_actions;
    }

    void Table::set_input_alphabet(Turing::Alphabet alphabet) {
        this->_input_alphabet = alphabet;
    }
    void Table::set_additional_alphabet(Turing::Alphabet alphabet) {
        this->_additional_alphabet = alphabet;
    }
    void Table::set_table(std::vector<std::map<QChar, Turing::Action>>& table) {
        this->_actions = table;
    }
}
