#include "pedidos.h"
#include "ui_pedidos.h"

extern int user_id;

Pedidos::Pedidos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pedidos)
{
    ui->setupUi(this);

    conn.connOpen();

    querypedidos=new QSqlQuery(conn.mydb);
    modalpedidos=new QSqlQueryModel();

    lerpedidos_user("","","", ST_ENTRADA, querypedidos, modalpedidos );
    mostrapedidos(ST_ENTRADA);

    querydetalhes=new QSqlQuery(conn.mydb);
    modaldetalhes=new QSqlQueryModel();

    querycolab=new QSqlQuery(conn.mydb);
    modalcolab=new QSqlQueryModel();
    lercolab();
    ui->comboBox_cozinha_2->addItem("Todos","");
    ui->comboBox_entrega->addItem("Balcão","Balcão");

    ui->comboBox_final->addItem("Todos","");
    ui->comboBox_final->addItem("Balcão","Balcão");

    for(int i=0;i<modalcolab->rowCount();i++){
        qDebug() << modalcolab->record(i).value(6).toInt();

        if(modalcolab->record(i).value(6).toInt()==F_COZINHA){
            ui->comboBox_cozinha->addItem(modalcolab->record(i).value(1).toString(),modalcolab->record(i).value(1).toString());
            ui->comboBox_cozinha_2->addItem(modalcolab->record(i).value(1).toString(),modalcolab->record(i).value(1).toString());
        }
        if(modalcolab->record(i).value(6).toInt()==F_ENTREGA){
            ui->comboBox_entrega->addItem(modalcolab->record(i).value(1).toString(),modalcolab->record(i).value(1).toString());
            ui->comboBox_final->addItem(modalcolab->record(i).value(1).toString(),modalcolab->record(i).value(1).toString());
        }
     }
  //  ui->comboBox_cozinha_2->setCurrentIndex(0);
    user_cozinha=ui->comboBox_cozinha->currentData().toString();
    user_entrega=ui->comboBox_entrega->currentData().toString();

    qDebug() << id_cozinha << user_cozinha;

    querypreparo=new QSqlQuery(conn.mydb);
    modalpreparo=new QSqlQueryModel();
    lerpedidos_user("", user_cozinha, "", ST_PREPARO, querypreparo, modalpreparo );
    mostrapedidos(ST_PREPARO);

   // ui->tableView_preparo->setModel(modalpreparo);

    querypreparodetalhe=new QSqlQuery(conn.mydb);
    modalpreparodetalhe=new QSqlQueryModel();

    queryentregadetalhe=new QSqlQuery(conn.mydb);
    modalentregadetalhe=new QSqlQueryModel();
    queryentrega=new QSqlQuery(conn.mydb);
    modalentrega=new QSqlQueryModel();
    lerpedidos_user("","","", ST_PRONTO, queryentrega, modalentrega );
    ui->tableView_entrega->setModel(modalentrega);
    mostrapedidos(ST_PRONTO);

    queryfinal=new QSqlQuery(conn.mydb);
    modalfinal=new QSqlQueryModel();
    lerpedidos_user("","","", ST_ENTREGUE, queryfinal, modalfinal );
    //ui->tableView_final->setModel(modalfinal);
    mostrapedidos(ST_ENTREGUE);

    queryfinaldetalhe=new QSqlQuery(conn.mydb);
    modalfinaldetalhe=new QSqlQueryModel();

}

Pedidos::~Pedidos()
{
    delete ui;

    conn.connClose();
}

void Pedidos::on_tableView_pedidos_activated(const QModelIndex &index)
{
    qDebug() << "on_tableView_endereco_activated";
    int linhas=ui->tableView_pedidos->model()->rowCount();

    if(!linhas)return;

    QString celula=ui->tableView_pedidos->model()->data(index).toString();
    int colunas=ui->tableView_pedidos->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();
    QModelIndexList indexList = ui->tableView_pedidos->selectionModel()->selectedIndexes();

    qDebug() << celula;
    qDebug() << linhas << colunas;
    qDebug() << linha;
    qDebug() << coluna << id_pedido << indexList.count();

    id_pedido=modalpedidos->record(linha).value(0).toInt();
    flagcozinha=linhas?true:false;
    flagentrega=modalpedidos->record(linha).value(3).toInt()==0?true:false;

    lerdetalhes(id_pedido, querydetalhes, modaldetalhes);
    //ui->tableView_detalhe->setModel(modaldetalhes);
    qDebug() << flagcozinha;

    showdetalhe(ST_ENTRADA);

}


