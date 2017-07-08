#include "cliente.h"
#include "ui_cliente.h"
#include "qdatetime.h"
#include "qmessagebox.h"

Cliente::Cliente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Cliente)
{
    ui->setupUi(this);

    conn.connOpen();

    query=new QSqlQuery(conn.mydb);
    modal=new QSqlQueryModel();
    queryendereco=new QSqlQuery(conn.mydb);
    modalendereco=new QSqlQueryModel();

    queryloja=new QSqlQuery(conn.mydb);
    modalloja=new QSqlQueryModel();
    lerloja();

    modalestados=new QSqlQueryModel();
    queryestados=new QSqlQuery(conn.mydb);
    modalcidades=new QSqlQueryModel();
    querycidades=new QSqlQuery(conn.mydb);
    modalruas=new QSqlQueryModel();
    queryruas=new QSqlQuery(conn.mydb);

    queryestados->prepare("select ufe_no, ufe_sg from log_faixa_uf");
    queryestados->exec();
    modalestados->setQuery(*queryestados);
    for(int i=0;i<modalestados->rowCount();i++){
        ui->comboBox_estados->addItem(modalestados->record(i).value(0).toString(), modalestados->record(i).value(1).toString());
    }
    int indice=ui->comboBox_estados->findData(modalloja->record(0).value(UF_LOJA).toString());

    if(indice!=-1){
        ui->comboBox_estados->setCurrentIndex(indice);
        estado=modalestados->record(indice).value(0).toString();
        lecidades(modalloja->record(0).value(UF_LOJA).toString());
        for(int i=0;i<modalcidades->rowCount();i++){
            ui->comboBox_cidade->addItem(modalcidades->record(i).value(0).toString(), modalcidades->record(i).value(0).toString());
        }
        indice=ui->comboBox_cidade->findData(modalloja->record(0).value(LOG_LOJA).toString());
        if(indice!=-1){
            ui->comboBox_cidade->setCurrentIndex(indice);
            cidade=modalcidades->record(indice).value(0).toString();
            loc_nu_sequencial=modalcidades->record(indice).value(1).toInt();
        }
    }
}

Cliente::~Cliente()
{
    delete ui;
    conn.close();
}

void Cliente::on_pushButton_fechar_clicked()
{
    ui->lineEdit_nome->setText("");
    ui->lineEdit_fone->setText("");
    ui->lineEdit_celular->setText("");
    ui->lineEdit_email->setText("");

    id_cliente=0;
    lerendereco(id_cliente);
    ui->tableView_endcadast->setModel(modalendereco);

}

void Cliente::on_pushButton_gravar_clicked()
{
    QString nome, email, fone, celular;
    uint current;

    nome=ui->lineEdit_nome->text();
    email=ui->lineEdit_email->text();
    fone=ui->lineEdit_fone->text();
    celular=ui->lineEdit_celular->text();

    if(!id_cliente){
        current = QDateTime::currentDateTime().toTime_t();
        qDebug()<<current;

        query->prepare("insert into cliente (nome, email, fone, celular, cadastro)"
                       "values (:nome, :email, :fone, :celular, :cadastro) ");
        query->bindValue(":cadastro",current);
    }
    else{
        query->prepare("update cliente set nome=:nome, email=:email, fone=:fone, celular=:celular \
                        where id_cliente=:id_cliente ");

        query->bindValue(":id_cliente",id_cliente);

    }

    query->bindValue(":nome",nome);
    query->bindValue(":email",email);
    query->bindValue(":fone",fone);
    query->bindValue(":celular",celular);

    if(query->exec()){
        if(!id_cliente){
            query->prepare("select * from cliente order by cadastro desc limit 1");
            query->exec();
            modal->setQuery(*query);
            id_cliente=modal->record(0).value(0).toInt();
        }

        QMessageBox::critical(this,tr("Salvar"),tr("Dados inseridos com sucesso!"));
    }
    else{
        QMessageBox::critical(this,tr("error::"),query->lastError().text());
    }

    if(id_cliente){
        lerendereco(id_cliente);
        ui->tableView_endcadast->setModel(modalendereco);
    }

    qDebug() <<id_cliente;

}

