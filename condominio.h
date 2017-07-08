#ifndef CONDOMINIO_H
#define CONDOMINIO_H

#include <QDialog>
#include "login.h"
#include "numberformatdelegate.h"

namespace Ui {
class Condominio;
}

class Condominio : public QDialog
{
    Q_OBJECT

public:
    explicit Condominio(QWidget *parent = 0);
    ~Condominio();

    void lerlogradouro(int log_nu){
        querylog->prepare("select log_logradouro.log_nome, log_bairro.bai_no from log_logradouro \
                          inner join log_bairro on log_logradouro.bai_nu_sequencial_ini=log_bairro.bai_nu_sequencial\
                          where log_logradouro.log_nu_sequencial=:log_nu");
        querylog->bindValue(":log_nu",log_nu);
        querylog->exec();
        modallog->setQuery(*querylog);

        qDebug() << querylog->lastError().text();
    }

    void lerestados(){
        queryestados->prepare("select ufe_no, ufe_sg from log_faixa_uf");
        queryestados->exec();
        modalestados->setQuery(*queryestados);
    }

    void lecondominio(){
        querycondominio->prepare("select condominio.id_condominio, condominio.nome_condo, log_logradouro.log_nome, condominio.numero, log_bairro.bai_no, \
                                         condominio.valor, condominio.percentual, condominio.extra, log_logradouro.log_nu_sequencial from condominio\
                                  inner join log_logradouro on log_logradouro.log_nu_sequencial=condominio.log_nu_sequencial \
                                  inner join log_bairro on log_logradouro.bai_nu_sequencial_ini=log_bairro.bai_nu_sequencial" );
        querycondominio->exec();
        modalcondominio->setQuery(*querycondominio);
        qDebug() << querycondominio->lastError().text();
    }

    void lecidades(){
        querycidades->prepare("select log_localidade.loc_nosub, log_localidade.ufe_sg, log_localidade.loc_nu_sequencial \
                               from log_localidade \
                               WHERE log_localidade.ufe_sg=:ufe ");
        querycidades->bindValue(":ufe",ufe);
        querycidades->exec();
        modalcidades->setQuery(*querycidades);

        qDebug()<<querycidades->lastError().text();

    }

    void leruas(QString &rua){

        queryrua->prepare("SELECT log_logradouro.log_tipo_logradouro,log_logradouro.log_no, log_logradouro.cep, \
                            log_bairro.bai_no, log_bairro.bai_nu_sequencial, log_logradouro.log_nu_sequencial \
                           FROM log_logradouro \
                           INNER JOIN log_bairro ON log_logradouro.bai_nu_sequencial_ini=log_bairro.bai_nu_sequencial \
                           WHERE (log_logradouro.loc_nu_sequencial=:loc_nu ) and \
                                 ((log_bairro.bai_no like :bairro) or (log_logradouro.log_no like :rua ))");
        queryrua->bindValue(":ufe",ufe);
        queryrua->bindValue(":loc_nu",loc_nu_sequencial);
        queryrua->bindValue( ":rua", "%" + rua + "%" );
        queryrua->bindValue( ":bairro", "%" + rua + "%" );

        queryrua->exec();

        modalrua->setQuery(*queryrua);
        qDebug()<< ufe <<loc_nu_sequencial<<queryrua->lastError().text();

    }

private slots:
    void on_comboBox_estados_activated(const QString &arg1);

    void on_pushButton_procurar_clicked();

    void on_comboBox_cidade_activated(const QString &arg1);

    void on_tableView_enderecos_activated(const QModelIndex &index);

    void on_pushButton_gravar_clicked();

    void on_tableView_condominio_activated(const QModelIndex &index);

private:
    Ui::Condominio *ui;
    Login conn;
    QSqlQueryModel *modalestados, *modalcidades, *modalrua, *modalcondominio, *modallog;
    QSqlQuery *queryestados, *querycidades, *queryrua, *querycondominio, *querylog;
    QString cidade, ufe, estado;
    int loc_nu_sequencial=0, log_nu_sequencial=0, id_condominio=0;
    bool flag=false;
};

#endif // CONDOMINIO_H
