#include "menu.h"
#include "ui_menu.h"
#include "qmessagebox.h"

Menu::Menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
    conn.connOpen();

    modalmenu=new QSqlQueryModel();
    querymenu=new QSqlQuery(conn.mydb);
    lermenu();
    //ui->listView_menu->setModel(modalmenu);
    //ui->tableView_menu->setModel(modalmenu);

    QString str;
    int nl=modalmenu->rowCount();
    ui->tableWidget_menu->setRowCount(nl);
    ui->tableWidget_menu->setColumnCount(4);

    ui->tableWidget_menu->setHorizontalHeaderItem(0, new QTableWidgetItem("Nome Menu"));
    ui->tableWidget_menu->setHorizontalHeaderItem(1, new QTableWidgetItem("Tipo Menu"));
    ui->tableWidget_menu->setHorizontalHeaderItem(2, new QTableWidgetItem("Seleções"));
    ui->tableWidget_menu->setHorizontalHeaderItem(3, new QTableWidgetItem("Ordem"));

    for (int r=0; r<nl; r++)
       for (int c=1; c<5; c++){
          str=modalmenu->record(r).value(c).toString();
          if(c==2){
              switch (str.toInt()) {
              case 1:
                  str="Menu";
                  break;
              case 2:
                  str="Adicional";
                  break;
              case 4:
                  str="Opcional";
                  break;
              }
          }
          ui->tableWidget_menu->setItem( r, c-1, new QTableWidgetItem(str));
       }
}

Menu::~Menu()
{
    delete ui;
    conn.close();
}

void Menu::on_pushButton_cancelar_clicked()
{
    close();
}

void Menu::on_pushButton_novo_clicked()
{
    QString opc_str;

    if((ui->radioButton_menu->isChecked() || ui->radioButton_adicional->isChecked() || ui->radioButton_opcional->isChecked()) && ui->lineEdit_menu->text()!=""){

        if(id_menu){
            querymenu->prepare("update menu set tipomenu=:tipomenu, itemmenu=:itemmenu, opcoes=:opcoes, sequencia=:sequencia where id_menu=:id_menu");
            querymenu->bindValue(":id_menu",id_menu);
        }
        else
            querymenu->prepare("insert into menu (itemmenu, tipomenu, opcoes, sequencia) values (:itemmenu, :tipomenu, :opcoes, :sequencia)");
        int tipomenu=1;

        ui->radioButton_menu->setChecked(false);
        if(ui->radioButton_menu->isChecked()){
            opcoes=0;
        }

        if(ui->radioButton_adicional->isChecked()){
            tipomenu=2; // adicional
            opc_str=ui->lineEdit_maxsel->text();
            opcoes=opc_str.toInt();
            ui->radioButton_adicional->setChecked(false);
        }
        else if(ui->radioButton_opcional->isChecked()){
            tipomenu=4;  // opcional
            opcoes=1;
            ui->radioButton_opcional->setChecked(false);
        }

        querymenu->bindValue(":tipomenu",tipomenu);
        querymenu->bindValue(":itemmenu", ui->lineEdit_menu->text());
        querymenu->bindValue(":opcoes", opcoes);
        querymenu->bindValue(":sequencia", ui->lineEdit_sequencia->text());
        querymenu->exec();

        qDebug()<<id_menu<< querymenu->lastError().text();

        lermenu();

        ui->tableWidget_menu->clear();
        QString str;
        int nl=modalmenu->rowCount();
        ui->tableWidget_menu->setRowCount(nl);
        ui->tableWidget_menu->setColumnCount(4);

        ui->tableWidget_menu->setHorizontalHeaderItem(0, new QTableWidgetItem("Nome Menu"));
        ui->tableWidget_menu->setHorizontalHeaderItem(1, new QTableWidgetItem("Tipo Menu"));
        ui->tableWidget_menu->setHorizontalHeaderItem(2, new QTableWidgetItem("Seleções"));
        ui->tableWidget_menu->setHorizontalHeaderItem(3, new QTableWidgetItem("Ordem"));

        for (int r=0; r<nl; r++)
           for (int c=1; c<5; c++){
              str=modalmenu->record(r).value(c).toString();
              if(c==2){
                  switch (str.toInt()) {
                  case 1:
                      str="Menu";
                      break;
                  case 2:
                      str="Adicional";
                      break;
                  case 4:
                      str="Opcional";
                      break;
                  }
              }
              ui->tableWidget_menu->setItem( r, c-1, new QTableWidgetItem(str));
           }

        ui->lineEdit_menu->setText("");
        ui->lineEdit_maxsel->setText("");
        ui->lineEdit_sequencia->setText("");
        ui->pushButton_novo->setText("Novo");
        id_menu=0;

    }
    else{
        QMessageBox::critical(this,tr("Selecionar tipo"),tr("Favor selecionar um tipo!"));
    }
}

