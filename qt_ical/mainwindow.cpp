#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString ics_str = ui->label_id->text() + "\n" + ui->label_version->text() + "\n" + ui->prod_id->text() + "\n";
    QString ics_str2 = ui->titel->toPlainText() + "\n" + ui->beschreibung->toPlainText();

    QDate ics_dtstart = ui->input_dtstart->date();
    QDate ics_dtend = ui->input_dtend->date();

    QString dstart_dtend = ics_dtstart.toString() + "->" + ics_dtend.toString();

    ui->output->setText(ics_str + "\n" + ics_str2 + "\n" + dstart_dtend);

    QString fileText = ics_str + "\n" + ics_str2 + "\n" + dstart_dtend;
    std::string utf8_text = fileText.toUtf8().constData();

    QString filename = QFileDialog::getSaveFileName(this, "Save file");

    FILE *o_file = fopen(filename.toUtf8(), "w+");
    if (o_file){
        fwrite(utf8_text.c_str(), 1, utf8_text.size(), o_file);
        cout << "Done Writing!" << endl;
    }
    else{
        cout << "Something went wrong!" << endl;
    }
    fclose(o_file);
}

