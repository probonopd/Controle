#include "formatcolumndelegate.h"

FormatColumnDelegate::FormatColumnDelegate(QObject *parent) :
QStyledItemDelegate(parent)
{
}

QString FunctionColumnDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    if(value.toString()=="")
        return "";
    QString formattedNum = locale.toString(value.toDouble(), 'f', 2);
    return formattedNum;
}
