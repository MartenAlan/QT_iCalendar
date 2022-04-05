#ifndef EDITCALENDAR_H
#define EDITCALENDAR_H

#include <QDialog>

namespace Ui {
class editCalendar;
}

class editCalendar : public QDialog
{
    Q_OBJECT

public:
    explicit editCalendar(QWidget *parent = nullptr);
    ~editCalendar();
    void setValues(QString n, QString d, QString v, QString pid);
    QString * getValues();

private:
    Ui::editCalendar *ui;
};

#endif // EDITCALENDAR_H
