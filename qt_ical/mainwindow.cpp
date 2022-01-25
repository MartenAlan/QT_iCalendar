#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <string.h>

using namespace std;


class RRule {
public:
    string rruleText;
    string state;
    string interval;
    string countOrUntil;
    string byday;
    string bysetpos;
public:
    string getState(int statenumber){
        switch ( statenumber ) {
        case 0:
            cout << "Keine Wiederholung" << endl;
            state = "empty";
          break;
        case 1:
            cout << "Tägliche Wiederholung" << endl;
            state = "DAILY";
          break;
        case 2:
            cout << "Wöchentliche Wiederholung" << endl;
            state = "WEEKLY";
          break;
        case 3:
            cout << "Monatliche Wiederholung" << endl;
            state = "MONTHLY";
          break;
        case 4:
            cout << "Jährliche Wiederholung" << endl;
            state = "YEARLY";
          break;
        default:
          // Code
          break;
        }
    return state;
    }

    string buildRRuleText(){

        if(state.compare("empty")==0){
            return "";}

        else {
            rruleText = "RRULE:";
            rruleText += "FREQ=" + state + ";";
            rruleText += bysetpos;
            rruleText += byday;
              rruleText += interval;
              rruleText += countOrUntil;
              rruleText += "\n";
                return rruleText;
            }
}

};

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
    RRule rrule;
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

        icsText += rrule.buildRRuleText();


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


    RRule r = RRule();
    r.state = r.getState(ui->tabWidget->currentIndex());

    if(r.state.compare("DAILY")==0){
        r.interval = "INTERVAL=" + to_string(ui->interval_daily->value())+";";
    }
    else if(r.state.compare("WEEKLY")==0){
        r.byday = "BYDAY=";
        if(ui->monday->isChecked()==1){
            r.byday += "MO,";
        }
        if(ui->tuesday->isChecked()==1){
            r.byday += "TU,";
        }
        if(ui->wednesday->isChecked()==1){
            r.byday += "WE,";
        }
        if(ui->thursday->isChecked()==1){
            r.byday += "TH,";
        }
        if(ui->friday->isChecked()==1){
            r.byday += "FR,";
        }
        if(ui->saturday->isChecked()==1){
            r.byday += "SA,";
        }
        if(ui->sunday->isChecked()==1){
            r.byday += "SU,";
        }
        r.byday += ";";
        r.interval = "INTERVAL=" + to_string(ui->interval_weekly->value()) + ";";

    }
    else if(r.state.compare("MONTHLY")==0){
        r.interval = "INTERVAL=" + to_string(ui->interval_monthly->value()) + ";";

        if(ui->onday_monthly->isChecked()==1){
            r.byday = "BYMONTHDAY=" + to_string(ui->monthly_spin->value()) + ";";
        }
        else if(ui->on_monthly->isChecked()==1){
            r.bysetpos = "BYSETPOS=";
            cout << ui->monthly_combo->currentIndex() << endl;

            switch (ui->monthly_combo->currentIndex()){
            case 0:
                r.bysetpos += "1;";
                break;
            case 1:
                r.bysetpos += "2;";
                break;
            case 2:
                r.bysetpos += "3;";
                break;
            case 3:
                r.bysetpos += "4;";
                break;
            case 4:
                r.bysetpos += "-1;";
                break;
            default:
                break;}

            r.byday = "BYDAY=";

            switch (ui->monthly_combo2->currentIndex()){
            case 0:
                r.byday += "MO;";
                break;
            case 1:
                r.byday += "TU;";
                break;
            case 2:
                r.byday += "WE;";
                break;
            case 3:
                r.byday += "TH;";
                break;
            case 4:
                r.byday += "FR;";
                break;
            case 5:
                r.byday += "SA;";
                break;
            case 6:
                r.byday += "SO;";
                break;
            default:
                break;}

        }

    }
    else if(r.state.compare("YEARLY")==0){
        cout << "Jährlich" << endl;



    }

    if(ui->count_radio->isChecked()){
        r.countOrUntil = "COUNT=" + to_string(ui->count->value());
    }
    else if(ui->until_radio->isChecked()){
        r.countOrUntil = "UNTIL=" + ui->until->date().toString("yyyyMMdd").toUtf8() + "T000000Z";
    }


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

    ical.rrule = r;


    ui->output->setText(ics_str + "\n" + ics_str2 + "\n" + dtstart_dtend + "\n" + currentTime.toString() + "\n");
    ui->output->setText(ical.buildICSText().c_str());

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

    cout << ui->tabWidget->currentIndex() << endl;

}

