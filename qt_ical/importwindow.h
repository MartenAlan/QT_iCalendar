#ifndef IMPORTWINDOW_H
#define IMPORTWINDOW_H

#include <QDialog>

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

private:
    Ui::importWindow *ui;
};

#endif // IMPORTWINDOW_H