void Pedidos::on_pushButton_marcarcozinha_clicked()
{
    if(flagcozinha){
        querypedidos->prepare("update pedido set user_cozinha=:user_cozinha, status=:status where id_pedido=:id_pedido");
        querypedidos->bindValue(":id_pedido", id_pedido);
        querypedidos->bindValue(":user_cozinha", user_cozinha);
        querypedidos->bindValue(":status", ST_PREPARO);
        querypedidos->exec();
        //ui->tableView_pedidos->setModel(modalpedidos);
        id_pedido=0;
        lerdetalhes(id_pedido, querydetalhes, modaldetalhes);
        showdetalhe(ST_ENTRADA);
        lerpedidos_user("","","",ST_ENTRADA, querypedidos, modalpedidos );
        lerpedidos_user("","","",ST_PREPARO, querypreparo, modalpreparo );

        mostrapedidos(ST_ENTRADA);

    }
}

void Pedidos::on_comboBox_cozinha_activated(const QString &arg1)
{
    user_cozinha=ui->comboBox_cozinha->currentData().toString();
}

void Pedidos::on_comboBox_entrega_activated(const QString &arg1)
{
    user_entrega=ui->comboBox_entrega->currentData().toString();
    qDebug() << user_entrega;
  //  lerpedidos_user(0,0, id_entrega, ST_PRONTO, queryentrega, modalentrega );
    mostrapedidos(ST_PRONTO);
}

void Pedidos::on_pushButton_entrega_clicked()
{
    QModelIndexList indexList = ui->tableView_entrega->selectionModel()->selectedIndexes();
    int row, linha=-1, balcao;
    float valorentrega;
    bool flag;
    QVariantList listpedido, listentrega, liststatus, list_balcao, list_valorentrega;

    queryentrega->prepare("update pedido set user_entrega=:user_entrega, status=:status,"
                          "       entrega=:entrega "
                          "where id_pedido=:id_pedido");
    foreach (QModelIndex index, indexList) {
            row = index.row();
            flag=false;
            if(linha!=row){
                flag=true;
                id_pedido=modalentrega->record(row).value(0).toInt();
                listpedido << id_pedido;
                listentrega << user_entrega;
                liststatus << ST_ENTREGUE;
                balcao=modalentrega->record(row).value(9).toInt();//id_endereco
                valorentrega=modalentrega->record(row).value(4).toFloat();//valor entrega
                if(balcao){// id_endereco dif de balcao
                    valorentrega=0.0;
                }
                list_valorentrega << valorentrega;
            }
            linha=row;
    }

    queryentrega->bindValue(":id_pedido", listpedido);
    queryentrega->bindValue(":user_entrega", listentrega);
    queryentrega->bindValue(":status", liststatus);
    queryentrega->bindValue(":entrega", list_valorentrega);
    queryentrega->execBatch();
    qDebug() << queryentrega->lastError().text();

    lerpedidos_user("","","", ST_PRONTO, queryentrega, modalentrega );
    ui->tableView_entrega->setModel(modalentrega);

    ui->tableWidget_entrega->clear();
    lerpedidos_user("","","",ST_ENTREGUE, queryfinal, modalfinal);
    mostrapedidos(ST_PRONTO);
}

void Pedidos::on_tableView_preparo_activated(const QModelIndex &index)
{
    qDebug() << "on_tableView_endereco_activated";

    QString celula=ui->tableView_preparo->model()->data(index).toString();
    int colunas=ui->tableView_preparo->model()->columnCount();
    int linhas=ui->tableView_preparo->model()->rowCount();
    int linha=index.row();
    int coluna=index.column();
    QModelIndexList indexList = ui->tableView_preparo->selectionModel()->selectedIndexes();

    id_preparo=modalpreparo->record(linha).value(0).toInt();

    lerdetalhes( id_preparo, querypreparodetalhe, modalpreparodetalhe);
    showdetalhe(ST_PREPARO);

    qDebug() << celula;
    qDebug() << linhas << colunas;
    qDebug() << linha;
    qDebug() << coluna << id_preparo << modalpreparodetalhe->rowCount();
}

void Pedidos::on_comboBox_cozinha_2_activated(const QString &arg1)
{
    //user_cozinha=ui->comboBox_cozinha_2->itemData(ui->comboBox_cozinha_2->currentIndex()).toString();
    user_cozinha=ui->comboBox_cozinha_2->currentData().toString();
    lerpedidos_user("",user_cozinha, "", ST_PREPARO, querypreparo, modalpreparo );
    mostrapedidos(ST_PREPARO);
    qDebug()<< user_cozinha << ui->comboBox_cozinha_2->currentIndex();
    ui->tableWidget_preparo->clear();
}


