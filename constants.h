#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NFUNC 6

#define MENU       1
#define ADICIONAL  2
#define OPCIONAL   4

#define F_ADMIN   1
#define F_GERENTE 2
#define F_CAIXA   3
#define F_GARCOM  4
#define F_ENTREGA 5
#define F_COZINHA 6

//STATUS DE PEDIDO
#define ST_ENTRADA  0
#define ST_PREPARO  1
#define ST_PRONTO   2
#define ST_ENTREGUE 3

// ENTREGA
#define ENTREGA   0  // bit 0 > 0-ABS 1-PERCENTUAL
#define SELECOES  1  // bit 1 > 0-MAIOR VALOR 1-MEDIA

// MODO DE PAGAMENTO
#define NPAG            4

#define PAG_DINHEIRO    1
#define PAG_CARTAO      2
#define PAG_CHEQUE      4
#define PAG_FIADO       8

#define IDX_DINHEIRO    0
#define IDX_CHEQUE      1
#define IDX_CARTAO      2
#define IDX_FIADO       3

#endif // CONSTANTS_H
