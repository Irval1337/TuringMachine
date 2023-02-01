#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include "stringview.h"
#include <QDialog>
#include <QPainter>
#include <QWidget>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , exec(new Turing::Executer)
{
    ui->setupUi(this);
    ui->splitter->setSizes(QList<int>({600, 200}));
    ui->splitter_3->setSizes(QList<int>({200, 800}));
    ui->splitter_2->setSizes(QList<int>({200, 200}));

    this->_monospace = QFont("Anonymous Pro", 18);

    this->_changed = false;

    this->_timer = new QTimer(this); // Создаем объект класса QTimer и передаем адрес переменной
    this->_timer->setInterval(10); // Задаем интервал таймера
    connect(this->_timer, SIGNAL(timeout()), this, SLOT(update_time())); // Подключаем сигнал таймера к нашему слоту
    this->_timer->start(); // Запускаем таймер

    this->ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    QTableWidgetItem* protoitem = new QTableWidgetItem("");
    protoitem->setTextAlignment(Qt::AlignCenter);
    this->ui->tableWidget->setItemPrototype(protoitem);
    this->ui->tableWidget->setColumnCount(1);
    this->ui->tableWidget->setRowCount(1);
    this->ui->tableWidget->setItem(0, 0, new QTableWidgetItem(""));
    this->ui->tableWidget->setHorizontalHeaderLabels({"_"});
    this->ui->tableWidget->setVerticalHeaderLabels({"Q0"});
    this->ui->tableWidget->setColumnWidth(0, 50);
    std::vector<QString> actions = { "" };
    this->exec->get_table()->add_action(0, actions);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete exec;
    delete _timer;
    delete _tape;
    for(qsizetype i = 0; i < _lines.size(); ++i) {
        delete _lines[i];
    }
}

void MainWindow::move_left() {
    this->exec->update_offset(-1);
    this->repaint();
}

void MainWindow::move_right() {
    this->exec->update_offset(+1);
    this->repaint();
}

void MainWindow::set_enabled(bool value) {
    value = !value;
    this->ui->input_alphabetBox->setDisabled(value);
    this->ui->additional_alphabetBox->setDisabled(value);
    this->ui->tableWidget->setDisabled(value);
    this->ui->addRowBtn->setDisabled(value);
    this->ui->delRowBtn->setDisabled(value);
    this->ui->problemBox->setDisabled(value);
    this->ui->commentsBox->setDisabled(value);
    this->ui->action->setDisabled(value);
    this->ui->action_11->setDisabled(value);
    this->ui->action_15->setDisabled(value);
}

