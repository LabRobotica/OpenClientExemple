#include "clientsoket.h"
#include "ui_clientsoket.h"


clientSoket::clientSoket(QWidget *parent):QMainWindow(parent),ui(new Ui::referenciajunta)
{
    ui->setupUi(this);
}

clientSoket::~clientSoket()
{
    delete ui;
}
