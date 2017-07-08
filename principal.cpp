#include "principal.h"
#include "ui_principal.h"

#include "userinfo.h"
#include "estabelecimento.h"
#include "cliente.h"
#include "endereco.h"
#include "produto.h"
#include "preco.h"
#include "menu.h"
#include "cardapio.h"
#include "venda.h"
#include "submenu.h"
#include "entrega.h"
#include "condominio.h"
#include "pedidos.h"
#include "precoentrega.h"
#include "ligarcardapio.h"
#include "analisevendas.h"

extern QString funcao[NFUNC];
extern QString user_logado, user_status, user_nome;
extern int user_id, user_tipo;

Principal::Principal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Principal)
{
    ui->setupUi(this);

    ui->label_nomeuser->setText(user_logado);
    ui->label_status->setText(funcao[user_tipo]);
    ui->label_nome->setText(user_nome);

    qDebug() << user_logado << user_status;
}

Principal::~Principal()
{
    delete ui;
}

void Principal::on_pushButton_clicked()
{
    Userinfo userinfo;

    userinfo.setModal(true);
    userinfo.exec();
}

void Principal::on_pushButton_configloja_clicked()
{

    Estabelecimento Loja;

    Loja.setModal(true);
    Loja.exec();
}

void Principal::on_pushButton_cadastrocliente_clicked()
{
    Cliente Comprador;

    Comprador.setModal(true);
    Comprador.exec();
}

void Principal::on_pushButton_cadastroendereco_clicked()
{
    Endereco End_cliente;

    End_cliente.setModal(true);
    End_cliente.exec();
}

void Principal::on_pushButton_produto_clicked()
{
    Produto Itemcompleto;

    Itemcompleto.setModal(true);
    Itemcompleto.exec();
}

void Principal::on_pushButton_precoitem_clicked()
{
   Preco Precoitem;

   Precoitem.setModal(true);
   Precoitem.exec();
}

void Principal::on_pushButton_cadastrarmenu_clicked()
{
    Menu Menuitem;

    Menuitem.setModal(true);
    Menuitem.exec();
}

void Principal::on_pushButton_montarcardapio_clicked()
{
    Cardapio CardapioCompleto;

    CardapioCompleto.setModal(true);
    CardapioCompleto.exec();
}

void Principal::on_pushButton_venda_clicked()
{
    Venda VendaDireta;

   // VendaDireta.setuser(user_logado, user_tipo, id_user );

    VendaDireta.setModal(true);
    VendaDireta.exec();
}

void Principal::on_pushButton_submenu_clicked()
{
    Submenu Adicional;

    Adicional.setModal(true);
    Adicional.exec();
}

void Principal::on_pushButton_entrega_clicked()
{
    Entrega entrega;

    entrega.setModal(true);
    entrega.exec();
}

void Principal::on_pushButton_condominio_clicked()
{
    Condominio condominio;

    condominio.setModal(true);
    condominio.exec();
}

void Principal::on_pushButton_pedidos_clicked()
{
    Pedidos clpedidos;

    clpedidos.setModal(true);
    clpedidos.exec();
}

void Principal::on_pushButton_precosentrega_clicked()
{
    PrecoEntrega precoentrega;

    precoentrega.setModal(true);
    precoentrega.exec();
}

void Principal::on_pushButton_ligar_clicked()
{
    LigarCardapio ligarcardapio;

    ligarcardapio.setModal(true);
    ligarcardapio.exec();
}

void Principal::on_pushButton_analisevendas_clicked()
{
  AnaliseVendas analisevendas;

  analisevendas.setModal(true);
  analisevendas.exec();
}
