#include "produto.h"
#include "ui_produto.h"
#include "qmessagebox.h"

Produto::Produto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Produto)
{
    ui->setupUi(this);

    conn.connOpen();

    modaltipo=new QSqlQueryModel();
    querytipo=new QSqlQuery(conn.mydb);

    letipo();
    ui->listView_tipo->setModel(modaltipo);

    modaltamanho=new QSqlQueryModel();
    querytamanho=new QSqlQuery(conn.mydb);
    lertamanho();
    modaltamanho->setHeaderData(1, Qt::Horizontal, tr("Descrição"));
    modaltamanho->setHeaderData(2, Qt::Horizontal, tr("Seleções permitidas"));

    ui->tableView_tamanho->setModel(modaltamanho);
    ui->tableView_tamanho->resizeColumnsToContents();
    ui->tableView_tamanho->setColumnHidden(0,true);

    modalitem=new QSqlQueryModel();
    queryitem=new QSqlQuery(conn.mydb);
    leitens();

    modalitem->setHeaderData(1, Qt::Horizontal, tr("ID"));
    modalitem->setHeaderData(2, Qt::Horizontal, tr("Item"));
    modalitem->setHeaderData(3, Qt::Horizontal, tr("Descrição"));

    ui->tableView_itens->setModel(modalitem);
    ui->tableView_itens->resizeColumnsToContents();
    ui->tableView_itens->setColumnHidden(0,true);

    qDebug() << "construtor";
}

Produto::~Produto()
{
    delete ui;
    conn.close();
    qDebug() << "destrutor";

}

void Produto::on_pushButton_inserirtipo_clicked()
{
    QString tipostr=ui->lineEdit_tipo->text();

    querytipo->prepare("insert into tipo (tipo) values (:tipostr)");

    querytipo->bindValue(":tipostr",tipostr);
    querytipo->exec();
    letipo();
    ui->lineEdit_tipo->setText("");
}

void Produto::on_pushButton_apagartipo_clicked()
{
    querytipo->prepare("DELETE FROM tipo where id_tipo=:id_tipo");
    querytipo->bindValue(":id_tipo", id_tipo);
    if(id_tipo){
        if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "Apagar Tipo!", "Confirma?", QMessageBox::Yes|QMessageBox::No).exec())
        {
            if(querytipo->exec()){
                QMessageBox::critical(this,tr("Apagar Tipo"),tr("Tipo apagado com sucesso!"));
                id_tipo=0;
                querytipo->prepare("select * from tipo");
                querytipo->exec();

                letipo();

                ui->lineEdit_tipo->setText("");
            }
            else{
                QMessageBox::critical(this,tr("error::"),querytipo->lastError().text());
            }
        }
    }
    else{
        if(!id_tipo)
            QMessageBox::critical(this,tr("Apagar Tipo"),tr("Favor selecionar um tipo!"));
    }

}

void Produto::on_pushButton_instamanho_clicked()
{

    qDebug() << "pushButton_instamanho";

    QString tamanhostr, selstr;

    tamanhostr=ui->lineEdit_desctamanho->text();
    selstr=ui->lineEdit_seltamanho->text();

    if(selstr=="")
        selstr="1";

    querytamanho->prepare("insert into tamanho (tamanho, selecao) values (:tamanhostr, :selstr)");
 /*
    else{
        querytamanho->prepare("update tamanho set tamanho=:ttamanhostr where id_tamanho=:id_tamanho");
        querytamanho->bindValue(":id_tamanho",id_tamanho);
    }
*/
    querytamanho->bindValue(":tamanhostr",tamanhostr);
    querytamanho->bindValue(":selstr",selstr);
    querytamanho->exec();
    lertamanho();
    ui->tableView_tamanho->setModel(modaltamanho);
    ui->tableView_tamanho->setColumnHidden(0,true);
    ui->lineEdit_desctamanho->setText("");
    ui->lineEdit_seltamanho->setText("");
}

void Produto::on_tableView_tamanho_activated(const QModelIndex &index)
{
    qDebug() << "tableView_tamanho_activated";

    QString celula=ui->tableView_tamanho->model()->data(index).toString();
    int colunas=ui->tableView_tamanho->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;


    id_tamanho=modaltamanho->record(linha).value(0).toInt();

   // qDebug() << "contador " << contador;
    ui->lineEdit_desctamanho->setText(modaltamanho->record(linha).value(1).toString());
    ui->lineEdit_seltamanho->setText(modaltamanho->record(linha).value(2).toString());

    //id_tamanho=querytamanho->value(0).toInt();

}

