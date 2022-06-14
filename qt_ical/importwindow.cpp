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

void importWindow::fillTable(list<VEvent> eventlist){
    int row = ui->table_events->rowCount();
for (VEvent x : eventlist){
    ui->table_events->insertRow(row);
     QTableWidgetItem* description = new QTableWidgetItem(QString::fromStdString(x.description));
    ui->table_events->setItem(row,3,description);
    /*ui->table_events->setItem(row,0,name);
    ui->table_events->setItem(row,1,loeschen);
    ui->table_events->setItem(row,2,dtstart);
    ui->table_events->setItem(row,3,dtend);
    ui->table_events->setItem(row,4,beschreibung);
    ui->table_events->setItem(row,5,rrule);
    ui->table_events->setItem(row,6,valarm);
    ui->table_events->setItem(row,7,geo);
    ui->table_events->setItem(row,8,location);
    ui->table_events->setItem(row,9,priority);
    ui->table_events->setItem(row,10,dtstamp);
    ui->table_events->setItem(row,11,uid);*/
}
}


