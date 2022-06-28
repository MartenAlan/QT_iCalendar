#include "importwindow.h"
#include "ui_importwindow.h"

importWindow::importWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importWindow)
{
    ui->setupUi(this);
}

importWindow::~importWindow()
{
    delete ui;
}

void importWindow::setCalendarName(QString prodid){
    ui->label_prodid->setText(prodid);
}

void importWindow::getCount(int count){

    ui->label_count->setText(QString::number(count));
}


