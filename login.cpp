#include "login.h"
#include "principal.h"

#include "ui_login.h"

extern QString user_logado, user_status, user_nome;
extern int user_id, user_tipo;

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

 //   QPixmap pix("/mnt/midia4/prj/qt/sql/sql_app/selo_sabordelivery.png");
 //   ui->label_pic->setPixmap(pix);

    if(!connOpen())
        ui->label_db->setText("Falha ao conectar DataBase");
    else
        ui->label_db->setText("DataBase Conectado...");

}

Login::~Login()
{
    delete ui;
   // connClose();

    qDebug() << "destrutor login";

}

void Login::on_pushButton_login_clicked()
{
    QString username, password;

    username=ui->lineEdit_user->text();
    password=ui->lineEdit_pass->text();

    if(!connOpen()){
        qDebug() << "Falha ao conectar Database";
        return;
    }
    else{
        qDebug() << "conectado";
    }

    connOpen();

    QSqlQuery qry;
    qry.prepare("select username, tipo, id_usuario, nome from usuario where username=? and password=?");
    qry.addBindValue(username);
    qry.addBindValue(password);

    int count=0, id_usuario=0, tipo;
    QString usuario, nome;

    if(qry.exec()){
      while(qry.next()){
        count++;
        usuario=qry.value(0).toString();
        tipo=qry.value(1).toInt();
        id_usuario=qry.value(2).toInt();
        nome=qry.value(3).toString();
      }
    }

    if(count==1){
      ui->label_db->setText("Usuário e senha corretos");

      connClose();

  //    MyGlobal.setuser(usuario, tipo, id_usuario);

      this->hide();

      user_logado=usuario;
      user_tipo=tipo;
      user_id=id_usuario;
      user_nome=nome;

      Principal principal;
      principal.setModal(true);

      qDebug() <<"login"<< usuario << tipo << id_usuario;

      qDebug() << "userlogado -"  << user_id;
      principal.exec();

    }

    if(count>1){
      ui->label_db->setText("Usuário e senha duplicados");
    }
    if(count==0){
      ui->label_db->setText("Usuário não encontrado");
    }
}
