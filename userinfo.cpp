#include "userinfo.h"
#include "ui_userinfo.h"
#include <qmessagebox.h>
#include "dateformatdelegate.h"

Userinfo::Userinfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Userinfo)
{
    ui->setupUi(this);

    conn.connOpen();

    modeluser=new QSqlQueryModel();
    queryuser=new QSqlQuery(conn.mydb);

    lerusuario();

    ui->tableView->setModel(modeluser);
    ui->tableView->setColumnHidden(0,true); // id
    ui->tableView->setColumnHidden(7,true); // password
    ui->tableView->setColumnHidden(8,true); // ativo
    ui->tableView->setItemDelegateForColumn(5, new DateFormatDelegate(this));
    ui->tableView->setItemDelegateForColumn(6, new JobFunctionColumnDelegate(this));

    for(int i=0;i<NFUNC;i++)
        ui->comboBox_funcao->addItem(funcao[i], i+1);

    index_funcao=2;
    ui->comboBox_funcao->setCurrentIndex(index_funcao);

}

Userinfo::~Userinfo()
{
    delete ui;
    conn.connClose();
}

void Userinfo::on_pushButton_save_clicked()
{
    QString nome, email, senha, fone, user;
    int funcao;

    user=ui->lineEdit_user->text();
    senha=ui->lineEdit_pass->text();
    email=ui->lineEdit_email->text();
    fone=ui->lineEdit_fone->text();
    nome=ui->lineEdit_nome->text();

    funcao=ui->comboBox_funcao->currentData().toInt();

    qDebug() << "1" <<id_user;

    if(!id_user)
        queryuser->prepare("insert into usuario (username, nome, email, password, fone, tipo)"
                           "values (:username, :nome, :email, :password, :fone, :tipo)");
    else{
      queryuser->prepare("update usuario set username=:username, nome=:nome, "
                                            "email=:email, password=:password, fone=:fone, tipo=:tipo \
                          where id_usuario=:id_user ");
                          queryuser->bindValue(":id_user",id_user);
    }
    qDebug() << "2";

    queryuser->bindValue(":username",user);
    queryuser->bindValue(":nome", nome);
    queryuser->bindValue(":email", email);
    queryuser->bindValue(":password",senha);
    queryuser->bindValue(":fone", fone);
    queryuser->bindValue(":tipo", funcao);

    ui->pushButton_save->setText("Salvar");
    id_user=0;
    qDebug() << "3";

    if(queryuser->exec()){
        QMessageBox::critical(this,tr("Salvar"),tr("Dados inseridos com sucesso!"));
    }
    else{
        QMessageBox::critical(this,tr("error::"),queryuser->lastError().text());
    }

    lerusuario();
    ui->tableView->setModel(modeluser);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(7,true);
    ui->tableView->setItemDelegateForColumn(6, new JobFunctionColumnDelegate(this));

}

void Userinfo::on_tableView_activated(const QModelIndex &index)
{
    QString name=ui->tableView->model()->data(index).toString();
    QString nome, email, senha, fone, user;
    int row=index.row();

     id_user=modeluser->record(row).value(0).toInt();
     ui->lineEdit_user->setText(modeluser->record(row).value(1).toString());
     ui->lineEdit_pass->setText(modeluser->record(row).value(7).toString());
     ui->lineEdit_email->setText(modeluser->record(row).value(4).toString());
     ui->lineEdit_fone->setText(modeluser->record(row).value(3).toString());
     ui->lineEdit_nome->setText(modeluser->record(row).value(2).toString());

     ui->pushButton_save->setText("Atualizar");

     int indice = ui->comboBox_funcao->findData(modeluser->record(row).value(6).toInt());
     if ( indice != -1 ) { // -1 for not found
        ui->comboBox_funcao->setCurrentIndex(indice);
     }
}

void Userinfo::on_pushButton_delete_clicked()
{
    QString nome, email, senha, fone, user;

    user=ui->lineEdit_user->text();
    senha=ui->lineEdit_pass->text();
    email=ui->lineEdit_email->text();
    fone=ui->lineEdit_fone->text();
    nome=ui->lineEdit_nome->text();

    queryuser->prepare("delete from usuario where username=?");
    queryuser->addBindValue(user);
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "Titulo", "Confirma?", QMessageBox::Yes|QMessageBox::No).exec())
    {
        if(queryuser->exec()){
            QMessageBox::critical(this,tr("Apagar"),tr("Dados apagados com sucesso!"));
        }
        else{
            QMessageBox::critical(this,tr("error::"),queryuser->lastError().text());
        }
    }
}

void Userinfo::on_comboBox_funcao_activated(const QString &arg1)
{
    index_funcao=ui->comboBox_funcao->currentData().toInt();
}

void Userinfo::on_pushButton_limpar_clicked()
{
    index_funcao=2;
    ui->comboBox_funcao->setCurrentIndex(index_funcao);

    id_user=0;
    ui->lineEdit_user->setText("");
    ui->lineEdit_pass->setText("");
    ui->lineEdit_email->setText("");
    ui->lineEdit_fone->setText("");
    ui->lineEdit_nome->setText("");

    ui->pushButton_save->setText("Salvar");

}
