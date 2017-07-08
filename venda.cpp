#include "venda.h"
#include "ui_venda.h"
#include "qmessagebox.h"
#include "numberformatdelegate.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include "qprintpreviewdialog.h"

Venda::Venda(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Venda)
{
    ui->setupUi(this);
    conn.connOpen();

    modaltipo=new QSqlQueryModel();
    querytipo=new QSqlQuery(conn.mydb);
    letipo();
    ui->comboBox_tipo->setModel(modaltipo);

    filtrar="";

    modalmenu=new QSqlQueryModel();
    querymenu2=new QSqlQuery(conn.mydb);
    querymenu2->prepare("select itemmenu, id_menu from menu "
                        "where tipomenu=1 "
                        "order by sequencia asc");
    querymenu2->exec();
    modalmenu->setQuery(*querymenu2);
    ui->comboBox_menu->setModel(modalmenu);
    id_menu=modalmenu->record(0).value(1).toInt();

    modaltamanho=new QSqlQueryModel();
    querytamanho=new QSqlQuery(conn.mydb);
    querytamanho->prepare("select tamanho, id_tamanho, selecao from tamanho");
    querytamanho->exec();
    modaltamanho->setQuery(*querytamanho);
    ui->comboBox_tamanho->setModel(modaltamanho);
    id_tamanho=modaltamanho->record(0).value(1).toInt();
    max_sel=modaltamanho->record(0).value(2).toInt();
    ui->label_maxsel->setText(QString("%1").arg(max_sel));

    modalcardapio=new QSqlQueryModel();
    querycardapio=new QSqlQuery(conn.mydb);

    lecardapio();

    modalcardapio->setHeaderData(1, Qt::Horizontal, tr("ID"));
    modalcardapio->setHeaderData(2, Qt::Horizontal, tr("Item"));
    modalcardapio->setHeaderData(3, Qt::Horizontal, tr("Descrição"));
    modalcardapio->setHeaderData(6, Qt::Horizontal, tr("Preço"));

    ui->tableView_cardapio->setModel(modalcardapio);
    ui->tableView_cardapio->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
    ui->tableView_cardapio->setColumnHidden(0,true);
    ui->tableView_cardapio->setColumnHidden(4,true);
    ui->tableView_cardapio->setColumnHidden(6,true);
    ui->tableView_cardapio->setColumnHidden(7,true);
    ui->tableView_cardapio->resizeColumnsToContents();

    qDebug() << querycardapio->lastError().text();

    modalsubmenu=new QSqlQueryModel();
    querysubmenu=new QSqlQuery(conn.mydb);

    modaladicionais=new QSqlQueryModel();
    queryadicionais=new QSqlQuery(conn.mydb);

    cliente=new QSqlQuery(conn.mydb);
    modalcliente=new QSqlQueryModel();

    endereco=new QSqlQuery(conn.mydb);
    modalendereco=new QSqlQueryModel();

    ui->label_desconto->setText(descontostr.sprintf("%8.2f", desconto));

    querycart=new QSqlQuery(conn.mydb);
    modalcart=new QSqlQueryModel();

    qDebug()<< "vendas" << user_id;

    lecart();
    showcart();
    modalcart->setHeaderData(4, Qt::Horizontal, tr("Qde"));
    modalcart->setHeaderData(5, Qt::Horizontal, tr("Item"));
    modalcart->setHeaderData(6, Qt::Horizontal, tr("Tipo"));
    modalcart->setHeaderData(7, Qt::Horizontal, tr("Tamanho"));
    modalcart->setHeaderData(8, Qt::Horizontal, tr("Obs."));
    modalcart->setHeaderData(9, Qt::Horizontal, tr("Valor (R$)"));

   // ui->tableView_cart->setModel(modalcart);
  //  ui->tableView_cart->setItemDelegateForColumn(9, new NumberFormatDelegate(this));
    /*ui->tableView_cart->setColumnHidden(0,true);
    ui->tableView_cart->setColumnHidden(1,true);
    ui->tableView_cart->setColumnHidden(2,true);
    ui->tableView_cart->setColumnHidden(3,true);
    ui->tableView_cart->resizeColumnsToContents();*/
    showcart();
    calculatotal();
    ui->label_total->setText(totalstr.sprintf("%8.2f", total));
    ui->label_desconto->setText(descontostr.sprintf("%8.2f", desconto));
    ui->label_valorentrega->setText(entregastr.sprintf("%8.2f", taxa_entrega));

    queryentrega=new QSqlQuery(conn.mydb);
    modalentrega=new QSqlQueryModel();

    querypedido=new QSqlQuery(conn.mydb);
    modalpedido=new QSqlQueryModel();

    ui->spinBox_qtde->setRange(1, 10);
    ui->spinBox_qtdecardapio->setRange(1, 10);

}

Venda::~Venda()
{
    delete ui;

    qDebug() << "destrutor venda";

    conn.close();

}

void Venda::on_comboBox_menu_activated(const QString &arg1)
{

    int linha=0;

    while(modalmenu->record(linha).value(0).toString()!=arg1)
        linha++;

    id_menu=modalmenu->record(linha).value(1).toInt();

    lecardapio();

    modalcardapio->setHeaderData(1, Qt::Horizontal, tr("ID"));
    modalcardapio->setHeaderData(2, Qt::Horizontal, tr("Item"));
    modalcardapio->setHeaderData(3, Qt::Horizontal, tr("Descrição"));
    modalcardapio->setHeaderData(6, Qt::Horizontal, tr("Preço"));

    ui->tableView_cardapio->setModel(modalcardapio);
    ui->tableView_cardapio->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
    ui->tableView_cardapio->setColumnHidden(0,true);
    ui->tableView_cardapio->setColumnHidden(4,true);
    ui->tableView_cardapio->setColumnHidden(6,true);
    ui->tableView_cardapio->setColumnHidden(7,true);
    ui->tableView_cardapio->resizeColumnsToContents();

}

