#include "preco.h"
#include "ui_preco.h"
#include "qmessagebox.h"

Preco::Preco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preco)
{
    ui->setupUi(this);

    conn.connOpen();

    modalitem=new QSqlQueryModel();
    queryitem=new QSqlQuery(conn.mydb);
    leritens();
    modalitem->setHeaderData(1, Qt::Horizontal, tr("ID"));
    modalitem->setHeaderData(2, Qt::Horizontal, tr("Item"));
    modalitem->setHeaderData(3, Qt::Horizontal, tr("Descrição"));
    ui->tableView_itens->setModel(modalitem);
    ui->tableView_itens->setColumnHidden(0,true);
    ui->tableView_itens->resizeColumnsToContents();
    filtraritem="";

    modaltipo=new QSqlQueryModel();
    querytipo=new QSqlQuery(conn.mydb);
    lertipo();
    ui->comboBox_tipoitem->setModel(modaltipo);
    id_tipo=modaltipo->record(0).value(1).toInt();

    modaltamanho=new QSqlQueryModel();
    querytamanho=new QSqlQuery(conn.mydb);
    lertamanho();
    ui->comboBox_tamanhoitem->setModel(modaltamanho);
    id_tamanho=modaltamanho->record(0).value(1).toInt();

    querypreco=new QSqlQuery(conn.mydb);
    modelpreco2=new QSqlQueryModel();
    lerprecos();

    modelpreco2->setHeaderData(4, Qt::Horizontal, tr("ID"));
    modelpreco2->setHeaderData(5, Qt::Horizontal, tr("Item"));
    modelpreco2->setHeaderData(6, Qt::Horizontal, tr("Tipo"));
    modelpreco2->setHeaderData(7, Qt::Horizontal, tr("Tamanho"));
    modelpreco2->setHeaderData(8, Qt::Horizontal, tr("Preço"));

    ui->tableView_precos->setModel(modelpreco2);
    ui->tableView_precos->setItemDelegateForColumn(8, new NumberFormatDelegate(this));
    ui->tableView_precos->setColumnHidden(0,true);
    ui->tableView_precos->setColumnHidden(1,true);
    ui->tableView_precos->setColumnHidden(2,true);
    ui->tableView_precos->setColumnHidden(3,true);
    ui->tableView_precos->resizeColumnsToContents();
    filtrarpreco="";

}

Preco::~Preco()
{
    delete ui;

    conn.close();
}

void Preco::on_pushButton_fechar_clicked()
{
    close();
}

void Preco::on_comboBox_tipoitem_activated(const QString &arg1)
{

    qDebug() << "comboBox_tipoitem" << arg1;
    querytipo->first();
    while(arg1!=querytipo->value(0).toString())
        querytipo->next();

    id_tipo=querytipo->value(1).toInt();

}

void Preco::on_comboBox_tamanhoitem_activated(const QString &arg1)
{
    querytamanho->first();
    while(arg1!=querytamanho->value(0).toString())
        querytamanho->next();

    id_tamanho=querytamanho->value(1).toInt();
}


void Preco::on_pushButton_inserir_clicked()
{
    qDebug() << "inserir_clicked";

    bool ok;
    int i;
    QString precoitem=ui->lineEdit_preco->text();
    QString id=ui->lineEdit_id->text();
    float fpreco=precoitem.toFloat(&ok);

    qDebug()<<precoitem << id_item << id_tamanho << id_tipo << ok;

    if(ok && ((id_item && id_tamanho && id_tipo) or id_preco )){
/*
        ok=true;
        for(i=0;i<modelpreco2->rowCount();i++)
            if(modelpreco2->record(i).value(1).toInt()==id_item &&
               modelpreco2->record(i).value(2).toInt()==id_tipo &&
               modelpreco2->record(i).value(3).toInt()==id_tamanho)
                ok=false; // preco ja inserido
  */
        if(ok){
            if(!id_preco)
                querypreco->prepare("insert into preco (id_item, id_tipo, id_tamanho, id, preco) values (:id_item,:id_tipo,:id_tamanho,:id, :preco)");
            else{
                querypreco->prepare("update preco set id_tipo=:id_tipo, id_tamanho=:id_tamanho, preco=:preco, id=:id where id_preco=:id_preco");
                querypreco->bindValue(":id_preco",id_preco);
            }

            querypreco->bindValue(":id",id);
            querypreco->bindValue(":id_item",id_item);
            querypreco->bindValue(":id_tipo",id_tipo);
            querypreco->bindValue(":id_tamanho",id_tamanho);
            querypreco->bindValue(":preco",precoitem);

            if(querypreco->exec()){
                if(!id_preco)
                    QMessageBox::critical(this,tr("Inserir preço"),tr("Preço inserido com sucesso!"));
                else
                    QMessageBox::critical(this,tr("Alterar preço"),tr("Preço alterado com sucesso!"));
                ui->lineEdit_preco->setText("");
                ui->label_item->setText("[ Selecione um item ]");

                //modelpreco->select();
                lerprecos();
                ui->tableView_precos->setModel(modelpreco2);
                id_preco=0;
                ui->tableView_precos->setItemDelegateForColumn(8, new NumberFormatDelegate(this));
                ui->tableView_precos->setColumnHidden(0,true);
                ui->tableView_precos->setColumnHidden(1,true);
                ui->tableView_precos->setColumnHidden(2,true);
                ui->tableView_precos->setColumnHidden(3,true);
                ui->tableView_precos->resizeColumnsToContents();

                ui->lineEdit_id->setText("");;
                ui->lineEdit_preco->setText("");
            }
            else{
                QMessageBox::critical(this,tr("Inserir Preço"),querypreco->lastError().text());
            }
            qDebug() << "inserir preco "<< querypreco->lastError().text();
        }
    }
    else{
        if(!ok){
            QMessageBox::critical(this,tr("Inserir Preço"),tr("Valor inválido"));
        }
        if(!id_item){
            QMessageBox::critical(this,tr("Inserir Preço"),tr("Favor selecionar um item"));
        }
    }

}

