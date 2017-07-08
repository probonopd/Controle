#ifndef CARDAPIO_H
#define CARDAPIO_H

#include <QDialog>
#include "login.h"
#include "numberformatdelegate.h"

namespace Ui {
class Cardapio;
}

class Cardapio : public QDialog
{
    Q_OBJECT

public:
    explicit Cardapio(QWidget *parent = 0);
    ~Cardapio();

    void lercardapio(){
        querycardapio->prepare("SELECT cardapio.id_cardapio, item.item_nome, tipo.tipo, tamanho.tamanho, preco.preco, cardapio.id_menu, cardapio.id_preco FROM cardapio \
                                INNER JOIN preco ON cardapio.id_preco=preco.id_preco \
                                INNER JOIN item  ON preco.id_item=item.id_item \
                                INNER JOIN tipo  ON preco.id_tipo=tipo.id_tipo \
                                INNER JOIN tamanho ON preco.id_tamanho=tamanho.id_tamanho \
                                WHERE (cardapio.id_menu = :id_menu AND \
                                       preco.ativo = 1 and \
                                       (item.item_nome like :filtrar2 OR tipo.tipo like :filtrar2 OR tamanho.tamanho like :filtrar2)  ) ");
        querycardapio->bindValue(":id_menu", id_menu);
        querycardapio->bindValue( ":filtrar2", "%"+filtrarcardapio+"%" );

        querycardapio->exec();
        modalcardapio2->setQuery(*querycardapio);

    }

    void lerpreco(){
        querypreco->prepare("select preco.id_preco, item.item_nome, tipo.tipo, tamanho.tamanho, preco.preco from preco \
                                INNER JOIN item ON preco.id_item=item.id_item \
                                INNER JOIN tipo ON preco.id_tipo=tipo.id_tipo \
                                INNER JOIN tamanho ON preco.id_tamanho=tamanho.id_tamanho \
                                WHERE  (tipo.tipo like :procurar ) or (item.item_nome = :procurar ) or (tamanho.tamanho like :procurar )");
        querypreco->bindValue( ":procurar", "%"+procurar+"%" );

        querypreco->exec();
        modalpreco2->setQuery(*querypreco);

    }

private slots:
    void on_pushButton_procurar_clicked();

    void on_listView_menu_activated(const QModelIndex &index);

    void on_tableView_itens_activated(const QModelIndex &index);

    void on_pushButton_inserircardapio_clicked();

    void on_pushButton_apagar_clicked();

    void on_pushButton_filtrar_cardapio_clicked();

private:
    Ui::Cardapio *ui;
    Login conn;
    QSqlQueryModel *modalmenu, *modalpreco2, *modalcardapio2;
    QSqlQuery *querymenu, *querypreco, *querycardapio;
  //  QSqlRelationalTableModel *modelpreco, *modelcardapio;
    int id_menu=0;
    QString filtrarcardapio, procurar;
};

#endif // CARDAPIO_H