void MainWindow::move_pointer(bool right) {
    if (right) {
        if (this->_pointer_x + 35 > this->_lines.back()->x1()) {
            move_right();
        }
        else {
            this->_pointer_move_type = 1;
            this->_pointer_x += 7;
        }
    } else {
        if (this->_pointer_x - 35 < this->_lines[0]->x1() - 35) {
            move_left();
        } else {
            this->_pointer_move_type = -1;
            this->_pointer_x -= 7;
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event) // Отрисовка ленты
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    QWidget* left = this->ui->horizontalLayout_2->itemAt(0)->widget();
    QWidget* right = this->ui->horizontalLayout_2->itemAt(1)->widget();
    auto window = this->geometry();
    if (this->_tape != nullptr) {
        delete this->_tape;
    }
    for(qsizetype i = 0; i < _lines.size(); ++i) {
        delete _lines[i];
    }
    _lines.clear();

    painter.setBrush(QBrush(QColor(0xfefee2), Qt::SolidPattern));
    this->_tape = new QRect(left->mapToGlobal(QPoint(0,0)).x() - window.x() + 19, left->mapToGlobal(QPoint(0,0)).y() - window.y() + 1,
                        right->mapToGlobal(QPoint(0, 0)).x() - left->mapToGlobal(QPoint(0,0)).x() - 19, 37);
    painter.drawRect(*this->_tape);

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    int prev_pos = 0;
    while (prev_pos + 35 <= right->mapToGlobal(QPoint(0, 0)).x() - left->mapToGlobal(QPoint(0,0)).x() - 19) {
        prev_pos += 35;
        _lines.push_back(new QLine(left->mapToGlobal(QPoint(0,0)).x() - window.x() + 19 + prev_pos, left->mapToGlobal(QPoint(0,0)).y() - window.y() + 1,
                                   left->mapToGlobal(QPoint(0,0)).x() - window.x() + 19 + prev_pos, left->mapToGlobal(QPoint(0,0)).y() - window.y() + 1 + 37));
        painter.drawLine(*_lines.back());
    }
    painter.setFont(this->_monospace);

    QString str = this->exec->get_current_string(_lines.size());
    prev_pos = 12;
    int sz = 0;
    while(prev_pos <= right->mapToGlobal(QPoint(0, 0)).x() - left->mapToGlobal(QPoint(0,0)).x() - 19 && sz < str.size()) {
        painter.drawText(left->mapToGlobal(QPoint(0,0)).x() - window.x() + 19 + prev_pos, left->mapToGlobal(QPoint(0,0)).y() - window.y() + 1 + 27, str[sz]);
        prev_pos += 35;
        ++sz;
    }
    if (this->_pointer == nullptr) {
        this->_pointer = new QImage(":/head/img/head.png");
        this->_pointer_x = left->mapToGlobal(QPoint(0,0)).x() - window.x() + 19 + 3;
    }
    if (this->_pointer_x + 30 > this->_lines.back()->x1()) {
        if (this->_pointer_x - 35 < this->_lines[0]->x1() - 35) {
            return;
        }
        this->exec->update_offset(+1);
        this->_pointer_x -= 35;
    }

    painter.drawImage(QRect(this->_pointer_x, left->mapToGlobal(QPoint(0,0)).y() - window.y() + 1 - 38, 30, 44), *this->_pointer);
}

void MainWindow::update_table() { // Обновление таблицы
    this->_no_edit = true;
    QMap<QString, QVector<QString>> m;
    for(qsizetype i = 0; i < this->ui->tableWidget->columnCount(); ++i) {
        QVector<QString> vec;
        for(qsizetype j = 0; j < this->ui->tableWidget->rowCount(); ++j) {
            vec.push_back(this->ui->tableWidget->item(j, i)->text());
            delete this->ui->tableWidget->item(j, i);
        }
        m[this->ui->tableWidget->horizontalHeaderItem(i)->text()] = vec;
    }
    int row = this->ui->tableWidget->rowCount();
    this->ui->tableWidget->setRowCount(0);
    this->ui->tableWidget->clear();
    this->ui->tableWidget->setRowCount(row);

    int columns = this->ui->input_alphabetBox->text().size() + this->ui->additional_alphabetBox->text().size() + 1;
    this->ui->tableWidget->setColumnCount(columns);
    QStringList labels;
    for(qsizetype i = 0; i < this->ui->input_alphabetBox->text().size(); ++i) {
        labels.push_back(this->ui->input_alphabetBox->text().at(i));
    }
    labels.push_back("_");
    for(qsizetype i = 0; i < this->ui->additional_alphabetBox->text().size(); ++i) {
        labels.push_back(this->ui->additional_alphabetBox->text().at(i));
    }
    this->ui->tableWidget->setHorizontalHeaderLabels(labels);

    for(int i = 0; i < columns; ++i) {
        this->ui->tableWidget->setColumnWidth(i, 50);
        for(qsizetype j = 0; j < this->ui->tableWidget->rowCount(); ++j) {
            this->ui->tableWidget->setItem(j, i, new QTableWidgetItem(""));
        }
        if (!m.contains(labels[i]))
            continue;
        for(qsizetype j = 0; j < this->ui->tableWidget->rowCount(); ++j) {
            this->ui->tableWidget->item(j, i)->setText(m[labels[i]].at(j));
        }
    }

    labels.clear();
    for(qsizetype i = 0; i < this->ui->tableWidget->rowCount(); ++i) {
        labels.push_back("Q" + QString::number(i));
    }
    this->ui->tableWidget->setVerticalHeaderLabels(labels);

    if (this->ui->tableWidget->rowCount() == 0) {
        this->on_addRowBtn_clicked();
    }
    this->_no_edit = false;
}

void MainWindow::update_time() // Основной таймер
{
    QWidget* left = this->ui->horizontalLayout_2->itemAt(0)->widget();
    auto window = this->geometry();

    if (this->_pointer_move_type == 1) {
        if ((this->_pointer_x - (left->mapToGlobal(QPoint(0,0)).x() - window.x() + 19 + 3)) % 35 == 0) {
            this->_pointer_move_type = 0;
        } else {
            this->_pointer_x += 7;
        }
    } else if (this->_pointer_move_type == -1) {
        if ((this->_pointer_x - (left->mapToGlobal(QPoint(0,0)).x() - window.x() + 19 + 3)) % 35 == 0) {
            this->_pointer_move_type = 0;
        } else {
            this->_pointer_x -= 7;
        }
    }
    this->repaint();

    if (!this->_run) return;

    this->ui->tableWidget->selectRow(this->exec->get_current_state());
    int delta = 0;
    switch(*this->exec->get_settings()->get_speed()) {
    case 0:
        delta = 1;
        break;
    case 1:
        delta = 10;
        break;
    case 2:
        delta = 25;
        break;
    case 3:
        delta = 50;
        break;
    case 4:
        delta = 100;
        break;
    }

    if (this->_currect_tick % (5 + delta) == 0) {
        on_action_8_triggered();
    }

    this->_currect_tick++;
}

void MainWindow::resizeEvent(QResizeEvent* event) // Событие изменения размера окна
{
    QMainWindow::resizeEvent(event);
    this->repaint();
}

void MainWindow::closeEvent(QCloseEvent* event) // Проверка на наличие несохраненных данных
{
    if (!this->_changed) {
        event->accept();
        return;
    }

    QMessageBox messageBox(QMessageBox::Question,
                tr("TuringEmulator"),
                tr("В текущем файле есть несохраненные изменения. Вы действительно хотите выйти?"),
                QMessageBox::Default,
                this);
    messageBox.addButton("Да", QMessageBox::YesRole);
    messageBox.addButton("Нет", QMessageBox::NoRole);
    int quit = messageBox.exec();
    if (quit == 0) {
        event->accept();
    } else {
        event->ignore();
    }
}

QString action_to_string(Turing::Action action) {
    QString s = "";
    if (action.get_symbol() != '\0') {
        s += action.get_symbol();
    }
    if (action.get_move() != Turing::Action::None) {
        s += action.get_move() == Turing::Action::Left ? '<' : '>';
    }
    if (action.get_next_state() != -1) {
        if (action.get_next_state() == -2) {
            if (action.get_symbol() != '!') s += '!';
        } else {
            s += 'Q' + QString::number(action.get_next_state());
        }
    }
    return s;
}

void MainWindow::update_from_settings() { // Перенести данные из настроек в форму
    this->ui->problemBox->setPlainText(*this->exec->get_settings()->get_problem());
    this->ui->input_alphabetBox->setText(*this->exec->get_settings()->get_input_alphabet());
    this->ui->additional_alphabetBox->setText(*this->exec->get_settings()->get_additional_alphabet());
    this->ui->commentsBox->setPlainText(*this->exec->get_settings()->get_comments());
    this->set_speed(*this->exec->get_settings()->get_speed());
    this->exec->set_input_string(*this->exec->get_settings()->get_input_string());

    this->update_table();
    this->_no_edit = true;
    auto table = this->exec->get_table()->get_table();
    this->ui->tableWidget->setRowCount(table.size());
    for(std::size_t i = 0; i < table.size(); ++i) {
        std::size_t j = 0;
        for(std::size_t k = 0; k < this->exec->get_input_alphabet()->size(); ++k) {
            QChar c = this->exec->get_input_alphabet()->at(k);
            Turing::Action act = table.at(i).at(c);
            this->ui->tableWidget->setItem(i, j, new QTableWidgetItem(action_to_string(act)));
            ++j;
        }
        this->ui->tableWidget->setItem(i, j, new QTableWidgetItem(action_to_string(table.at(i).at('_'))));
        ++j;
        for(std::size_t k = 0; k < this->exec->get_additional_alphabet()->size(); ++k) {
            QChar c = this->exec->get_additional_alphabet()->at(k);
            Turing::Action act = table.at(i).at(c);
            this->ui->tableWidget->setItem(i, j, new QTableWidgetItem(action_to_string(act)));
            ++j;
        }
    }
    this->_no_edit = false;
    this->update_table();
}

void MainWindow::save_into_settings() { // Перенести данные из формы в настройки
    this->exec->set_problem(this->ui->problemBox->toPlainText());
    this->exec->set_input_alphabet(Turing::Alphabet(this->ui->input_alphabetBox->text()));
    this->exec->set_additional_alphabet(Turing::Alphabet(this->ui->additional_alphabetBox->text()));
    this->exec->set_comments(this->ui->commentsBox->toPlainText());
    this->exec->set_speed(this->get_speed());
}

void MainWindow::set_speed(int value) { // int -> Скорость
    switch(value) {
    case 0:
        this->ui->action_15->setChecked(true);
        break;
    case 1:
        this->ui->action_16->setChecked(true);
        break;
    case 2:
        this->ui->action_17->setChecked(true);
        break;
    case 3:
        this->ui->action_18->setChecked(true);
        break;
    case 4:
        this->ui->action_19->setChecked(true);
        break;
    default:
        break;
    }
}

int MainWindow::get_speed() { // Скорость -> int
    if (this->ui->action_15->isChecked()) {
        return 0;
    }
    if (this->ui->action_16->isChecked()) {
        return 1;
    }
    if (this->ui->action_17->isChecked()) {
        return 2;
    }
    if (this->ui->action_18->isChecked()) {
        return 3;
    }
    if (this->ui->action_19->isChecked()) {
        return 4;
    }
    return -1;
}

void MainWindow::on_action_15_toggled(bool state) // Скорость -> Очень быстро
{
    if (state) {
        this->ui->action_16->setChecked(false);
        this->ui->action_17->setChecked(false);
        this->ui->action_18->setChecked(false);
        this->ui->action_19->setChecked(false);
        this->_changed = true;
    } else {
        bool is_any_checked = this->ui->action_15->isChecked() || this->ui->action_16->isChecked() || this->ui->action_17->isChecked() ||
                this->ui->action_18->isChecked() || this->ui->action_19->isChecked();
        if (!is_any_checked) {
            this->ui->action_15->setChecked(true);
        } else {
            this->_changed = true;
        }
    }
}

void MainWindow::on_action_16_toggled(bool state) // Скорость -> Быстро
{
    if (state) {
        this->ui->action_15->setChecked(false);
        this->ui->action_17->setChecked(false);
        this->ui->action_18->setChecked(false);
        this->ui->action_19->setChecked(false);
        this->_changed = true;
    } else {
        bool is_any_checked = this->ui->action_15->isChecked() || this->ui->action_16->isChecked() || this->ui->action_17->isChecked() ||
                this->ui->action_18->isChecked() || this->ui->action_19->isChecked();
        if (!is_any_checked) {
            this->ui->action_16->setChecked(true);
        } else {
            this->_changed = true;
        }
    }
}

void MainWindow::on_action_17_toggled(bool state) // Скорость -> Средне
{
    if (state) {
        this->ui->action_16->setChecked(false);
        this->ui->action_15->setChecked(false);
        this->ui->action_18->setChecked(false);
        this->ui->action_19->setChecked(false);
        this->_changed = true;
    } else {
        bool is_any_checked = this->ui->action_15->isChecked() || this->ui->action_16->isChecked() || this->ui->action_17->isChecked() ||
                this->ui->action_18->isChecked() || this->ui->action_19->isChecked();
        if (!is_any_checked) {
            this->ui->action_17->setChecked(true);
        } else {
            this->_changed = true;
        }
    }
}

void MainWindow::on_action_18_toggled(bool state) // Скорость -> Медленно
{
    if (state) {
        this->ui->action_16->setChecked(false);
        this->ui->action_17->setChecked(false);
        this->ui->action_15->setChecked(false);
        this->ui->action_19->setChecked(false);
        this->_changed = true;
    } else {
        bool is_any_checked = this->ui->action_15->isChecked() || this->ui->action_16->isChecked() || this->ui->action_17->isChecked() ||
                this->ui->action_18->isChecked() || this->ui->action_19->isChecked();
        if (!is_any_checked) {
            this->ui->action_18->setChecked(true);
        } else {
            this->_changed = true;
        }
    }
}

void MainWindow::on_action_19_toggled(bool state) // Скорость -> Очень медленно
{
    if (state) {
        this->ui->action_16->setChecked(false);
        this->ui->action_17->setChecked(false);
        this->ui->action_18->setChecked(false);
        this->ui->action_15->setChecked(false);
        this->_changed = true;
    } else {
        bool is_any_checked = this->ui->action_15->isChecked() || this->ui->action_16->isChecked() || this->ui->action_17->isChecked() ||
                this->ui->action_18->isChecked() || this->ui->action_19->isChecked();
        if (!is_any_checked) {
            this->ui->action_19->setChecked(true);
        } else {
            this->_changed = true;
        }
    }
}

void MainWindow::on_action_14_triggered() // О программе -> Автор
{
    QDesktopServices::openUrl(QUrl("https://irval1337.github.io/"));
}


void MainWindow::on_actionQT_triggered() // О программе -> QT
{
    QDesktopServices::openUrl(QUrl("https://www.qt.io/"));
}


void MainWindow::on_action_triggered() // Файл -> Новый
{
    this->exec->get_settings()->clear();
    this->update_from_settings();
    this->setWindowTitle("TuringEmulator");
}

void MainWindow::on_action_2_triggered() // Файл -> Открыть...
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открытие"), "", tr("Turing settings (*.tur)"));
    if (!this->exec->get_settings()->open(fileName)) {
        QMessageBox::warning(this, "TuringEmulator", "В процессе открытия произошла ошибка. Возможно, файл поврежден.");
    } else {
        this->update_from_settings();
        pointer_reset();
        this->setWindowTitle("TuringEmulator (" + fileName + ")");
    }
}

