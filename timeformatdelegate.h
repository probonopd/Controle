#ifndef TIMEFORMATDELEGATE_H
#define TIMEFORMATDELEGATE_H

#include <QStyledItemDelegate>
#include <QDateTime>

class TimeFormatDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
explicit TimeFormatDelegate(QObject *parent = 0);
virtual QString displayText(const QVariant &value, const QLocale &locale) const;

signals:

public slots:

};
#endif // TIMEFORMATDELEGATE_H
