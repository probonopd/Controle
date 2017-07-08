#include "estabelecimento.h"
#include "ui_estabelecimento.h"
#include <QtDebug>
#include <qmessagebox.h>

extern int config;

Estabelecimento::Estabelecimento(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Estabelecimento)
{
    ui->setupUi(this);

    conn.connOpen();

    modalloja=new QSqlQueryModel();
    queryloja=new QSqlQuery(conn.mydb);

    lerloja();

    ui->lineEdit_nomeloja->setText(modalloja->record(0).value(0).toString());
    ui->lineEdit_nomerua->setText(modalloja->record(0).value(1).toString());
    ui->lineEdit_numero->setText(modalloja->record(0).value(2).toString());

    ui->label_bairro->setText(modalloja->record(0).value(3).toString());
    ui->label_cidade->setText(modalloja->record(0).value(4).toString());
    ui->label_estado->setText(modalloja->record(0).value(5).toString());
    ui->label_cep->setText(modalloja->record(0).value(6).toString());
    ui->textEdit_fone->setText(modalloja->record(0).value(7).toString());
    ui->lineEdit_inscrest->setText(modalloja->record(0).value(8).toString());
    ui->lineEdit_cnpj->setText(modalloja->record(0).value(9).toString());
    ui->lineEdit_site->setText(modalloja->record(0).value(10).toString());
    ui->lineEdit_facebook->setText(modalloja->record(0).value(11).toString());

    config=modalloja->record(0).value(13).toInt();
    configuracao=config;

    modalestados=new QSqlQueryModel();
    queryestados=new QSqlQuery(conn.mydb);
    lerestados();
    ui->comboBox_estados->setModel(modalestados);

    modalcidade=new QSqlQueryModel();
    querycidade=new QSqlQuery(conn.mydb);

    modalruas=new QSqlQueryModel();
    queryruas=new QSqlQuery(conn.mydb);

}

Estabelecimento::~Estabelecimento()
{
    delete ui;
}

void Estabelecimento::on_comboBox_estados_activated(const QString &arg1)
{
    estado=arg1;
    lercidades(arg1);
    ufe_sg=modalestados->record(ui->comboBox_estados->currentIndex()).value(1).toString();

    ui->comboBox_cidades->clear();
    for(int i=0;i<modalcidade->rowCount();i++){ qDebug() <<i;
       ui->comboBox_cidades->addItem(modalcidade->record(i).value(0).toString(), modalcidade->record(i).value(1).toInt());
    }
    qDebug() << cidade << ufe_sg;
}

void Estabelecimento::on_pushButton_procurar_clicked()
{

    QString rua=ui->lineEdit_procurarua->text();
    qDebug() << "on_pushButton_procurar";

    qDebug() << rua;
    qDebug() << estado;
    qDebug() << loc_nu_sequencial;

    lerruas(loc_nu_sequencial, rua);

    modalruas->setHeaderData(0, Qt::Horizontal, tr("Rua"));
    modalruas->setHeaderData(1, Qt::Horizontal, tr("Bairro"));
    modalruas->setHeaderData(2, Qt::Horizontal, tr("CEP"));

    ui->tableView->setModel(modalruas);

}

void Estabelecimento::on_tableView_activated(const QModelIndex &index)
{
    qDebug() << "tableView_activated";

    QString celula=ui->tableView->model()->data(index).toString();
    int colunas=ui->tableView->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;

    ui->lineEdit_nomerua->setText(modalruas->record(linha).value(0).toString());
    ui->label_cep->setText(modalruas->record(linha).value(2).toString());
    ui->label_bairro->setText(modalruas->record(linha).value(1).toString());
    ui->label_cidade->setText(cidade);
    ui->label_estado->setText(ufe_sg);
}

void Estabelecimento::on_pushButton_cancelar_clicked()
{
    close();
}

void Estabelecimento::on_pushButton_salvar_clicked()
{

    qDebug()<< "pushButton_salvar";

    if(!modalloja->rowCount()){
       queryloja->prepare("insert into loja (nome, endereco, numero, bairro, cidade, estado, fone, inscrest, cnpj, site, facebook, cep, config)"
                          "values (:nomeloja, :nomerua, :numero, :bairro, :cidade, :estado, :fone, :inscrest,:cnpj,:site,:facebook, :cep, :config) ");

        qDebug()<<"insert";
    }
    else{
        int id_loja=modalloja->record(0).value(12).toInt();
        queryloja->prepare("update loja set nome=:nomeloja, endereco=:nomerua, numero=:numero, bairro=:bairro, \
                                        cidade=:cidade, estado=:estado, fone=:fone, inscrest=:inscrest, cnpj=:cnpj, \
                                        site=:site, facebook=:facebook, cep=:cep, config=:config \
                            where id_loja=:id_loja ");

        queryloja->bindValue(":id_loja",id_loja);
    }

    queryloja->bindValue(":nomeloja",ui->lineEdit_nomeloja->text());
    queryloja->bindValue(":nomerua",ui->lineEdit_nomerua->text());

    queryloja->bindValue(":numero",ui->lineEdit_numero->text());
    queryloja->bindValue(":bairro",ui->label_bairro->text());
    queryloja->bindValue(":cidade",ui->label_cidade->text());
    queryloja->bindValue(":estado",ui->label_estado->text());
    queryloja->bindValue(":fone",ui->textEdit_fone->toPlainText());
    queryloja->bindValue(":inscrest",ui->lineEdit_inscrest->text());
    queryloja->bindValue(":cnpj",ui->lineEdit_cnpj->text());
    queryloja->bindValue(":site",ui->lineEdit_site->text());
    queryloja->bindValue(":facebook",ui->lineEdit_facebook->text());
    queryloja->bindValue(":cep",ui->label_cep->text());
    queryloja->bindValue(":config",configuracao);


    if(queryloja->exec()){
        config=configuracao;
        QMessageBox::critical(this,tr("Editar"),tr("Dados alterados com sucesso!"));
    }
    else{
        QMessageBox::critical(this,tr("error::"),queryloja->lastError().text());
    }

}

void Estabelecimento::on_comboBox_cidades_activated(const QString &arg1)
{
    filtro=ui->lineEdit_procurarua->text();
    cidade=arg1;
    loc_nu_sequencial=ui->comboBox_cidades->itemData(ui->comboBox_cidades->currentIndex()).toInt();
    lerruas(loc_nu_sequencial, filtro);

    modalruas->setHeaderData(0, Qt::Horizontal, tr("Rua"));
    modalruas->setHeaderData(1, Qt::Horizontal, tr("Bairro"));
    modalruas->setHeaderData(2, Qt::Horizontal, tr("CEP"));

    ui->tableView->setModel(modalruas);

}

void Estabelecimento::on_radioButton_entregavalor_clicked()
{
    configuracao=configuracao & ~(1 << ENTREGA);
    qDebug()<<configuracao;
}

void Estabelecimento::on_radioButton_entregaperc_clicked()
{
    configuracao|=(1 << ENTREGA);
    qDebug()<<configuracao;
}

void Estabelecimento::on_radioButton_mult_maior_clicked()
{
    configuracao=configuracao & ~(1 << SELECOES);
    qDebug()<<configuracao;
}

void Estabelecimento::on_radioButton_mult_media_clicked()
{
    configuracao|=(1 << SELECOES);
    qDebug()<<configuracao;
}
