#include "icalendar.h"

ICalendar::ICalendar()
{

}

RRule::RRule(){

}

void RRule::getDaily(string i){
    state = "DAILY";
    interval = "INTERVAL=" + i + ";";
}

void RRule::getWeekly(bool d[7], string i){
    state = "WEEKLY";
    byday = "BYDAY=";
    if(d[0]){
        byday += "MO,";
    }
    if(d[1]){
        byday += "TU,";
    }
    if(d[2]){
        byday += "WE,";
    }
    if(d[3]){
        byday += "TH,";
    }
    if(d[4]){
        byday += "FR,";
    }
    if(d[5]){
        byday += "SA,";
    }
    if(d[6]){
        byday += "SU,";
    }
    byday += ";";
    interval = "INTERVAL=" + i + ";";
}

void RRule::getMonthly_1(string i, string m_spin){
    state = "MONTHLY";
    interval = "INTERVAL=" + i + ";";
    byday = "BYMONTHDAY=" + m_spin + ";";
}

void RRule::getMonthly_2(string i, int m_comb, string m_comb2){
    state = "MONTHLY";
    interval = "INTERVAL=" + i + ";";
    bysetpos = "BYSETPOS=";

    if (m_comb == 4){
        bysetpos += "-1;";
    }
    else{
        bysetpos += to_string(m_comb + 1) + ";";
    }

    byday = "BYDAY=";
    byday += m_comb2;
}

void RRule::getYearly_1(string i, string y_day, int y_setpos){
    state = "YEARLY";
    bymonth = "BYMONTH=" + i + ";";
    byday = "BYDAY=";
    byday += y_day;
    if (y_setpos == 4){
        bysetpos += "-1;";
    }
    else{
        bysetpos += to_string(y_setpos + 1) + ";";
    }
}

void RRule::getYearly_2(string i, string by_md){
    bymonth = "BYMONTH=" + i + ";";
    byday = "BYMONTHDAY=" + by_md + ";";
}

string ICalendar::buildICSText(){
    string icsText = "BEGIN:VCALENDAR\n";
    icsText += "VERSION:" + version + "\n";
    icsText += "PRODID:" + prodid + "_" + cal_name + "\n";
    icsText += eventText;
    icsText += holidays;
    icsText += "END:VCALENDAR";
    return icsText;
}

string VAlarm::buildAlarm(){
    QString alarm;
    if (hinweis == true){
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

    }

    return alarm.toUtf8().constData();
}



string RRule::buildRRuleText(){

    if(state.compare("empty")==0){
        return "";
    }

    else {
        rruleText += "FREQ=" + state + ";";
        rruleText += bysetpos;
        rruleText += byday;
        rruleText += bymonth;
        rruleText += interval;
        rruleText += countOrUntil;
        return rruleText;
    }
}

string VEvent::createVEventText(){

    string eventText;
    eventText += "BEGIN:VEVENT\n";
    eventText += "SUMMARY:" + summary + "\n";
    eventText += "DESCRIPTION:" + description + "\n";
    eventText += "DTSTAMP:" + dtstamp + "\n";
    eventText += "DTSTART:" + dtstart + "\n";
    eventText += "DTEND:" + dtend + "\n";
    eventText += "UID:" + uid + "\n";

    string rruletext = rrule;
    if(rruletext.size()>1){
          eventText += "RRULE:" + rruletext + "\n";}
    string valarmtext = va;
    if(valarmtext.size()>1){
    eventText += "BEGIN:VALARM\n" + valarmtext + "END:VALARM\n";
    }
    if(priority.size()>0){
        eventText += "PRIORITY:" + priority + "\n";
    }
    eventText += "LOCATION:" + location + "\n";
    eventText += "GEO=" + geo + "\n";
    eventText += "END:VEVENT\n";
    return eventText;
}
