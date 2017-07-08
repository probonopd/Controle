#ifndef SUBMENU_H
#define SUBMENU_H

#include <QDialog>
#include "login.h"

namespace Ui {
class Submenu;
}

class Submenu : public QDialog
{
    Q_OBJECT

public:
    explicit Submenu(QWidget *parent = 0);
    ~Submenu();

    void lemenu(){
        querymenu->prepare("select itemmenu, id_menu from menu where tipomenu=1");
        querymenu->exec();
        modalmenu->setQuery(*querymenu);
        id_menu=modalmenu->record(0).value(1).toInt();
    }

    void lecardapio(){
        querycardapio->prepare("SELECT cardapio.id_cardapio, item.item_nome, item.item_descricao, tipo.tipo, item.id_item, tamanho.tamanho FROM cardapio \
                                INNER JOIN preco ON cardapio.id_preco=preco.id_preco \
                                INNER JOIN item  ON preco.id_item=item.id_item \
                                INNER JOIN tipo  ON preco.id_tipo=tipo.id_tipo \
                                INNER JOIN tamanho ON preco.id_tamanho=tamanho.id_tamanho \
                                WHERE cardapio.id_menu = :id_menu AND \
                                      (item.item_nome like :filtrar OR item.item_descricao like :filtrar OR tipo.tipo like :filtrar )");

        querycardapio->bindValue(":id_menu",id_menu);
        querycardapio->bindValue( ":filtrar", "%"+filtrar+"%" );

        querycardapio->exec();
        modalcardapio->setQuery(*querycardapio);

    }

    void lesubmenu(){
        querysubmenu->prepare("select itemmenu, id_menu from menu where tipomenu>1");
        querysubmenu->exec();
        modelsub->setQuery(*querysubmenu);
    }

    void lertipo(){
        querytipo->prepare("select tipo from tipo");
        querytipo->exec();
        modeltipo->setQuery(*querytipo);
    }

    void lertamanho(){
        querytamanho->prepare("select tamanho from tamanho");
        querytamanho->exec();
        modaltamanho->setQuery(*querytamanho);
    }

    void lesubmenuitens(){
        querysubmenuitens->prepare("SELECT submenu.id_submenu, item.item_nome, tipo.tipo, tamanho.tamanho, menu.itemmenu FROM submenu \
                                    INNER JOIN menu ON submenu.id_menu=menu.id_menu\
                                    INNER JOIN cardapio ON submenu.id_cardapio=cardapio.id_cardapio \
                                    INNER JOIN preco ON cardapio.id_preco=preco.id_preco \
                                    INNER JOIN item ON preco.id_item=item.id_item \
                                    INNER JOIN tipo ON preco.id_tipo=tipo.id_tipo \
                                    INNER JOIN tamanho ON preco.id_tamanho=tamanho.id_tamanho \
                                    WHERE (item.item_nome like :filtrar2 OR menu.itemmenu like :filtrar2 ) and "
                                          "tipo.tipo like :filtro and tamanho.tamanho like :tamanho");
        querysubmenuitens->bindValue( ":filtrar2", "%"+filtrarsubmenuitens+"%" );
        querysubmenuitens->bindValue( ":filtro", "%"+filtro+"%" );
        querysubmenuitens->bindValue( ":tamanho", "%"+tamanho+"%" );
        querysubmenuitens->exec();
        modelsubmenuitens->setQuery(*querysubmenuitens);

        qDebug() << querysubmenuitens->lastError().text();
    }


private slots:
    void on_pushButton_inserirsubmenu_clicked();

    void on_comboBox_menu_activated(const QString &arg1);

    void on_pushButton_filtrar_clicked();

    void on_pushButton_filtrarsubmenuitens_clicked();

    void on_pushButton_apagarsubmenuitens_clicked();


    void on_comboBox_tipo_activated(const QString &arg1);

    void on_comboBox_tamanho_activated(const QString &arg1);

private:
    Ui::Submenu *ui;
    Login conn;
    QSqlQueryModel *modalmenu, *modalcardapio, *modelsub, *modelrelacaoitens, *modelsubmenuitens, *modeltipo;
    QSqlQuery *querymenu, *querycardapio, *querysubmenu, *queryrelacaoitens, *querysubmenuitens, *querytipo;
    QSqlQueryModel *modaltamanho;
    QSqlQuery *querytamanho;
    int id_menu=0;
    QString filtrar, filtrarsubmenuitens, filtro, tamanho;

};

#endif // SUBMENU_H