void MainWindow::on_action_3_triggered() // Файл -> Сохранить
{
    this->save_into_settings();
    if (!this->exec->get_settings()->save()) {
        QMessageBox::warning(this, "TuringEmulator", "В процессе сохранения произошла ошибка. Возможно, " \
                                 "вы пытаетесь сохранить файл по пустому пути. В таком случае воспользуйтесь \"Сохранить как...\"");
    } else {
        this->_changed = false;
    }
}


void MainWindow::on_action_4_triggered() // Файл -> Сохранить как...
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранение"), "", tr("Turing settings (*.tur)"));
    this->save_into_settings();
    this->exec->get_settings()->save_as(fileName);
    this->setWindowTitle("TuringEmulator (" + fileName + ")");
    this->_changed = false;
}


void MainWindow::on_problemBox_textChanged() // Условие задачи
{
    this->_changed = true;
}


void MainWindow::on_input_alphabetBox_textEdited(const QString &text) // Исходный алфавит
{
    static QString prev_input;

    bool check_char = true;
    for (qsizetype i = 0; i < text.size(); ++i) {
        check_char = check_char && !this->ui->additional_alphabetBox->text().contains(text.at(i));
    }

    if (Turing::Alphabet(text).validate() && check_char) {
        this->_changed = true;
        this->exec->set_input_alphabet(Turing::Alphabet(text));
    } else {
        this->ui->input_alphabetBox->setText(prev_input);
    }
    prev_input = *this->exec->get_input_alphabet();
    this->update_table();
}