void Preco::on_tableView_itens_activated(const QModelIndex &index)
{

    QString celula=ui->tableView_itens->model()->data(index).toString();
    int colunas=ui->tableView_itens->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();


    id_item=modalitem->record(linha).value(0).toInt();
    qDebug() << celula << colunas << linha << coluna << id_item;

    ui->label_item->setText(queryitem->value(2).toString());
    ui->lineEdit_id->setText("");
}

void Preco::on_tableView_precos_activated(const QModelIndex &index)
{
    QString celula=ui->tableView_precos->model()->data(index).toString();
    int colunas=ui->tableView_precos->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula << colunas << linha << coluna;

    ui->lineEdit_id->setText(modelpreco2->record(linha).value(4).toString());
    ui->label_item->setText(modelpreco2->record(linha).value(5).toString());
    ui->lineEdit_preco->setText(modelpreco2->record(linha).value(8).toString());

    int contador=0;

    qDebug()<<ui->comboBox_tipoitem->itemText(contador);
    while(modelpreco2->record(linha).value(6).toString()!=ui->comboBox_tipoitem->itemText(contador))
        contador++;
    ui->comboBox_tipoitem->setCurrentIndex(contador);

    contador=0;
    while(modelpreco2->record(linha).value(7).toString()!=ui->comboBox_tamanhoitem->itemText(contador))
        contador++;
    ui->comboBox_tamanhoitem->setCurrentIndex(contador);

    id_preco=modelpreco2->record(linha).value(0).toInt();

}

void Preco::on_pushButton_apagar_clicked()
{
    QModelIndexList indexList = ui->tableView_precos->selectionModel()->selectedIndexes();
    int idpreco, row;
    QVariantList listprecos;

    if (QMessageBox::No == QMessageBox(QMessageBox::Information, "Apagar Item com Preço!", "Confirma?", QMessageBox::Yes|QMessageBox::No).exec()){
        return;
    }

    foreach (QModelIndex index, indexList) {
        row = index.row();
        qDebug() << row << index.column();
        idpreco=modelpreco2->record(row).value(0).toInt();
        listprecos << idpreco;
    }
    querypreco->prepare("DELETE FROM preco where id_preco=:id_preco");
    querypreco->bindValue(":id_preco", listprecos);
    if(querypreco->execBatch()){
        QMessageBox::critical(this,tr("Apagar"),tr("Preço apagado com sucesso!"));

        lerprecos();

        ui->tableView_precos->setModel(modelpreco2);

        id_preco=0;
        ui->lineEdit_id->setText("");;
        ui->lineEdit_preco->setText("");
        ui->comboBox_tipoitem->setCurrentIndex(0);
        ui->comboBox_tamanhoitem->setCurrentIndex(0);

    }
    else{
        QMessageBox::critical(this,tr("error::"),queryitem->lastError().text());
    }
}

void Preco::on_pushButton_filtraritens_clicked()
{
    filtraritem=ui->lineEdit_filtraritens->text();
    leritens();
    ui->tableView_itens->setModel(modalitem);

}

void Preco::on_pushButton_filtrarpreco_clicked()
{
    filtrarpreco=ui->lineEdit_filtrarpreco->text();
    lerprecos();
    ui->tableView_precos->setModel(modelpreco2);

}
