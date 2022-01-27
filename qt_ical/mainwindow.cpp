#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <string.h>
#include <map>


using namespace std;


QDate getOstersonntag(int year){
    int a = year % 19;
    int b = year / 100;
    int c = year % 100;
    int d = b / 4;
    int e = b % 4;
    int f = (b + 8) / 25;
    int g = (b - f + 1) / 3;
    int h = (19*a + b - d - g + 15) % 30;
    int i = c / 4;
    int k = c % 4;
    int l = (32 + 2*e + 2*i - h - k) % 7;
    int m = (a + 11*h + 22*l) / 451;
    int n = (h + l - 7*m + 114) / 31;
    int p = (h + l - 7*m + 114) % 31;

    QDate ostern = QDate::fromString(QString::number(p+1) + "." + QString::number(n) + "." + QString::number(year), "d.M.yyyy");
    return (ostern);
}

QString getOsterfeiertage(QDate ostern){

    QString feiertage;
    feiertage += "Ostersonntag - " + ostern.toString() + "\n";
    QDate rosenmontag = ostern.addDays(-48);
    feiertage += "Rosenmontag - " + rosenmontag.toString() + "\n";
    QDate faschingsdienstag = ostern.addDays(-47);
    feiertage += "Faschingsdienstag - " + faschingsdienstag.toString() + "\n";
    QDate aschenmittwoch = ostern.addDays(-46);
    feiertage += "Aschenmittwoch - " + aschenmittwoch.toString() + "\n";
    QDate palmsonnteg = ostern.addDays(-7);
    feiertage += "Palmsonntag - " + palmsonnteg.toString() + "\n";
    QDate gruendonnerstag = ostern.addDays(-3);
    feiertage += "Gründonnerstag - " + gruendonnerstag.toString() + "\n";
    QDate karfreitag = ostern.addDays(-2);
    feiertage += "Karfreitag - " + karfreitag.toString() + "\n";
    QDate karsamstag = ostern.addDays(-1);
    feiertage += "Karsamstag - " + karsamstag.toString() + "\n";
    QDate ostermontag = ostern.addDays(1);
    feiertage += "Ostermontag - " + ostermontag.toString() + "\n";
    QDate ch_himm = ostern.addDays(39);
    feiertage += "Christi Himmelfahrt - " + ch_himm.toString() + "\n";
    QDate pfingsmontag = ostern.addDays(49);
    feiertage += "Pfingsmontag - " + pfingsmontag.toString() + "\n";
    QDate fronleichnam = ostern.addDays(60);
    feiertage += "Fronleichnam - " + fronleichnam.toString() + "\n";

    return (feiertage);
}


string createHolidayText(bool checked){
    string holidayText;
    string dtstamp = QDateTime().currentDateTime().toString("yyyyMMddThhmmssZ").toUtf8().constData();
    if(checked==true){
        map <string, QDate> holidayDates = {{"Ostern", getOstersonntag(2022)},
                                           };  // hier füge ich die anderen Feiertage ein.


        for (const auto& elem : holidayDates)
        {
           holidayText+= "BEGIN:VEVENT\n";
           holidayText += "SUMMARY:" + elem.first + "\n";
           string start = elem.second.toString("yyyyMMdd").toUtf8().constData();
           holidayText += "DTSTART:" + start + "\n";
           holidayText += "DURATION:P1D\n";
           holidayText += "DTSTAMP:" + dtstamp + "\n";
           holidayText += "UID:" + elem.first + "_holidays\n";
           holidayText += "END:VEVENT\n";
        }

    }
    else{
        cout << "Holidays nicht einfügen" << endl;
        holidayText = "";
    }

    return holidayText;
}

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
    string locationOrGeo;
    string priority;
    string holidays;
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
        icsText += priority;
        icsText += locationOrGeo;
        icsText += rrule.buildRRuleText();
        icsText += va.buildAlarm().toUtf8();
        icsText += "END:VEVENT\n";

        icsText += holidays;

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

    switch(ui->tabWidget->currentIndex()){
        case 0:
        r.state = "empty";
      break;
        case 1:
        r.state = "DAILY";
        r.interval = "INTERVAL=" + to_string(ui->interval_daily->value())+";";
      break;
        case 2:
        r.state = "WEEKLY";
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
      break;
        case 3:
        r.state = "MONTHLY";
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
            r.byday += getWeekDay(ui->monthly_combo2->currentIndex());}
      break;
        case 4:
        r.state = "YEARLY";
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
      break;
    default:
      // Code
      break;
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

    QDate ics_dtstart = ui->input_dtstart->date();
    QTime ics_dtstart_time = ui->input_dtstart->time();
    QDate ics_dtend = ui->input_dtend->date();
    QTime ics_dtend_time = ui->input_dtend->time();

    QDateTime currentTime = QDateTime::currentDateTime();
    string currentTime_date = currentTime.date().toString("yyyyMMdd").toUtf8().constData();
    string currentTime_time = currentTime.time().toString("HHmmss").toUtf8().constData();

    QString filename = QFileDialog::getSaveFileName(this, "Save file");

    iCal ical;
    ical.prodid = ui->prod_id->text().toUtf8().constData();
    ical.version = ui->label_version->text().toUtf8().constData();
    ical.uid = ui->label_id->text().toUtf8().constData();
    ical.dtstart = ics_dtstart.toString("yyyyMMdd").toUtf8() + "T" + ics_dtstart_time.toString("HHmmss").toUtf8();
    ical.dtend = ics_dtend.toString("yyyyMMdd").toUtf8() + "T" +ics_dtend_time.toString("HHmmss").toUtf8();
    ical.dtstamp =  currentTime_date + "T" + currentTime_time;
    ical.summary = ui->titel->toPlainText().toUtf8().constData() ;

    if(ui->radio_adress->isChecked()==1){
        ical.locationOrGeo = "LOCATION:" + ui->plz_text->toPlainText().toUtf8() + " " + ui->city_text->toPlainText().toUtf8() + ", " + ui->street_text->toPlainText().toUtf8() + "\n";
    }
    else if(ui->radio_geo->isChecked()==1){
        ical.locationOrGeo = "GEO:" + to_string(ui->latitude->value()) + ";" + to_string(ui->longitude->value()) + "\n";
    }

    ical.rrule = r;
    ical.va = v;

    ical.holidays = createHolidayText(ui->holidays_check->isChecked());

    if(ui->priority_check->isChecked()==1){
        ical.priority = "PRIORITY=" + to_string(ui->priority->value()) + "\n";
    }

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
}

