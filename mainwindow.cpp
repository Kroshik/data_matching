#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    fillVector();
    ui->setupUi(this);
    qtextAd = ui->textBrowserAdmission;
    qtextAdCount = ui->textBrowserAdmissionCount;
    qtextSh = ui->textBrowserShipment;
    qtextShCount = ui->textBrowserShipmentCount;
    fillTextBrowser(vAdmission, qtextAd, qtextAdCount);
    fillTextBrowser(vShipment, qtextSh, qtextShCount);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    vAdmission.clear();
    vShipment.clear();
    fillXML();
    qtextAd->clear();
    qtextAdCount->clear();
    qtextSh->clear();
    qtextShCount->clear();
}

bool MainWindow::isHex(std::string &s)
{
    if (s.size() != HEX_SIZE)
        return false;
    for (size_t i = 0; i <s.size(); i++)
    {
        if (!isxdigit(s[i]))
            return false;
    }
    std::transform(s.begin(), s.end(), s.begin(), toupper);
    return true;
}

bool MainWindow::readXML(QString &s)
{
    QFile file("table.xml");
    file.open(QIODevice::ReadOnly);
    QString qs;
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        if(xmlReader.readNext() == QXmlStreamReader::StartElement && xmlReader.name() == "key")
        {
            if (xmlReader.readElementText() == s)
            {
                qDebug() << "dublicate";
                return true;
            }
        }
    }
    file.close();
    return false;
}

void MainWindow::fillVector()
{
    QFile file("table.xml");
    file.open(QIODevice::ReadOnly);
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    QString qs_key;
    QString qs_count;
    while(!xmlReader.atEnd() ) {
        while(xmlReader.name() != "output" ) {
            if( xmlReader.name() == "key")
                qs_key = xmlReader.readElementText();
            if( xmlReader.name() == "count"){
                qs_count = xmlReader.readElementText();
                vAdmission.emplace_back(qs_key, qs_count);
            }
            xmlReader.readNext();
        }
        while(!xmlReader.atEnd()) {
            if( xmlReader.name() == "key")
                qs_key = xmlReader.readElementText();
            if( xmlReader.name() == "count") {
                qs_count = xmlReader.readElementText();
                vShipment.emplace_back(qs_key, qs_count);
            }
            xmlReader.readNext();
        }
    }
    file.close();
}

void MainWindow::fillXML()
{
    QFile file("table.xml");
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("start");
    xmlWriter.writeStartElement("input");
    for (size_t i = 0; i < vAdmission.size(); i++)
    {
        xmlWriter.writeStartElement("data");
        xmlWriter.writeStartElement("key");
        xmlWriter.writeCharacters(vAdmission[i].first);
        xmlWriter.writeEndElement();
        xmlWriter.writeStartElement("count");
        xmlWriter.writeCharacters(vAdmission[i].second);
        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();
    xmlWriter.writeStartElement("output");
    for (size_t i = 0; i < vShipment.size(); i++)
    {
        xmlWriter.writeStartElement("data");
        xmlWriter.writeStartElement("key");
        xmlWriter.writeCharacters(vShipment[i].first);
        xmlWriter.writeEndElement();
        xmlWriter.writeStartElement("count");
        xmlWriter.writeCharacters(vShipment[i].second);
        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();
}

bool MainWindow::findValue(QString &qs, vec_Qs_Qs v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        if (v[i].first == qs)
            return true;
    }
    return false;
}

void MainWindow::increaseCount(QString &qs, vec_Qs_Qs &v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        if (v[i].first == qs)
        {
            int count = v[i].second.toInt();
            v[i].second = QString::number(++count);
        }
    }
}

void MainWindow::decreaseCount(QString &qs, vec_Qs_Qs &v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        if (v[i].first == qs)
        {
            int count = v[i].second.toInt();
            if (count == 1)
                v.erase(v.begin() + i);
            else
                v[i].second = QString::number(--count);
        }
    }
}

void MainWindow::fillTextBrowser(vec_Qs_Qs v, QTextBrowser *qtext, QTextBrowser *qtextC)
{
    qtext->clear();
    qtextC->clear();
//    QString qs;
    for (size_t i = 0; i < v.size(); i++)
    {
//        qs = v[i].first + " " + v[i].second;
        qtext->setAlignment(qtext->alignment());
        qtext->append(v[i].first);
        qtextC->append(v[i].second);
    }
}

void MainWindow::addAdmission(QLineEdit *qline)
{
    std::string s = qline->text().toStdString();
    s.append(" ");
    std::string tmp;
    QString qs;
    for (size_t i = 0; i < s.size(); i++)
    {
        if (s[i] == ' ')
        {
            if (isHex(tmp))
            {
                qs = QString::fromStdString(tmp);
                if (!findValue(qs, vAdmission))
                    vAdmission.emplace_back(qs, "1");
                else
                    increaseCount(qs, vAdmission);
                if (findValue(qs, vShipment))
                    decreaseCount(qs, vShipment);
            }
            tmp.clear();
        }
        else
            tmp += s[i];
    }
    fillTextBrowser(vAdmission, qtextAd, qtextAdCount);
    fillTextBrowser(vShipment, qtextSh, qtextShCount);
    fillXML();
}

void MainWindow::addShipment(QLineEdit *qline)
{
    std::string s = qline->text().toStdString();
    s.append(" ");
    std::string tmp;
    QString qs;
    for (size_t i = 0; i < s.size(); i++)
    {
        if (s[i] == ' ')
        {
            if (isHex(tmp))
            {
                qs = QString::fromStdString(tmp);
                if (!findValue(qs, vShipment))
                    vShipment.emplace_back(qs, "1");
                else
                    increaseCount(qs, vShipment);
                if (findValue(qs, vAdmission))
                    decreaseCount(qs, vAdmission);
            }
            tmp.clear();
        }
        else
            tmp += s[i];
    }
    fillTextBrowser(vAdmission, qtextAd, qtextAdCount);
    fillTextBrowser(vShipment, qtextSh, qtextShCount);
    fillXML();
}

void MainWindow::on_admission_returnPressed()
{
    if (ui->admission->text().size() == 0)
        return;
    addAdmission(ui->admission);
    ui->admission->clear();
}

void MainWindow::on_shipment_returnPressed()
{
    if (ui->shipment->text().size() == 0)
        return;
    addShipment(ui->shipment);
    ui->shipment->clear();
}

