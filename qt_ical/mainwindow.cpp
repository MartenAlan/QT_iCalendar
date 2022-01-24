#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <string.h>

using namespace std;

class iCal {
    public:
    string uid;
    string version;
    string prodid;
    string summary;
    string description;
    string dtstart;
    string dtend;
    string dtstamp;
    string geoLat;
    string geoLon;
    string location;

    string buildICSText(){

        string icsText = "BEGIN:VCALENDAR\n";
        icsText += "VERSION:" + version + "\n";
        icsText += "PRODID:" + prodid + "\n";
        icsText += "BEGIN:VEVENT\n";
        icsText += "SUMMARY:" + summary + "\n";
        icsText += "DTSTAMP:" + dtstamp + "\n";
        icsText += "DTSTART:" + dtstart + "\n";
        icsText += "DTEND:" + dtend + "\n";
        icsText += "UID:" + uid + "\n";

        if(geoLat != "0.000000")
        {
            icsText += "GEO:" + geoLat + ";" + geoLon + "\n";
        }
        else if(location.length()>0){
            icsText += "LOCATION:" + location + "\n";
        }


        icsText += "END:VEVENT\n";
        icsText += "END:VCALENDAR";
        return icsText;
    }


};

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
    QString ics_str2 = ui->titel->toPlainText() + "\n" + ui->beschreibung->toPlainText();

    QDate ics_dtstart = ui->input_dtstart->date();
    QTime ics_dtstart_time = ui->input_dtstart->time();
    QDate ics_dtend = ui->input_dtend->date();
    QTime ics_dtend_time = ui->input_dtend->time();
    QString dtstart_dtend = ics_dtstart.toString("yyyyMMdd") + "T" + ics_dtstart_time.toString("HHmmss") + "\n" +ics_dtend.toString();


    QDateTime currentTime = QDateTime::currentDateTime();
    string currentTime_date = currentTime.date().toString("yyyyMMdd").toUtf8().constData();
    string currentTime_time = currentTime.time().toString("HHmmss").toUtf8().constData();




    QString fileText = ics_str + "\n" + ics_str2 + "\n" + dtstart_dtend;
    std::string utf8_text = fileText.toUtf8().constData();

    QString filename = QFileDialog::getSaveFileName(this, "Save file");

    iCal ical;
    ical.prodid = ui->prod_id->text().toUtf8().constData();
    ical.version = ui->label_version->text().toUtf8().constData();
    ical.uid = ui->label_id->text().toUtf8().constData();
    ical.dtstart = ics_dtstart.toString("yyyyMMdd").toUtf8() + "T" + ics_dtstart_time.toString("HHmmss").toUtf8();
    ical.dtend = ics_dtend.toString("yyyyMMdd").toUtf8() + "T" +ics_dtend_time.toString("HHmmss").toUtf8();
    ical.dtstamp =  currentTime_date + "T" + currentTime_time;
    ical.summary = ui->titel->toPlainText().toUtf8().constData() ;

    string street = ui->street_text->toPlainText().toUtf8().constData();
    string city = ui->city_text->toPlainText().toUtf8().constData();
    string plz = ui->city_text->toPlainText().toUtf8().constData();

    ical.location = plz + " " + city + ", " + street;

    ical.geoLat = to_string(ui->latitude->value());

    ical.geoLon = to_string(ui->longitude->value());


    ui->output->setText(ics_str + "\n" + ics_str2 + "\n" + dtstart_dtend + "\n" + currentTime.toString() + "\n");

    FILE *o_file = fopen(filename.toUtf8(), "w+");
    if (o_file){
        fwrite(ical.buildICSText().c_str(), 1, ical.buildICSText().size(), o_file);
        QMessageBox::information(this, "iCal", "Done Writing!");
    }
    else{
        QMessageBox::critical(this, "iCal", "Something went wrong!");
    }
    fclose(o_file);

}


void MainWindow::on_btn_clear_clicked()
{
    ui->titel->clear();
    ui->beschreibung->clear();
    ui->output->clear();
    ui->city_text->clear();
    ui->street_text->clear();
    ui->plz_text->clear();
    ui->latitude->setValue(0.000000);
    ui->longitude->setValue(0.000000);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index==0){
        ui->groupBox->setDisabled(true);

    }
    else{
        ui->groupBox->setDisabled(false);
    }
}

