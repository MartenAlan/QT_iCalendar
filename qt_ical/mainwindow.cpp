#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString ics_str = ui->label_id->text() + "\n" + ui->label_version->text() + "\n" + ui->prod_id->text() + "\n";


    ui->output->setText(ics_str);
}

