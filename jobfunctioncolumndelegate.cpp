#include "jobfunctioncolumndelegate.h"

#include "constants.h"

extern QString funcao[NFUNC]; //={"ADMIN","GERENTE","CAIXA", "GARÇOM", "ENTREGADOR", "COZINHA"};

JobFunctionColumnDelegate::JobFunctionColumnDelegate(QObject *parent) :
QStyledItemDelegate(parent)
{
}

QString JobFunctionColumnDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
  return funcao[value.toInt()-1];
}
