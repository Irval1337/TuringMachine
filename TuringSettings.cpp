#include "TuringSettings.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

namespace Turing {
    Settings::Settings() {
        this->_problem = "";
        this->_input_string = "";
        this->_comments = "";
        this->_input_alphabet = "";
        this->_additional_alphabet = "";
        this->_turing_table = Turing::Table();
        this->_speed = 2;
        this->_path = QString();
    }

    bool Settings::open(QString path) {
        Turing::Settings prev = *this;
        try {
            QFile file(path);
            if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return false;
            }

            this->_path = path;
            QString data = file.readAll();
            file.close();

            QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
            QJsonObject json = doc.object();

            this->_speed = json["speed"].toInt();
            this->_problem = json["problem"].toString();
            this->_input_string = json["input_string"].toString();
            this->_comments = json["comments"].toString();
            this->_input_alphabet = Turing::Alphabet(json["input_alphabet"].toString());
            this->_additional_alphabet = Turing::Alphabet(json["additional_alphabet"].toString());

            std::vector<std::map<QChar, Turing::Action>> table;
            QJsonObject turing_table = json["turing_table"].toObject();
            QJsonArray actions = turing_table["actions"].toArray();
            for (const QJsonValue & val1 : actions) {
                std::map<QChar, Turing::Action> mp;
                QJsonArray action = val1.toArray();
                for(const QJsonValue & val2 : action) {
                    QJsonObject pair = val2.toObject();
                    QChar symb = pair["key"].toString()[0];
                    QJsonObject action = pair["value"].toObject();
                    mp.insert(std::make_pair(symb, Turing::Action(action["symbol"].toString()[0],
                              action["next_state"].toInt(), (Turing::Action::MoveType)action["move"].toInt())));
                }
                table.push_back(mp);
            }
            this->_turing_table.set_table(table);
            this->_turing_table.set_input_alphabet(this->_input_alphabet);
            this->_turing_table.set_additional_alphabet(this->_additional_alphabet);
            return true;
        } catch (...) {
            *this = prev;
            return false;
        }
    }
    void Settings::clear() {
        *this = Turing::Settings();
    }
    bool Settings::save() {
        if (this->_path == QString()) {
            return false;
        }
        try {
            QFile file(this->_path);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
                return false;
            }
            QJsonObject json;
            json["speed"] = this->_speed;
            json["problem"] = this->_problem;
            json["input_string"] = this->_input_string;
            json["comments"] = this->_comments;
            json["input_alphabet"] = (QString)this->_input_alphabet;
            json["additional_alphabet"] = (QString)this->_additional_alphabet;
            QJsonObject turing_table;
            QJsonArray actions;
            std::vector<std::map<QChar, Turing::Action>> table = this->_turing_table.get_table();
            for(size_t i = 0; i < table.size(); ++i) {
                QJsonArray mp;
                for(auto& it : table[i]) {
                    QJsonObject pair;
                    pair["key"] = QString(it.first);
                    QJsonObject action;
                    action["symbol"] = QString(it.second.get_symbol());
                    action["next_state"] = it.second.get_next_state();
                    action["move"] = (int)it.second.get_move();
                    pair["value"] = action;
                    mp.append(pair);
                }
                actions.append(mp);
            }
            turing_table["actions"] = actions;
            json["turing_table"] = turing_table;

            QTextStream stream(&file);
            stream << QJsonDocument(json).toJson();
            file.flush();
            file.close();

            return true;
        } catch (...) {
            return false;
        }
    }
    void Settings::save_as(QString path) {
        QString prev_path = this->_path;
        this->_path = path;
        if (!save()) {
            this->_path = prev_path;
        }
    }

    QString* Settings::get_problem() {
        return &this->_problem;
    }
    QString* Settings::get_input_string() {
        return &this->_input_string;
    }
    QString* Settings::get_comments() {
        return &this->_comments;
    }
    Turing::Alphabet* Settings::get_input_alphabet() {
        return &this->_input_alphabet;
    }
    Turing::Alphabet* Settings::get_additional_alphabet() {
        return &this->_additional_alphabet;
    }
    Turing::Table* Settings::get_turing_table() {
        return &this->_turing_table;
    }
    int* Settings::get_speed() {
        return &this->_speed;
    }

    void Settings::set_problem(QString s) {
        this->_problem = s;
    }
    void Settings::set_input_string(QString s) {
        this->_input_string = s;
    }
    void Settings::set_comments(QString s) {
        this->_comments = s;
    }
    void Settings::set_input_alphabet(Turing::Alphabet alph) {
        this->_input_alphabet = alph;
        this->_turing_table.set_input_alphabet(alph);
    }
    void Settings::set_additional_alphabet(Turing::Alphabet alph) {
        this->_additional_alphabet = alph;
        this->_turing_table.set_additional_alphabet(alph);
    }
    void Settings::set_turing_table(Turing::Table table) {
        this->_turing_table = table;
    }
    void Settings::set_speed(int speed) {
        this->_speed = speed;
    }
}
