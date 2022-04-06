#ifndef ICALENDAR_H
#define ICALENDAR_H
#include <string.h>
#include <QMainWindow>

using namespace std;


//Klasse Alarm definieren
class VAlarm{
private:
    QString wert_trigger;
    int index_einheit;
    int index_action;
    bool hinweis;
public:
    void setTrigger(QString t){
        wert_trigger = t;
    }
    void setEinheit(int e){
        index_einheit = e;
    }
    void setAction(int a){
        index_action = a;
    }
    void setHinweis(bool h){
        hinweis = h;
    }
    QString buildAlarm();

};

// Klasse zur geordneteren Erstellung des ICS-Textes einer Wiederholung
 class RRule{
         public:
     string rruleText;
     string state;
     string interval;
     string countOrUntil;
     string byday;
     string bymonth;
     string byyear;
     string bysetpos;

    RRule();
    string buildRRuleText();
           };


class ICalendar
{
public:
    ICalendar();
    string cal_name;
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
    string buildICSText();

};

#endif // ICALENDAR_H
