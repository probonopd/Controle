#include "analisevendas.h"
#include "ui_analisevendas.h"
#include <QDateTime>
#include "qdebug.h"

AnaliseVendas::AnaliseVendas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnaliseVendas)
{
    ui->setupUi(this);
/*
    uint current = QDateTime::currentDateTime().toTime_t();

    QDateTime dateTime = QDateTime::fromString("1.30.1", "dd.MM.yy");

    QString teste=dateTime.toString("dd/MM/yy");
    qDebug() << dateTime;
  */

    QDateTime agora;
    QString mes = agora.currentDateTime().toString("MM");

    QDateTime timeConvertor;
   // QString customDateString = "14-Nov-2010 05:27:03 +0100";
    QString customDateString = "01/10/2012";

    QString dateTime = customDateString.left(10);
    int timezoneOffset = customDateString.right(5).left(3).toInt();
    timeConvertor = QDateTime::fromString(customDateString, "dd/MM/yyyy");

    qDebug() << dateTime << timezoneOffset << timeConvertor << mes;
    // Mark this QDateTime as one with a certain offset from UTC, and set that
    // offset.
   // timeConvertor.setTimeSpec(Qt::OffsetFromUTC);
  //  timeConvertor.setUtcOffset(timezoneOffset * 3600);

    // Convert this QDateTime to UTC.
 //   timeConvertor = timeConvertor.toUTC();

    // Store the UTC timestamp.
    int timestamp = timeConvertor.toTime_t();

    qDebug() << timestamp << timeConvertor.toString("dd/MM/yy HH:mm:ss");

}

AnaliseVendas::~AnaliseVendas()
{
    delete ui;
}