void MainWindow::on_additional_alphabetBox_textEdited(const QString &text) // Дополнительный алфавит
{
    static QString prev_additional;

    bool check_char = true;
    for (qsizetype i = 0; i < text.size(); ++i) {
        check_char = check_char && !this->ui->input_alphabetBox->text().contains(text.at(i));
    }

    if (Turing::Alphabet(text).validate() && check_char) {
        this->_changed = true;
        this->exec->set_additional_alphabet(Turing::Alphabet(text));
    } else {
        this->ui->additional_alphabetBox->setText(prev_additional);
    }
    prev_additional = *this->exec->get_additional_alphabet();
    this->update_table();
}


void MainWindow::on_commentsBox_textChanged() // Комментарии
{
    this->_changed = true;
}

void MainWindow::pointer_reset() {
    QWidget* left = this->ui->horizontalLayout_2->itemAt(0)->widget();
    auto window = this->geometry();
    this->_pointer_x = left->mapToGlobal(QPoint(0,0)).x() - window.x() + 19 + 3;
    this->exec->reset_offset();
}

void MainWindow::on_action_11_triggered() // Лента -> Задать строку
{
    StringView* frm = new StringView(this, this->exec);
    frm->show();
    pointer_reset();
}


void MainWindow::on_action_5_triggered() // Лента -> Сохранить
{
    this->exec->get_settings()->set_input_string(this->exec->get_current_input_string());
    this->exec->set_current_string(this->exec->get_current_input_string());

    this->exec->set_input_string(this->exec->get_current_input_string());
    pointer_reset();
}


