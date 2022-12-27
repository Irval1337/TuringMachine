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

    void on_input_alphabetBox_textEdited();

    void on_additional_alphabetBox_textEdited();

    void on_commentsBox_textChanged();

    void on_action_11_triggered();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_leftButton_clicked();

    void updateTime();

private:
    void update_from_settings();
    void save_into_settings();
    void set_speed(int);
    int get_speed();

private:
    bool _changed;
    QRect* _tape = nullptr;
    QTimer* _timer;

private:
    Ui::MainWindow *ui;
    Turing::Executer *exec;
};
#endif // MAINWINDOW_H
