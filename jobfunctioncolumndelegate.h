#ifndef JOBFUNCTIONCOLUMNDELEGATE_H
#define JOBFUNCTIONCOLUMNDELEGATE_H

#include <QStyledItemDelegate>

class JobFunctionColumnDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
explicit JobFunctionColumnDelegate(QObject *parent = 0);
virtual QString displayText(const QVariant &value, const QLocale &locale) const;

signals:

public slots:

};
#endif // JOBFUNCTIONCOLUMNDELEGATE_H
