#include "submenu.h"
#include "ui_submenu.h"
#include "qmessagebox.h"

Submenu::Submenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Submenu)
{
    ui->setupUi(this);
    conn.connOpen();

    modalmenu=new QSqlQueryModel();
    querymenu=new QSqlQuery(conn.mydb);

    lemenu();

    ui->comboBox_menu->setModel(modalmenu);

    modalcardapio=new QSqlQueryModel();
    querycardapio=new QSqlQuery(conn.mydb);

    filtrar="";

    lecardapio();

    modalcardapio->setHeaderData(1, Qt::Horizontal, tr("Item Menu"));
    modalcardapio->setHeaderData(2, Qt::Horizontal, tr("Descrição do Item"));
    modalcardapio->setHeaderData(3, Qt::Horizontal, tr("Tipo"));
    modalcardapio->setHeaderData(5, Qt::Horizontal, tr("Tamanho"));

    ui->tableView_itens->setModel(modalcardapio);
    ui->tableView_itens->resizeColumnsToContents();
    ui->tableView_itens->setColumnHidden(0,true);
    ui->tableView_itens->setColumnHidden(4,true);

    modelsub=new QSqlQueryModel();
    querysubmenu=new QSqlQuery(conn.mydb);
    lesubmenu();
    ui->listView_submenu->setModel(modelsub);

    modelsubmenuitens=new QSqlQueryModel();
    querysubmenuitens=new QSqlQuery(conn.mydb);
    lesubmenuitens();

    modelsubmenuitens->setHeaderData(1, Qt::Horizontal, tr("Item Menu"));
    modelsubmenuitens->setHeaderData(2, Qt::Horizontal, tr("Tipo"));
    modelsubmenuitens->setHeaderData(3, Qt::Horizontal, tr("Tamanho"));
    modelsubmenuitens->setHeaderData(4, Qt::Horizontal, tr("Sub-Menu"));

    ui->tableView_submenuitens->setModel(modelsubmenuitens);
    ui->tableView_submenuitens->setColumnHidden(0,true);
    ui->tableView_submenuitens->resizeColumnsToContents();
    filtrarsubmenuitens="";

    modeltipo=new QSqlQueryModel();
    querytipo=new QSqlQuery(conn.mydb);
    lertipo();
    ui->comboBox_tipo->addItem("Tipos", "" );
    for(int i=0;i<modeltipo->rowCount();i++){
        ui->comboBox_tipo->addItem(modeltipo->record(i).value(0).toString(), modeltipo->record(i).value(0).toString() );
    }

    modaltamanho=new QSqlQueryModel();
    querytamanho=new QSqlQuery(conn.mydb);
    lertamanho();
    ui->comboBox_tamanho->addItem("Tamanhos", "" );
    for(int i=0;i<modaltamanho->rowCount();i++){
        ui->comboBox_tamanho->addItem(modaltamanho->record(i).value(0).toString(), modaltamanho->record(i).value(0).toString());
    }
}

Submenu::~Submenu()
{
    delete ui;
    conn.close();
}

