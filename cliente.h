#ifndef CLIENTE_H
#define CLIENTE_H

#include <QDialog>
#include "login.h"

#define BAI_LOJA        4
#define LOG_LOJA        5
#define UF_LOJA         6

namespace Ui {
class Cliente;
}

class Cliente : public QDialog
{
    Q_OBJECT
    Login conn;
    QSqlQuery *query, *queryendereco;
    QSqlQueryModel *modal, *modalendereco;
    QSqlQueryModel *modalestados, *modalcidades, *modalruas;
    QSqlQuery *queryestados, *querycidades, *queryruas;
    int id_cliente=0, id_endereco=0;
    QString cidade, estado;
    int loc_nu_sequencial, bai_nu=0, log_nu=0;
    QSqlQueryModel *modalloja;
    QSqlQuery *queryloja;

public:
    explicit Cliente(QWidget *parent = 0);
    ~Cliente();

    void lerendereco(int id){
        queryendereco->prepare("select * from endereco where id_cliente=:id_cliente");
        queryendereco->bindValue(":id_cliente",id);
        queryendereco->exec();
        modalendereco->setQuery(*queryendereco);
    }

    //INNER JOIN log_faixa_uf ON log_faixa_uf.ufe_sg=log_localidade.ufe_sg \

    void lecidades(QString ufe){
        querycidades->prepare("select log_localidade.loc_nosub, log_localidade.loc_nu_sequencial \
                               from log_localidade \
                               WHERE log_localidade.ufe_sg=:ufe ");
        querycidades->bindValue(":ufe",ufe);
        querycidades->exec();
        modalcidades->setQuery(*querycidades);

    }

    void leruas(QString filtro){
        queryruas->prepare("select log_logradouro.log_tipo_logradouro, log_logradouro.log_no, log_logradouro.log_nome, log_bairro.bai_no, log_logradouro.cep, log_bairro.bai_nu_sequencial, log_logradouro.log_nu_sequencial \
                            from log_logradouro \
                            INNER JOIN log_bairro ON log_logradouro.bai_nu_sequencial_ini=log_bairro.bai_nu_sequencial \
                            WHERE log_logradouro.loc_nu_sequencial=:loc_nu and \
                            ( \
                              (log_bairro.bai_no like :bairro) or \
                              (log_logradouro.log_nome like :rua) or \
                              (log_logradouro.cep like :cep)  ) ");
        queryruas->bindValue(":loc_nu",loc_nu_sequencial);
        queryruas->bindValue( ":rua", "%" + filtro + "%" );
        queryruas->bindValue( ":bairro", "%" + filtro + "%" );
        queryruas->bindValue( ":cep", "%" + filtro + "%" );
        queryruas->exec();
        modalruas->setQuery(*queryruas);
        qDebug()<<queryruas->lastError().text();
    }

    void lerloja(){
        queryloja->prepare("select * from loja");
        queryloja->exec();
        modalloja->setQuery(*queryloja);
    }

private slots:
    void on_pushButton_fechar_clicked();

    void on_pushButton_gravar_clicked();

    void on_pushButton_procurar_clicked();

    void on_tableView_clientes_activated(const QModelIndex &index);

    void on_tableView_clientes_doubleClicked(const QModelIndex &index);

    void on_tableView_endcadast_activated(const QModelIndex &index);

    void on_pushButton_limpar_clicked();

    void on_comboBox_estados_activated(const QString &arg1);

    void on_comboBox_cidade_activated(const QString &arg1);

    void on_pushButton_procurar_2_clicked();

    void on_tableView_enderecos_activated(const QModelIndex &index);

    void on_pushButton_gravar_2_clicked();

    void on_pushButton_apagar_clicked();

private:
    Ui::Cliente *ui;
};

#endif // CLIENTE_H