void Menu::on_pushButton_atualizar_clicked()
{
    ui->lineEdit_menu->setText("");
    ui->lineEdit_maxsel->setText("");
    ui->lineEdit_sequencia->setText("");
    ui->pushButton_novo->setText("Novo");
    id_menu=0;

}

void Menu::on_radioButton_menu_clicked()
{
    ui->lineEdit_maxsel->setEnabled(false);
}

void Menu::on_radioButton_adicional_clicked()
{
    ui->lineEdit_maxsel->setEnabled(true);
}

void Menu::on_radioButton_opcional_clicked()
{
    ui->lineEdit_maxsel->setEnabled(false);
}

void Menu::on_pushButton_apagar_clicked()
{
    QModelIndexList indexList = ui->tableWidget_menu->selectionModel()->selectedIndexes();
    int idmenu, row;

    if (QMessageBox::No == QMessageBox(QMessageBox::Information, "Apagar Menu!", "Confirma?", QMessageBox::Yes|QMessageBox::No).exec())
        return;

    querymenu->prepare("delete from menu where id_menu=:id_menu");
    QVariantList listmenu;

    foreach (QModelIndex index, indexList) {
        row = index.row();
        qDebug() << row << index.column();
        idmenu=modalmenu->record(row).value(0).toInt();
        listmenu << idmenu;
    }
    querymenu->bindValue(":id_menu", listmenu);

    querymenu->execBatch();

    qDebug() << querymenu->lastError().text();

    lermenu();
    ui->tableWidget_menu->clear();
    QString str;
    int nl=modalmenu->rowCount();
    ui->tableWidget_menu->setRowCount(nl);
    ui->tableWidget_menu->setColumnCount(3);

    ui->tableWidget_menu->setHorizontalHeaderItem(0, new QTableWidgetItem("Nome Menu"));
    ui->tableWidget_menu->setHorizontalHeaderItem(1, new QTableWidgetItem("Tipo Menu"));
    ui->tableWidget_menu->setHorizontalHeaderItem(2, new QTableWidgetItem("Seleções"));

    for (int r=0; r<nl; r++)
       for (int c=1; c<4; c++){
          str=modalmenu->record(r).value(c).toString();
          if(c==2){
              switch (str.toInt()) {
              case 1:
                  str="Menu";
                  break;
              case 2:
                  str="Adicional";
                  break;
              case 4:
                  str="Opcional";
                  break;
              }
          }
          ui->tableWidget_menu->setItem( r, c-1, new QTableWidgetItem(str));
       }

    ui->lineEdit_menu->setText("");

    ui->radioButton_menu->setChecked(false);
    ui->radioButton_adicional->setChecked(false);
    ui->radioButton_opcional->setChecked(false);

}

void Menu::on_tableWidget_menu_activated(const QModelIndex &index)
{
    QString celula=ui->tableWidget_menu->model()->data(index).toString();
    int colunas=ui->tableWidget_menu->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();
//    int opcoes;

    id_menu=modalmenu->record(linha).value(0).toInt();
  //  opcoes=modalmenu->record(linha).value(2).toInt();
    ui->lineEdit_menu->setText(modalmenu->record(linha).value(1).toString());
    ui->lineEdit_maxsel->setText(modalmenu->record(linha).value(3).toString());
    ui->lineEdit_sequencia->setText(modalmenu->record(linha).value(4).toString());

    //qDebug() << (opcoes & 2);

    switch (modalmenu->record(linha).value(2).toInt()) {
    case 1:
       ui->radioButton_menu->setChecked(true);
       ui->lineEdit_maxsel->setEnabled(false);
        break;
    case 2:
       ui->radioButton_adicional->setChecked(true);
        break;
    case 4:
       ui->radioButton_opcional->setChecked(true);
        break;

    }

    ui->pushButton_novo->setText("Atualizar");

}
