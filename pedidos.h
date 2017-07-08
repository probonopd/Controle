#ifndef PEDIDOS_H
#define PEDIDOS_H

#include <QDialog>
#include "login.h"
#include "constants.h"
#include "numberformatdelegate.h"
#include "timeformatdelegate.h"
#include "modopagtodelegate.h"

#define COL_ID      0
#define COL_TIPO    COL_ID+1
#define COL_HORA    COL_TIPO+1
#define COL_QTDE    COL_HORA+1
#define COL_ITNOME  COL_QTDE+1
#define COL_ITMENU  COL_ITNOME+1
#define COL_TAM     COL_ITMENU+1
#define COL_OBS     COL_TAM+1

#define COL_PREP  5
#define WGDT_QTDE 0
#define WGDT_ID   1
#define WGDT_ITEM 2
#define WGDT_TIPO 3
#define WGDT_TAM  4
#define WGDT_OBS  5



namespace Ui {
class Pedidos;
}

class Pedidos : public QDialog
{
    Q_OBJECT

public:
    explicit Pedidos(QWidget *parent = 0);
    ~Pedidos();

    void lerpedidos_user(QString caixa, QString cozinha, QString entrega, int status, QSqlQuery *qry, QSqlQueryModel *mdl){
        if(caixa=="" && cozinha=="" && entrega==""){
            switch (status) {
            case ST_ENTRADA:
                qry->prepare("select pedido.id_pedido, pedido.user_caixa, log_bairro.bai_no, pedido.desconto, "
                                    "pedido.entrega, pedido.total, pedido.pagamento, pedido.troco, pedido.horario,"
                             "       pedido.id_endereco from pedido "
                             "inner join entrega on pedido.id_endereco=entrega.id_entrega "
                             "inner join log_bairro on entrega.bai_nu_sequencial=log_bairro.bai_nu_sequencial "
                             "where status=:status");
                break;
            case ST_PREPARO:
                qry->prepare("select pedido.id_pedido, pedido.user_cozinha, log_bairro.bai_no, pedido.desconto, "
                                    "pedido.entrega, pedido.total, pedido.pagamento, pedido.troco, pedido.horario,"
                             "       pedido.id_endereco from pedido "
                             "inner join entrega on pedido.id_endereco=entrega.id_entrega "
                             "inner join log_bairro on entrega.bai_nu_sequencial=log_bairro.bai_nu_sequencial "
                             "where status=:status");
                break;
            case ST_PRONTO:
                qry->prepare("select pedido.id_pedido, pedido.user_entrega, log_bairro.bai_no, pedido.desconto, "
                                    "pedido.entrega, pedido.total, pedido.pagamento, pedido.troco, pedido.horario,"
                             "       pedido.id_endereco from pedido "
                             "inner join entrega on pedido.id_endereco=entrega.id_entrega "
                             "inner join log_bairro on entrega.bai_nu_sequencial=log_bairro.bai_nu_sequencial "
                             "where status=:status");
                break;
            case ST_ENTREGUE:
                qry->prepare("select pedido.id_pedido, pedido.user_entrega, log_bairro.bai_no, pedido.desconto, "
                                    "pedido.entrega, pedido.total, pedido.pagamento, pedido.troco, pedido.horario,"
                             "       id_endereco from pedido "
                             "inner join entrega on pedido.id_endereco=entrega.id_entrega "
                             "inner join log_bairro on entrega.bai_nu_sequencial=log_bairro.bai_nu_sequencial "
                             "where status=:status");
                break;
            }
        }

        if(caixa!="" && cozinha=="" && entrega==""){
            qry->prepare("select pedido.id_pedido, pedido.user_caixa, log_bairro.bai_no, pedido.desconto, "
                            "pedido.entrega, pedido.total, pedido.pagamento, pedido.troco, pedido.horario,"
                         "   pedido.id_endereco from pedido "
                         "inner join entrega on pedido.id_endereco=entrega.id_entrega "
                         "inner join log_bairro on entrega.bai_nu_sequencial=log_bairro.bai_nu_sequencial "
                         "where status=:status and user_caixa=:id");
            qry->bindValue(":id", caixa);
        }
        if(caixa=="" && cozinha!="" && entrega==""){
            qry->prepare("select pedido.id_pedido, pedido.user_cozinha, log_bairro.bai_no, pedido.desconto, "
                            "pedido.entrega, pedido.total, pedido.pagamento, pedido.troco, pedido.horario,"
                         "   pedido.id_endereco from pedido "
                         "inner join entrega on pedido.id_endereco=entrega.id_entrega "
                         "inner join log_bairro on entrega.bai_nu_sequencial=log_bairro.bai_nu_sequencial "
                         "where status=:status and user_cozinha=:id");
            qry->bindValue(":id", cozinha);
        }
        if(caixa=="" && cozinha=="" && entrega!=""){
            qry->prepare("select pedido.id_pedido, pedido.user_entrega, log_bairro.bai_no, pedido.desconto, "
                            "    pedido.entrega, pedido.total, pedido.pagamento, pedido.troco, pedido.horario,"
                            "    pedido.id_endereco from pedido "
                         "inner join entrega on pedido.id_endereco=entrega.id_entrega "
                         "inner join log_bairro on entrega.bai_nu_sequencial=log_bairro.bai_nu_sequencial "
                         "where status=:status and user_entrega=:id");
           qry->bindValue(":id", entrega);
        }
        qry->bindValue(":status", status);
        qry->exec();
        mdl->setQuery(*qry);
    }

