#ifndef MENU_H
#define MENU_H

#include <QDialog>
#include "login.h"

namespace Ui {
class Menu;
}

class Menu : public QDialog
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = 0);
    ~Menu();

    void lermenu(){
        querymenu->prepare("select * from menu");
        querymenu->exec();
        modalmenu->setQuery(*querymenu);
    }

private slots:
    void on_pushButton_cancelar_clicked();

     void on_pushButton_novo_clicked();

    void on_pushButton_atualizar_clicked();

    void on_radioButton_menu_clicked();

    void on_radioButton_adicional_clicked();

    void on_radioButton_opcional_clicked();

    void on_pushButton_apagar_clicked();

    void on_tableWidget_menu_activated(const QModelIndex &index);

private:
    Ui::Menu *ui;
    Login conn;
    QSqlQueryModel *modalmenu;
    QSqlQuery *querymenu;
    int id_menu=0, opcoes;
};

#endif // MENU_H