void Venda::on_comboBox_tamanho_activated(const QString &arg1)
{
    int linha=0, nl;

    while(modaltamanho->record(linha).value(0).toString()!=arg1)
        linha++;

    id_tamanho=modaltamanho->record(linha).value(1).toInt();
    max_sel=modaltamanho->record(linha).value(2).toInt();

    lecardapio();

    modalcardapio->setHeaderData(1, Qt::Horizontal, tr("ID"));
    modalcardapio->setHeaderData(2, Qt::Horizontal, tr("Item"));
    modalcardapio->setHeaderData(3, Qt::Horizontal, tr("Descrição"));
    modalcardapio->setHeaderData(6, Qt::Horizontal, tr("Preço"));

    ui->tableView_cardapio->setModel(modalcardapio);
    ui->tableView_cardapio->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
    ui->tableView_cardapio->setColumnHidden(0,true);
    ui->tableView_cardapio->setColumnHidden(4,true);
    ui->tableView_cardapio->setColumnHidden(6,true);
    ui->tableView_cardapio->setColumnHidden(7,true);
    ui->tableView_cardapio->resizeColumnsToContents();

    ui->label_maxsel->setText(QString("%1").arg(max_sel));

    // limpar tabela
    limparselecoes();
}

void Venda::on_pushButton_filtrar_clicked()
{
    filtrar=ui->lineEdit_filtrar->text();

    lecardapio();

    modalcardapio->setHeaderData(1, Qt::Horizontal, tr("ID"));
    modalcardapio->setHeaderData(2, Qt::Horizontal, tr("Item"));
    modalcardapio->setHeaderData(3, Qt::Horizontal, tr("Descrição"));
    modalcardapio->setHeaderData(6, Qt::Horizontal, tr("Preço"));

    ui->tableView_cardapio->setModel(modalcardapio);
    ui->tableView_cardapio->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
    ui->tableView_cardapio->setColumnHidden(0,true);
    ui->tableView_cardapio->setColumnHidden(4,true);
    ui->tableView_cardapio->setColumnHidden(6,true);
    ui->tableView_cardapio->setColumnHidden(7,true);
    ui->tableView_cardapio->resizeColumnsToContents();

}

void Venda::on_tableView_cardapio_activated(const QModelIndex &index)
{
    qDebug() << "tableView_cardapio_activated";

    QString celula=ui->tableView_cardapio->model()->data(index).toString();

    int colunas=ui->tableView_cardapio->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

/*
    ui->tableWidget_selecao->setRowCount(3);
    ui->tableWidget_selecao->setColumnCount(3);

    for (int r=0; r<3; r++)
         for (int c=0; c<3; c++)
              ui->tableWidget_selecao->setItem( r, c, new QTableWidgetItem("stringData(r,c)"));
*/
}

void Venda::on_pushButton_inserir_clicked()
{
    int anterior, i, j, row, column, nlinhas, novas, nitens;
    bool insert;

    QModelIndexList indexList = ui->tableView_cardapio->selectionModel()->selectedIndexes();

    QString str, idnovo, id;

    nlinhas=ui->tableWidget_selecao->rowCount();
    novas=0;
    anterior=-1;
    foreach (QModelIndex index, indexList) {
        row=index.row();
        if(anterior!=row)
            novas++;
        anterior=row;
    }

    nitens=0;
    for(i=0;i<nlinhas;i++){
        if(ui->tableWidget_selecao->item(i,6)->text()=="1")
            nitens++;
    }

    qDebug() << "insert" << nlinhas << novas;

    if(nitens+novas>max_sel){
        QMessageBox::critical(this,tr("Seleção"),tr("Máximo de seleções excedidos"));
        return;
    }

    anterior=-1;
    foreach (QModelIndex index, indexList) {
        insert=true;
        row=index.row();
        column=index.column();
        if(anterior==row)
            insert=false;
        anterior=row;

        if(nlinhas){
            insert=true;
            idnovo=modalcardapio->record(row).value(1).toString();
            for(j=0;j<nlinhas;j++){
                id=ui->tableWidget_selecao->item(j,1)->text();
                if(idnovo==id)
                    insert=false;
            }
        }

        if(insert){
            if(!ui->tableWidget_selecao->rowCount()){  // vai inserir primeira linha
                ui->tableWidget_selecao->clear();
                ui->tableWidget_selecao->insertRow(0);//insere na linha 0
                ui->tableWidget_selecao->setColumnCount(NUM_ITENS); // cria colunas
                ui->tableWidget_selecao->setHorizontalHeaderItem(1, new QTableWidgetItem("ID"));
                ui->tableWidget_selecao->setHorizontalHeaderItem(2, new QTableWidgetItem("Item"));
                ui->tableWidget_selecao->setHorizontalHeaderItem(4, new QTableWidgetItem("Tipo"));
/*                ui->tableWidget_selecao->hideColumn(0);
                ui->tableWidget_selecao->hideColumn(3);
                ui->tableWidget_selecao->hideColumn(6);
                ui->tableWidget_selecao->hideColumn(7);// tipomenu menu adc opc
  */
            }
            else{
                ui->tableWidget_selecao->insertRow(0);//insere na linha 0
                ui->tableWidget_selecao->setColumnCount(NUM_ITENS); // cria 6 colunas
            }

            for(j=0;j<NUM_ITENS;j++){
                str=modalcardapio->record(row).value(j).toString();
                ui->tableWidget_selecao->setItem( 0, j, new QTableWidgetItem(str));

            }
        }
    }
    ui->tableWidget_selecao->setItemDelegateForColumn(5, new NumberFormatDelegate(this));

}