void MainWindow::on_action_6_triggered() // Лента -> Восстановить
{
    this->exec->set_current_string(*this->exec->get_settings()->get_input_string());

    this->exec->set_input_string(this->exec->get_current_input_string());
    pointer_reset();
}


void MainWindow::on_leftButton_clicked()
{
    if (this->_pointer_x - 35 < this->_lines[0]->x1() - 35) {
        return;
    }
    move_right();
    this->_pointer_x -= 35;
}


void MainWindow::on_addRowBtn_clicked() // Добавить
{
    this->_no_edit = true;
    this->ui->tableWidget->setRowCount(this->ui->tableWidget->rowCount() + 1);
    for(qsizetype i = 0; i < this->ui->tableWidget->columnCount(); ++i) {
        this->ui->tableWidget->setItem(this->ui->tableWidget->rowCount() - 1, i, new QTableWidgetItem(""));
    }

    QStringList labels;
    for(qsizetype i = 0; i < this->ui->tableWidget->rowCount(); ++i) {
        labels.push_back("Q" + QString::number(i));
    }
    this->ui->tableWidget->setVerticalHeaderLabels(labels);
    std::vector<QString> actions(this->ui->tableWidget->columnCount(), "");
    this->exec->get_table()->add_action(this->ui->tableWidget->rowCount() - 1, actions);
    this->_no_edit = false;
}


