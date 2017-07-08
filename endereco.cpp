#include "endereco.h"
#include "ui_endereco.h"
#include <qmessagebox.h>

Endereco::Endereco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Endereco)
{
    ui->setupUi(this);

    conn.connOpen();

    modalestados=new QSqlQueryModel();
    queryestados=new QSqlQuery(conn.mydb);

    queryestados->prepare("select ufe_no, ufe_sg from log_faixa_uf");
    queryestados->exec();
    modalestados->setQuery(*queryestados);
    ui->comboBox_estados->setModel(modalestados);

    cliente=new QSqlQuery(conn.mydb);
    endereco=new QSqlQuery(conn.mydb);

    modalcidades=new QSqlQueryModel();
    querycidades=new QSqlQuery(conn.mydb);

    modalruas=new QSqlQueryModel();
    queryruas=new QSqlQuery(conn.mydb);

}

Endereco::~Endereco()
{
    delete ui;
    conn.close();
}

void Endereco::on_pushButton_procurarcliente_clicked()
{
    QString procura=ui->lineEdit_procurarcliente->text();
    qDebug() << "on_pushButton_procurarcliente";

    qDebug() << procura;

    QSqlQueryModel *modal=new QSqlQueryModel();

    modal=new QSqlQueryModel();

    cliente->prepare("SELECT id_cliente, cliente.nome, cliente.fone, cliente.celular \
                   FROM cliente \
                   WHERE (cliente.nome like :procura ) or\
                         (cliente.fone like :procura ) or\
                         (cliente.celular like :procura ) ");
    cliente->bindValue( ":procura", "%" + procura + "%" );

    cliente->exec();

    modal->setQuery(*cliente);
    ui->tableView_clientes->setModel(modal);
    ui->tableView_clientes->setColumnHidden(0,true);

}

void Endereco::on_comboBox_estados_activated(const QString &arg1)
{

    lecidades(arg1);
    ui->comboBox_cidade->setModel(modalcidades);

    cidade=queryestados->value(0).toString();
    estado=queryestados->value(1).toString();
    loc_nu_sequencial=queryestados->value(2).toInt();

    qDebug() << cidade;

}

void Endereco::on_pushButton_procurar_clicked()
{
    QString rua=ui->lineEdit_procurarua->text();
    qDebug() << "on_pushButton_procurar";

    qDebug() << rua;
    qDebug() << estado;
    qDebug() << loc_nu_sequencial;

    leruas(rua);
    ui->tableView_enderecos->setModel(modalruas);

}

void Endereco::on_comboBox_cidade_activated(const QString &arg1)
{
    QString filtro="";

    qDebug() << "on_comboBoxcidade_activated";

    cidade=arg1;
    qDebug() << cidade;
    qDebug() << estado;

    for(int i=0;i<modalcidades->rowCount();i++){
        if(modalcidades->record(i).value(0).toString()==cidade){
            loc_nu_sequencial=modalcidades->record(i).value(2).toInt();
            break;
        }
    }

    leruas(filtro);
    ui->tableView_enderecos->setModel(modalruas);

}

void Endereco::on_tableView_enderecos_activated(const QModelIndex &index)
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

    ui->lineEdit_rua->setText(modalruas->record(linha).value(0).toString());
    ui->label_bairro->setText(modalruas->record(linha).value(1).toString());
    ui->label_cep->setText(modalruas->record(linha).value(2).toString());
    ui->label_cidade->setText(cidade);
    ui->label_estado->setText(estado);

    bai_nu=modalruas->record(linha).value(3).toInt();
    log_nu=modalruas->record(linha).value(4).toInt();
}

void Endereco::on_pushButton_cancelar_clicked()
{
    conn.connClose();
    close();
}

