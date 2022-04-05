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

void editCalendar::setValues(QString n, QString d, QString v, QString pid){
    ui->name->setText(n);
    ui->description->setPlainText(d);
    ui->version->setText(v);
    ui->prodid->setText(pid);
}

QString* editCalendar::getValues(){
    static QString values[2];
    values[0] = ui->name->text();
    values[1] = ui->description->toPlainText();

    return values;
}
