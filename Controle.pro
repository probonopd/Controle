#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T14:51:52
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Controle
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        login.cpp \
    userinfo.cpp \
    principal.cpp \
    estabelecimento.cpp \
    cliente.cpp \
    endereco.cpp \
    produto.cpp \
    preco.cpp \
    menu.cpp \
    cardapio.cpp \
    venda.cpp \
    submenu.cpp \
    numberformatdelegate.cpp \
    controleglobal.cpp \
    entrega.cpp \
    condominio.cpp \
    pedidos.cpp \
    precoentrega.cpp \
    timeformatdelegate.cpp \
    ligarcardapio.cpp \
    jobfunctioncolumndelegate.cpp \
    dateformatdelegate.cpp \
    modopagtodelegate.cpp \
    analisevendas.cpp

HEADERS += \
        login.h \
    userinfo.h \
    principal.h \
    estabelecimento.h \
    cliente.h \
    endereco.h \
    produto.h \
    preco.h \
    menu.h \
    cardapio.h \
    venda.h \
    submenu.h \
    numberformatdelegate.h \
    controleglobal.h \
    entrega.h \
    condominio.h \
    pedidos.h \
    constants.h \
    precoentrega.h \
    timeformatdelegate.h \
    ligarcardapio.h \
    jobfunctioncolumndelegate.h \
    dateformatdelegate.h \
    modopagtodelegate.h \
    analisevendas.h

FORMS += \
        login.ui \
    userinfo.ui \
    principal.ui \
    estabelecimento.ui \
    cliente.ui \
    endereco.ui \
    produto.ui \
    preco.ui \
    menu.ui \
    cardapio.ui \
    venda.ui \
    submenu.ui \
    entrega.ui \
    condominio.ui \
    pedidos.ui \
    precoentrega.ui \
    ligarcardapio.ui \
    analisevendas.ui