void MainWindow::on_delRowBtn_clicked() // Удалить
{
    this->_no_edit = true;
    int row = this->ui->tableWidget->currentRow();
    if (row <= 0 || row >= this->ui->tableWidget->rowCount()) return;
    for(qsizetype i = 0; i < this->ui->tableWidget->columnCount(); ++i) {
        delete this->ui->tableWidget->item(row, i);
    }
    this->exec->get_table()->remove_action(row);
    this->ui->tableWidget->removeRow(row);

    QStringList labels;
    for(qsizetype i = 0; i < this->ui->tableWidget->rowCount(); ++i) {
        labels.push_back("Q" + QString::number(i));
    }
    this->ui->tableWidget->setVerticalHeaderLabels(labels);
    this->_no_edit = false;
}


void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    if (this->_no_edit) return;
    std::vector<QString> actions;
    for(qsizetype i = 0; i < this->ui->tableWidget->columnCount(); ++i) {
        actions.push_back(this->ui->tableWidget->item(row, i)->text());
    }
    if (!this->exec->get_table()->change_action(row, actions)) {
        this->_no_edit = true;
        QMessageBox::warning(this, "TuringEmulator", "Неверный формат оформления таблицы.");
        delete this->ui->tableWidget->item(row, column);
        auto table = this->exec->get_table()->get_table();
        QString alph = *this->exec->get_input_alphabet() + '_' + *this->exec->get_additional_alphabet();
        QChar c = alph.at(column);
        Turing::Action act = table.at(row).at(c);
        this->ui->tableWidget->setItem(row, column, new QTableWidgetItem(action_to_string(act)));
        this->_no_edit = false;
    }
}

void MainWindow::on_rightButton_clicked()
{
    if (this->_pointer_x + 35 > this->_lines.back()->x1()) {
        return;
    }
    move_left();
    this->_pointer_x += 35;
}


void MainWindow::on_action_8_triggered()
{
    Turing::Action action = this->exec->next_action(this->_lines.size());
    if (action.get_symbol() == '\0' && action.get_move() == Turing::Action::None && action.get_next_state() == -2) {
        this->_run = false;
        set_enabled(true);
        QMessageBox::warning(this, "TuringEmulator", "Ошибка в заполнении таблицы. Текущая ячейка не заполнена.");
    } else {
        if (action.get_move() == Turing::Action::Right) move_pointer(true);
        else if (action.get_move() == Turing::Action::Left) move_pointer(false);

        if (action.get_next_state() == -2 || action.get_symbol() == '!') {
            this->_run = false;
            set_enabled(true);
        }
    }
}


void MainWindow::on_action_9_triggered()
{
    set_enabled(true);
    this->_run = false;
}


void MainWindow::on_action_7_triggered()
{
    set_enabled(false);
    this->save_into_settings();
    this->_run = true;
    this->_currect_tick = 0;
}