void Venda::on_pushButton_apagar_clicked()
{
    QModelIndexList indexList = ui->tableWidget_selecao->selectionModel()->selectedIndexes();
    int i, nl;
    bool flag=false;


    foreach (QModelIndex index, indexList) {
        ui->tableWidget_selecao->removeRow(index.row());
        flag=true;
    }
    if(!flag){
      QMessageBox::critical(this,tr("Apagar item"),tr("Nenhum item selecionado"));
      return;
    }

    qDebug() << ui->tableWidget_selecao->rowCount();

    flag=false;
    nl=ui->tableWidget_selecao->rowCount();
    for(i=0;i<nl;i++){
        if(ui->tableWidget_selecao->item(i,7)->text()=="1"){  // item de menu
            flag=true;
            break;
         }
         qDebug() << i << flag;
    }
    if(!flag){  // se nenhum item de menu selecionado apaga opcionais
        for(i=0;i<nl;i++) {
            ui->tableWidget_selecao->removeRow(0);
        }
    }
    maxseladc=0;
    ui->label_maxseladc->setText("[max]");
    modaladicionais->setQuery(*queryadicionais);

    if(!ui->tableWidget_selecao->rowCount()){
        id_menuadc=0;
        leadicionais();
        ui->label_maxseladc->setText("[max]");
        ui->textEdit_obs->setText("");
        id_cardapio=0;
        lesubmenu();

    }
}

void Venda::limparselecoes(){
    int nl;

    qDebug() << "limpar selecao";
    // limpar tabela
    nl=ui->tableWidget_selecao->rowCount();
    for(int i=0;i<nl;i++)
        ui->tableWidget_selecao->removeRow(0);

    id_cardapio=0;
    lesubmenu();

    id_menuadc=0;
    leadicionais();

}

void Venda::on_tableWidget_selecao_activated(const QModelIndex &index)
{
    QString celula=ui->tableWidget_selecao->model()->data(index).toString();
    QString item;

    int colunas=ui->tableWidget_selecao->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << colunas << linha << coluna;

   // id_cardapio=modalcardapio->record(linha).value(0).toInt();
     item=ui->tableWidget_selecao->item(linha, 0)->text();
     id_cardapio=item.toInt();

    lesubmenu();
    ui->listView_menuadicionais->setModel(modalsubmenu);

    id_menuadc=0;
    leadicionais();
    ui->label_maxseladc->setText("[max]");

}

void Venda::on_listView_menuadicionais_activated(const QModelIndex &index)
{
    QString celula=ui->tableWidget_selecao->model()->data(index).toString();

    int colunas=ui->tableWidget_selecao->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();

    qDebug() << colunas << linha << coluna;

    id_menuadc=modalsubmenu->record(linha).value(1).toInt();
    tipomenu=modalsubmenu->record(linha).value(2).toString();
    maxseladc=modalsubmenu->record(linha).value(3).toInt();

    qDebug() << colunas << linha << coluna << id_menuadc << tipomenu;

    leadicionais();
    modaladicionais->setHeaderData(1, Qt::Horizontal, tr("ID"));
    modaladicionais->setHeaderData(2, Qt::Horizontal, tr("Adicional"));
    modaladicionais->setHeaderData(5, Qt::Horizontal, tr("R$"));

    ui->tableView_itensadicionais->setModel(modaladicionais);
    ui->tableView_itensadicionais->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
    ui->tableView_itensadicionais->setColumnHidden(0,true);
    ui->tableView_itensadicionais->setColumnHidden(3,true);
    ui->tableView_itensadicionais->setColumnHidden(4,true);
    ui->tableView_itensadicionais->setColumnHidden(6,true);
    ui->tableView_itensadicionais->setColumnHidden(7,true);
    ui->tableView_itensadicionais->resizeColumnsToContents();

    ui->label_maxseladc->setText(QString("%1").arg(maxseladc));

}

