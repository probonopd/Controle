#ifndef ENTREGA_H
#define ENTREGA_H

#include <QDialog>
#include "login.h"
#include "numberformatdelegate.h"

namespace Ui {
class Entrega;
}

class Entrega : public QDialog
{
    Q_OBJECT
    Login conn;

public:
    explicit Entrega(QWidget *parent = 0);
    ~Entrega();

    void lerestados(){
        queryestados->prepare("select ufe_no from log_faixa_uf");
        queryestados->exec();
        modalestados->setQuery(*queryestados);
    }

    void lerbairros(int loc_nu, QString filtro){
        querybairros->prepare("select log_bairro.bai_no, log_bairro.bai_nu_sequencial \
                       from log_bairro \
                       WHERE log_bairro.loc_nu_sequencial=:loc_nu_sequencial and \
                             (log_bairro.bai_no like :filtro) ");
        querybairros->bindValue(":filtro", "%" + filtro + "%" );
        querybairros->bindValue(":loc_nu_sequencial",loc_nu);
        querybairros->exec();
        modalbairros->setQuery(*querybairros);

    }

    void lercidades(QString estado){
        querycidades->prepare("select log_localidade.loc_nosub, log_localidade.ufe_sg, log_localidade.loc_nu_sequencial \
                       from log_localidade \
                       INNER JOIN log_faixa_uf ON log_faixa_uf.ufe_sg=log_localidade.ufe_sg \
                       WHERE log_faixa_uf.ufe_no=:ufe ");
        querycidades->bindValue(":ufe",estado);
        querycidades->exec();
        modalcidades->setQuery(*querycidades);
    }

    void leentrega(QString filtro){
        queryentrega->prepare("select entrega.id_entrega, entrega.bai_nu_sequencial, entrega.loc_nu_sequencial, log_bairro.bai_no, \
                               log_localidade.loc_nosub, entrega.valor, entrega.percentual, entrega.extra from entrega\
                               INNER JOIN log_bairro ON entrega.bai_nu_sequencial=log_bairro.bai_nu_sequencial \
                               INNER JOIN log_localidade ON entrega.loc_nu_sequencial=log_localidade.loc_nu_sequencial \
                               WHERE (log_bairro.bai_no like :filtro) ");
        queryentrega->bindValue(":filtro", "%" + filtro + "%" );
        queryentrega->exec();
        modalentrega->setQuery(*queryentrega);

        qDebug() << queryentrega->lastError().text();

    }

private slots:
    void on_comboBox_estados_activated(const QString &arg1);

    void on_comboBox_cidade_activated(const QString &arg1);

    void on_pushButton_inserirbairros_clicked();

    void on_pushButton_apagar_clicked();

    void on_pushButton_procurar_clicked();

private:
    Ui::Entrega *ui;
    QSqlQueryModel *modalestados, *modalcidades, *modalbairros, *modalentrega;
    QSqlQuery *queryestados, *querycidades, *querybairros, *queryentrega;
    QString cidade, estado, filtro="";
    int loc_nu_sequencial=0;
};

#endif // ENTREGA_H
