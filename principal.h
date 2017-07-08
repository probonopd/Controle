#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QDialog>
#include <QtDebug>
#include <QtSql>

namespace Ui {
class Principal;
}

class Principal : public QDialog
{
    Q_OBJECT

public:
    explicit Principal(QWidget *parent = 0);
    ~Principal();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_configloja_clicked();

    void on_pushButton_cadastrocliente_clicked();

    void on_pushButton_cadastroendereco_clicked();

    void on_pushButton_produto_clicked();

    void on_pushButton_precoitem_clicked();

    void on_pushButton_cadastrarmenu_clicked();

    void on_pushButton_montarcardapio_clicked();

    void on_pushButton_venda_clicked();

    void on_pushButton_submenu_clicked();

    void on_pushButton_entrega_clicked();

    void on_pushButton_condominio_clicked();

    void on_pushButton_pedidos_clicked();

    void on_pushButton_precosentrega_clicked();

    void on_pushButton_ligar_clicked();

    void on_pushButton_analisevendas_clicked();

private:
    Ui::Principal *ui;
};

#endif // PRINCIPAL_H
