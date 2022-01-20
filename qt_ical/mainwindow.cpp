#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <iostream>

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


    string buildICSText(){

        string icsText = "BEGIN VCALENDAR\n";
        icsText += "VERSION:" + version + "\n";
        icsText += "PRODID:" + prodid + "\n";
        icsText += "BEGIN:VEVENT\n";
        icsText += "SUMMARY:" + summary + "\n";
        icsText += "DTSTART:" + dtstart + "\n";
        icsText += "DTEND:" + dtend + "\n";
        icsText += "UID:" + uid + "\n";
        icsText += "END:VEVENT\n";
        icsText += "END VCALENDAR";
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
    QDate ics_dtend = ui->input_dtend->date();

    QString dtstart_dtend = ics_dtstart.toString() + "->" + ics_dtend.toString();

    ui->output->setText(ics_str + "\n" + ics_str2 + "\n" + dtstart_dtend);

    QString fileText = ics_str + "\n" + ics_str2 + "\n" + dtstart_dtend;
    std::string utf8_text = fileText.toUtf8().constData();

    QString filename = QFileDialog::getSaveFileName(this, "Save file");

    iCal ical;
    ical.prodid = ui->prod_id->text().toUtf8().constData();
    ical.version = ui->label_version->text().toUtf8().constData();
    ical.uid = ui->label_id->text().toUtf8().constData();
    ical.dtstart = ics_dtstart.toString().toUtf8();
    ical.dtend = ics_dtend.toString().toUtf8();
    ical.summary = ui->titel->toPlainText().toUtf8().constData();

    FILE *o_file = fopen(filename.toUtf8(), "w+");
    if (o_file){
        fwrite(ical.buildICSText().c_str(), 1, ical.buildICSText().size(), o_file);
        cout << "Done Writing!" << endl;
    }
    else{
        cout << "Something went wrong!" << endl;
    }
    fclose(o_file);

}