void Cliente::on_pushButton_procurar_clicked()
{
    QString procura=ui->lineEdit_procura->text();
    qDebug() << "on_pushButton_procurar";

    qDebug() << procura;

    query->prepare("SELECT id_cliente, cliente.nome, cliente.fone, cliente.celular, cliente.email \
                   FROM cliente \
                   WHERE (cliente.nome like :procura ) or\
                         (cliente.fone like :procura ) or\
                         (cliente.celular like :procura ) ");
    query->bindValue( ":procura", "%" + procura + "%" );

    query->exec();

    modal->setQuery(*query);
    modal->setHeaderData(1, Qt::Horizontal, tr("Nome"));
    modal->setHeaderData(2, Qt::Horizontal, tr("Fone"));
    modal->setHeaderData(3, Qt::Horizontal, tr("Celular"));
    modal->setHeaderData(4, Qt::Horizontal, tr("E-mail"));

    ui->tableView_clientes->setModel(modal);
    ui->tableView_clientes->setColumnHidden(0, true);
    ui->tableView_clientes->resizeColumnsToContents();
}

void Cliente::on_tableView_clientes_activated(const QModelIndex &index)
{
    qDebug() << "tableViewclientes_activated";

    QString celula=ui->tableView_clientes->model()->data(index).toString();
    int colunas=ui->tableView_clientes->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;

    ui->lineEdit_nome->setText(modal->record(linha).value(1).toString());
    ui->lineEdit_fone->setText(modal->record(linha).value(2).toString());
    ui->lineEdit_celular->setText(modal->record(linha).value(3).toString());
    ui->lineEdit_email->setText(modal->record(linha).value(4).toString());

    id_cliente=modal->record(linha).value(0).toInt();

    if(id_cliente){
        lerendereco(id_cliente);
        modalendereco->setHeaderData(2, Qt::Horizontal, tr("Rua"));
        modalendereco->setHeaderData(3, Qt::Horizontal, tr("Número"));
        modalendereco->setHeaderData(4, Qt::Horizontal, tr("Bloco"));
        modalendereco->setHeaderData(5, Qt::Horizontal, tr("Num. Apto"));
        modalendereco->setHeaderData(6, Qt::Horizontal, tr("Bairro"));
        modalendereco->setHeaderData(7, Qt::Horizontal, tr("CEP"));
        modalendereco->setHeaderData(8, Qt::Horizontal, tr("Cidade"));
        modalendereco->setHeaderData(9, Qt::Horizontal, tr("Estado"));

        ui->tableView_endcadast->setModel(modalendereco);
        ui->tableView_endcadast->hideColumn(0);
        ui->tableView_endcadast->hideColumn(1);
        ui->tableView_endcadast->hideColumn(10);
        ui->tableView_endcadast->hideColumn(11);
        ui->tableView_endcadast->resizeColumnsToContents();
    }

}

void Cliente::on_tableView_clientes_doubleClicked(const QModelIndex &index)
{
   qDebug() << "on_tableView_clientes_doubleClicked";


   close();

}

void Cliente::on_tableView_endcadast_activated(const QModelIndex &index)
{
    QString celula=ui->tableView_endcadast->model()->data(index).toString();
    int colunas=ui->tableView_endcadast->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;

    ui->lineEdit_rua->setText(modalendereco->record(linha).value(2).toString());
    ui->lineEdit_numero->setText(modalendereco->record(linha).value(3).toString());
    ui->lineEdit_bloco->setText(modalendereco->record(linha).value(4).toString());
    ui->lineEdit_numapto->setText(modalendereco->record(linha).value(5).toString());
    ui->label_bairro->setText(modalendereco->record(linha).value(6).toString());
    ui->label_cep->setText(modalendereco->record(linha).value(7).toString());
    ui->label_cidade->setText(modalendereco->record(linha).value(8).toString());
    ui->label_estado->setText(modalendereco->record(linha).value(9).toString());

    id_endereco=modalendereco->record(linha).value(0).toInt();

}

