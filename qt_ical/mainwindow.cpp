#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "icalendar.h"
#include "editcalendar.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <string.h>
#include <map>
#include <regex>

using namespace std;

// Zufälliger Hexastring für den Unique Identifier
string hex_string(int length)
{
  char hex_characters[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  char str[length];
  int i;
  srand(time(0));
  for(i=0;i<length;i++)
  {
    str[i]=hex_characters[rand()%16];
  }
  str[length]=0;
  return str;
}


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

// Hinzufügen von allen Feiertagen ausgehend vom Osterdatum
map <string, QDate> getOsterfeiertage(QDate ostern){

    map <string, QDate> feiertage = {{"Ostersonntag", ostern},
                                       };
    feiertage.insert({"Rosenmontag", ostern.addDays(-48)});
    feiertage.insert({"Faschingsdienstag", ostern.addDays(-47)});
    feiertage.insert({"Aschermittwoch", ostern.addDays(-46)});
    feiertage.insert({"Palmsonntag", ostern.addDays(-7)});
    feiertage.insert({"Gründonnerstag", ostern.addDays(-3)});
    feiertage.insert({"Karfreitag", ostern.addDays(-2)});
    feiertage.insert({"Karsamstag", ostern.addDays(-1)});
    feiertage.insert({"Ostermontag", ostern.addDays(1)});
    feiertage.insert({"Christi Himmelfahrt", ostern.addDays(39)});
    feiertage.insert({"Pfingstmontag", ostern.addDays(49)});
    feiertage.insert({"Fronleichnam", ostern.addDays(60)});

    return (feiertage);
}

// falls die Ferien hinzugefügt werden sollen, wird diese Funktion ausgeführt.
string createHolidayText(int year){
    string holidayText;
    string dtstamp = QDateTime().currentDateTime().toString("yyyyMMddThhmmssZ").toUtf8().constData();
        map <string, QDate> holidayDates = getOsterfeiertage(getOstersonntag(year));
        // geht alle eingetragenen Feiertage in der Map durch, um jeweils ein VEvent zu erstellen
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

QString re(QString name)
{
    QString tname = name;
    static QRegularExpression re1("[^!a-zA-Z0-9\\sßÜüÄäÖö]");
    static QRegularExpression re2("[!]");
    static QRegularExpression re3("\\s+");
    name.remove(re1);
    name.remove(re2);
    name.replace(re3, " ");
    QMessageBox msgBox;
    msgBox.setText("Soll Event als " + name + " gespeichert werden?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Ja");
    msgBox.setButtonText(QMessageBox::No, "Nein");
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes){
        tname = name;
    }

    return tname;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_jahr->setText(QString::number(QDate::currentDate().year()));
    ui->input_dtstart->setDate(QDate::currentDate());
    ui->input_dtend->setDate(QDate::currentDate());
    ui->until->setDate(QDate::currentDate());
    ui->label_id->setText(QString::fromStdString(hex_string(10)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void uncheckwithAutoExclusive(QRadioButton *radiobutton)
{       radiobutton->setAutoExclusive(false);
        radiobutton->setChecked(false);
        radiobutton->setAutoExclusive(true);
}

void MainWindow::on_btn_clear_clicked()
{
    clearInputs();
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

void MainWindow::clearInputs(){
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

    ui->count_radio->setChecked(true);

    ui->alarm_type->setCurrentIndex(0);
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_editCalendar_button_clicked()
{
    QString calendar_name = ui->label_calendar_name->text();
    QString calendar_proid = ui->label_prod_id->text();
    QString calendar_version = ui->label_version->text();
    editCalendar editC;
    editC.setModal(true);
    editC.setValues(calendar_name, calendar_version, calendar_proid);
    editC.exec();
    QString name = editC.getValue();
    ui->label_calendar_name->setText(name);

}

// Fügt einen Termin in die Termintabelle hinzu
void MainWindow::on_button_add_event_clicked()
{
     QTableWidgetItem* name = new QTableWidgetItem(re(ui->titel->toPlainText()));
     QTableWidgetItem* dtstart = new QTableWidgetItem(ui->input_dtstart->dateTime().toString("yyyyMMddTHHmmss"));
     QTableWidgetItem* dtend = new QTableWidgetItem(ui->input_dtend->dateTime().toString("yyyyMMddTHHmmss"));
     QTableWidgetItem* priority = new QTableWidgetItem();
     QTableWidgetItem* location = new QTableWidgetItem();
     QTableWidgetItem* geo = new QTableWidgetItem();
     if(ui->radio_adress->isChecked()==1){
         location->setText(ui->plz_text->toPlainText() + " " + ui->city_text->toPlainText() + ", " + ui->street_text->toPlainText());
     }
     else if(ui->radio_geo->isChecked()==1){
         geo->setText(QString::number(ui->latitude->value()) + ";" + QString::number(ui->longitude->value()));
     }
    if (ui->priority_check->isChecked()==1){
        priority->setText(QString::number(ui->priority->value()));
    }
     QTableWidgetItem* beschreibung = new QTableWidgetItem(ui->beschreibung->toPlainText());

     // Datum und Zeit für DtStamp
     QString currentTime_date = QDateTime::currentDateTime().date().toString("yyyyMMdd");
     QString currentTime_time = QDateTime::currentDateTime().time().toString("HHmmss");
     QTableWidgetItem* dtstamp = new QTableWidgetItem(currentTime_date + "T" + currentTime_time + "Z");

     RRule r = RRule();
     switch(ui->tabWidget->currentIndex()){  //verwendet den gerade ausgewählten Tab als Expression
         case 0:
         r.state = "empty";
       break;
         case 1:
         r.getDaily(to_string(ui->interval_daily->value()));
       break;
     case 2:{
         bool days[7] = {ui->monday->isChecked(), ui->tuesday->isChecked(), ui->wednesday->isChecked(),
                 ui->thursday->isChecked(), ui->friday->isChecked() ,
                        ui->saturday->isChecked(), ui->sunday->isChecked()};
         r.getWeekly(days, to_string(ui->interval_weekly->value()));}
       break;
      case 3:
        if(ui->onday_monthly->isChecked()==1){
             r.getMonthly_1(to_string(ui->interval_monthly->value()), to_string(ui->monthly_spin->value()));
         }
         else if(ui->on_monthly->isChecked()==1){
             r.getMonthly_2(to_string(ui->interval_monthly->value()),ui->monthly_combo->currentIndex(),getWeekDay(ui->monthly_combo2->currentIndex()));
             }
       break;
         case 4:
         if(ui->yearly_radio1->isChecked()==1){
             r.getYearly_1(to_string(ui->month_combo->currentIndex()+1), getWeekDay(ui->yearly_day->currentIndex()),
                           ui->yearly_setpos->currentIndex());

         }
         else if(ui->yearly_radio2->isChecked()==1)
         {
             r.getYearly_2(to_string(ui->month_combo2->currentIndex()+1), to_string(ui->monthday->value()));
         }
       break;
     default:
       break;
     }

     // Kontrolliert, ob eine Dauer oder ein Enddatum für die Wiederholung angegeben wurde
     if(ui->count_radio->isChecked()){
         r.countOrUntil = "COUNT=" + to_string(ui->count->value());
     }
     else if(ui->until_radio->isChecked()){
         r.countOrUntil = "UNTIL=" + ui->until->date().toString("yyyyMMdd").toUtf8() + "T000000Z";
     }

    QTableWidgetItem* rrule = new QTableWidgetItem(QString::fromStdString(r.buildRRuleText()));

    VAlarm v;
    v.setHinweis(ui->alarm_checkBox->isChecked());
    v.setTrigger( ui->alarm_line->text());
    v.setEinheit(ui->alarm_comboBox->currentIndex());
    v.setAction(ui->alarm_type->currentIndex());

    QTableWidgetItem* valarm = new QTableWidgetItem(QString::fromStdString(v.buildAlarm()));
    QTableWidgetItem* loeschen = new QTableWidgetItem("Löschen");

    QTableWidgetItem* uid = new QTableWidgetItem(ui->label_id->text());

    int row = ui->table_events->rowCount();
    ui->table_events->insertRow(row);
    ui->table_events->setItem(row,0,name);
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
    ui->table_events->setItem(row,11,uid);

    clearInputs();
    ui->label_id->setText(QString::fromStdString(hex_string(10))); // Ändern des Unique Identifiers
}


void MainWindow::on_table_events_cellClicked(int row, int column)
// wenn die Spalte Löschen angeklickt wurde, wird der Termin gelöscht
{   if(column==1){
    ui->table_events->removeRow(row);
    QMessageBox::information(this, "iCal", "Event entfernt");}
}


void MainWindow::on_button_create_ics_clicked()
{
    ICalendar ical;
    ical.cal_name = ui->label_calendar_name->text().toStdString();
    ical.prodid = ui->label_prod_id->text().toStdString();
    ical.version = ui->label_version->text().toStdString();
    int rows = ui->table_events->rowCount();

    for(int i = 0; i<rows;i++){
        VEvent newEvent;
        newEvent.summary = ui->table_events->model()->index(i, 0).data().toString().toStdString();
        newEvent.description = ui->table_events->model()->index(i, 4).data().toString().toStdString();
        newEvent.dtstart = ui->table_events->model()->index(i, 2).data().toString().toStdString();
        newEvent.dtend = ui->table_events->model()->index(i, 3).data().toString().toStdString();
        newEvent.dtstamp = ui->table_events->model()->index(i, 10).data().toString().toStdString();
        newEvent.rrule = ui->table_events->model()->index(i, 5).data().toString().toStdString();
        newEvent.va = ui->table_events->model()->index(i, 6).data().toString().toStdString();
        newEvent.location = ui->table_events->model()->index(i, 8).data().toString().toStdString();
        newEvent.geo = ui->table_events->model()->index(i, 7).data().toString().toStdString();
        newEvent.priority = ui->table_events->model()->index(i, 9).data().toString().toStdString();
        newEvent.uid = ui->table_events->model()->index(i, 11).data().toString().toStdString();
        ical.eventText += newEvent.createVEventText();
    }

    if(ui->checkBox_feiertage->isChecked())
    {
        ical.holidays += createHolidayText(ui->lineEdit_jahr->text().toInt());
    }

   QString filename = QFileDialog::getSaveFileName(this, "Save file","","*.ics");

   FILE *o_file = fopen(filename.toUtf8(), "w+");
   if (o_file){
       fwrite(ical.buildICSText().c_str(), 1, ical.buildICSText().size(), o_file);
       QMessageBox::information(this, "iCal", "Done Writing!");
       ui->table_events->setRowCount(0); // leert die Tabelle

   }
   fclose(o_file);
}

string importCalenderinfo(string t, string exp){
    string test = exp;
    test += "(.*)\n";
    regex expression(test);

    smatch m;
    string cal_name;
    while(regex_search(t, m, expression)){
        cal_name = m[1];
    t = m.suffix();
        }
    return cal_name;
}

string getAlarm(string et){
    string va;
    regex expression("BEGIN:VALARM\n((\n|.)*)END:VALARM");
    smatch m;
    string alarmText;

    while(regex_search(et, m, expression)){
        alarmText = m[1];
    et = m.suffix();
        }
    return alarmText;
}

VEvent fillEvent(string et){
    VEvent newEvent;
    newEvent.description = importCalenderinfo(et, "DESCRIPTION:");
    newEvent.summary = importCalenderinfo(et, "SUMMARY:");
    newEvent.dtstart = importCalenderinfo(et, "DTSTART:");
    newEvent.dtend = importCalenderinfo(et, "DTEND:");
    newEvent.dtstamp = importCalenderinfo(et, "DTSTAMP:");
    newEvent.priority = importCalenderinfo(et, "PRIORITY:");
    newEvent.geo = importCalenderinfo(et, "GEO:");
    newEvent.location = importCalenderinfo(et, "LOCATION:");
    newEvent.uid = importCalenderinfo(et, "UID:");
    newEvent.rrule = importCalenderinfo(et, "RRULE:");

    // VAlarm noch, etwas komplizierter
    newEvent.va = getAlarm(et);

    return newEvent;
}

list<VEvent> getEvents(string t){
    list<VEvent> eventlist;
    regex expression("BEGIN:VEVENT(\n|.)*?END:VEVENT");
    smatch m;
    string eventText;

    while(regex_search(t, m, expression)){
        eventText = m[0];
    t = m.suffix();
    eventlist.push_back(fillEvent(eventText));
        }
    return eventlist;
}

void MainWindow::fillTable(list<VEvent> eL){
    for (VEvent x : eL){
        QTableWidgetItem* name = new QTableWidgetItem(QString::fromStdString(x.summary));
        QTableWidgetItem* dtstart = new QTableWidgetItem(QString::fromStdString(x.dtstart));
        QTableWidgetItem* dtend = new QTableWidgetItem(QString::fromStdString(x.dtend));
        QTableWidgetItem* priority = new QTableWidgetItem(QString::fromStdString(x.priority));
        QTableWidgetItem* location = new QTableWidgetItem(QString::fromStdString(x.location));
        QTableWidgetItem* geo = new QTableWidgetItem(QString::fromStdString(x.geo));
        QTableWidgetItem* beschreibung = new QTableWidgetItem(QString::fromStdString(x.description));
        QTableWidgetItem* rrule = new QTableWidgetItem(QString::fromStdString(x.rrule));
        QTableWidgetItem* valarm = new QTableWidgetItem(QString::fromStdString(x.va));
        QTableWidgetItem* dtstamp = new QTableWidgetItem(QString::fromStdString(x.dtstamp));
        QTableWidgetItem* uid = new QTableWidgetItem(QString::fromStdString(x.uid));
        QTableWidgetItem* loeschen = new QTableWidgetItem("Löschen");

        int row = ui->table_events->rowCount();
        ui->table_events->insertRow(row);
        ui->table_events->setItem(row,0,name);
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
        ui->table_events->setItem(row,11,uid);
    }
}


void MainWindow::on_button_ics_import_clicked()
{
    string line;
    string text;
    QString filename = QFileDialog::getOpenFileName(this, "Open file","");
    ifstream myfile(filename.toStdString());
    if(myfile.is_open()){
        ICalendar newics = ICalendar();
    while(getline(myfile, line)){
        text += line + "\n";
        }
    myfile.close();

    string cal_name = importCalenderinfo(text, "PRODID:");
    string cal_version = importCalenderinfo(text, "VERSION:");
    list<VEvent> eventList = getEvents(text);
    int event_count = eventList.size();

    QMessageBox importMsgBox;
    importMsgBox.setModal(true);
    importMsgBox.setText("Soll der Kalender " + QString::fromStdString(cal_name)
                         + " importiert werden?\n Der Kalender enthält "
                         + QString::number(event_count) + " Termine.");
    importMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    importMsgBox.setButtonText(QMessageBox::Yes, "Ja");
    importMsgBox.setButtonText(QMessageBox::No, "Nein");
    importMsgBox.setDefaultButton(QMessageBox::Yes);


    int ret = importMsgBox.exec();
    if (ret == QMessageBox::Yes){
        cout << "Yes" << endl;
        ui->label_calendar_name->setText(QString::fromStdString(cal_name));
        ui->label_version->setText(QString::fromStdString(cal_version));
        ui->table_events->setRowCount(0);
        fillTable(eventList);
        }
    }
}

