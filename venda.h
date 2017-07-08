#ifndef VENDA_H
#define VENDA_H

#define CARDAPIO_ID_CARDAPIO 0
#define CARDAPIO_ID          1

#include <QDialog>
#include <QDateTime>

#include "constants.h"
#include "login.h"
#include "endereco.h"

extern int config;

#define NUM_ITENS   8

#define COL_CART        7
#define CART_OFFSET     4

#define CART_MENU       2
#define CART_HORA       3
#define CART_QDE        4
#define CART_ITEM       5
#define CART_TIPO       6
#define CART_TAMANHO    7
#define CART_OBS        8
#define CART_PRECO      9

extern QString user_logado, user_status;
extern int user_id, user_tipo;

namespace Ui {
class Venda;
}

class Venda : public QDialog
{
    Q_OBJECT

public:
    explicit Venda(QWidget *parent = 0);
    ~Venda();
   // QSqlDatabase  DB_principal;

    void setdb(QSqlDatabase DB){
      //  DB_principal=DB;
    }

    void lerendereco(int id){
        endereco->prepare("select * from endereco WHERE id_cliente=:id_cliente");
        endereco->bindValue(":id_cliente",id);
        endereco->exec();
        modalendereco->setQuery(*endereco);
    }

    void lecardapio(){
        querycardapio->prepare("SELECT cardapio.id_cardapio, item.id, item.item_nome, item.item_descricao, tipo.tipo, \
                                        preco.preco, cardapio.id_menu, menu.tipomenu FROM cardapio \
                                INNER JOIN preco ON cardapio.id_preco=preco.id_preco \
                                INNER JOIN item  ON preco.id_item=item.id_item \
                                INNER JOIN tipo  ON preco.id_tipo=tipo.id_tipo \
                                INNER JOIN menu  ON cardapio.id_menu=menu.id_menu \
                                WHERE cardapio.id_menu = :id_menu AND \
                                      cardapio.ativo =1 AND \
                                      preco.id_tamanho=:id_tamanho AND \
                                      ((tipo.tipo like :filtrartipo ) and ((item.id like :filtrar2 ) or (item.item_nome like :filtrar2) or\
                                         (item.item_descricao like :filtrar2 ))) ");

        querycardapio->bindValue(":id_menu",id_menu);
        querycardapio->bindValue(":id_tamanho",id_tamanho);
        querycardapio->bindValue( ":filtrar2", "%"+filtrar+"%" );
        querycardapio->bindValue( ":filtrartipo", "%"+tipostr+"%" );

        querycardapio->exec();
        modalcardapio->setQuery(*querycardapio);

        qDebug()<< tipostr << querycardapio->lastError().text();

    }

    void lesubmenu(){

        qDebug() << id_cardapio;
        querysubmenu->prepare("SELECT menu.itemmenu, menu.id_menu, menu.tipomenu, menu.opcoes FROM submenu \
                                INNER JOIN menu ON submenu.id_menu=menu.id_menu \
                                WHERE submenu.id_cardapio = :id_cardapio");

        querysubmenu->bindValue(":id_cardapio",id_cardapio);

        querysubmenu->exec();
        modalsubmenu->setQuery(*querysubmenu);

    }

    void letipo(){

        qDebug() << id_cardapio;
        querytipo->prepare("SELECT tipo.tipo, tipo.id_tipo FROM tipo");

        querytipo->exec();
        modaltipo->setQuery(*querytipo);
        tipostr=modaltipo->record(0).value(0).toString();

    }

    void leadicionais(){
        queryadicionais->prepare("SELECT cardapio.id_cardapio, item.id, item.item_nome, item.item_descricao, tipo.tipo, preco.preco, cardapio.id_menu, menu.tipomenu FROM cardapio \
                                INNER JOIN preco ON cardapio.id_preco=preco.id_preco \
                                INNER JOIN item  ON preco.id_item=item.id_item \
                                INNER JOIN tipo  ON preco.id_tipo=tipo.id_tipo \
                                INNER JOIN menu  ON cardapio.id_menu=menu.id_menu \
                                WHERE cardapio.id_menu = :id_menu");

        queryadicionais->bindValue(":id_menu",id_menuadc);

        queryadicionais->exec();
        modaladicionais->setQuery(*queryadicionais);

        qDebug() << queryadicionais->lastError().text();

    }

