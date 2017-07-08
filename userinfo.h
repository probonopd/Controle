#ifndef USERINFO_H
#define USERINFO_H

#include <QDialog>
#include "constants.h"
#include "login.h"
#include "jobfunctioncolumndelegate.h"

extern QString funcao[NFUNC];

namespace Ui {
class Userinfo;
}

class Userinfo : public QDialog
{
    Q_OBJECT

public:
    explicit Userinfo(QWidget *parent = 0);
    ~Userinfo();

    void lerusuario(){
        queryuser->prepare("select * from usuario where nome like :filtro");
        queryuser->bindValue( ":filtro", "%"+filtro+"%" );
        queryuser->exec();
        modeluser->setQuery(*queryuser);

        qDebug()<<queryuser->lastError().text();
    }


private slots:
    void on_pushButton_save_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_pushButton_delete_clicked();

    void on_comboBox_funcao_activated(const QString &arg1);

    void on_pushButton_limpar_clicked();

private:
    Ui::Userinfo *ui;
    Login conn;
    QSqlQuery *queryuser;
    QSqlQueryModel *modeluser;
    QString filtro="";
    int id_user=0, index_funcao;
};

#endif // USERINFO_H
