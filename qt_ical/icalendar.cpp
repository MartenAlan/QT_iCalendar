#include "icalendar.h"

ICalendar::ICalendar()
{

}

RRule::RRule(){

}

string ICalendar::buildICSText(){
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

QString VAlarm::buildAlarm(){
    QString alarm;
    if (hinweis == true){
        alarm += "BEGIN:VALARM\n";
        switch (index_action) {
        case 0:
            alarm += "ACTION:AUDIO\nATTACH;VALUE=URI:Chord\n";
            break;
        case 1:
            alarm += "ACTION:DISPLAY\n";
            alarm += "DESCRIPTION:Erinnerung\n";
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



string RRule::buildRRuleText(){

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
