#ifndef MODOPAGTODELEGATE_H
#define MODOPAGTODELEGATE_H

#include <QStyledItemDelegate>

class ModoPagtoDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
explicit ModoPagtoDelegate(QObject *parent = 0);
virtual QString displayText(const QVariant &value, const QLocale &locale) const;

signals:

public slots:

};
#endif // MODOPAGTODELEGATE_H