    void lerdetalhes(const int idpedido, QSqlQuery *qry, QSqlQueryModel *mdl){
            qry->prepare("select preco.id, menu.tipomenu, pedido_detalhe.hora, pedido_detalhe.qtde, item.item_nome, menu.itemmenu, "
                         "       tamanho.tamanho, pedido_detalhe.obs from pedido_detalhe \
                          inner join cardapio on pedido_detalhe.id_cardapio=cardapio.id_cardapio \
                          inner join menu on cardapio.id_menu=menu.id_menu \
                          inner join preco on cardapio.id_preco=preco.id_preco \
                          inner join item on preco.id_item=item.id_item \
                          inner join tamanho on preco.id_tamanho=tamanho.id_tamanho \
                          where pedido_detalhe.id_pedido=:idpedido "
                         "order by pedido_detalhe.hora asc, menu.tipomenu asc");
            qry->bindValue(":idpedido", idpedido);
            qry->exec();
            mdl->setQuery(*qry);

            qDebug()<<"ler detalhes error " << idpedido << qry->lastError().text();
    }

    void lercolab(){
        querycolab->prepare("select * from usuario");
        querycolab->exec();
        modalcolab->setQuery(*querycolab);

        qDebug()<< querycolab->lastError().text();

    }


private slots:

    void on_tableView_pedidos_activated(const QModelIndex &index);

    void on_pushButton_marcarcozinha_clicked();

    void on_comboBox_cozinha_activated(const QString &arg1);

    void on_comboBox_entrega_activated(const QString &arg1);

    void on_pushButton_entrega_clicked();

    void on_tableView_preparo_activated(const QModelIndex &index);

    void on_comboBox_cozinha_2_activated(const QString &arg1);

    void on_pushButton_marcarcozinha_2_clicked();

    void showdetalhe(int);

    void mostrapedidos(int);

    void on_tableView_entrega_activated(const QModelIndex &index);

    void on_tableView_final_activated(const QModelIndex &index);

    void on_comboBox_final_activated(const QString &arg1);

private:
    Ui::Pedidos *ui;
    Login conn;
    QSqlQuery *querypedidos, *querydetalhes, *querycolab;
    QSqlQueryModel *modalpedidos, *modaldetalhes, *modalcolab;
    QSqlQuery *querypreparo, *querypreparodetalhe, *queryentrega, *queryfinal;
    QSqlQueryModel *modalpreparo, *modalpreparodetalhe, *modalentrega, *modalfinal;
    QSqlQueryModel *modalentregadetalhe, *modalfinaldetalhe;
    QSqlQuery *queryentregadetalhe, *queryfinaldetalhe;
    int id_pedido=0, id_cozinha=0, id_entrega=0, id_preparo=0;
    bool flagcozinha=false, flagentrega=false;
    QString user_cozinha, user_caixa, user_entrega;
};

#endif // PEDIDOS_H
