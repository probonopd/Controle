#include "precoentrega.h"
#include "ui_precoentrega.h"
#include "qmessagebox.h"

PrecoEntrega::PrecoEntrega(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrecoEntrega)
{
    ui->setupUi(this);

    conn.connOpen();

    modalentrega=new QSqlQueryModel();
    queryentrega=new QSqlQuery(conn.mydb);
    leentrega(filtro);
    modalentrega->setHeaderData(3, Qt::Horizontal, tr("Bairro"));
    modalentrega->setHeaderData(4, Qt::Horizontal, tr("Cidade"));
    modalentrega->setHeaderData(5, Qt::Horizontal, tr("Valor (R$)"));
    modalentrega->setHeaderData(6, Qt::Horizontal, tr("%"));
    modalentrega->setHeaderData(7, Qt::Horizontal, tr("Taxa Extra (R$)"));

    ui->tableView_entrega->setModel(modalentrega);
    ui->tableView_entrega->setColumnHidden(0,true);
    ui->tableView_entrega->setColumnHidden(1,true);
    ui->tableView_entrega->setColumnHidden(2,true);
    ui->tableView_entrega->resizeColumnsToContents();
    ui->tableView_entrega->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
    ui->tableView_entrega->setItemDelegateForColumn(7, new NumberFormatDelegate(this));

}

PrecoEntrega::~PrecoEntrega()
{
    delete ui;
}

void PrecoEntrega::on_pushButton_procurar_clicked()
{
    QString procura=ui->lineEdit_procurabairro->text();
    qDebug() << "on_pushButton_procurarcliente";

    qDebug() << procura;

    leentrega(procura);

    modalentrega->setHeaderData(3, Qt::Horizontal, tr("Bairro"));
    modalentrega->setHeaderData(4, Qt::Horizontal, tr("Cidade"));
    modalentrega->setHeaderData(5, Qt::Horizontal, tr("Valor (R$)"));
    modalentrega->setHeaderData(6, Qt::Horizontal, tr("%"));
    modalentrega->setHeaderData(7, Qt::Horizontal, tr("Taxa Extra (R$)"));

    ui->tableView_entrega->setModel(modalentrega);
    ui->tableView_entrega->setColumnHidden(0,true);
    ui->tableView_entrega->setColumnHidden(1,true);
    ui->tableView_entrega->setColumnHidden(2,true);
    ui->tableView_entrega->resizeColumnsToContents();
    ui->tableView_entrega->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
    ui->tableView_entrega->setItemDelegateForColumn(7, new NumberFormatDelegate(this));

}

void PrecoEntrega::on_tableView_entrega_activated(const QModelIndex &index)
{
    QString celula=ui->tableView_entrega->model()->data(index).toString();
    qDebug() << celula;
    int linha=index.row();

    id_entrega=modalentrega->record(linha).value(0).toInt();

    ui->label_bairro->setText(modalentrega->record(linha).value(3).toString());
    ui->label_cidade->setText(modalentrega->record(linha).value(4).toString());
    ui->lineEdit_valor->setText(modalentrega->record(linha).value(5).toString());
    ui->lineEdit_percentual->setText(modalentrega->record(linha).value(6).toString());
    ui->lineEdit_extra->setText(modalentrega->record(linha).value(7).toString());
}

void PrecoEntrega::on_pushButton_gravar_clicked()
{
    QString procura=ui->lineEdit_procurabairro->text();
    queryentrega->prepare("update entrega set valor=:valor, percentual=:percentual, extra=:extra where id_entrega=:id_entrega ");
    queryentrega->bindValue(":id_entrega",id_entrega);
    queryentrega->bindValue(":valor",ui->lineEdit_valor->text());
    queryentrega->bindValue(":percentual",ui->lineEdit_percentual->text());
    queryentrega->bindValue(":extra",ui->lineEdit_extra->text());

    if(!queryentrega->exec()){
        QMessageBox::critical(this,tr("Inserir Valor de Entrega"),tr("Erro na gravação de dados!"));
    }
    else{
        ui->label_bairro->setText("");
        ui->label_cidade->setText("");
        ui->lineEdit_valor->setText("");
        ui->lineEdit_percentual->setText("");
        ui->lineEdit_extra->setText("");
        id_entrega=0;
        leentrega(procura);
        modalentrega->setHeaderData(3, Qt::Horizontal, tr("Bairro"));
        modalentrega->setHeaderData(4, Qt::Horizontal, tr("Cidade"));
        modalentrega->setHeaderData(5, Qt::Horizontal, tr("Valor (R$)"));
        modalentrega->setHeaderData(6, Qt::Horizontal, tr("%"));
        modalentrega->setHeaderData(7, Qt::Horizontal, tr("Taxa Extra (R$)"));

        ui->tableView_entrega->setModel(modalentrega);
        ui->tableView_entrega->setColumnHidden(0,true);
        ui->tableView_entrega->setColumnHidden(1,true);
        ui->tableView_entrega->setColumnHidden(2,true);
        ui->tableView_entrega->resizeColumnsToContents();
        ui->tableView_entrega->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
        ui->tableView_entrega->setItemDelegateForColumn(7, new NumberFormatDelegate(this));
    }

}
