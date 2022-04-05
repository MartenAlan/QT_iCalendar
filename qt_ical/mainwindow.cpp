#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "icalendar.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <string.h>
#include <map>

using namespace std;

//Ostersonntag nach Jahr berechnen und als Datum zurückgeben
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

map <string, QDate> getOsterfeiertage(QDate ostern){



    map <string, QDate> feiertage = {{"Osternsonntag", ostern},
                                       };
    feiertage.insert({"Rosenmontag", ostern.addDays(-48)});
    feiertage.insert({"Faschingsdienstag", ostern.addDays(-47)});
    feiertage.insert({"Aschenmittwoch", ostern.addDays(-46)});
    feiertage.insert({"Palmsonntag", ostern.addDays(-7)});
    feiertage.insert({"Gründonnerstag", ostern.addDays(-3)});
    feiertage.insert({"Karfreitag", ostern.addDays(-2)});
    feiertage.insert({"Karsamstag", ostern.addDays(-1)});
    feiertage.insert({"Ostermontag", ostern.addDays(1)});
    feiertage.insert({"Christi Himmelfahrt", ostern.addDays(39)});
    feiertage.insert({"Pfingsmontag", ostern.addDays(49)});
    feiertage.insert({"Fronleichnam", ostern.addDays(60)});

    return (feiertage);
}


// falls die Ferien hinzugefügt werden sollen, wird diese Funktion ausgeführt.
string createHolidayText(int year){
    string holidayText;
    string dtstamp = QDateTime().currentDateTime().toString("yyyyMMddThhmmssZ").toUtf8().constData();
        map <string, QDate> holidayDates = getOsterfeiertage(getOstersonntag(year));
        // geht alle eingetragenen Feiertage in der Map durch, um jeweils ein VEvent zu erstellen ( map könnte auch als Parameter übergeben werden
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

    return holidayText;
}

