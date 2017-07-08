#include "ligarcardapio.h"
#include "ui_ligarcardapio.h"
#include "qmessagebox.h"

LigarCardapio::LigarCardapio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LigarCardapio)
{
    ui->setupUi(this);
    conn.connOpen();

    querycardapio=new QSqlQuery(conn.mydb);
    modalcardapio=new QSqlQueryModel();
    querycardapio_off=new QSqlQuery(conn.mydb);
    modalcardapio_off=new QSqlQueryModel();

    lercardapio();

    ui->tableView_on->setModel(modalcardapio);
    ui->tableView_off->setModel(modalcardapio_off);
    ui->tableView_on->hideColumn(0);
    ui->tableView_on->hideColumn(5);
    ui->tableView_on->hideColumn(6);
    ui->tableView_on->hideColumn(7);
    ui->tableView_off->hideColumn(0);
    ui->tableView_off->hideColumn(5);
    ui->tableView_off->hideColumn(6);
    ui->tableView_off->hideColumn(7);

    ui->tableView_on->resizeColumnsToContents();
    ui->tableView_off->resizeColumnsToContents();
}

LigarCardapio::~LigarCardapio()
{
    delete ui;

    conn.connClose();
}

void LigarCardapio::on_tableView_on_activated(const QModelIndex &index)
{
    if(modalcardapio->rowCount()==0)return;
    qDebug() << "tableView_cardapioon_activated";

    QString celula=ui->tableView_on->model()->data(index).toString();

    int colunas=ui->tableView_on->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

}

void LigarCardapio::on_pushButton_desligar_clicked()
{
    QModelIndexList indexList = ui->tableView_on->selectionModel()->selectedIndexes();

    QString str, idnovo;
    int anterior, row, id;
    QVariantList list_id;


    anterior=-1;
    foreach (QModelIndex index, indexList) {
        row=index.row();
        if(anterior!=row){
            id=modalcardapio->record(row).value(0).toInt();
            list_id << id;
            qDebug() << row<< id;
        }
        anterior=row;
    }
    querycardapio->prepare("update cardapio set ativo=0 \
                    where id_cardapio=:id_cardapio ");
    querycardapio->bindValue(":id_cardapio", list_id);
    if(!querycardapio->execBatch()){
        QMessageBox::critical(this,tr("Erro"),tr("Erro na gravação de dados!"));
    }
    else{
        lercardapio();
        ui->tableView_on->setModel(modalcardapio);
        ui->tableView_off->setModel(modalcardapio_off);
        ui->tableView_on->resizeColumnsToContents();
        ui->tableView_off->resizeColumnsToContents();
    }

}

void LigarCardapio::on_pushButton_ligar_clicked()
{
    QModelIndexList indexList = ui->tableView_off->selectionModel()->selectedIndexes();

    QString str, idnovo;
    int anterior, row, id;
    QVariantList list_id;

    anterior=-1;
    foreach (QModelIndex index, indexList) {
        row=index.row();
        if(anterior!=row){
            id=modalcardapio_off->record(row).value(0).toInt();
            list_id << id;
            qDebug() << row<< id;
        }
        anterior=row;
    }
    querycardapio_off->prepare("update cardapio set ativo=1 \
                    where id_cardapio=:id_cardapio ");
    querycardapio_off->bindValue(":id_cardapio", list_id);
    if(!querycardapio_off->execBatch()){
        QMessageBox::critical(this,tr("Erro"),tr("Erro na gravação de dados!"));
    }
    else{
        lercardapio();
        ui->tableView_on->setModel(modalcardapio);
        ui->tableView_off->setModel(modalcardapio_off);
        ui->tableView_on->resizeColumnsToContents();
        ui->tableView_off->resizeColumnsToContents();
    }

}