void Venda::on_pushButton_inseriradicionais_clicked()
{
    qDebug() << "inserir adic" << ui->tableView_itensadicionais->currentIndex().column();

    if(ui->tableView_itensadicionais->currentIndex().column()<0){
        QMessageBox::critical(this,tr("Seleção"),tr("Nenhum item selecionado"));
        return;
    }

    QModelIndexList indexList = ui->tableView_itensadicionais->selectionModel()->selectedIndexes();
    int anterior, j, k, row, column, nlinhas=ui->tableWidget_selecao->rowCount(), novas, totalsel, temp;
    bool insert;

    QString str, idnovo, id, tiposubmenu;

    novas=indexList.count();
    qDebug() << "inserir adic" << novas;


    if(novas>maxseladc){
        QMessageBox::critical(this,tr("Seleção"),tr("Máximo de seleções excedidos"));
        return;
    }

    anterior=-1;
    foreach (QModelIndex index, indexList) {
        insert=true;
        row=index.row();
        column=index.column();

        if(anterior==row){
            insert=false;
        }
        anterior=row;

        idnovo=modaladicionais->record(row).value(0).toString();
        tiposubmenu=modaladicionais->record(row).value(4).toString();

        qDebug() << "novas" << row << column;

        if(nlinhas){

            totalsel=0;
            for(j=0;j<nlinhas;j++){
               id=ui->tableWidget_selecao->item(j,0)->text();
               str=ui->tableWidget_selecao->item(j,6)->text();
               temp=str.toInt();
                if(temp==modaladicionais->record(row).value(6).toInt())
                    totalsel++;
                if(idnovo==id){
                    insert=false;
                    break;
                }

                qDebug() << "totalsel" << totalsel << temp << modaladicionais->record(row).value(6).toInt();

            }
            if(totalsel>=maxseladc){
                insert=false;
                QMessageBox::critical(this,tr("Adicionais/Opcionais"),tr("Máximo de seleções excedidos"));
            }

        }

        if(insert){

            for(k=0;k<ui->tableWidget_selecao->rowCount();k++){
              if(ui->tableWidget_selecao->item(k,6)->text()==modaladicionais->record(row).value(6).toString()){
                  qDebug() << "achou linha " << k;
                  break; //achou a linha
              }
            }
            ui->tableWidget_selecao->insertRow(k);
            ui->tableWidget_selecao->setColumnCount(NUM_ITENS);

            for(j=0;j<NUM_ITENS;j++){
                str=modaladicionais->record(row).value(j).toString();
                ui->tableWidget_selecao->setItem( k, j, new QTableWidgetItem(str));

            }
        }
    }
}

void Venda::on_comboBox_tipo_activated(const QString &arg1)
{
    tipostr=arg1;

    lecardapio();

    modalcardapio->setHeaderData(1, Qt::Horizontal, tr("ID"));
    modalcardapio->setHeaderData(2, Qt::Horizontal, tr("Item"));
    modalcardapio->setHeaderData(3, Qt::Horizontal, tr("Descrição"));
    modalcardapio->setHeaderData(6, Qt::Horizontal, tr("Preço"));

    ui->tableView_cardapio->setModel(modalcardapio);
    ui->tableView_cardapio->setItemDelegateForColumn(5, new NumberFormatDelegate(this));
    ui->tableView_cardapio->setColumnHidden(0,true);
    ui->tableView_cardapio->setColumnHidden(4,true);
    ui->tableView_cardapio->setColumnHidden(6,true);
    ui->tableView_cardapio->setColumnHidden(7,true);
    ui->tableView_cardapio->resizeColumnsToContents();

}


void Venda::on_pushButton_procurarcliente_clicked()
{
    QString procura=ui->lineEdit_procurarcliente->text();
    qDebug() << "on_pushButton_procurarcliente";

    qDebug() << procura;

    cliente->prepare("SELECT cliente.nome, cliente.fone, cliente.celular, id_cliente \
                   FROM cliente \
                   WHERE (cliente.nome like :procura ) or\
                         (cliente.fone like :procura ) or\
                         (cliente.celular like :procura ) ");
    cliente->bindValue( ":procura", "%" + procura + "%" );

    cliente->exec();

    modalcliente->setQuery(*cliente);
    ui->listView_procurarcliente->setModel(modalcliente);
    //ui->tableView_clientes->setColumnHidden(0,true);

}

void Venda::on_listView_procurarcliente_activated(const QModelIndex &index)
{
    qDebug() << "on_listView_procurarcliente_activated";

    QString celula=ui->listView_procurarcliente->model()->data(index).toString();
    int colunas=ui->listView_procurarcliente->model()->columnCount();
    int linha=index.row();
    int coluna=index.column();
    int idnovo;

    qDebug() << celula;
    qDebug() << colunas;
    qDebug() << linha;
    qDebug() << coluna;

    if(CartEmUso)
        return;

    idnovo=modalcliente->record(linha).value(3).toInt();
    if(idnovo!=id_cliente){
        id_cliente=idnovo;
        ui->label_nomecliente->setText(modalcliente->record(linha).value(0).toString());
        ui->label_fonecliente->setText(modalcliente->record(linha).value(1).toString());
        ui->label_ruacliente->setText("[rua");
        ui->label_bairrocliente->setText("[bairro]");
        id_endereco=0;

        lerendereco(id_cliente);
        modalendereco->setHeaderData(2, Qt::Horizontal, tr("Rua"));
        modalendereco->setHeaderData(3, Qt::Horizontal, tr("Número"));
        modalendereco->setHeaderData(4, Qt::Horizontal, tr("Bloco"));
        modalendereco->setHeaderData(5, Qt::Horizontal, tr("Apto"));
        modalendereco->setHeaderData(6, Qt::Horizontal, tr("Bairro"));
        modalendereco->setHeaderData(7, Qt::Horizontal, tr("CEP"));
        modalendereco->setHeaderData(8, Qt::Horizontal, tr("Cidade"));

        ui->tableView_endereco->setModel(modalendereco);
        ui->tableView_endereco->setColumnHidden(0,true);
        ui->tableView_endereco->setColumnHidden(1,true);
        ui->tableView_endereco->setColumnHidden(9,true);
        ui->tableView_endereco->setColumnHidden(10,true);
        ui->tableView_endereco->setColumnHidden(11,true);
        ui->tableView_endereco->resizeColumnsToContents();

  }

    valorentrega=0;
    calculatotal();
    ui->label_total->setText(totalstr.sprintf("%8.2f", total));
    ui->label_desconto->setText(descontostr.sprintf("%8.2f", desconto));
    ui->label_valorentrega->setText(entregastr.sprintf("%8.2f", taxa_entrega));

}

