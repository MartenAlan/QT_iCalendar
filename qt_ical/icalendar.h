#ifndef ICALENDAR_H
#define ICALENDAR_H
#include <string.h>
#include <QMainWindow>

using namespace std;


//Klasse zur Erstellung des ICS-Textes eines Alarms
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

    string buildAlarm();

};

// Klasse zur Erstellung des ICS-Textes einer Wiederholung
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
    void getYearly_1(string i, string y_day, int y_setpos);
    void getYearly_2(string i, string by_md);
    void getMonthly_1(string i, string m_spin);
    void getMonthly_2(string i, int m_comb, string m_comb2);
    void getWeekly(bool d[7], string i);
    void getDaily(string i);
 };

 // Klasse zur Erstellung des ICS-Textes eines Termins
 class VEvent{
 public:
     string summary;
     string description;
     string dtstart;
     string dtend;
     string dtstamp;
     string location;
     string geo;
     string priority;
     string uid;
     string createVEventText();
     string rrule;
     string va;
 };


//Klasse zur Erstellung des ICS-Textes eines Kalenders
class ICalendar
{
public:
    ICalendar();
    string cal_name;
    string eventText;
    string version;
    string prodid;
    string holidays;
    string buildICSText();

};

#endif // ICALENDAR_H
