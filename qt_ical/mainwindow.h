#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void clearInputs();

private slots:

    void on_btn_clear_clicked();

    void on_tabWidget_currentChanged(int index);


    void on_input_dtstart_dateTimeChanged(const QDateTime &dateTime);

    void on_editCalendar_button_clicked();

    void on_button_add_event_clicked();

    void on_table_events_cellClicked(int row, int column);

    void on_button_create_ics_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
