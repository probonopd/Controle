#ifndef FORMATCOLUMNDELEGATE_H
#define FORMATCOLUMNDELEGATE_H

#include <QStyledItemDelegate>

class FormatColumnDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
xplicit FunctionColumnDelegate(QObject *parent = 0);
virtual QString displayText(const QVariant &value, const QLocale &locale) const;

signals:

public slots:

};

#endif // FORMATCOLUMNDELEGATE_H
