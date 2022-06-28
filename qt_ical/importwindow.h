#ifndef IMPORTWINDOW_H
#define IMPORTWINDOW_H

#include <QDialog>
#include <map>
#include <icalendar.h>

namespace Ui {
class importWindow;
}

class importWindow : public QDialog
{
    Q_OBJECT

public:
    explicit importWindow(QWidget *parent = nullptr);
    ~importWindow();
    void setCalendarName(QString prodid);
    void getCount(int count);

private slots:



private:
    Ui::importWindow *ui;
};

#endif // IMPORTWINDOW_H
