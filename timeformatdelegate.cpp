#include "timeformatdelegate.h"

TimeFormatDelegate::TimeFormatDelegate(QObject *parent) :
QStyledItemDelegate(parent)
{
}

QString TimeFormatDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    int current = QDateTime::currentDateTime().toTime_t();

    int hora=(current - value.toInt())/60;
    QString formattedTime = QString("%1").arg(hora);
    return formattedTime;
}
