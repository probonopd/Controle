#ifndef PRODUTO_H
#define PRODUTO_H

#include <QDialog>
#include "login.h"

namespace Ui {
class Produto;
}

class Produto : public QDialog
{
    Q_OBJECT

public:
    explicit Produto(QWidget *parent = 0);
    ~Produto();

    void letipo(){
        querytipo->prepare("select tipo, id_tipo from tipo");
        querytipo->exec();
        modaltipo->setQuery(*querytipo);
    }

    void lertamanho(){
        querytamanho->prepare("select * from tamanho");
        querytamanho->exec();
        modaltamanho->setQuery(*querytamanho);
    }

    void leitens(){
        queryitem->prepare("select * from item");
        queryitem->exec();
        modalitem->setQuery(*queryitem);
    }

private slots:
    void on_pushButton_inserirtipo_clicked();

    void on_pushButton_apagartipo_clicked();

    void on_pushButton_instamanho_clicked();

    void on_tableView_tamanho_activated(const QModelIndex &index);

    void on_pushButton_tamanho_atualizar_clicked();

    void on_pushButton_del_tamanho_clicked();

    void on_pushButton_atualizatipo_clicked();

    void on_pushButton_procuraritem_clicked();

    void on_pushButton_novoitem_clicked();

    void on_tableView_itens_activated(const QModelIndex &index);

    void on_pushButton_atualizaritem_clicked();

    void on_pushButton_apagaritem_clicked();

    void on_listView_tipo_activated(const QModelIndex &index);

private:
    Ui::Produto *ui;
    Login conn;
    QSqlQueryModel *modaltipo, *modaltamanho, *modalitem;
    QSqlQuery *querytipo, *querytamanho, *queryitem;
    int id_tipo=0, id_tamanho=0, id_item=0;

};

#endif // PRODUTO_H
