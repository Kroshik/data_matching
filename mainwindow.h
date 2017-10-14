#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtWidgets>
#include <QTextBrowser>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QFile>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
typedef std::vector<std::pair<QString, QString>> vec_Qs_Qs;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_admission_returnPressed();
    void on_shipment_returnPressed();

private:
    bool isHex(std::string &s);
    bool readXML(QString &s);
    void addAdmission(QLineEdit *qline);
    void addShipment(QLineEdit *qline);

    bool findValue(QString &s, vec_Qs_Qs);

    void increaseCount(QString &qs, vec_Qs_Qs &);
    void decreaseCount(QString &qs, vec_Qs_Qs &);

    void fillVector();
    void fillTextBrowser(vec_Qs_Qs, QTextBrowser *, QTextBrowser *);
    void fillXML();




private:
    const size_t  HEX_SIZE = 4;

    vec_Qs_Qs vAdmission;
    vec_Qs_Qs vShipment;
    Ui::MainWindow *ui;
    QTextBrowser *qtextAd;
    QTextBrowser *qtextAdCount;
    QTextBrowser *qtextSh;
    QTextBrowser *qtextShCount;
};

#endif // MAINWINDOW_H
