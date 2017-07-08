#include "entrega.h"
#include "ui_entrega.h"
#include "qmessagebox.h"

//extern Login w;

Entrega::Entrega(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Entrega)
{
    ui->setupUi(this);

    conn.connOpen();

    modalestados=new QSqlQueryModel();
    queryestados=new QSqlQuery(conn.mydb);
    lerestados();
    ui->comboBox_estados->setModel(modalestados);

    modalcidades=new QSqlQueryModel();
    querycidades=new QSqlQuery(conn.mydb);

    modalbairros=new QSqlQueryModel();
    querybairros=new QSqlQuery(conn.mydb);

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

Entrega::~Entrega()
{
    delete ui;

    conn.close();
}

void Entrega::on_comboBox_estados_activated(const QString &arg1)
{
    querycidades->clear();
    modalcidades->clear();
    lercidades(arg1);
    ui->comboBox_cidade->setModel(modalcidades);

    cidade=modalcidades->record(0).value(0).toString();
    estado=modalcidades->record(0).value(1).toString();
    loc_nu_sequencial=modalcidades->record(0).value(2).toInt();

    qDebug() << cidade << arg1;

}

void Entrega::on_comboBox_cidade_activated(const QString &arg1)
{
    for(int i=0;i<modalcidades->rowCount();i++){
        if(arg1==modalcidades->record(i).value(0).toString()){
            cidade=modalcidades->record(i).value(0).toString();
            estado=modalcidades->record(i).value(1).toString();
            loc_nu_sequencial=modalcidades->record(i).value(2).toInt();
            break;
        }
    }
   // querybairros->clear();
   // modalbairros->clear();
    filtro=ui->lineEdit_procurabairro->text();
    lerbairros(loc_nu_sequencial, filtro);
    modalbairros->setHeaderData(0, Qt::Horizontal, tr("Bairro"));
    ui->tableView_bairros->setModel(modalbairros);
    ui->tableView_bairros->resizeColumnsToContents();
    ui->tableView_bairros->setColumnHidden(1,true);

    qDebug() << cidade << arg1;

}

void Entrega::on_pushButton_inserirbairros_clicked()
{
    QModelIndexList indexList = ui->tableView_bairros->selectionModel()->selectedIndexes();
    QString item;
    QVariantList listbairros, listlocalidade;
    int nlinhas=indexList.count();
    int row, i;
    bool flag;
    qDebug()<< "tableview entrega" << nlinhas;

    filtro=ui->lineEdit_procurabairro->text();
    modalentrega->clear();
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

    foreach (QModelIndex index, indexList) {
      row=index.row();
      item=modalbairros->record(row).value(1).toString();
      flag=true;
      for(i=0;i<modalentrega->rowCount();i++){
          if(item==modalentrega->record(i).value(1).toString()){
              flag=false;
              break;
           }
      }
      if(flag){
          listbairros << item;
          listlocalidade << loc_nu_sequencial;
      }
    }

    queryentrega->prepare("insert into entrega (bai_nu_sequencial, loc_nu_sequencial) values (:bai_nu,:loc_nu)");

    queryentrega->bindValue(":bai_nu",listbairros);
    queryentrega->bindValue(":loc_nu",listlocalidade);

    queryentrega->execBatch();

    modalentrega->clear();
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

void Entrega::on_pushButton_apagar_clicked()
{
    QModelIndexList indexList = ui->tableView_entrega->selectionModel()->selectedIndexes();
    QVariantList listentrega;

    int anterior, row, id_entrega, hora=0, tipomenu;
    bool flag;

    qDebug() << indexList.count();

    if(!indexList.count()){
        QMessageBox::critical(this,tr("Seleção"),tr("Nenhum item selecionado"));
        return;
    }

    queryentrega->prepare("delete from entrega where id_entrega=:id_entrega");
    anterior=-1;
    foreach (QModelIndex index, indexList) {
        row=index.row();
        qDebug() << "row " << row;
        if(anterior!=row){
            id_entrega=modalentrega->record(row).value(0).toInt();
            listentrega << id_entrega;

        }
        anterior=row;
    }
    queryentrega->bindValue(":id_entrega",listentrega);

    if (!queryentrega->execBatch()) {
        qDebug() << "erro " << queryentrega->lastError();
    } else {
        qDebug() << "sucesso " << queryentrega->executedQuery();
        modalentrega->clear();
        filtro=ui->lineEdit_procurabairro->text();
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

}

void Entrega::on_pushButton_procurar_clicked()
{
    QString procura=ui->lineEdit_procurabairro->text();
    qDebug() << "on_pushButton_procurarcliente";

    qDebug() << procura;

    lerbairros(loc_nu_sequencial, procura);
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