void Produto::on_pushButton_tamanho_atualizar_clicked()
{

    qDebug() << "pushButton_atualizaramanho";

    QString tamanhostr, selstr;

    tamanhostr=ui->lineEdit_desctamanho->text();
    selstr=ui->lineEdit_seltamanho->text();

    if(selstr=="")
        selstr="1";

    if(id_tamanho){
        querytamanho->prepare("update tamanho set tamanho=:tamanhostr, selecao=:selstr where id_tamanho=:id_tamanhostr");
        querytamanho->bindValue(":id_tamanhostr",id_tamanho);
        querytamanho->bindValue(":tamanhostr",tamanhostr);
        querytamanho->bindValue(":selstr",selstr);
        querytamanho->exec();
        qDebug() << querytamanho->lastError().text();

        lertamanho();
        ui->tableView_tamanho->setModel(modaltamanho);
        ui->tableView_tamanho->setColumnHidden(0,true);

        ui->lineEdit_desctamanho->setText("");
        ui->lineEdit_seltamanho->setText("");
        id_tamanho=0;

    }
    else{
        if(!id_tamanho)
            QMessageBox::critical(this,tr("Atualizar Tamanho"),tr("Favor selecionar um tamanho!"));

    }

}

void Produto::on_pushButton_del_tamanho_clicked()
{
    querytamanho->prepare("DELETE FROM tamanho where id_tamanho=:id_tamanho");
    querytamanho->bindValue(":id_tamanho", id_tamanho);
    if(id_tamanho){
        if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "Apagar Tamanho!", "Confirma?", QMessageBox::Yes|QMessageBox::No).exec())
        {
            if(querytamanho->exec()){
                QMessageBox::critical(this,tr("Apagar"),tr("Tamanho apagado com sucesso!"));
                lertamanho();
                ui->tableView_tamanho->setModel(modaltamanho);
                ui->lineEdit_desctamanho->setText("");
                ui->lineEdit_seltamanho->setText("");
                id_tamanho=0;
            }
            else{
                QMessageBox::critical(this,tr("error::"),querytamanho->lastError().text());
            }
        }
    }
    else{
        if(!id_tamanho)
            QMessageBox::critical(this,tr("Apagar Tipo"),tr("Favor selecionar um tipo!"));
    }

}

void Produto::on_pushButton_atualizatipo_clicked()
{
    QString tipostr=ui->lineEdit_tipo->text();

    if(id_tipo){
        querytipo->prepare("update tipo set tipo=:tipostr where id_tipo=:id_tipo");
        querytipo->bindValue(":id_tipo",id_tipo);

        querytipo->bindValue(":tipostr",tipostr);
        querytipo->exec();
        letipo();
        //ui->tableView_tipo->setModel(modaltipo);
       // ui->tableView_tipo->setColumnHidden(0,true);

        ui->lineEdit_tipo->setText("");
        id_tipo=0;
    }
    else{
       QMessageBox::critical(this,tr("Atualizar Tipo"),tr("Favor selecionar um tipo!"));
    }

}

void Produto::on_pushButton_procuraritem_clicked()
{
    QString procurar=ui->lineEdit_procurar->text();

   //queryitem->prepare("select * from item WHERE (item.id = :id_edit ) or (item.item_nome like :nome_edit ) or (item.item_descricao like :descricao_edit )");
    if(procurar!=""){
        queryitem->prepare("select * from item WHERE (item.id = :procurar ) or (item.item_nome like :procurar ) or (item.item_descricao like :procurar )");
        queryitem->bindValue( ":procurar", procurar );
    }
    else
        queryitem->prepare("select * from item");

    queryitem->exec();
    modalitem->setQuery(*queryitem);
    ui->tableView_itens->setModel(modalitem);
    ui->tableView_itens->setColumnHidden(0,true);

    qDebug() << queryitem->lastError().text();

}

void Produto::on_pushButton_novoitem_clicked()
{
    QString id_edit, nome_edit, descricao_edit;

    id_edit=ui->lineEdit_iditem->text();
    nome_edit=ui->lineEdit_nomeitem->text();
    descricao_edit=ui->lineEdit_descricaoitem->text();

    qDebug()<<id_edit <<" "<< nome_edit << " " << descricao_edit;
    queryitem->prepare("insert into item (id,item_nome,item_descricao) values (:id, :nome, :descricao)");

    queryitem->bindValue( ":id", id_edit );
    queryitem->bindValue( ":nome", nome_edit );
    queryitem->bindValue( ":descricao", descricao_edit );
    queryitem->exec();
    modalitem->setQuery(*queryitem);
    ui->tableView_itens->setModel(modalitem);
    ui->tableView_itens->setColumnHidden(0,true);

    qDebug() << queryitem->lastError().text();
    queryitem->prepare("select * from item");
    queryitem->exec();
    modalitem->setQuery(*queryitem);
    ui->tableView_itens->setModel(modalitem);
    ui->tableView_itens->setColumnHidden(0,true);

    ui->lineEdit_iditem->setText("");
    ui->lineEdit_nomeitem->setText("");
    ui->lineEdit_descricaoitem->setText("");

}