void Cliente::on_pushButton_limpar_clicked()
{
    ui->lineEdit_rua->setText("");
    ui->lineEdit_numero->setText("");
    ui->lineEdit_bloco->setText("");
    ui->lineEdit_numapto->setText("");
    ui->label_bairro->setText("");
    ui->label_cep->setText("");
    ui->label_cidade->setText("");
    ui->label_estado->setText("");

    id_endereco=0;

}

void Cliente::on_comboBox_estados_activated(const QString &arg1)
{
/*
    lecidades(arg1);
    ui->comboBox_cidade->setModel(modalcidades);

    cidade=queryestados->value(0).toString();
    estado=queryestados->value(1).toString();
    loc_nu_sequencial=queryestados->value(2).toInt();

    qDebug() << cidade;
  */
qDebug()<<ui->comboBox_estados->currentData().toString();
        ui->comboBox_cidade->clear();
        lecidades(ui->comboBox_estados->currentData().toString());
        for(int i=0;i<modalcidades->rowCount();i++){
            ui->comboBox_cidade->addItem(modalcidades->record(i).value(0).toString(), modalcidades->record(i).value(0).toString());
        }
        ui->comboBox_cidade->setCurrentIndex(0);
        cidade=modalcidades->record(0).value(0).toString();
        loc_nu_sequencial=modalcidades->record(0).value(1).toInt();
        qDebug()<< arg1 << cidade << loc_nu_sequencial;
}

void Cliente::on_comboBox_cidade_activated(const QString &arg1)
{
    QString filtro="";

    qDebug() << "on_comboBoxcidade_activated";
/*
    cidade=arg1;
    qDebug() << cidade;
    qDebug() << estado;

    for(int i=0;i<modalcidades->rowCount();i++){
        if(modalcidades->record(i).value(0).toString()==cidade){
            loc_nu_sequencial=modalcidades->record(i).value(2).toInt();
            break;
        }
    }
*/
    /////////
    int indice=ui->comboBox_cidade->findData(arg1);
    if(indice!=-1){
        ui->comboBox_cidade->setCurrentIndex(indice);
        cidade=modalcidades->record(indice).value(0).toString();
        loc_nu_sequencial=modalcidades->record(indice).value(1).toInt();
    }

    /////////////////
    leruas(filtro);

    modalruas->setHeaderData(2, Qt::Horizontal, tr("Rua"));
    modalruas->setHeaderData(3, Qt::Horizontal, tr("Bairro"));
    modalruas->setHeaderData(4, Qt::Horizontal, tr("CEP"));

    ui->tableView_enderecos->setModel(modalruas);

    ui->tableView_enderecos->hideColumn(0);
    ui->tableView_enderecos->hideColumn(1);
    ui->tableView_enderecos->hideColumn(5);
    ui->tableView_enderecos->hideColumn(6);
    ui->tableView_enderecos->resizeColumnsToContents();

}

void Cliente::on_pushButton_procurar_2_clicked()
{
    QString rua=ui->lineEdit_procurarua->text();
    qDebug() << "on_pushButton_procurar";

    qDebug() << rua;
    qDebug() << estado;
    qDebug() << loc_nu_sequencial;

    leruas(rua);
    ui->tableView_enderecos->setModel(modalruas);

}

void Cliente::on_tableView_enderecos_activated(const QModelIndex &index)
{
    qDebug() << "tableView enderecos_activated";

    QString celula=ui->tableView_enderecos->model()->data(index).toString();
    QString rua;
    int colunas=ui->tableView_enderecos->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;

    rua=modalruas->record(linha).value(0).toString()+" "+modalruas->record(linha).value(1).toString();
    ui->lineEdit_rua->setText(rua);
    ui->label_bairro->setText(modalruas->record(linha).value(3).toString());
    ui->label_cep->setText(modalruas->record(linha).value(4).toString());
    ui->label_cidade->setText(cidade);
    ui->label_estado->setText(estado);
    ui->lineEdit_numero->setText("");
    ui->lineEdit_bloco->setText("");
    ui->lineEdit_numapto->setText("");

    bai_nu=modalruas->record(linha).value(5).toInt();
    log_nu=modalruas->record(linha).value(6).toInt();

    id_endereco=0;  //novo endereco para gravar
}

