#ifndef DATEFORMATDELEGATE_H
#define DATEFORMATDELEGATE_H

#include <QStyledItemDelegate>
#include <QDateTime>

class DateFormatDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
explicit DateFormatDelegate(QObject *parent = 0 );
virtual QString displayText(const QVariant &value, const QLocale &locale) const;

signals:

public slots:

};
#endif // DATEFORMATDELEGATE_H
