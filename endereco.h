#ifndef ENDERECO_H
#define ENDERECO_H

#include <QDialog>
#include "login.h"
#include "cliente.h"

namespace Ui {
class Endereco;
}

class Endereco : public QDialog
{
    Q_OBJECT
    Login conn;
    QSqlQuery *query, *cliente, *endereco;
    QString cidade, estado;
    int loc_nu_sequencial, id_cliente=0, id_endereco=0, id_loja=0, bai_nu=0, log_nu=0;

public:
    explicit Endereco(QWidget *parent = 0);
    ~Endereco();

    void lecidades(QString ufe){
        querycidades->prepare("select log_localidade.loc_nosub, log_localidade.ufe_sg, log_localidade.loc_nu_sequencial \
                       from log_localidade \
                       INNER JOIN log_faixa_uf ON log_faixa_uf.ufe_sg=log_localidade.ufe_sg \
                       WHERE log_faixa_uf.ufe_no=:ufe ");
        querycidades->bindValue(":ufe",ufe);
        querycidades->exec();
        modalcidades->setQuery(*querycidades);

    }

    void leruas(QString filtro){
        queryruas->prepare("select log_logradouro.log_nome, log_bairro.bai_no, log_logradouro.cep, log_bairro.bai_nu_sequencial, log_logradouro.log_nu_sequencial \
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

private slots:
    void on_pushButton_procurarcliente_clicked();

    void on_comboBox_estados_activated(const QString &arg1);

    void on_pushButton_procurar_clicked();

    void on_comboBox_cidade_activated(const QString &arg1);

    void on_tableView_enderecos_activated(const QModelIndex &index);

    void on_pushButton_cancelar_clicked();

    void on_pushButton_gravar_clicked();

    void on_tableView_clientes_activated(const QModelIndex &index);

    void on_tableView_endcadast_activated(const QModelIndex &index);

    void on_pushButton_apagar_clicked();

    void on_pushButton_novocliente_clicked();

private:
    Ui::Endereco *ui;
    QSqlQueryModel *modalestados, *modalcidades, *modalruas;
    QSqlQuery *queryestados, *querycidades, *queryruas;

};

#endif // ENDERECO_H
