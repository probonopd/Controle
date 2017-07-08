#ifndef ANALISEVENDAS_H
#define ANALISEVENDAS_H

#include <QDialog>

namespace Ui {
class AnaliseVendas;
}

class AnaliseVendas : public QDialog
{
    Q_OBJECT

public:
    explicit AnaliseVendas(QWidget *parent = 0);
    ~AnaliseVendas();

private:
    Ui::AnaliseVendas *ui;
};

#endif // ANALISEVENDAS_H
