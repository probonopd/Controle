#ifndef ESTABELECIMENTO_H
#define ESTABELECIMENTO_H

#include <QDialog>
#include "login.h"
#include "constants.h"

namespace Ui {
class Estabelecimento;
}

class Estabelecimento : public QDialog
{
    Q_OBJECT

public:
    explicit Estabelecimento(QWidget *parent = 0);
    ~Estabelecimento();

    void lerloja(){
        queryloja->prepare("SELECT nome, endereco, numero, bairro, cidade, estado, cep, fone, inscrest, cnpj, site, facebook, id_loja, config FROM loja");
        queryloja->exec();
        modalloja->setQuery(*queryloja);
    }

    void lerestados(){
        queryestados->prepare("select ufe_no, ufe_sg from log_faixa_uf");
        queryestados->exec();
        modalestados->setQuery(*queryestados);
    }

    void lercidades(QString estado){
        querycidade->prepare("select log_localidade.loc_nosub, log_localidade.loc_nu_sequencial \
                       from log_faixa_uf \
                       INNER JOIN log_localidade ON log_faixa_uf.ufe_sg=log_localidade.ufe_sg \
                       WHERE log_faixa_uf.ufe_no=:ufe ");
        querycidade->bindValue(":ufe", estado);
        querycidade->exec();
        modalcidade->setQuery(*querycidade);
        qDebug()<< querycidade->lastError().text();
    }

    void lerruas(int loc_nu, QString filtrar){
        queryruas->prepare("select log_logradouro.log_nome, log_bairro.bai_no, log_logradouro.cep  \
                            from log_logradouro \
                            INNER JOIN log_bairro ON log_logradouro.bai_nu_sequencial_ini=log_bairro.bai_nu_sequencial \
                            WHERE log_logradouro.loc_nu_sequencial=:loc_nu and \
                                   ( (log_logradouro.log_nome like :filtrar) or (log_bairro.bai_no like :filtrar) ) ");
        queryruas->bindValue( ":filtrar", "%"+filtrar+"%" );
        queryruas->bindValue(":loc_nu",loc_nu);
        queryruas->exec();
        modalruas->setQuery(*queryruas);
        qDebug()<< queryruas->lastError().text() << loc_nu;
    }

private slots:
    void on_comboBox_estados_activated(const QString &arg1);

    void on_pushButton_procurar_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_pushButton_cancelar_clicked();

    void on_pushButton_salvar_clicked();

    void on_comboBox_cidades_activated(const QString &arg1);

    void on_radioButton_entregavalor_clicked();

    void on_radioButton_entregaperc_clicked();

    void on_radioButton_mult_maior_clicked();

    void on_radioButton_mult_media_clicked();

private:
    Ui::Estabelecimento *ui;
    QSqlQueryModel *modalloja, *modalestados, *modalruas, *modalcidade;
    QSqlQuery *queryloja, *queryestados, *queryruas, *querycidade;
    QString cidade, estado, ufe_sg, filtro="";
    int loc_nu_sequencial, configuracao=0;
    Login conn;
};

#endif // ESTABELECIMENTO_H