void Pedidos::on_pushButton_marcarcozinha_2_clicked()
{
    qDebug() << "on_pushButton_marcarcozinha_2_clicked";

        querypreparo->prepare("update pedido set status=:status where id_pedido=:id_pedido");
        querypreparo->bindValue(":id_pedido", id_preparo);
        querypreparo->bindValue(":status", ST_PRONTO);
        querypreparo->exec();
        lerpedidos_user("","","", ST_PREPARO, querypreparo, modalpreparo );
        ui->tableView_preparo->setModel(modalpreparo);
        id_pedido=0;
        lerdetalhes(id_pedido, querypreparodetalhe, modalpreparodetalhe);
        showdetalhe(ST_PREPARO);

        lerpedidos_user("","","",ST_PRONTO, queryentrega, modalentrega );
        mostrapedidos(ST_PRONTO);
}

void Pedidos::showdetalhe(int status){
    int linha, coluna, hora, item, qtde, nl, i, nitens;
    QString str;
    qDebug() << "showdetalhe";

    switch(status){

        case ST_ENTRADA:
            nl=modaldetalhes->rowCount();

            ui->tableWidget_detalhe->clear();
            ui->tableWidget_detalhe->setRowCount(nl);
            ui->tableWidget_detalhe->setColumnCount(COL_PREP);
            ui->tableWidget_detalhe->setHorizontalHeaderItem(WGDT_QTDE, new QTableWidgetItem("Qde"));
            ui->tableWidget_detalhe->setHorizontalHeaderItem(WGDT_ID, new QTableWidgetItem("ID"));
            ui->tableWidget_detalhe->setHorizontalHeaderItem(WGDT_ITEM, new QTableWidgetItem("Item"));
            ui->tableWidget_detalhe->setHorizontalHeaderItem(WGDT_TIPO, new QTableWidgetItem("Tipo"));
            ui->tableWidget_detalhe->setHorizontalHeaderItem(WGDT_TAM, new QTableWidgetItem("Tamanho"));
            ui->tableWidget_detalhe->setHorizontalHeaderItem(WGDT_OBS, new QTableWidgetItem("Obs."));
            hora=0;
            item=0;
            for(linha=0;linha<nl;linha++){
                item++;
                if(modaldetalhes->record(linha).value(COL_TIPO).toInt()==MENU){
                    if(hora!=modaldetalhes->record(linha).value(COL_HORA).toInt()){
                        nitens=1;
                        item=1;
                        hora=modaldetalhes->record(linha).value(COL_HORA).toInt();
                        for(i=linha+1;i<nl;i++){
                            if(hora==modaldetalhes->record(i).value(COL_HORA).toInt() && modaldetalhes->record(i).value(COL_TIPO).toInt()==MENU ){
                                nitens++;
                            }
                            else
                                break;
                        }
                    }
                }
                else{
                    nitens=1;
                }

                qDebug() << modaldetalhes->record(linha).value(COL_ITNOME).toString();

                if(modaldetalhes->record(linha).value(COL_TIPO).toInt()==MENU){
                    qDebug() << "menu" << nitens;
                    if(nitens>1){
                      str=QString("1/%1 ").arg(nitens)+modaldetalhes->record(linha).value(COL_ITNOME).toString();
                    }
                    else
                      str=modaldetalhes->record(linha).value(COL_ITNOME).toString();
                    ui->tableWidget_detalhe->setItem( linha, WGDT_ITEM, new QTableWidgetItem(str));

                    if(item==1){
                        ui->tableWidget_detalhe->setItem( linha, WGDT_QTDE , new QTableWidgetItem(modaldetalhes->record(linha).value(COL_ID).toString()));
                        ui->tableWidget_detalhe->setItem( linha, WGDT_QTDE , new QTableWidgetItem(modaldetalhes->record(linha).value(COL_QTDE).toString()));
                        ui->tableWidget_detalhe->setItem( linha, WGDT_OBS, new QTableWidgetItem(modaldetalhes->record(linha).value(COL_OBS).toString()));
                        ui->tableWidget_detalhe->setItem( linha, WGDT_TIPO, new QTableWidgetItem(modaldetalhes->record(linha).value(COL_ITMENU).toString()));
                        ui->tableWidget_detalhe->setItem( linha, WGDT_TAM, new QTableWidgetItem(modaldetalhes->record(linha).value(COL_TAM).toString()));
                    }
                    else{
                        ui->tableWidget_detalhe->setItem( linha,WGDT_ID, new QTableWidgetItem(""));
                        ui->tableWidget_detalhe->setItem( linha,WGDT_QTDE, new QTableWidgetItem(""));
                        ui->tableWidget_detalhe->setItem( linha,WGDT_OBS, new QTableWidgetItem(""));
                        ui->tableWidget_detalhe->setItem( linha,WGDT_TIPO, new QTableWidgetItem(""));
                        ui->tableWidget_detalhe->setItem( linha,WGDT_TAM, new QTableWidgetItem(""));
                    }
                }
                else{  // opcionais e adicionais
                    qDebug() << " adic";
                    ui->tableWidget_detalhe->setItem( linha,WGDT_ID, new QTableWidgetItem(""));
                    ui->tableWidget_detalhe->setItem( linha,WGDT_ITEM , new QTableWidgetItem(modaldetalhes->record(linha).value(COL_ITNOME).toString()));
                    ui->tableWidget_detalhe->setItem( linha,WGDT_QTDE, new QTableWidgetItem(""));
                    ui->tableWidget_detalhe->setItem( linha,WGDT_OBS , new QTableWidgetItem(""));
                    ui->tableWidget_detalhe->setItem( linha,WGDT_TAM , new QTableWidgetItem(""));
                    ui->tableWidget_detalhe->setItem( linha, WGDT_TIPO, new QTableWidgetItem(modaldetalhes->record(linha).value(COL_ITMENU).toString()));
                }
            }

            ui->tableWidget_detalhe->resizeColumnsToContents();
            break;

        case ST_PREPARO:
            nl=modalpreparodetalhe ->rowCount();

            qDebug()<<"show detalhe " << nl;

            ui->tableWidget_preparo->clear();
            ui->tableWidget_preparo->setRowCount(nl);
            ui->tableWidget_preparo->setColumnCount(COL_PREP);
            ui->tableWidget_preparo->setHorizontalHeaderItem(WGDT_QTDE, new QTableWidgetItem("Qde"));
            ui->tableWidget_preparo->setHorizontalHeaderItem(WGDT_ID, new QTableWidgetItem("ID"));
            ui->tableWidget_preparo->setHorizontalHeaderItem(WGDT_ITEM, new QTableWidgetItem("Item"));
            ui->tableWidget_preparo->setHorizontalHeaderItem(WGDT_TIPO, new QTableWidgetItem("Tipo"));
            ui->tableWidget_preparo->setHorizontalHeaderItem(WGDT_TAM, new QTableWidgetItem("Tamanho"));
            ui->tableWidget_preparo->setHorizontalHeaderItem(WGDT_OBS, new QTableWidgetItem("Obs."));
            hora=0;
            item=0;
            for(linha=0;linha<nl;linha++){
                item++;
                if(modalpreparodetalhe->record(linha).value(COL_TIPO).toInt()==MENU){
                    if(hora!=modalpreparodetalhe->record(linha).value(COL_HORA).toInt()){
                        nitens=1;
                        item=1;
                        hora=modalpreparodetalhe->record(linha).value(COL_HORA).toInt();
                        for(i=linha+1;i<nl;i++){
                            if(hora==modalpreparodetalhe->record(i).value(COL_HORA).toInt() && modalpreparodetalhe->record(i).value(COL_TIPO).toInt()==MENU ){
                                nitens++;
                            }
                            else
                                break;
                        }
                    }
                }
                else{
                    nitens=1;
                }

                qDebug() << modalpreparodetalhe->record(linha).value(COL_ITNOME).toString();

                if(modalpreparodetalhe->record(linha).value(COL_TIPO).toInt()==MENU){
                    qDebug() << "menu" << nitens;
                    if(nitens>1){
                      str=QString("1/%1 ").arg(nitens)+modalpreparodetalhe->record(linha).value(COL_ITNOME).toString();
                    }
                    else
                      str=modalpreparodetalhe->record(linha).value(COL_ITNOME).toString();
                    ui->tableWidget_preparo->setItem( linha, WGDT_ITEM, new QTableWidgetItem(str));

                    if(item==1){
                        ui->tableWidget_preparo->setItem( linha, WGDT_QTDE , new QTableWidgetItem(modalpreparodetalhe->record(linha).value(COL_ID).toString()));
                        ui->tableWidget_preparo->setItem( linha, WGDT_QTDE , new QTableWidgetItem(modalpreparodetalhe->record(linha).value(COL_QTDE).toString()));
                        ui->tableWidget_preparo->setItem( linha, WGDT_OBS, new QTableWidgetItem(modalpreparodetalhe->record(linha).value(COL_OBS).toString()));
                        ui->tableWidget_preparo->setItem( linha, WGDT_TIPO, new QTableWidgetItem(modalpreparodetalhe->record(linha).value(COL_ITMENU).toString()));
                        ui->tableWidget_preparo->setItem( linha, WGDT_TAM, new QTableWidgetItem(modalpreparodetalhe->record(linha).value(COL_TAM).toString()));
                    }
                    else{
                        ui->tableWidget_preparo->setItem( linha,WGDT_ID, new QTableWidgetItem(""));
                        ui->tableWidget_preparo->setItem( linha,WGDT_QTDE, new QTableWidgetItem(""));
                        ui->tableWidget_preparo->setItem( linha,WGDT_OBS, new QTableWidgetItem(""));
                        ui->tableWidget_preparo->setItem( linha,WGDT_TIPO, new QTableWidgetItem(""));
                        ui->tableWidget_preparo->setItem( linha,WGDT_TAM, new QTableWidgetItem(""));
                    }
                }
                else{  // opcionais e adicionais
                    qDebug() << " adic";
                    ui->tableWidget_preparo->setItem( linha,WGDT_ID, new QTableWidgetItem(""));
                    ui->tableWidget_preparo->setItem( linha,WGDT_ITEM , new QTableWidgetItem(modalpreparodetalhe->record(linha).value(COL_ITNOME).toString()));
                    ui->tableWidget_preparo->setItem( linha,WGDT_QTDE, new QTableWidgetItem(""));
                    ui->tableWidget_preparo->setItem( linha,WGDT_OBS , new QTableWidgetItem(""));
                    ui->tableWidget_preparo->setItem( linha,WGDT_TAM , new QTableWidgetItem(""));
                    ui->tableWidget_preparo->setItem( linha, WGDT_TIPO, new QTableWidgetItem(modalpreparodetalhe->record(linha).value(COL_ITMENU).toString()));
                }
            }

            ui->tableWidget_preparo->resizeColumnsToContents();
            break;

        //////
    case ST_PRONTO:
        nl=modalentregadetalhe ->rowCount();

        ui->tableWidget_entrega->clear();
        ui->tableWidget_entrega->setRowCount(nl);
        ui->tableWidget_entrega->setColumnCount(COL_PREP);
        ui->tableWidget_entrega->setHorizontalHeaderItem(WGDT_QTDE, new QTableWidgetItem("Qde"));
        ui->tableWidget_entrega->setHorizontalHeaderItem(WGDT_ID, new QTableWidgetItem("ID"));
        ui->tableWidget_entrega->setHorizontalHeaderItem(WGDT_ITEM, new QTableWidgetItem("Item"));
        ui->tableWidget_entrega->setHorizontalHeaderItem(WGDT_TIPO, new QTableWidgetItem("Tipo"));
        ui->tableWidget_entrega->setHorizontalHeaderItem(WGDT_TAM, new QTableWidgetItem("Tamanho"));
        ui->tableWidget_entrega->setHorizontalHeaderItem(WGDT_OBS, new QTableWidgetItem("Obs."));
        hora=0;
        item=0;
        for(linha=0;linha<nl;linha++){
            item++;
            if(modalentregadetalhe->record(linha).value(COL_TIPO).toInt()==MENU){
                if(hora!=modalentregadetalhe->record(linha).value(COL_HORA).toInt()){
                    nitens=1;
                    item=1;
                    hora=modalentregadetalhe->record(linha).value(COL_HORA).toInt();
                    for(i=linha+1;i<nl;i++){
                        if(hora==modalentregadetalhe->record(i).value(COL_HORA).toInt() && modalentregadetalhe->record(i).value(COL_TIPO).toInt()==MENU ){
                            nitens++;
                        }
                        else
                            break;
                    }
                }
            }
            else{
                nitens=1;
            }

            qDebug() << modalentregadetalhe->record(linha).value(COL_ITNOME).toString();

            if(modalentregadetalhe->record(linha).value(COL_TIPO).toInt()==MENU){
                qDebug() << "menu" << nitens;
                if(nitens>1){
                  str=QString("1/%1 ").arg(nitens)+modalentregadetalhe->record(linha).value(COL_ITNOME).toString();
                }
                else
                  str=modalentregadetalhe->record(linha).value(COL_ITNOME).toString();
                ui->tableWidget_entrega->setItem( linha, WGDT_ITEM, new QTableWidgetItem(str));

                if(item==1){
                    ui->tableWidget_entrega->setItem( linha, WGDT_QTDE , new QTableWidgetItem(modalentregadetalhe->record(linha).value(COL_ID).toString()));
                    ui->tableWidget_entrega->setItem( linha, WGDT_QTDE , new QTableWidgetItem(modalentregadetalhe->record(linha).value(COL_QTDE).toString()));
                    ui->tableWidget_entrega->setItem( linha, WGDT_OBS, new QTableWidgetItem(modalentregadetalhe->record(linha).value(COL_OBS).toString()));
                    ui->tableWidget_entrega->setItem( linha, WGDT_TIPO, new QTableWidgetItem(modalentregadetalhe->record(linha).value(COL_ITMENU).toString()));
                    ui->tableWidget_entrega->setItem( linha, WGDT_TAM, new QTableWidgetItem(modalentregadetalhe->record(linha).value(COL_TAM).toString()));
                }
                else{
                    ui->tableWidget_entrega->setItem( linha,WGDT_ID, new QTableWidgetItem(""));
                    ui->tableWidget_entrega->setItem( linha,WGDT_QTDE, new QTableWidgetItem(""));
                    ui->tableWidget_entrega->setItem( linha,WGDT_OBS, new QTableWidgetItem(""));
                    ui->tableWidget_entrega->setItem( linha,WGDT_TIPO, new QTableWidgetItem(""));
                    ui->tableWidget_entrega->setItem( linha,WGDT_TAM, new QTableWidgetItem(""));
                }
            }
            else{  // opcionais e adicionais
                qDebug() << " adic";
                ui->tableWidget_entrega->setItem( linha,WGDT_ID, new QTableWidgetItem(""));
                ui->tableWidget_entrega->setItem( linha,WGDT_ITEM , new QTableWidgetItem(modalentregadetalhe->record(linha).value(COL_ITNOME).toString()));
                ui->tableWidget_entrega->setItem( linha,WGDT_QTDE, new QTableWidgetItem(""));
                ui->tableWidget_entrega->setItem( linha,WGDT_OBS , new QTableWidgetItem(""));
                ui->tableWidget_entrega->setItem( linha,WGDT_TAM , new QTableWidgetItem(""));
                ui->tableWidget_entrega->setItem( linha, WGDT_TIPO, new QTableWidgetItem(modalentregadetalhe->record(linha).value(COL_ITMENU).toString()));
            }
        }
        ui->tableWidget_entrega->resizeColumnsToContents();
        break;

    case ST_ENTREGUE:
        nl=modalfinaldetalhe ->rowCount();

        ui->tableWidget_final->clear();
        ui->tableWidget_final->setRowCount(nl);
        ui->tableWidget_final->setColumnCount(COL_PREP);
        ui->tableWidget_final->setHorizontalHeaderItem(WGDT_QTDE, new QTableWidgetItem("Qde"));
        ui->tableWidget_final->setHorizontalHeaderItem(WGDT_ID, new QTableWidgetItem("ID"));
        ui->tableWidget_final->setHorizontalHeaderItem(WGDT_ITEM, new QTableWidgetItem("Item"));
        ui->tableWidget_final->setHorizontalHeaderItem(WGDT_TIPO, new QTableWidgetItem("Tipo"));
        ui->tableWidget_final->setHorizontalHeaderItem(WGDT_TAM, new QTableWidgetItem("Tamanho"));
        ui->tableWidget_final->setHorizontalHeaderItem(WGDT_OBS, new QTableWidgetItem("Obs."));
        hora=0;
        item=0;
        for(linha=0;linha<nl;linha++){
            item++;
            if(modalfinaldetalhe->record(linha).value(COL_TIPO).toInt()==MENU){
                if(hora!=modalfinaldetalhe->record(linha).value(COL_HORA).toInt()){
                    nitens=1;
                    item=1;
                    hora=modalfinaldetalhe->record(linha).value(COL_HORA).toInt();
                    for(i=linha+1;i<nl;i++){
                        if(hora==modalfinaldetalhe->record(i).value(COL_HORA).toInt() && modalfinaldetalhe->record(i).value(COL_TIPO).toInt()==MENU ){
                            nitens++;
                        }
                        else
                            break;
                    }
                }
            }
            else{
                nitens=1;
            }

            qDebug() << modalfinaldetalhe->record(linha).value(COL_ITNOME).toString();

            if(modalfinaldetalhe->record(linha).value(COL_TIPO).toInt()==MENU){
                qDebug() << "menu" << nitens;
                if(nitens>1){
                  str=QString("1/%1 ").arg(nitens)+modalfinaldetalhe->record(linha).value(COL_ITNOME).toString();
                }
                else
                  str=modalfinaldetalhe->record(linha).value(COL_ITNOME).toString();
                ui->tableWidget_final->setItem( linha, WGDT_ITEM, new QTableWidgetItem(str));

                if(item==1){
                    ui->tableWidget_final->setItem( linha, WGDT_QTDE , new QTableWidgetItem(modalfinaldetalhe->record(linha).value(COL_ID).toString()));
                    ui->tableWidget_final->setItem( linha, WGDT_QTDE , new QTableWidgetItem(modalfinaldetalhe->record(linha).value(COL_QTDE).toString()));
                    ui->tableWidget_final->setItem( linha, WGDT_OBS, new QTableWidgetItem(modalfinaldetalhe->record(linha).value(COL_OBS).toString()));
                    ui->tableWidget_final->setItem( linha, WGDT_TIPO, new QTableWidgetItem(modalfinaldetalhe->record(linha).value(COL_ITMENU).toString()));
                    ui->tableWidget_final->setItem( linha, WGDT_TAM, new QTableWidgetItem(modalfinaldetalhe->record(linha).value(COL_TAM).toString()));
                }
                else{
                    ui->tableWidget_final->setItem( linha,WGDT_ID, new QTableWidgetItem(""));
                    ui->tableWidget_final->setItem( linha,WGDT_QTDE, new QTableWidgetItem(""));
                    ui->tableWidget_final->setItem( linha,WGDT_OBS, new QTableWidgetItem(""));
                    ui->tableWidget_final->setItem( linha,WGDT_TIPO, new QTableWidgetItem(""));
                    ui->tableWidget_final->setItem( linha,WGDT_TAM, new QTableWidgetItem(""));
                }
            }
            else{  // opcionais e adicionais
                qDebug() << " adic";
                ui->tableWidget_final->setItem( linha,WGDT_ID, new QTableWidgetItem(""));
                ui->tableWidget_final->setItem( linha,WGDT_ITEM , new QTableWidgetItem(modalfinaldetalhe->record(linha).value(COL_ITNOME).toString()));
                ui->tableWidget_final->setItem( linha,WGDT_QTDE, new QTableWidgetItem(""));
                ui->tableWidget_final->setItem( linha,WGDT_OBS , new QTableWidgetItem(""));
                ui->tableWidget_final->setItem( linha,WGDT_TAM , new QTableWidgetItem(""));
                ui->tableWidget_final->setItem( linha, WGDT_TIPO, new QTableWidgetItem(modalfinaldetalhe->record(linha).value(COL_ITMENU).toString()));
            }
        }
        ui->tableWidget_final->resizeColumnsToContents();
        break;
    }
}