// für alle Wochentage: nimmt den Index aus einer ComboBox und gibt die passende Abkürzung zurück
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_jahr->setText(QString::number(QDate::currentDate().year()));
    ui->input_dtstart->setDate(QDate::currentDate());
    ui->input_dtend->setDate(QDate::currentDate());
    ui->until->setDate(QDate::currentDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    RRule r = RRule();

    switch(ui->tabWidget->currentIndex()){  //verwendet den gerade ausgewählten Tab als Expression
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
      // nichts
      break;
    }

    // Kontrolliert, ob eine Dauer oder ein Enddatum für die Wiederholung angegeben wurde
    if(ui->count_radio->isChecked()){
        r.countOrUntil = "COUNT=" + to_string(ui->count->value());
    }
    else if(ui->until_radio->isChecked()){
        r.countOrUntil = "UNTIL=" + ui->until->date().toString("yyyyMMdd").toUtf8() + "T000000Z";
    }

    VAlarm v;
    v.setHinweis(ui->alarm_checkBox->isChecked());
    v.setTrigger( ui->alarm_line->text());
    v.setEinheit(ui->alarm_comboBox->currentIndex());
    v.setAction(ui->alarm_type->currentIndex());

    QDate ics_dtstart = ui->input_dtstart->date();
    QTime ics_dtstart_time = ui->input_dtstart->time();
    QDate ics_dtend = ui->input_dtend->date();
    QTime ics_dtend_time = ui->input_dtend->time();

    // Datum und Zeit für DtStamp
    string currentTime_date = QDateTime::currentDateTime().date().toString("yyyyMMdd").toUtf8().constData();
    string currentTime_time = QDateTime::currentDateTime().time().toString("HHmmss").toUtf8().constData();

    ICalendar ical;
    ical.prodid = ui->prod_id->text().toUtf8().constData();
    ical.version = ui->label_version->text().toUtf8().constData();
    ical.uid = ui->label_id->text().toUtf8().constData();
    ical.dtstart = ics_dtstart.toString("yyyyMMdd").toUtf8() + "T" + ics_dtstart_time.toString("HHmmss").toUtf8();
    ical.dtend = ics_dtend.toString("yyyyMMdd").toUtf8() + "T" +ics_dtend_time.toString("HHmmss").toUtf8();
    ical.dtstamp =  currentTime_date + "T" + currentTime_time;
    ical.summary = ui->titel->toPlainText().toUtf8().constData();

    if(ui->priority_check->isChecked()==1){
        ical.priority = "PRIORITY=" + to_string(ui->priority->value()) + "\n";
    }

    if(ui->radio_adress->isChecked()==1){
        ical.locationOrGeo = "LOCATION:" + ui->plz_text->toPlainText().toUtf8() + " " + ui->city_text->toPlainText().toUtf8() + ", " + ui->street_text->toPlainText().toUtf8() + "\n";
    }
    else if(ui->radio_geo->isChecked()==1){
        ical.locationOrGeo = "GEO:" + to_string(ui->latitude->value()) + ";" + to_string(ui->longitude->value()) + "\n";
    }

    // Übertragen von RRule und Alarm ins ICalendar Objekt
    ical.rrule = r;
    ical.va = v;

    // Feiertagselemente, falls vorhanden, werden hinzugefügt
    if(ui->checkBox_feiertage->isChecked()){
        ical.holidays = createHolidayText(ui->lineEdit_jahr->text().toInt());
    }
    else{
        ical.holidays = "";
    }

     QString filename = QFileDialog::getSaveFileName(this, "Save file","","*.ics");

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

void uncheckwithAutoExclusive(QRadioButton *radiobutton)
{       radiobutton->setAutoExclusive(false);
        radiobutton->setChecked(false);
        radiobutton->setAutoExclusive(true);
}

void MainWindow::on_btn_clear_clicked()
{
    ui->titel->clear();
    ui->beschreibung->clear();
    ui->city_text->clear();
    ui->street_text->clear();
    ui->plz_text->clear();
    ui->latitude->setValue(0.000000);
    ui->longitude->setValue(0.000000);

    uncheckwithAutoExclusive(ui->radio_geo);
    uncheckwithAutoExclusive(ui->radio_adress);

    ui->input_dtstart->setDateTime(QDateTime(QDate::currentDate(),QTime(0,0)));
    ui->input_dtend->setDateTime(QDateTime(QDate::currentDate(),QTime(0,0)));

    ui->priority->setValue(0);
    ui->priority_check->setChecked(false);

    ui->alarm_line->setText("15");
    ui->alarm_checkBox->setChecked(false);
    ui->alarm_comboBox->setCurrentIndex(0);

    ui->until->setDate(QDate::currentDate());
    ui->count->setValue(1);
    ui->checkBox_feiertage->setChecked(false);

    ui->interval_daily->setValue(1);
    ui->interval_weekly->setValue(1);
    ui->interval_monthly->setValue(1);

    ui->monday->setChecked(false);
    ui->tuesday->setChecked(false);
    ui->wednesday->setChecked(false);
    ui->thursday->setChecked(false);
    ui->friday->setChecked(false);
    ui->saturday->setChecked(false);
    ui->sunday->setChecked(false);

    uncheckwithAutoExclusive(ui->onday_monthly);
    uncheckwithAutoExclusive(ui->on_monthly);
    ui->monthly_combo->setCurrentIndex(0);
    ui->monthly_combo2->setCurrentIndex(0);
    ui->monthly_spin->setValue(1);

    uncheckwithAutoExclusive(ui->yearly_radio1);
    uncheckwithAutoExclusive(ui->yearly_radio2);
    ui->yearly_day->setCurrentIndex(0);
    ui->yearly_setpos->setCurrentIndex(0);
    ui->month_combo->setCurrentIndex(0);
    ui->month_combo2->setCurrentIndex(0);
    ui->monthday->setValue(1);

    ui->until_radio->setChecked(true);

    ui->alarm_type->setCurrentIndex(0);

    ui->lineEdit_jahr->setText(QString::number(QDate::currentDate().year()));

    ui->verticalLayoutWidget_6->repaint();
    ui->verticalLayoutWidget_7->repaint();
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    // wenn Tab 1 ( Keine Wiederholung ) ausgewählt ist, wird die "endet"-Box disabled
    if(index==0){
        ui->groupBox->setDisabled(true);

    // sonst ist sie enabled
    }
    else{
        ui->groupBox->setDisabled(false);
    }
}


void MainWindow::on_input_dtstart_dateTimeChanged(const QDateTime &dateTime)
{
    ui->input_dtend->setDateTime(dateTime);
}

