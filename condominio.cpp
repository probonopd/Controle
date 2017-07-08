#include "condominio.h"
#include "ui_condominio.h"


Condominio::Condominio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Condominio)
{
    ui->setupUi(this);

    conn.connOpen();

    queryestados=new QSqlQuery(conn.mydb);
    modalestados=new QSqlQueryModel();

    lerestados();
    ui->comboBox_estados->setModel(modalestados);
    ufe=queryestados->value(1).toString();
    estado=queryestados->value(0).toString();

    querycidades=new QSqlQuery(conn.mydb);
    modalcidades=new QSqlQueryModel();

    lecidades();
    ui->comboBox_cidade->setModel(modalcidades);

    queryrua=new QSqlQuery(conn.mydb);
    modalrua=new QSqlQueryModel();

    querycondominio=new QSqlQuery(conn.mydb);
    modalcondominio=new QSqlQueryModel();

    lecondominio();
    modalcondominio->setHeaderData(1, Qt::Horizontal, tr("Nome do Condomínio"));
    modalcondominio->setHeaderData(2, Qt::Horizontal, tr("Rua"));
    modalcondominio->setHeaderData(3, Qt::Horizontal, tr("Número"));
    modalcondominio->setHeaderData(4, Qt::Horizontal, tr("Bairro"));
    modalcondominio->setHeaderData(5, Qt::Horizontal, tr("Valor (R$)"));
    modalcondominio->setHeaderData(6, Qt::Horizontal, tr("%"));
    modalcondominio->setHeaderData(7, Qt::Horizontal, tr("Taxa Extra (R$)"));

    ui->tableView_condominio->setModel(modalcondominio);
    ui->tableView_condominio->setColumnHidden(0,true);
    ui->tableView_condominio->setColumnHidden(8,true);
    ui->tableView_condominio->resizeColumnsToContents();
    ui->tableView_condominio->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
    ui->tableView_condominio->setItemDelegateForColumn(7, new NumberFormatDelegate(this));

    querylog=new QSqlQuery(conn.mydb);
    modallog=new QSqlQueryModel();
}

Condominio::~Condominio()
{
    delete ui;

    conn.close();
}

void Condominio::on_comboBox_estados_activated(const QString &arg1)
{

    estado=arg1;
    for(int i=0;i<modalestados->rowCount();i++)
        if(modalestados->record(i).value(0)==estado){
            ufe=modalestados->record(i).value(1).toString();
            break;
         }
    modalcidades->clear();
    querycidades->clear();
    lecidades();

    ui->comboBox_cidade->setModel(modalcidades);


}

void Condominio::on_pushButton_procurar_clicked()
{
    QString rua=ui->lineEdit_procurarua->text();
    qDebug() << "on_pushButton_procurar";

    leruas(rua);
    ui->tableView_enderecos->setModel(modalrua);

    QTableView *teste=new QTableView();
    teste->setModel(modalcondominio);
    teste->show();

 }

void Condominio::on_comboBox_cidade_activated(const QString &arg1)
{
    qDebug() << "on_comboBoxcidade_activated";

    QString str="";

    cidade=arg1;
    qDebug() << cidade;

    for(int i=0;i<modalcidades->rowCount();i++){
        qDebug() << modalcidades->record(i).value(0).toString();
        if(modalcidades->record(i).value(0).toString()==cidade){
            loc_nu_sequencial=modalcidades->record(i).value(2).toInt();
            break;
         }
    }
    leruas(str);

    modalrua->setHeaderData(0, Qt::Horizontal, tr("Tipo"));
    modalrua->setHeaderData(1, Qt::Horizontal, tr("Nome da Rua"));
    modalrua->setHeaderData(2, Qt::Horizontal, tr("CEP"));
    modalrua->setHeaderData(3, Qt::Horizontal, tr("Bairro"));

    ui->tableView_enderecos->setModel(modalrua);

    ui->tableView_enderecos->setColumnHidden(2,true);
    ui->tableView_enderecos->setColumnHidden(4,true);
    ui->tableView_enderecos->setColumnHidden(5,true);
    ui->tableView_enderecos->resizeColumnsToContents();


}