    void lecart(){
        querycart->prepare("SELECT cart.id_cart, cart.id_cardapio, menu.tipomenu, cart.hora, cart.qde, \
                                    item.item_nome, menu.itemmenu, tamanho.tamanho, cart.obs, cart.preco FROM cart \
                                INNER JOIN cardapio ON cart.id_cardapio=cardapio.id_cardapio \
                                INNER JOIN menu ON cardapio.id_menu=menu.id_menu \
                                INNER JOIN preco ON cardapio.id_preco=preco.id_preco \
                                INNER JOIN item  ON preco.id_item=item.id_item \
                                INNER JOIN tamanho ON tamanho.id_tamanho=preco.id_tamanho \
                                WHERE cart.id_usuario = :id_usuario order by cart.id_cart asc");

        querycart->bindValue(":id_usuario",user_id);

        querycart->exec();
        modalcart->setQuery(*querycart);

         qDebug()<<"lecart" << user_id << querycart->lastError().text();
    }

    void levalorentrega(){
        queryentrega->prepare("select * from entrega where bai_nu_sequencial=:bai_nu");
        queryentrega->bindValue(":bai_nu",bairro);
        queryentrega->exec();
        modalentrega->setQuery(*queryentrega);
        valorentrega=modalentrega->record(0).value(3).toFloat();

        qDebug()<<"levalorentrega" << valorentrega;

    }

    void calculatotal2(){
        total=taxa_entrega;
        for(int i=0;i<modalcart->rowCount();i++){
            total+=(modalcart->record(i).value(4).toInt()*modalcart->record(i).value(9).toFloat());
        }
        total-=desconto;
        if(total<0.0)
            total=0.0;
    }

    void calculatotal(){
        int nitens, hora=0, i;
        float maior, soma=0.0, preco;

        total=taxa_entrega;

        for(int linha=0;linha<modalcart->rowCount();linha++){
            if(modalcart->record(linha).value(CART_MENU).toInt()==MENU){
                if(hora!=modalcart->record(linha).value(CART_HORA).toInt()){
                    nitens=1;
                    soma=modalcart->record(linha).value(CART_PRECO).toFloat();
                    maior=soma;
                    hora=modalcart->record(linha).value(CART_HORA).toInt();
                    for(i=linha+1;i<modalcart->rowCount();i++){
                        if(hora==modalcart->record(i).value(CART_HORA).toInt() && modalcart->record(i).value(CART_MENU).toInt()==MENU ){
                            soma+=modalcart->record(i).value(CART_PRECO).toFloat();
                            if(modalcart->record(i).value(CART_PRECO).toFloat()>maior)
                                maior=modalcart->record(i).value(CART_PRECO).toFloat();
                            nitens++;
                        }
                        else
                            break;
                    }
                    if(!(config && (1<<SELECOES))){
                        preco=maior;
                    }
                    else{
                        preco=soma/nitens;
                    }
                    total+=preco*modalcart->record(linha).value(CART_QDE).toInt();
                }
            }
        }
        for(int linha=0;linha<modalcart->rowCount();linha++){
            if(modalcart->record(linha).value(CART_MENU).toInt()!=MENU){
              total+=modalcart->record(linha).value(CART_PRECO).toFloat()*modalcart->record(linha).value(CART_QDE).toInt();
              qDebug()<<modalcart->record(linha).value(CART_PRECO).toFloat();
            }
        }
        total-=desconto;
        if(total<0.0)
            total=0.0;
    }

