#include "adduserdialog.h"
#include "ui_adduserdialog.h"

adduserdialog::adduserdialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::adduserdialog)
{
    ui->setupUi(this);
}

adduserdialog::~adduserdialog()
{
    delete ui;
}
