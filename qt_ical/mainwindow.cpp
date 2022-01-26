#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <string.h>

using namespace std;

string getWeekDay(int num){
    string day;
    switch (num){
    case 0:
        day += "MO;";
        break;
    case 1:
        day += "TU;";
        break;
    case 2:
        day += "WE;";
        break;
    case 3:
        day += "TH;";
        break;
    case 4:
        day += "FR;";
        break;
    case 5:
        day += "SA;";
        break;
    case 6:
        day += "SU;";
        break;
    default:
        break;}
    return day;
}

class RRule {
public:
    string rruleText;
    string state;
    string interval;
    string countOrUntil;
    string byday;
    string bymonth;
    string byyear;
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
            rruleText += bymonth;
              rruleText += interval;
              rruleText += countOrUntil;
              rruleText += "\n";
                return rruleText;
            }
}

};

class VAlarm{
public:
    QString wert_trigger;
    int index_einheit;
    int index_action;
    bool hinweis;
    QString buildAlarm(){
        QString alarm;
        if (hinweis == true){
            alarm += "BEGIN:VALARM\n";
            switch (index_action) {
            case 0:
                alarm += "ACTION:AUDIO\nATTACH;VALUE=URI:Chord\n";
                break;
            case 1:
                alarm += "ACTION:DISPLAY\n";
                break;
            default:
                break;
            }
            switch (index_einheit) {
            case 0:
                alarm += "TRIGGER:-PT" + wert_trigger + "M\n";
                break;
            case 1:
                alarm += "TRIGGER:-PT" + wert_trigger + "H\n";
                break;
            case 2:
                alarm += "TRIGGER:-PT" + wert_trigger + "D\n";
                break;
            default:
                break;
            }
            alarm += "END:VALARM\n";
        }

        return alarm;
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
    VAlarm va;
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
        icsText += va.buildAlarm().toUtf8();

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

            if (ui->monthly_combo->currentIndex() == 4){
                r.bysetpos += "-1;";
            }
            else{
                r.bysetpos += to_string(ui->monthly_combo->currentIndex() + 1) + ";";
            }

            r.byday = "BYDAY=";
            r.byday += getWeekDay(ui->monthly_combo2->currentIndex());


        }

    }
    else if(r.state.compare("YEARLY")==0){
        cout << "Jährlich" << endl;
        if(ui->yearly_radio1->isChecked()==1){
        r.bymonth = "BYMONTH=" + to_string(ui->month_combo->currentIndex()+1) + ";";
        r.byday = "BYDAY=";
        r.byday += getWeekDay(ui->yearly_day->currentIndex());
        if (ui->yearly_setpos->currentIndex() == 4){
            r.bysetpos += "-1;";
        }
        else{
            r.bysetpos += to_string(ui->yearly_setpos->currentIndex() + 1) + ";";
        }
        }
        else if(ui->yearly_radio2->isChecked()==1)
        {
            r.bymonth = "BYMONTH=" + to_string(ui->month_combo2->currentIndex()+1) + ";";
            r.byday = "BYMONTHDAY=" + to_string(ui->monthday->value()) + ";";
        }

    }

    if(ui->count_radio->isChecked()){
        r.countOrUntil = "COUNT=" + to_string(ui->count->value());
    }
    else if(ui->until_radio->isChecked()){
        r.countOrUntil = "UNTIL=" + ui->until->date().toString("yyyyMMdd").toUtf8() + "T000000Z";
    }

    VAlarm v;
        v.hinweis = ui->alarm_checkBox->isChecked();
        v.wert_trigger = ui->alarm_line->text();
        v.index_einheit = ui->alarm_comboBox->currentIndex();
        v.index_action = ui->alarm_type->currentIndex();

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
    ical.va = v;

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