    void cart_apagaitem(QModelIndexList indexList){
        QVariantList listitens;
        int anterior, row, id_cart, hora=0, tipomenu;
        bool flag;

        querycart->prepare("delete from cart where id_cart=:id_cart");
        anterior=-1;
        foreach (QModelIndex index, indexList) {
            row=index.row();
            qDebug() << "row " << row;
            if(anterior!=row){
                id_cart=modalcart->record(row).value(0).toInt();
                listitens << id_cart;

            }
            anterior=row;
        }
        querycart->bindValue(":id_cart",listitens);

        if (!querycart->execBatch()) {
            qDebug() << "erro " << querycart->lastError();
        } else {
            qDebug() << "sucesso " << querycart->executedQuery();
            lecart();
            //ui->tableView_cart->setModel(modalcart);

            querycart->prepare("delete from cart where id_cart=:id_cart");

            hora=0;
            listitens.clear();

            for(int i=0;i<modalcart->rowCount();i++){
              tipomenu=modalcart->record(i).value(2).toInt();
              id_cart=modalcart->record(i).value(0).toInt();
              if(hora!=modalcart->record(i).value(3).toInt()){
                  flag=false;
                  if(tipomenu!=1)
                    flag=true;
               }
              hora=modalcart->record(i).value(3).toInt();
              qDebug() << i << flag;
              if(flag)
                  listitens << id_cart;

            }
            querycart->bindValue(":id_cart",listitens);
            querycart->execBatch();

            qDebug() << "erro " << querycart->lastError();
            qDebug() << "sucesso " << querycart->executedQuery();

        }

        lecart();

    }

private slots:
    void on_comboBox_menu_activated(const QString &arg1);

    void on_tableView_cardapio_activated(const QModelIndex &index);

    void on_comboBox_tamanho_activated(const QString &arg1);

    void on_pushButton_filtrar_clicked();

    void on_pushButton_inserir_clicked();

    void on_pushButton_apagar_clicked();

    void on_tableWidget_selecao_activated(const QModelIndex &index);

    void on_listView_menuadicionais_activated(const QModelIndex &index);

    void on_pushButton_inseriradicionais_clicked();

    void limparselecoes();

    void on_comboBox_tipo_activated(const QString &arg1);

    void on_pushButton_procurarcliente_clicked();

    void on_listView_procurarcliente_activated(const QModelIndex &index);

    void on_pushButton_carrinho_clicked();

    void on_pushButton_desconto_clicked();

    void on_pushButton_apagaritemcarrinho_clicked();

    void on_pushButton_finalizarcompra_clicked();

    void on_pushButton_troco_clicked();

    void on_radioButton_dinheiro_clicked();

    void on_radioButton_cartao_clicked();

    void on_radioButton_cheque_clicked();

    void on_radioButton_fiado_clicked();

    void on_radioButton_entregar_clicked();

    void on_radioButton_balcao_clicked();

    void on_tableView_cart_activated(const QModelIndex &index);

    void on_pushButton_qtde_clicked();

    void on_pushButton_selecionarcliente_clicked();

    void on_pushButton_limparcliente_clicked();

    void showcart();

    void on_tableWidget_cart_activated(const QModelIndex &index);

    void on_pushButton_clicked();

private:
    Ui::Venda *ui;
    Login conn;
    QModelIndex *modalselecao;
    QSqlQueryModel *modalmenu, *modaltipo, *modaltamanho, *modalcardapio, *modalsubmenu, *modaladicionais, *modalcliente, *modalendereco, *modalcart, *modalentrega,*modalpedido;
    QSqlQuery *querymenu2, *querytipo, *querytamanho, *querycardapio, *querysubmenu, *queryadicionais, *cliente, *endereco, *querycart, *queryentrega, *querypedido;
    int id_menu, id_tipo, id_tamanho, max_sel, id_preco, id_submenu=0, id_cardapio=0, id_menuadc=0, maxseladc=0, id_cliente=0, id_endereco=0, cep=0, bairro=0, pagamento=0;
    int horaitem=0;
    QString tipomenu, filtrar, tipostr, totalstr, descontostr, entregastr, trocostr="";
    float desconto=0.0, total=0.0, valorentrega=0.0, taxa_entrega=0.0;
    bool CartEmUso=false, flag_entrega=false;
};

#endif // VENDA_H
