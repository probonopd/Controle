#include "dateformatdelegate.h"

DateFormatDelegate::DateFormatDelegate(QObject *parent) :
QStyledItemDelegate(parent)
{
}

QString DateFormatDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    QDateTime ts;

    ts.setTime_t(value.toInt());

    return ts.toString("dd/MM/yy");
}