void Produto::on_tableView_itens_activated(const QModelIndex &index)
{
    qDebug() << "tableView_itens_activated";

    QString celula=ui->tableView_itens->model()->data(index).toString();
    int colunas=ui->tableView_itens->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;
/*
    queryitem->first();//primeiro registro da query
    int contador=0;
    while(contador<linha){
        queryitem->next();
        contador++;
    }
  */
    id_item=modalitem->record(linha).value(0).toInt();

   // qDebug() << "contador " << contador;
    ui->lineEdit_iditem->setText(queryitem->value(1).toString());
    ui->lineEdit_nomeitem->setText(queryitem->value(2).toString());
    ui->lineEdit_descricaoitem->setText(queryitem->value(3).toString());

    //id_item=queryitem->value(0).toInt();

}

void Produto::on_pushButton_atualizaritem_clicked()
{
    QString id_edit, nome_edit, descricao_edit;

    id_edit=ui->lineEdit_iditem->text();
    nome_edit=ui->lineEdit_nomeitem->text();
    descricao_edit=ui->lineEdit_descricaoitem->text();

    qDebug()<<id_edit <<" "<< nome_edit << " " << descricao_edit << id_item;

    if(id_item){
        queryitem->prepare("update item set id=:id_edit, item_nome=:nome_edit, item_descricao=:descricao_edit where id_item=:id_item");

        queryitem->bindValue( ":id_edit", id_edit );
        queryitem->bindValue( ":nome_edit", nome_edit );
        queryitem->bindValue( ":descricao_edit", descricao_edit );
        queryitem->bindValue( ":id_item", id_item );

        queryitem->exec();
        qDebug() << "erro: " << queryitem->lastError().text();

        queryitem->prepare("select * from item");
        queryitem->exec();
        modalitem->setQuery(*queryitem);
        ui->tableView_itens->setModel(modalitem);
        ui->tableView_itens->setColumnHidden(0,true);

        ui->lineEdit_iditem->setText("");
        ui->lineEdit_iditem->setText("");
        ui->lineEdit_nomeitem->setText("");
        ui->lineEdit_descricaoitem->setText("");
        id_item=0;
    }
    else{
       QMessageBox::critical(this,tr("Atualizar Itens"),tr("Favor selecionar um item!"));
    }

}

void Produto::on_pushButton_apagaritem_clicked()
{
    if(!modalitem->rowCount())
        return;

    QModelIndexList indexList = ui->tableView_itens->selectionModel()->selectedIndexes();
    int iditem, row;


    if (QMessageBox::No == QMessageBox(QMessageBox::Information, "Apagar Tamanho!", "Confirma?", QMessageBox::Yes|QMessageBox::No).exec()){
        return;
    }

    queryitem->prepare("delete from item where id_item=:id_item");
    QVariantList listItens;

    foreach (QModelIndex index, indexList) {
        row = index.row();
        qDebug() << row << index.column();
        iditem=modalitem->record(row).value(0).toInt();
        listItens << iditem;
    }
    queryitem->bindValue(":id_item", listItens);

    queryitem->execBatch();

    leitens();

    ui->tableView_itens->setModel(modalitem);
    //ui->tableView_menu->setColumnHidden(0,true);

    ui->lineEdit_iditem->setText("");
    ui->lineEdit_nomeitem->setText("");
    ui->lineEdit_descricaoitem->setText("");

}

void Produto::on_listView_tipo_activated(const QModelIndex &index)
{
    qDebug() << "tableView_tipo_activated";

    QString celula=ui->listView_tipo->model()->data(index).toString();
    int colunas=ui->listView_tipo->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;
/*
    querytipo->first();//primeiro registro da query
    int contador=0;
    while(contador<linha){
        querytipo->next();
        contador++;
    }
  */
    id_tipo=modaltipo->record(linha).value(1).toInt();

   // qDebug() << "contador " << contador;
    ui->lineEdit_tipo->setText(querytipo->value(0).toString());

    //id_tipo=querytipo->value(0).toInt();

}
