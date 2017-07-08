#ifndef LIGARCARDAPIO_H
#define LIGARCARDAPIO_H

#include <QDialog>

#include "login.h"

namespace Ui {
class LigarCardapio;
}

class LigarCardapio : public QDialog
{
    Q_OBJECT

    void lercardapio(){
        querycardapio->prepare("SELECT cardapio.id_cardapio, item.item_nome, item.item_descricao, "
                               "       tipo.tipo, tamanho.tamanho, preco.preco, cardapio.id_menu, "
                               "       cardapio.id_preco FROM cardapio \
                                INNER JOIN preco ON cardapio.id_preco=preco.id_preco \
                                INNER JOIN item  ON preco.id_item=item.id_item \
                                INNER JOIN tipo  ON preco.id_tipo=tipo.id_tipo \
                                INNER JOIN tamanho ON preco.id_tamanho=tamanho.id_tamanho \
                                WHERE (cardapio.ativo=1) ");
        //querycardapio->bindValue(":id_menu", id_menu);
        //querycardapio->bindValue( ":filtrar2", "%"+filtrarcardapio+"%" );
        querycardapio->exec();
        modalcardapio->setQuery(*querycardapio);

        querycardapio_off->prepare("SELECT cardapio.id_cardapio, item.item_nome, item.item_descricao, "
                                   "       tipo.tipo, tamanho.tamanho, preco.preco, cardapio.id_menu, "
                                   "       cardapio.id_preco FROM cardapio \
                                INNER JOIN preco ON cardapio.id_preco=preco.id_preco \
                                INNER JOIN item  ON preco.id_item=item.id_item \
                                INNER JOIN tipo  ON preco.id_tipo=tipo.id_tipo \
                                INNER JOIN tamanho ON preco.id_tamanho=tamanho.id_tamanho \
                                WHERE (cardapio.ativo=0) ");
        //querycardapio->bindValue(":id_menu", id_menu);
        //querycardapio->bindValue( ":filtrar2", "%"+filtrarcardapio+"%" );
        querycardapio_off->exec();
        modalcardapio_off->setQuery(*querycardapio_off);

        modalcardapio->setHeaderData(1, Qt::Horizontal, tr("Item"));
        modalcardapio->setHeaderData(2, Qt::Horizontal, tr("Descrição"));
        modalcardapio->setHeaderData(3, Qt::Horizontal, tr("Tipo"));
        modalcardapio->setHeaderData(4, Qt::Horizontal, tr("Tamanho"));
        modalcardapio_off->setHeaderData(1, Qt::Horizontal, tr("Item"));
        modalcardapio_off->setHeaderData(2, Qt::Horizontal, tr("Descrição"));
        modalcardapio_off->setHeaderData(3, Qt::Horizontal, tr("Tipo"));
        modalcardapio_off->setHeaderData(4, Qt::Horizontal, tr("Tamanho"));

    }

public:
    explicit LigarCardapio(QWidget *parent = 0);
    ~LigarCardapio();

private slots:
    void on_tableView_on_activated(const QModelIndex &index);

    void on_pushButton_desligar_clicked();

    void on_pushButton_ligar_clicked();

private:
    Ui::LigarCardapio *ui;
    Login conn;
    QSqlQueryModel *modalcardapio, *modalcardapio_off;
    QSqlQuery *querycardapio, *querycardapio_off;
};

#endif // LIGARCARDAPIO_H