void Cliente::on_pushButton_gravar_2_clicked()
{
    QString tipo_log, rua, numero, apto, bairro, cidade, cep, ufe, bloco;
    //QSqlQuery *novo_end=new QSqlQuery(conn.mydb);

    rua=ui->lineEdit_rua->text();
    cep=ui->label_cep->text();
    bairro=ui->label_bairro->text();
    cidade=ui->label_cidade->text();
    numero=ui->lineEdit_numero->text();
    bloco=ui->lineEdit_bloco->text();
    apto=ui->lineEdit_numapto->text();
    ufe=ui->label_estado->text();

    qDebug()<<numero;

    if(id_cliente && numero!=""){
        if(!id_endereco){
            queryendereco->prepare("insert into endereco (rua, numero, bloco, apto, bairro, cep, cidade, estado, id_cliente, bai_nu_sequencial, log_nu_sequencial)"
                         "values (:nomerua, :numero, :bloco, :apto, :bairro, :cep, :cidade, :estado, :id_cliente, :bai_nu, :log_nu) ");
            queryendereco->bindValue(":id_cliente",id_cliente);

            qDebug()<<"insert";
        }
        else{
            queryendereco->prepare("update endereco set rua=:nomerua, numero=:numero, bloco=:bloco, apto=:apto, bairro=:bairro, \
                               cep=:cep, cidade=:cidade, estado=:estado, bai_nu_sequencial=:bai_nu, log_nu_sequencial=:log_nu \
                               where id_endereco=:id_endereco ");
            queryendereco->bindValue(":id_endereco",id_endereco);
        }
        queryendereco->bindValue(":nomerua",rua);
        queryendereco->bindValue(":numero",numero);
        queryendereco->bindValue(":bloco",bloco);
        queryendereco->bindValue(":apto",apto);
        queryendereco->bindValue(":bairro",bairro);
        queryendereco->bindValue(":cep",cep);
        queryendereco->bindValue(":cidade",cidade);
        queryendereco->bindValue(":estado",ufe);
        queryendereco->bindValue(":bai_nu",bai_nu);
        queryendereco->bindValue(":log_nu",log_nu);

        if(queryendereco->exec()){
            lerendereco(id_cliente);
            QMessageBox::critical(this,tr("Editar"),tr("Dados alterados com sucesso!"));
        }
        else{
            QMessageBox::critical(this,tr("error::"),queryendereco->lastError().text());
        }

    }
    else{
        if(!id_cliente)
            QMessageBox::critical(this,tr("Gravar Endereço"),tr("Favor selecionar cliente"));
        if(numero=="")
            QMessageBox::critical(this,tr("Gravar Endereço"),tr("Favor digitar número do endereço"));
    }
}

void Cliente::on_pushButton_apagar_clicked()
{
    QModelIndexList indexList = ui->tableView_endcadast->selectionModel()->selectedIndexes();
    QVariantList listid;
    int row;

    if(!indexList.count()){
        QMessageBox::critical(this,tr("Apagar Endereço"),tr("Favor selecionar endereço!"));
        return;
    }

    if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "Apagar Endereço!", "Confirma?", QMessageBox::Yes|QMessageBox::No).exec()){
        foreach (QModelIndex index, indexList) {
          row=index.row();
          listid << modalendereco->record(row).value(0).toInt();
        }
        queryendereco->prepare("DELETE FROM endereco where id_endereco=:id_endereco");
        queryendereco->bindValue(":id_endereco", listid);
        if(queryendereco->execBatch()){
            lerendereco(id_cliente);
            QMessageBox::critical(this,tr("Apagar"),tr("Endereço(s) apagado(s) com sucesso!"));
        }
        else{
            QMessageBox::critical(this,tr("error::"),queryendereco->lastError().text());
        }

    }
}
