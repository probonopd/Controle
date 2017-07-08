#include "controleglobal.h"
#include "login.h"
#include <QApplication>

#include "constants.h"

QString user_logado, user_status, user_nome;
int user_id=0, config=0, user_tipo;
QString funcao[NFUNC]={"ADMIN","GERENTE","CAIXA", "GARÇOM", "ENTREGADOR", "COZINHA"};
QString modo_pag[NPAG]={"DINHEIRO","CHEQUE","CARTÃO", "FIADO"};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    ControleGlobal MyGlobal;

    w.show();

    return a.exec();
}
