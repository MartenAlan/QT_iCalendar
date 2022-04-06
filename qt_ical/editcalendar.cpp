#include "editcalendar.h"
#include "ui_editcalendar.h"

editCalendar::editCalendar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editCalendar)
{
    ui->setupUi(this);
}

editCalendar::~editCalendar()
{
    delete ui;
}

void editCalendar::setValues(QString n, QString v, QString pid){
    ui->name->setText(n);
    ui->version->setText(v);
    ui->prodid->setText(pid);
}

QString editCalendar::getValue(){
    QString value = ui->name->text();
    return value;
}
