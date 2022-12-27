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

    this->_changed = false;

    this->_timer = new QTimer(this); // Создаем объект класса QTimer и передаем адрес переменной
        this->_timer->setInterval(50); // Задаем интервал таймера
        connect(this->_timer, SIGNAL(timeout()), this, SLOT(updateTime())); // Подключаем сигнал таймера к нашему слоту
        this->_timer->start(); // Запускаем таймер
}

MainWindow::~MainWindow()
{
    delete ui;
    delete exec;
    delete _timer;
}

void MainWindow::paintEvent(QPaintEvent *event)
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
    painter.setBrush(QBrush(QColor(0xfefee2), Qt::SolidPattern));
    this->_tape = new QRect(left->mapToGlobal(QPoint(0,0)).x() - window.x() + 19, left->mapToGlobal(QPoint(0,0)).y() - window.y() + 1,
                        right->mapToGlobal(QPoint(0, 0)).x() - left->mapToGlobal(QPoint(0,0)).x() - 19, 37);
    painter.drawRect(*this->_tape);
}

void MainWindow::updateTime()
{
    this->repaint();
}

void MainWindow::resizeEvent(QResizeEvent* event)
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


void MainWindow::update_from_settings() { // Перенести данные из настроек в форму
    this->ui->problemBox->setPlainText(*this->exec->get_settings()->get_problem());
    this->ui->input_alphabetBox->setText(*this->exec->get_settings()->get_input_alphabet());
    this->ui->additional_alphabetBox->setText(*this->exec->get_settings()->get_additional_alphabet());
    this->ui->commentsBox->setPlainText(*this->exec->get_settings()->get_comments());
    this->set_speed(*this->exec->get_settings()->get_speed());
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


void MainWindow::on_input_alphabetBox_textEdited() // Исходный алфавит
{
    this->_changed = true;
}


void MainWindow::on_additional_alphabetBox_textEdited() // Дополнительный алфавит
{
    this->_changed = true;
}


void MainWindow::on_commentsBox_textChanged() // Комментарии
{
    this->_changed = true;
}


void MainWindow::on_action_11_triggered() // Лента -> Задать строку
{
    StringView* frm = new StringView(this, this->exec);
    frm->show();
}


void MainWindow::on_action_5_triggered() // Лента -> Сохранить
{
    this->exec->get_settings()->set_input_string(this->exec->get_current_string());
    this->exec->set_current_string(this->exec->get_current_string());
}


void MainWindow::on_action_6_triggered() // Лента -> Восстановить
{
    this->exec->set_current_string(*this->exec->get_settings()->get_input_string());
}


void MainWindow::on_leftButton_clicked()
{

}