void Venda::on_pushButton_carrinho_clicked()
{
    int nl;
    QVariantList list_iduser, list_idcliente, list_idendereco, list_idcardapio;
    QVariantList list_quantidade, list_obs, list_datetime, list_preco;
    QString str;
    QDateTime current = QDateTime::currentDateTime();

    if(!id_endereco || !id_cliente){
        QMessageBox::critical(this,tr("Cliente"),tr("Verifique Cliente ou Endereço"));
        return;
    }
    nl=ui->tableWidget_selecao->rowCount();
    if(!nl){
        QMessageBox::critical(this,tr("Itens"),tr("Nenhum item para adicionar"));
        return;
    }

    qDebug() << user_logado << user_id;

    querycart->prepare("insert into cart (id_usuario, id_cliente, id_endereco, id_cardapio, qde, obs, hora, preco)"
                       "values (:id_usuario, :id_cliente, :id_endereco, :id_cardapio, :quantidade, :observacao, :timestamp, :preco) ");

    for(int k=0;k<nl;k++){
        list_iduser << user_id;
        list_idcliente << id_cliente;
        list_idendereco << id_endereco;
        str=ui->tableWidget_selecao->item(k,0)->text();
        list_idcardapio << str;
        list_quantidade << ui->spinBox_qtdecardapio->value();
        str=ui->textEdit_obs->toPlainText();
        list_obs << str;
        list_datetime << current.toTime_t();
        list_preco << ui->tableWidget_selecao->item(k,5)->text();
    }

    querycart->bindValue(":id_usuario",list_iduser);
    querycart->bindValue(":id_cliente",list_idcliente);
    querycart->bindValue(":id_endereco",list_idendereco);
    querycart->bindValue(":id_cardapio",list_idcardapio);
    querycart->bindValue(":quantidade",list_quantidade);
    querycart->bindValue(":observacao",list_obs);
    querycart->bindValue(":timestamp",list_datetime);
    querycart->bindValue(":preco",list_preco);

    if (!querycart->execBatch()) {
        qDebug() << "erro " << querycart->lastError();
    } else {
        qDebug() << "sucesso " << querycart->executedQuery();

        ui->textEdit_obs->setText("");
        id_cardapio=0;
        id_menuadc=0;
        lesubmenu();
        leadicionais();
        for(int k=0;k<nl;k++)
            ui->tableWidget_selecao->removeRow(0);

        CartEmUso=true;

        lecart();

        showcart();

        calculatotal();
        ui->label_total->setText(totalstr.sprintf("%8.2f", total));
        ui->label_desconto->setText(descontostr.sprintf("%8.2f", desconto));
        ui->label_valorentrega->setText(entregastr.sprintf("%8.2f", taxa_entrega));
    }
}

