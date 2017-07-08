#ifndef PRECO_H
#define PRECO_H

#include <QDialog>
#include "login.h"
#include "numberformatdelegate.h"

namespace Ui {
class Preco;
}

class Preco : public QDialog
{
    Q_OBJECT

public:
    explicit Preco(QWidget *parent = 0);
    ~Preco();

    void lertamanho(){
        querytamanho->prepare("select tamanho, id_tamanho from tamanho");
        querytamanho->exec();
        modaltamanho->setQuery(*querytamanho);
    }

    void lertipo(){
        querytipo->prepare("select tipo, id_tipo from tipo");
        querytipo->exec();
        modaltipo->setQuery(*querytipo);
    }

    void leritens(){
        queryitem->prepare("select * from item WHERE (item.item_nome like :filtrar OR item.item_descricao like :filtrar)");
        queryitem->bindValue(":filtrar", "%"+filtraritem+"%" );
        queryitem->exec();
        modalitem->setQuery(*queryitem);
    }

    void lerprecos(){
        querypreco->prepare("SELECT preco.id_preco, preco.id_item, preco.id_tipo, preco.id_tamanho, preco.id, item.item_nome, tipo.tipo, tamanho.tamanho, preco.preco FROM preco \
                            INNER JOIN item ON preco.id_item=item.id_item\
                            INNER JOIN tipo ON preco.id_tipo=tipo.id_tipo\
                            INNER JOIN tamanho ON preco.id_tamanho=tamanho.id_tamanho \
                            WHERE (item.item_nome like :filtrar2 OR tipo.tipo like :filtrar2 OR tamanho.tamanho like :filtrar2)\
                            " );
        querypreco->bindValue(":filtrar2", "%"+filtrarpreco+"%" );
        querypreco->exec();
        modelpreco2->setQuery(*querypreco);
        qDebug() << querypreco->lastError().text();
    }

private slots:
    void on_pushButton_fechar_clicked();

    void on_comboBox_tipoitem_activated(const QString &arg1);

    void on_comboBox_tamanhoitem_activated(const QString &arg1);

    void on_pushButton_inserir_clicked();

    void on_tableView_itens_activated(const QModelIndex &index);

    void on_tableView_precos_activated(const QModelIndex &index);

    void on_pushButton_apagar_clicked();

    void on_pushButton_filtraritens_clicked();

    void on_pushButton_filtrarpreco_clicked();

private:
    Ui::Preco *ui;
    Login conn;
  //  QSqlRelationalTableModel *modelpreco;
    QSqlQueryModel *modalitem, *modaltipo, *modaltamanho, *modelpreco2;
    QSqlQuery *queryitem, *querytipo, *querytamanho, *querypreco;
    int id_item=0, id_tipo, id_tamanho, id_preco=0;
    QString filtraritem, filtrarpreco;
};

#endif // PRECO_H