void Pedidos::mostrapedidos(int status){
    switch (status) {
        case ST_ENTRADA:
            modalpedidos->setHeaderData(0, Qt::Horizontal, tr("Pedido"));
            modalpedidos->setHeaderData(1, Qt::Horizontal, tr("Caixa"));
            modalpedidos->setHeaderData(2, Qt::Horizontal, tr("Bairro"));
            modalpedidos->setHeaderData(3, Qt::Horizontal, tr("Desconto (R$)"));
            modalpedidos->setHeaderData(4, Qt::Horizontal, tr("Entrega (R$)"));
            modalpedidos->setHeaderData(5, Qt::Horizontal, tr("Total (R$)"));
            modalpedidos->setHeaderData(6, Qt::Horizontal, tr("Pagamento"));
            modalpedidos->setHeaderData(7, Qt::Horizontal, tr("Troco"));
            ui->tableView_pedidos->setModel(modalpedidos);
            ui->tableView_pedidos->setColumnHidden(9,true);
            ui->tableView_pedidos->resizeColumnsToContents();
            ui->tableView_pedidos->setItemDelegateForColumn(3, new NumberFormatDelegate(this));
            ui->tableView_pedidos->setItemDelegateForColumn(4, new NumberFormatDelegate(this));
            ui->tableView_pedidos->setItemDelegateForColumn(4, new NumberFormatDelegate(this));
            ui->tableView_pedidos->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
            ui->tableView_pedidos->setItemDelegateForColumn(6, new ModoPagtoDelegate(this));
            ui->tableView_pedidos->setItemDelegateForColumn(8, new TimeFormatDelegate(this));
            break;
        case ST_PREPARO:
            modalpreparo->setHeaderData(0, Qt::Horizontal, tr("Pedido"));
            modalpreparo->setHeaderData(1, Qt::Horizontal, tr("Cozinha"));
            modalpreparo->setHeaderData(2, Qt::Horizontal, tr("Bairro"));
            modalpreparo->setHeaderData(3, Qt::Horizontal, tr("Desconto (R$)"));
            modalpreparo->setHeaderData(4, Qt::Horizontal, tr("Entrega (R$)"));
            modalpreparo->setHeaderData(5, Qt::Horizontal, tr("Total (R$)"));
            modalpreparo->setHeaderData(6, Qt::Horizontal, tr("Pagamento"));
            modalpreparo->setHeaderData(7, Qt::Horizontal, tr("Troco"));
            ui->tableView_preparo->setModel(modalpreparo);
            ui->tableView_preparo->setColumnHidden(9,true);
            ui->tableView_preparo->resizeColumnsToContents();
            ui->tableView_preparo->setItemDelegateForColumn(3, new NumberFormatDelegate(this));
            ui->tableView_preparo->setItemDelegateForColumn(4, new NumberFormatDelegate(this));
            ui->tableView_preparo->setItemDelegateForColumn(4, new NumberFormatDelegate(this));
            ui->tableView_preparo->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
            ui->tableView_preparo->setItemDelegateForColumn(6, new ModoPagtoDelegate(this));
            ui->tableView_preparo->setItemDelegateForColumn(8, new TimeFormatDelegate(this));
            break;
    case ST_PRONTO:
        modalentrega->setHeaderData(0, Qt::Horizontal, tr("Pedido"));
        modalentrega->setHeaderData(1, Qt::Horizontal, tr("Entregador"));
        modalentrega->setHeaderData(2, Qt::Horizontal, tr("Bairro"));
        modalentrega->setHeaderData(3, Qt::Horizontal, tr("Desconto (R$)"));
        modalentrega->setHeaderData(4, Qt::Horizontal, tr("Entrega (R$)"));
        modalentrega->setHeaderData(5, Qt::Horizontal, tr("Total (R$)"));
        modalentrega->setHeaderData(6, Qt::Horizontal, tr("Pagamento"));
        modalentrega->setHeaderData(7, Qt::Horizontal, tr("Troco"));
        ui->tableView_entrega->setModel(modalentrega);
        ui->tableView_entrega->setColumnHidden(9,true);
        ui->tableView_entrega->resizeColumnsToContents();
        ui->tableView_entrega->setItemDelegateForColumn(3, new NumberFormatDelegate(this));
        ui->tableView_entrega->setItemDelegateForColumn(4, new NumberFormatDelegate(this));
        ui->tableView_entrega->setItemDelegateForColumn(4, new NumberFormatDelegate(this));
        ui->tableView_entrega->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
        ui->tableView_entrega->setItemDelegateForColumn(6, new ModoPagtoDelegate(this));
        ui->tableView_entrega->setItemDelegateForColumn(8, new TimeFormatDelegate(this));
        break;
    case ST_ENTREGUE:
        modalfinal->setHeaderData(0, Qt::Horizontal, tr("Pedido"));
        modalfinal->setHeaderData(1, Qt::Horizontal, tr("Entregador"));
        modalfinal->setHeaderData(2, Qt::Horizontal, tr("Bairro"));
        modalfinal->setHeaderData(3, Qt::Horizontal, tr("Desconto (R$)"));
        modalfinal->setHeaderData(4, Qt::Horizontal, tr("Entrega (R$)"));
        modalfinal->setHeaderData(5, Qt::Horizontal, tr("Total (R$)"));
        modalfinal->setHeaderData(6, Qt::Horizontal, tr("Pagamento"));
        modalfinal->setHeaderData(7, Qt::Horizontal, tr("Troco"));
        ui->tableView_final->setModel(modalfinal);
        ui->tableView_final->setColumnHidden(8,true);
        ui->tableView_final->setColumnHidden(9,true);
        ui->tableView_final->resizeColumnsToContents();
        ui->tableView_final->setItemDelegateForColumn(3, new NumberFormatDelegate(this));
        ui->tableView_final->setItemDelegateForColumn(4, new NumberFormatDelegate(this));
        ui->tableView_final->setItemDelegateForColumn(4, new NumberFormatDelegate(this));
        ui->tableView_final->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
        ui->tableView_final->setItemDelegateForColumn(6, new ModoPagtoDelegate(this));
     //   ui->tableView_final->setItemDelegateForColumn(8, new TimeFormatDelegate(this));
        break;
     }
}

