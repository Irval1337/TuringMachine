#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setSizes(QList<int>({600, 200}));
    ui->splitter_3->setSizes(QList<int>({200, 800}));
    ui->splitter_2->setSizes(QList<int>({200, 200}));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_15_toggled(bool state)
{
    if (state) {
        ui->action_16->setChecked(false);
        ui->action_17->setChecked(false);
        ui->action_18->setChecked(false);
        ui->action_19->setChecked(false);
    } else {
        bool is_any_checked = ui->action_15->isChecked() || ui->action_16->isChecked() || ui->action_17->isChecked() ||
                ui->action_18->isChecked() || ui->action_19->isChecked();
        if (!is_any_checked) {
            ui->action_15->setChecked(true);
        }
    }
}

void MainWindow::on_action_16_toggled(bool state)
{
    if (state) {
        ui->action_15->setChecked(false);
        ui->action_17->setChecked(false);
        ui->action_18->setChecked(false);
        ui->action_19->setChecked(false);
    } else {
        bool is_any_checked = ui->action_15->isChecked() || ui->action_16->isChecked() || ui->action_17->isChecked() ||
                ui->action_18->isChecked() || ui->action_19->isChecked();
        if (!is_any_checked) {
            ui->action_16->setChecked(true);
        }
    }
}

void MainWindow::on_action_17_toggled(bool state)
{
    if (state) {
        ui->action_16->setChecked(false);
        ui->action_15->setChecked(false);
        ui->action_18->setChecked(false);
        ui->action_19->setChecked(false);
    } else {
        bool is_any_checked = ui->action_15->isChecked() || ui->action_16->isChecked() || ui->action_17->isChecked() ||
                ui->action_18->isChecked() || ui->action_19->isChecked();
        if (!is_any_checked) {
            ui->action_17->setChecked(true);
        }
    }
}

void MainWindow::on_action_18_toggled(bool state)
{
    if (state) {
        ui->action_16->setChecked(false);
        ui->action_17->setChecked(false);
        ui->action_15->setChecked(false);
        ui->action_19->setChecked(false);
    } else {
        bool is_any_checked = ui->action_15->isChecked() || ui->action_16->isChecked() || ui->action_17->isChecked() ||
                ui->action_18->isChecked() || ui->action_19->isChecked();
        if (!is_any_checked) {
            ui->action_18->setChecked(true);
        }
    }
}

void MainWindow::on_action_19_toggled(bool state)
{
    if (state) {
        ui->action_16->setChecked(false);
        ui->action_17->setChecked(false);
        ui->action_18->setChecked(false);
        ui->action_15->setChecked(false);
    } else {
        bool is_any_checked = ui->action_15->isChecked() || ui->action_16->isChecked() || ui->action_17->isChecked() ||
                ui->action_18->isChecked() || ui->action_19->isChecked();
        if (!is_any_checked) {
            ui->action_19->setChecked(true);
        }
    }
}