void Endereco::on_pushButton_gravar_clicked()
{

    QString tipo_log, rua, numero, apto, bairro, cidade, cep, ufe;
    QSqlQuery *novo_end=new QSqlQuery(conn.mydb);

    rua=ui->lineEdit_rua->text();
    cep=ui->label_cep->text();
    bairro=ui->label_bairro->text();
    cidade=ui->label_cidade->text();
    numero=ui->lineEdit_numero->text();
    apto=ui->lineEdit_numapto->text();
    ufe=ui->label_estado->text();

    qDebug()<<numero;

    if(id_cliente && numero!=""){
        if(!id_endereco){
            novo_end->prepare("insert into endereco (rua, numero, apto, bairro, cep, cidade, estado, id_cliente, bai_nu_sequencial, log_nu_sequencial)"
                         "values (:nomerua, :numero, :apto, :bairro, :cep, :cidade, :estado, :id_cliente, :bai_nu, :log_nu) ");

            qDebug()<<"insert";
        }
        else{
            novo_end->prepare("update endereco set rua=:nomerua, numero=:numero, apto=:apto, bairro=:bairro, \
                               cep=:cep, cidade=:cidade, estado=:estado, bai_nu_sequencial=:bai_nu, log_nu_sequencial=:log_nu \
                               where id_cliente=:id_cliente ");

        }
        novo_end->bindValue(":id_cliente",id_cliente);
        novo_end->bindValue(":nomerua",rua);
        novo_end->bindValue(":numero",numero);
        novo_end->bindValue(":apto",apto);
        novo_end->bindValue(":bairro",bairro);
        novo_end->bindValue(":cep",cep);
        novo_end->bindValue(":cidade",cidade);
        novo_end->bindValue(":estado",ufe);
        novo_end->bindValue(":bai_nu",bai_nu);
        novo_end->bindValue(":log_nu",log_nu);

        if(novo_end->exec()){
            QMessageBox::critical(this,tr("Editar"),tr("Dados alterados com sucesso!"));
            conn.connClose();
            close();
        }
        else{
            QMessageBox::critical(this,tr("error::"),novo_end->lastError().text());
        }

    }
    else{
        if(!id_cliente)
            QMessageBox::critical(this,tr("Gravar Endereço"),tr("Favor selecionar cliente"));
        if(numero=="")
            QMessageBox::critical(this,tr("Gravar Endereço"),tr("Favor digitar número do endereço"));
    }
}

void Endereco::on_tableView_clientes_activated(const QModelIndex &index)
{
    qDebug() << "tableView_clientes";

    QString celula=ui->tableView_clientes->model()->data(index).toString();
    int colunas=ui->tableView_clientes->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;

    cliente->first();//primeiro registro da query
    int contador=0;
    while(contador<linha){
        cliente->next();
        contador++;
    }

    ui->label_nomecliente->setText(cliente->value(1).toString());
    id_cliente=cliente->value(0).toInt();

    endereco->prepare("select * from endereco WHERE id_cliente=:id_cliente");
    endereco->bindValue(":id_cliente",id_cliente);
    endereco->exec();

    QSqlQueryModel *modal_end=new QSqlQueryModel();
    modal_end->setQuery(*endereco);
    ui->tableView_endcadast->setModel(modal_end);

}

void Endereco::on_tableView_endcadast_activated(const QModelIndex &index)
{
    QString celula=ui->tableView_endcadast->model()->data(index).toString();
    int colunas=ui->tableView_endcadast->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;

    endereco->first();//primeiro registro da query
    int contador=0;
    while(contador<linha){
        endereco->next();
        contador++;
    }
    ui->lineEdit_rua->setText(endereco->value(2).toString());
    ui->lineEdit_numero->setText(endereco->value(3).toString());
    ui->lineEdit_numapto->setText(endereco->value(4).toString());
  //  ui->lineEdit_bairro->setText(endereco->value(5).toString());
  //  ui->lineEdit_cep->setText(endereco->value(6).toString());
    ui->label_cidade->setText(endereco->value(7).toString());
    ui->label_estado->setText(endereco->value(8).toString());
  //  ui->lineEdit_tipolog->setText(endereco->value(9).toString());

    id_endereco=endereco->value(0).toInt();

}

void Endereco::on_pushButton_apagar_clicked()
{
    endereco->prepare("DELETE FROM endereco where id_endereco=:id_endereco");
    endereco->bindValue(":id_endereco", id_endereco);
    if(id_endereco){
        if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "Apagar Endereço!", "Confirma?", QMessageBox::Yes|QMessageBox::No).exec())
        {
            if(endereco->exec()){
                QMessageBox::critical(this,tr("Apagar"),tr("Endereço apagado com sucesso!"));
                conn.connClose();
                close();
            }
            else{
                QMessageBox::critical(this,tr("error::"),endereco->lastError().text());
            }
        }
    }
    else{
        if(!id_endereco)
            QMessageBox::critical(this,tr("Apagar Endereço"),tr("Favor selecionar endereço!"));
    }

}

void Endereco::on_pushButton_novocliente_clicked()
{
    Cliente NovoCliente;

    NovoCliente.setModal(true);
    NovoCliente.exec();
}