void Pedidos::on_tableView_entrega_activated(const QModelIndex &index)
{
    qDebug() << "on_tableView_endereco_activated";

    QString celula=ui->tableView_entrega->model()->data(index).toString();
    int colunas=ui->tableView_entrega->model()->columnCount();
    int linhas=ui->tableView_entrega->model()->rowCount();
    int linha=index.row();
    int coluna=index.column();
    QModelIndexList indexList = ui->tableView_entrega->selectionModel()->selectedIndexes();

        id_entrega=modalentrega->record(linha).value(0).toInt();

        lerdetalhes( id_entrega, queryentregadetalhe, modalentregadetalhe);
        showdetalhe(ST_PRONTO);

        qDebug() << celula;
        qDebug() << linhas << colunas << id_entrega;
        qDebug() << linha;
        qDebug() << coluna << flagcozinha;

}

void Pedidos::on_tableView_final_activated(const QModelIndex &index)
{
    qDebug() << "on_tableView_final_activated";

    QString celula=ui->tableView_final->model()->data(index).toString();
    int colunas=ui->tableView_final->model()->columnCount();
    int linhas=ui->tableView_final->model()->rowCount();
    int linha=index.row();
    int coluna=index.column();
    QModelIndexList indexList = ui->tableView_final->selectionModel()->selectedIndexes();

        int id_final=modalfinal->record(linha).value(0).toInt();

        lerdetalhes( id_final, queryfinaldetalhe, modalfinaldetalhe);
        showdetalhe(ST_ENTREGUE);

        qDebug() << celula;
        qDebug() << linhas << colunas << id_final;
        qDebug() << linha;
        qDebug() << coluna;

}

void Pedidos::on_comboBox_final_activated(const QString &arg1)
{
    user_entrega=ui->comboBox_final->currentData().toString();
    qDebug() << user_entrega;
    lerpedidos_user("","", user_entrega, ST_ENTREGUE, queryfinal, modalfinal );
    mostrapedidos(ST_ENTREGUE);

    ui->tableWidget_final->clear();
}
