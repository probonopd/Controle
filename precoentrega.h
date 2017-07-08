#ifndef PRECOENTREGA_H
#define PRECOENTREGA_H

#include <QDialog>
#include "login.h"
#include "numberformatdelegate.h"

namespace Ui {
class PrecoEntrega;
}

class PrecoEntrega : public QDialog
{
    Q_OBJECT

    void leentrega(QString filtro){
        queryentrega->prepare("select entrega.id_entrega, entrega.bai_nu_sequencial, entrega.loc_nu_sequencial, log_bairro.bai_no, \
                               log_localidade.loc_nosub, entrega.valor, entrega.percentual, entrega.extra from entrega\
                               INNER JOIN log_bairro ON entrega.bai_nu_sequencial=log_bairro.bai_nu_sequencial \
                               INNER JOIN log_localidade ON entrega.loc_nu_sequencial=log_localidade.loc_nu_sequencial \
                               WHERE (log_bairro.bai_no like :filtro) ");
        queryentrega->bindValue(":filtro", "%" + filtro + "%" );
        queryentrega->exec();
        modalentrega->setQuery(*queryentrega);
    }

public:
    explicit PrecoEntrega(QWidget *parent = 0);
    ~PrecoEntrega();

private slots:
    void on_pushButton_procurar_clicked();

    void on_tableView_entrega_activated(const QModelIndex &index);

    void on_pushButton_gravar_clicked();

private:
    Ui::PrecoEntrega *ui;
    Login conn;

    QSqlQueryModel *modalentrega;
    QSqlQuery *queryentrega;
    QString filtro="";
    int id_entrega=0;

};

#endif // PRECOENTREGA_H
