#ifndef STRINGVIEW_H
#define STRINGVIEW_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class StringView;
}

class StringView : public QDialog
{
    Q_OBJECT

public:
    StringView(QWidget *parent = nullptr);
    StringView(QWidget *parent, Turing::Executer *exec);
    ~StringView();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::StringView *ui;
    Turing::Executer *exec;
};

#endif // STRINGVIEW_H
