#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>

#include "constants.h"
#include "controleglobal.h"

extern ControleGlobal MyGlobal;

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    QSqlDatabase mydb;

    void connClose(){
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
    }

    bool connOpen(){

        mydb=QSqlDatabase::addDatabase("QMYSQL");
        mydb.setDatabaseName("controle_restaurante");

        mydb.setHostName("localhost");
        mydb.setPort(3306);
        mydb.setUserName("root");
        mydb.setPassword("ncc1701");

        if(!mydb.open()){
            qDebug() << "Falha ao conectar ao DataBase";
            return false;
        }
        else{
            qDebug() << ("Conectado...");
            return true;
        }

    }

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_pushButton_login_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