void Submenu::on_pushButton_inserirsubmenu_clicked()
{

    int rowitem, rowsub, iditem, idsubmenu, i, nrelacoes;
    QVariantList listitem, listsubmenu;
    bool insererelacao;

    QSqlQueryModel *modelrelacaoitens;
    QSqlQuery *queryrelacaoitens;

    modelrelacaoitens=new QSqlQueryModel();
    queryrelacaoitens=new QSqlQuery(conn.mydb);

    qDebug() << "item";

    QModelIndexList indexListitens = ui->tableView_itens->selectionModel()->selectedIndexes();
    QModelIndexList indexListsubmenu = ui->listView_submenu->selectionModel()->selectedIndexes();

    querysubmenu->prepare("INSERT INTO submenu (id_cardapio, id_menu) VALUES(:id_cardapio, :id_menu)");

  //  nrelacoes=modelrelacaoitens->rowCount();
    qDebug() << "item";

    foreach (QModelIndex indexItem, indexListitens) {
        rowitem = indexItem.row();
        iditem=modalcardapio->record(rowitem).value(0).toInt();

        queryrelacaoitens->prepare("select id_item, id_menu from submenu where id_item=:iditem");
        queryrelacaoitens->bindValue(":iditem", iditem);
        queryrelacaoitens->exec();
        modelrelacaoitens->setQuery(*queryrelacaoitens);

        qDebug() << "item" << rowitem << indexItem.column() << iditem;

        insererelacao=true;

        foreach (QModelIndex indexSubmenu, indexListsubmenu) {
          rowsub = indexSubmenu.row();
          idsubmenu=modelsub->record(rowsub).value(1).toInt();

          for(i=0;i<nrelacoes;i++){
             if(modelrelacaoitens->record(i).value(0).toInt()==iditem && modelrelacaoitens->record(i).value(1).toInt()==idsubmenu){
                insererelacao=false;
                break;
             }
          }

          if(insererelacao){
                listitem << iditem;
                listsubmenu << idsubmenu;
            }
            qDebug() << "submenu" << rowitem << indexSubmenu.column() << idsubmenu;
        }
    }
    querysubmenu->bindValue(":id_cardapio",listitem);
    querysubmenu->bindValue(":id_menu",listsubmenu);

    if (!querysubmenu->execBatch()) {
        qDebug() << "erro " << querysubmenu->lastError();
    } else {
        qDebug() << "sucesso " << querysubmenu->executedQuery();

        lesubmenuitens();
        ui->tableView_submenuitens->setModel(modelsubmenuitens);
    }

}

void Submenu::on_comboBox_menu_activated(const QString &arg1)
{
    int i, nlinhas;
    QString str;

    qDebug() << "comboBox_menu" << arg1;

    nlinhas=modalmenu->rowCount();

    for(i=0;i<nlinhas; i++){
        str=modalmenu->record(i).value(0).toString();
        qDebug()<< i<< str;
        if(str==arg1){
            id_menu=modalmenu->record(i).value(1).toInt();
            qDebug() << "idmenu" << id_menu;

        }
    }
    qDebug()<< nlinhas << id_menu;

    lecardapio();

    ui->tableView_itens->setModel(modalcardapio);
    ui->tableView_itens->setColumnHidden(0,true);
    ui->tableView_itens->setColumnHidden(4,true);
    ui->tableView_itens->resizeColumnsToContents();

}

void Submenu::on_pushButton_filtrar_clicked()
{
    filtrar=ui->lineEdit_filtrar->text();
    qDebug() << filtrar;
    lecardapio();

    ui->tableView_itens->setModel(modalcardapio);
    ui->tableView_itens->setColumnHidden(0,true);
    ui->tableView_itens->setColumnHidden(4,true);

}

void Submenu::on_pushButton_filtrarsubmenuitens_clicked()
{
    filtrarsubmenuitens=ui->lineEdit_filtrarsubmenuitens->text();
    qDebug() << filtrarsubmenuitens;
    lesubmenuitens();


}

void Submenu::on_pushButton_apagarsubmenuitens_clicked()
{
    /*
    if(!id_menu){
        QMessageBox::critical(this,tr("Montar Cardápio"),tr("Favor selecionar um menu!"));
        qDebug() << "saiu";
        return;
    }
*/
    QModelIndexList indexList = ui->tableView_submenuitens->selectionModel()->selectedIndexes();
    int id_submenuitens, row;

    querysubmenuitens->prepare("delete from submenu where id_submenu=:id_submenu");
    QVariantList listsubmenu;

    foreach (QModelIndex index, indexList) {
        row = index.row();
        qDebug() << row << index.column();
        id_submenuitens=modelsubmenuitens->record(row).value(0).toInt();
        listsubmenu << id_submenuitens;
    }
    querysubmenuitens->bindValue(":id_submenu", listsubmenu);

    querysubmenuitens->execBatch();

    lesubmenuitens();

    ui->tableView_submenuitens->setModel(modelsubmenuitens);
    //ui->tableView_menu->setColumnHidden(0,true);


}


void Submenu::on_comboBox_tipo_activated(const QString &arg1)
{
    filtro=ui->comboBox_tipo->currentData().toString();

    lesubmenuitens();

}

void Submenu::on_comboBox_tamanho_activated(const QString &arg1)
{
    tamanho=ui->comboBox_tamanho->currentData().toString();
    lesubmenuitens();
}
