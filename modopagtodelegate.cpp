#include "modopagtodelegate.h"
#include "constants.h"

extern QString modo_pag[NPAG];

ModoPagtoDelegate::ModoPagtoDelegate(QObject *parent) :
QStyledItemDelegate(parent)
{
}

QString ModoPagtoDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    QString str;

    switch (value.toInt()) {
    case PAG_DINHEIRO:
        str=modo_pag[IDX_DINHEIRO];
        break;
    case PAG_CARTAO:
        str=modo_pag[IDX_CARTAO];
        break;
    case PAG_FIADO:
        str=modo_pag[IDX_FIADO];
        break;
    case PAG_CHEQUE:
        str=modo_pag[IDX_CHEQUE];
        break;
    }

    return str;
}
