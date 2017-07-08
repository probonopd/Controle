#include "cardapio.h"
#include "ui_cardapio.h"
#include "qmessagebox.h"

Cardapio::Cardapio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Cardapio)
{
    ui->setupUi(this);
    conn.connOpen();

    modalmenu=new QSqlQueryModel();
    querymenu=new QSqlQuery(conn.mydb);
    querymenu->prepare("select itemmenu, tipomenu, id_menu from menu");
    querymenu->exec();
    modalmenu->setQuery(*querymenu);
    ui->listView_menu->setModel(modalmenu);

    querypreco=new QSqlQuery(conn.mydb);
    modalpreco2=new QSqlQueryModel();

    lerpreco();
    modalpreco2->setHeaderData(1, Qt::Horizontal, tr("Item Menu"));
    modalpreco2->setHeaderData(2, Qt::Horizontal, tr("Tipo"));
    modalpreco2->setHeaderData(3, Qt::Horizontal, tr("Tamanho"));
    modalpreco2->setHeaderData(4, Qt::Horizontal, tr("Preço"));

    ui->tableView_itens->setModel(modalpreco2);
    ui->tableView_itens->setItemDelegateForColumn(4, new NumberFormatDelegate(this));
    ui->tableView_itens->setColumnHidden(0,true);
    procurar="";

    querycardapio=new QSqlQuery(conn.mydb);
    modalcardapio2=new QSqlQueryModel();
  //  modalcardapio2->setQuery(*querycardapio);
    filtrarcardapio="";
    lercardapio();

    modalcardapio2->setHeaderData(1, Qt::Horizontal, tr("Item Menu"));
    modalcardapio2->setHeaderData(2, Qt::Horizontal, tr("Tipo"));
    modalcardapio2->setHeaderData(3, Qt::Horizontal, tr("Tamanho"));
    modalcardapio2->setHeaderData(4, Qt::Horizontal, tr("Preço"));

    ui->tableView_menu->setModel(modalcardapio2);
    ui->tableView_menu->setItemDelegateForColumn(4, new NumberFormatDelegate(this));
    ui->tableView_menu->setColumnHidden(0,true);
    ui->tableView_menu->setColumnHidden(5,true);
    ui->tableView_menu->setColumnHidden(6,true);
}

Cardapio::~Cardapio()
{
    delete ui;
    conn.connClose();
}

void Cardapio::on_pushButton_procurar_clicked()
{

    procurar=ui->lineEdit_procurar->text();

    lerpreco();

    ui->tableView_itens->setModel(modalpreco2);
    ui->tableView_itens->setColumnHidden(0,true);

    qDebug() << querypreco->lastError().text();

}

void Cardapio::on_listView_menu_activated(const QModelIndex &index)
{
    QString celula=ui->listView_menu->model()->data(index).toString();
    int colunas=ui->listView_menu->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;

    id_menu=modalmenu->record(linha).value(2).toInt();
    ui->label_menu->setText(modalmenu->record(linha).value(0).toString());
    qDebug() << "id_menu "  << id_menu;

    lercardapio();
    ui->tableView_menu->setModel(modalcardapio2);
    ui->tableView_menu->setColumnHidden(0,true);

    qDebug() << querycardapio->lastError().text();

}

void Cardapio::on_tableView_itens_activated(const QModelIndex &index)
{

}

void Cardapio::on_pushButton_inserircardapio_clicked()
{
    int row, id_item, id_preco, linha;
    bool inserir;

    if(!id_menu){
        QMessageBox::critical(this,tr("Montar Cardápio"),tr("Favor selecionar um menu!"));
        qDebug() << "saiu";
        return;
    }

    QModelIndexList indexList = ui->tableView_itens->selectionModel()->selectedIndexes();

    qDebug()<< indexList;

    querycardapio->prepare("INSERT INTO cardapio (id_menu, id_preco) VALUES(:id_menu, :id_preco)");
    QVariantList listmenu, listpreco;

    inserir=true;
    foreach (QModelIndex index, indexList) {
        row = index.row();
        id_preco=modalpreco2->record(row).value(0).toInt();
        qDebug() << row << index.column() << id_preco;
        id_item=modalpreco2->record(row).value(1).toInt();
        for(linha=0;linha<modalcardapio2->rowCount();linha++){
            if(modalcardapio2->record(linha).value(5).toInt()==id_menu && modalcardapio2->record(linha).value(6).toInt()==id_preco ){
                inserir=false;
                break;
            }
        }
        if(inserir){
            listmenu << id_menu;
            listpreco << id_preco;
        }
    }
    querycardapio->bindValue(":id_menu",listmenu);
    querycardapio->bindValue(":id_preco",listpreco);

    if (!querycardapio->execBatch()) {
        qDebug() << "erro " << querycardapio->lastError();
    } else {
        qDebug() << "sucesso " << querycardapio->executedQuery();
    }

    lercardapio();

    ui->tableView_menu->setModel(modalcardapio2);
    ui->tableView_menu->setColumnHidden(0,true);

}

void Cardapio::on_pushButton_apagar_clicked()
{
    if(!id_menu){
        QMessageBox::critical(this,tr("Montar Cardápio"),tr("Favor selecionar um menu!"));
        qDebug() << "saiu";
        return;
    }

    QModelIndexList indexList = ui->tableView_menu->selectionModel()->selectedIndexes();
    int id_cardapio, row;

    querycardapio->prepare("delete from cardapio where id_cardapio=:id_cardapio");
    QVariantList listcardapio;

    foreach (QModelIndex index, indexList) {
        row = index.row();
        qDebug() << row << index.column();
        id_cardapio=modalcardapio2->record(row).value(0).toInt();
        listcardapio << id_cardapio;
    }
    querycardapio->bindValue(":id_cardapio", listcardapio);

    querycardapio->execBatch();

    qDebug() << querycardapio->lastError().text();

    lercardapio();
    ui->tableView_menu->setModel(modalcardapio2);
    ui->tableView_menu->setColumnHidden(0,true);


}

void Cardapio::on_pushButton_filtrar_cardapio_clicked()
{
    filtrarcardapio=ui->lineEdit_filtrar_cardapio->text();

    lercardapio();
    ui->tableView_menu->setModel(modalcardapio2);
    ui->tableView_menu->setColumnHidden(0,true);

}