void Venda::showcart()
{
    int nl=modalcart->rowCount(), i, nitens;
    int linha, coluna, hora, item, qtde;
    QString str, strpreco;
    float maior=0.0, soma=0.0, preco, total, extra;

    ui->tableWidget_cart->clear();
    ui->tableWidget_cart->setRowCount(nl);
    ui->tableWidget_cart->setColumnCount(COL_CART); // cria colunas
    ui->tableWidget_cart->setHorizontalHeaderItem(0, new QTableWidgetItem("Qde"));
    ui->tableWidget_cart->setHorizontalHeaderItem(1, new QTableWidgetItem("Item"));
    ui->tableWidget_cart->setHorizontalHeaderItem(2, new QTableWidgetItem("Tipo"));
    ui->tableWidget_cart->setHorizontalHeaderItem(3, new QTableWidgetItem("Tamanho"));
    ui->tableWidget_cart->setHorizontalHeaderItem(4, new QTableWidgetItem("Obs."));
    ui->tableWidget_cart->setHorizontalHeaderItem(5, new QTableWidgetItem("Valor Unit."));
    ui->tableWidget_cart->setHorizontalHeaderItem(6, new QTableWidgetItem("Valor Item"));
    hora=0;
    item=0;
    for(linha=0;linha<nl;linha++){
        item++;
        if(modalcart->record(linha).value(CART_MENU).toInt()==MENU){
            if(hora!=modalcart->record(linha).value(CART_HORA).toInt()){
                nitens=1;
                item=1;
                soma=modalcart->record(linha).value(CART_PRECO).toFloat();
                maior=soma;
                hora=modalcart->record(linha).value(CART_HORA).toInt();
                for(i=linha+1;i<nl;i++){
                    if(hora==modalcart->record(i).value(CART_HORA).toInt() && modalcart->record(i).value(CART_MENU).toInt()==MENU ){
                        soma+=modalcart->record(i).value(CART_PRECO).toFloat();
                        if(modalcart->record(i).value(CART_PRECO).toFloat()>maior)
                            maior=modalcart->record(i).value(CART_PRECO).toFloat();
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
        if(modalcart->record(linha).value(CART_MENU).toInt()==MENU){

            if(!(config && (1<<SELECOES))){
                preco=maior;
            }
            else{
                preco=soma/nitens;
            }
            total=modalcart->record(linha).value(CART_QDE).toInt()*preco;
            strpreco=QString("%1").arg(preco);

            if(nitens>1){
              str=QString("1/%1 ").arg(nitens)+modalcart->record(linha).value(CART_ITEM).toString();
            }
            else
                str=modalcart->record(linha).value(CART_ITEM).toString();
            ui->tableWidget_cart->setItem( linha, CART_ITEM-CART_OFFSET, new QTableWidgetItem(str));

            if(item==1){
                ui->tableWidget_cart->setItem( linha, CART_QDE-CART_OFFSET, new QTableWidgetItem(modalcart->record(linha).value(CART_QDE).toString()));
                ui->tableWidget_cart->setItem( linha, CART_OBS-CART_OFFSET, new QTableWidgetItem(modalcart->record(linha).value(CART_OBS).toString()));
                ui->tableWidget_cart->setItem( linha, CART_TIPO-CART_OFFSET, new QTableWidgetItem(modalcart->record(linha).value(CART_TIPO).toString()));
                ui->tableWidget_cart->setItem( linha, CART_TAMANHO-CART_OFFSET, new QTableWidgetItem(modalcart->record(linha).value(CART_TAMANHO).toString()));
                ui->tableWidget_cart->setItem( linha, CART_PRECO-CART_OFFSET, new QTableWidgetItem(strpreco));
                ui->tableWidget_cart->setItem( linha, 6, new QTableWidgetItem(QString("%1").arg(total)));
            }
            else{
                ui->tableWidget_cart->setItem( linha, CART_QDE-CART_OFFSET, new QTableWidgetItem(""));
                ui->tableWidget_cart->setItem( linha, CART_OBS-CART_OFFSET, new QTableWidgetItem(""));
                ui->tableWidget_cart->setItem( linha, CART_TIPO-CART_OFFSET, new QTableWidgetItem(""));
                ui->tableWidget_cart->setItem( linha, CART_TAMANHO-CART_OFFSET, new QTableWidgetItem(""));
                ui->tableWidget_cart->setItem( linha, CART_PRECO-CART_OFFSET, new QTableWidgetItem(""));
                ui->tableWidget_cart->setItem( linha, 6, new QTableWidgetItem(""));
            }
        }
        else{  // opcionais e adicionais
            qtde=modalcart->record(linha).value(CART_QDE).toInt();
            extra=modalcart->record(linha).value(CART_PRECO).toFloat();
            ui->tableWidget_cart->setItem( linha, CART_ITEM-CART_OFFSET, new QTableWidgetItem(modalcart->record(linha).value(CART_ITEM).toString()));
            ui->tableWidget_cart->setItem( linha, CART_QDE-CART_OFFSET, new QTableWidgetItem(""));
            ui->tableWidget_cart->setItem( linha, CART_OBS-CART_OFFSET, new QTableWidgetItem(""));
            ui->tableWidget_cart->setItem( linha, CART_TIPO-CART_OFFSET, new QTableWidgetItem(modalcart->record(linha).value(CART_TIPO).toString()));
            ui->tableWidget_cart->setItem( linha, CART_TAMANHO-CART_OFFSET, new QTableWidgetItem(""));
            ui->tableWidget_cart->setItem( linha, CART_PRECO-CART_OFFSET, new QTableWidgetItem(QString("%1").arg(extra)));
            ui->tableWidget_cart->setItem( linha, 6, new QTableWidgetItem(QString("%1").arg(extra*qtde)));
        }
    }
    ui->tableWidget_cart->setItemDelegateForColumn(CART_PRECO-CART_OFFSET, new NumberFormatDelegate(this));
    ui->tableWidget_cart->setItemDelegateForColumn(6, new NumberFormatDelegate(this));
    ui->tableWidget_cart->resizeColumnsToContents();

}

void Venda::on_pushButton_desconto_clicked()
{
    QVariantList list_iduser, list_idcliente, list_idendereco, list_idcardapio, list_quantidade, list_obs;
    QString str;

    descontostr=ui->lineEdit_desconto->text();
    desconto=descontostr.toFloat();

    calculatotal();

    ui->label_total->setText(totalstr.sprintf("%8.2f", total));
    ui->label_desconto->setText(descontostr.sprintf("%8.2f", desconto));
    ui->label_valorentrega->setText(entregastr.sprintf("%8.2f", taxa_entrega));

    ui->lineEdit_desconto->setText("");

    qDebug() << desconto;

}

void Venda::on_pushButton_apagaritemcarrinho_clicked()
{
    //QModelIndexList indexList = ui->tableView_cart->selectionModel()->selectedIndexes();
    QModelIndexList indexList = ui->tableWidget_cart->selectionModel()->selectedIndexes();

    qDebug() << indexList.count();

    if(!indexList.count()){
        QMessageBox::critical(this,tr("Seleção"),tr("Nenhum item selecionado"));
        return;
    }

    cart_apagaitem(indexList);

    lecart();
    showcart();

   // ui->tableView_cart->setModel(modalcart);

    calculatotal();

    ui->label_total->setText(totalstr.sprintf("%8.2f", total));
    ui->label_desconto->setText(descontostr.sprintf("%8.2f", desconto));
    ui->label_valorentrega->setText(entregastr.sprintf("%8.2f", taxa_entrega));
}

void Venda::on_pushButton_finalizarcompra_clicked()
{
    QDateTime current = QDateTime::currentDateTime();
    int lastid, idcardapio, hora, qtde;
    float preco;
    QString obs;
    QVariantList list_idpedido, list_idcardapio, list_hora, list_qtde, list_obs, list_preco, list_cart;

    if(!flag_entrega){
        QMessageBox::critical(this,tr("Entrega"),tr("Favor Selecionar modo de entrega!"));
        return;
    }

    if(!id_endereco){
        QMessageBox::critical(this,tr("Endereço"),tr("Favor Selecionar um endereço!"));
        return;
    }
    if(!pagamento){
        QMessageBox::critical(this,tr("Modo de pagamento"),tr("Selecione modo de pagamento!"));
        return;
    }

    conn.mydb.transaction();

    querypedido->prepare("insert into pedido (user_caixa, id_endereco, desconto, entrega, total, horario, pagamento, troco )"
                         "values (:user_caixa, :id_endereco, :desconto, :entrega, :total, :horario, :pagamento, :troco) ");

    querypedido->bindValue(":user_caixa",user_logado);
    querypedido->bindValue(":id_endereco",id_endereco);
    querypedido->bindValue(":desconto",desconto);
    querypedido->bindValue(":entrega",valorentrega);
    querypedido->bindValue(":total",total);
    querypedido->bindValue(":horario",current.toTime_t());
    querypedido->bindValue(":pagamento",pagamento);
    querypedido->bindValue(":troco",trocostr);


        querypedido->exec();
        querypedido->prepare("select max(id_pedido) from pedido where user_caixa=:user_caixa");
        querypedido->bindValue(":user_caixa",user_logado);
        querypedido->exec();
        modalpedido->setQuery(*querypedido);
        lastid=modalpedido->record(0).value(0).toInt();

        querypedido->prepare("insert into pedido_detalhe (id_pedido, id_cardapio, hora, qtde, obs, preco  )"
                             "values (:id_pedido, :id_cardapio, :hora, :qtde, :obs, :preco) ");
        for(int i=0;i<modalcart->rowCount();i++){
            list_idpedido << lastid;
            list_idcardapio << modalcart->record(i).value(1).toInt();
            list_hora << modalcart->record(i).value(3).toInt();
            list_qtde << modalcart->record(i).value(4).toInt();
            list_obs << modalcart->record(i).value(8).toString();
            list_preco << modalcart->record(i).value(9).toFloat();
        }
        querypedido->bindValue(":id_pedido",list_idpedido);
        querypedido->bindValue(":id_cardapio",list_idcardapio);
        querypedido->bindValue(":hora",list_hora);
        querypedido->bindValue(":qtde",list_qtde);
        querypedido->bindValue(":obs",list_obs);
        querypedido->bindValue(":preco",list_preco);
        if(querypedido->execBatch()){
            conn.mydb.commit();

            for(int i=0;i<modalcart->rowCount();i++){
                list_cart << modalcart->record(i).value(0).toInt();
            }
            querycart->prepare("delete FROM cart where id_cart=:id_cart");
            querycart->bindValue(":id_cart",list_cart);
            querycart->execBatch();

            QMessageBox::critical(this,tr("Sucesso"),tr("Pedido inserido com sucesso!"));

            lecart();
            showcart();
          //  ui->tableView_cart->setModel(modalcart);

        }
        else{
            conn.mydb.rollback();
            QMessageBox::critical(this,tr("Falha ao gerar pedido"),tr("Falha ao gravar pedido!!!"));
        }

    qDebug()<<"insert" << querypedido->lastError().text() << lastid;
}

void Venda::on_pushButton_troco_clicked()
{
    trocostr=ui->lineEdit_troco->text();
    ui->label_troco->setText(trocostr);
}

void Venda::on_radioButton_dinheiro_clicked()
{
    pagamento=PAG_DINHEIRO;

    qDebug() << "dinheiro";
}

void Venda::on_radioButton_cartao_clicked()
{
    pagamento=PAG_CARTAO;
}

void Venda::on_radioButton_cheque_clicked()
{
    pagamento=PAG_CHEQUE;
}

void Venda::on_radioButton_fiado_clicked()
{
    pagamento=PAG_FIADO;
}


void Venda::on_radioButton_entregar_clicked()
{
    flag_entrega=true;
    taxa_entrega=valorentrega;
    calculatotal();
    ui->label_total->setText(totalstr.sprintf("%8.2f", total));
    ui->label_desconto->setText(descontostr.sprintf("%8.2f", desconto));
    ui->label_valorentrega->setText(entregastr.sprintf("%8.2f", taxa_entrega));
}

void Venda::on_radioButton_balcao_clicked()
{
    flag_entrega=true;
    taxa_entrega=0.0;
    calculatotal();

    ui->label_total->setText(totalstr.sprintf("%8.2f", total));
    ui->label_desconto->setText(descontostr.sprintf("%8.2f", desconto));
    ui->label_valorentrega->setText(entregastr.sprintf("%8.2f", taxa_entrega));
}

void Venda::on_tableView_cart_activated(const QModelIndex &index)
{
    int row=index.row(), column=index.column();

    ui->spinBox_qtde->setValue(modalcart->record(row).value(4).toInt());
    horaitem=modalcart->record(row).value(3).toInt();

}

void Venda::on_pushButton_qtde_clicked()
{
    int novaqte=ui->spinBox_qtde->value();
    QVariantList list_item, list_id;

    for(int i=0;i<modalcart->rowCount();i++){
        if(modalcart->record(i).value(3).toInt()==horaitem){
            list_item << novaqte;
            list_id << modalcart->record(i).value(0).toInt();
        }
    }
    querycart->prepare("update cart set qde=:qtde where id_cart=:id_cart ");
    querycart->bindValue(":id_cart", list_id);
    querycart->bindValue(":qtde", list_item);
    querycart->execBatch();
    lecart();
    showcart();

    calculatotal();
    ui->label_total->setText(totalstr.sprintf("%8.2f", total));
    ui->label_desconto->setText(descontostr.sprintf("%8.2f", desconto));
    ui->label_valorentrega->setText(entregastr.sprintf("%8.2f", taxa_entrega));

}

void Venda::on_pushButton_selecionarcliente_clicked()
{
    int linha;
    qDebug() << "on_tableView_endereco_activated";

    if(!modalendereco->rowCount()){
        QMessageBox::critical(this,tr("Seleção"),tr("Nenhum cliente ou endereço cadastrado selecionado!"));
        return;
    }

    QModelIndexList indexList = ui->tableView_endereco->selectionModel()->selectedIndexes();

    qDebug() << indexList.count();

    if(indexList.count()==1){
        foreach (QModelIndex index, indexList) {
            linha=index.row();
        }
        ui->label_ruacliente->setText(modalendereco->record(linha).value(2).toString());
        ui->label_bairrocliente->setText(modalendereco->record(linha).value(6).toString());
        id_endereco=modalendereco->record(linha).value(0).toInt();
        cep=modalendereco->record(linha).value(7).toInt();
        bairro=modalendereco->record(linha).value(10).toInt();
        qDebug() << "bairro" << bairro;

        levalorentrega();

        calculatotal();
        ui->label_total->setText(totalstr.sprintf("%8.2f", total));
        ui->label_desconto->setText(descontostr.sprintf("%8.2f", desconto));
        ui->label_valorentrega->setText(entregastr.sprintf("%8.2f", taxa_entrega));

        id_endereco=modalendereco->record(linha).value(0).toInt();
    }
    else{
        QMessageBox::critical(this,tr("Seleção"),tr("Erro na seleção de endereço!"));
    }

}

void Venda::on_pushButton_limparcliente_clicked()
{
    ui->label_nomecliente->setText("");
    ui->label_fonecliente->setText("");
    ui->label_ruacliente->setText("[rua");
    ui->label_bairrocliente->setText("[bairro]");
    id_endereco=0;
    id_cliente=0;

    ui->listView_procurarcliente->clearSelection();
    ui->tableView_endereco->clearSelection();

}

void Venda::on_tableWidget_cart_activated(const QModelIndex &index)
{
    int row=index.row(), column=index.column();

    ui->spinBox_qtde->setValue(modalcart->record(row).value(4).toInt());
    horaitem=modalcart->record(row).value(3).toInt();

}

void Venda::on_pushButton_clicked()
{

    QString text ="\n"
            "1PRINT BY PRINTER            sectetur adipisicing elit, sed do\n"
            "2eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut\n"
            "3eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut\n"
            "4nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor\n"
            "5in reprehenderit in voluptate velit esse cillum dolore eu fugiat\n"
            "6nulla pariatur. Excepteur sint occaecat cupidatat non proident,\n"
            "7sunt in culpa qui officia deserunt mollit anim id est laborum.\n"
            "8Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do\n"
            "9eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut\n"
            "20sunt in culpa qui officia deserunt mollit anim      testao.\n";

    QPrinter printer;
     printer.setFullPage(true); // must be before setmargin
     qDebug() << "new margins:" << printer.pageLayout().margins();
     printer.setPageMargins(QMarginsF(00,00,00,00));
     qDebug() << "new margins:" << printer.pageLayout().margins() << printer.pageRect().size();
    // printer.setPageMargins(left, right, bottom, top, QPrinter::Millimeter );
    // printer.getPageMargins(&left, &top, &right, &bottom, QPrinter::Millimeter);
    // qDebug() << left << right << top << bottom;


 //   printer.setPageSize(QPrinter::A4);
 //   printer.setOrientation(QPrinter::Portrait);
 //   qreal left=0.0, right=0.0, top=0.0, bottom=0.0;
 //   printer.setPageMargins(left, right, bottom, top, QPrinter::Millimeter );

  //  printer.getPageMargins(&left, &top, &right, &bottom, QPrinter::Point);
   // qDebug() << left << right << top << bottom;

    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Imprimir Nota");

    //qDebug()<<


   /* //  no margins here!!!!
        QPainter painter;
        painter.begin(&printer)

        painter.drawText(0, 0, 1000, 300, Qt::AlignLeft|Qt::AlignTop, text);

        painter.end();
*/

    if (dialog->exec() == QDialog::Rejected) return;
/*
    QPainter painter(this);
    painter.begin(&printer);
    painter.drawText(0, 0, 100, 50, Qt::AlignLeft, "print by Painter");
    painter.end();
*/
/* test ok mas com margem
    QTextEdit teste3;

    teste3.setPlainText(text);
    teste3.print(&printer);
*/
    const qreal PT_MM = 25.4/72.0;

    QTextDocument *document = new QTextDocument();
    document->setPageSize(QSizeF(printer.pageRect().size()));
    document->setDocumentMargin(5.0/PT_MM);
    document->setPlainText(text);
    document->print(&printer);

}