void Condominio::on_tableView_enderecos_activated(const QModelIndex &index)
{
    qDebug() << "tableView enderecos_activated";

    QString celula=ui->tableView_enderecos->model()->data(index).toString();
    int colunas=ui->tableView_enderecos->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;

  //  ui->label_tipolog->setText(modalrua->record(linha).value(0).toString());
    ui->lineEdit_nome->setText("");
    ui->lineEdit_numero->setText("");
    ui->lineEdit_perc->setText("");
    ui->lineEdit_valor->setText("");
    ui->lineEdit_extra->setText("");
    ui->label_rua->setText(modalrua->record(linha).value(1).toString());
    ui->label_bairro->setText(modalrua->record(linha).value(3).toString());
    log_nu_sequencial=modalrua->record(linha).value(5).toInt();


}

void Condominio::on_pushButton_gravar_clicked()
{
    if(!flag){
        querycondominio->prepare("insert into condominio (log_nu_sequencial, nome_condo, numero, valor, percentual, extra)"
                                  "values (:log_nu, :nomecondo, :numero, :valor, :percentual, :extra) ");
    }
    else{
        querycondominio->prepare("update condominio set nome_condo=:nomecondo, numero=:numero, valor=:valor, percentual=:percentual, extra=:extra \
                                  where id_condominio=:id_condominio ");
        querycondominio->bindValue(":id_condominio",id_condominio);
    }

    querycondominio->bindValue(":log_nu",log_nu_sequencial);
    querycondominio->bindValue(":nomecondo",ui->lineEdit_nome->text());
    querycondominio->bindValue(":numero",ui->lineEdit_numero->text());
    querycondominio->bindValue(":valor",ui->lineEdit_valor->text());
    querycondominio->bindValue(":percentual",ui->lineEdit_perc->text());
    querycondominio->bindValue(":extra",ui->lineEdit_extra->text());

    querycondominio->exec();

    lecondominio();
    ui->tableView_condominio->setModel(modalcondominio);
    ui->lineEdit_nome->setText("");
    ui->lineEdit_numero->setText("");
    ui->lineEdit_valor->setText("");
    ui->lineEdit_perc->setText("");
    ui->lineEdit_extra->setText("");

    flag=false;
    id_condominio=0;

    ui->pushButton_gravar->setText("Gravar\nCondomínio");

}

void Condominio::on_tableView_condominio_activated(const QModelIndex &index)
{
    flag=true;
    qDebug() << "tableView enderecos_activated";

    QString celula=ui->tableView_condominio->model()->data(index).toString();
    int colunas=ui->tableView_condominio->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;

    log_nu_sequencial=modalcondominio->record(linha).value(8).toInt();
    lerlogradouro(log_nu_sequencial);

    ui->label_rua->setText(modallog->record(0).value(2).toString());
    ui->label_bairro->setText(modallog->record(0).value(4).toString());

    id_condominio=modalcondominio->record(linha).value(0).toInt();
    ui->lineEdit_nome->setText(modalcondominio->record(linha).value(1).toString());
    ui->label_rua->setText(modalcondominio->record(linha).value(2).toString());
    ui->lineEdit_numero->setText(modalcondominio->record(linha).value(3).toString());
    ui->label_bairro->setText(modalcondominio->record(linha).value(4).toString());
    ui->lineEdit_valor->setText(modalcondominio->record(linha).value(5).toString());
    ui->lineEdit_perc->setText(modalcondominio->record(linha).value(6).toString());
    ui->lineEdit_extra->setText(modalcondominio->record(linha).value(7).toString());

    ui->pushButton_gravar->setText("Atualizar\nCondomínio");

}
