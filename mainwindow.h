#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TuringExecuter.h"
#include "QCloseEvent"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent*) override;

private slots:
    void on_action_15_toggled(bool);
    void on_action_16_toggled(bool);
    void on_action_17_toggled(bool);
    void on_action_18_toggled(bool);
    void on_action_19_toggled(bool);

    void on_action_14_triggered();
    void on_actionQT_triggered();

    void on_action_triggered();

    void on_action_3_triggered();

    void on_action_2_triggered();

    void on_action_4_triggered();

    void on_problemBox_textChanged();

    void on_input_alphabetBox_textEdited(const QString&);

    void on_additional_alphabetBox_textEdited(const QString&);

    void on_commentsBox_textChanged();

    void on_action_11_triggered();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_leftButton_clicked();

    void update_time();

    void on_addRowBtn_clicked();

    void on_delRowBtn_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_rightButton_clicked();

    void on_action_8_triggered();

    void on_action_9_triggered();

    void on_action_7_triggered();

private:
    void update_from_settings();
    void save_into_settings();
    void set_speed(int);
    int get_speed();
    void update_table();
    void move_left();
    void move_right();
    void move_pointer(bool);
    void set_enabled(bool);
    void pointer_reset();

private:
    bool _undo_input = false, _undo_additional = false;
    bool _changed;
    bool _no_edit = false;
    bool _run = false;
    int _currect_tick = 0;
    QRect* _tape = nullptr;
    QImage* _pointer = nullptr;
    int _pointer_x = 0;
    int _pointer_move_type = 0;

    QVector<QLine*> _lines;
    QTimer* _timer;
    QFont _monospace;

private:
    Ui::MainWindow *ui;
    Turing::Executer *exec;
};
#endif // MAINWINDOW_H
