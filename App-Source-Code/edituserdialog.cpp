#include "edituserdialog.h"
#include "ui_edituserdialog.h"

edituserdialog::edituserdialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::edituserdialog)
{
    ui->setupUi(this);
}

edituserdialog::~edituserdialog()
{
    delete ui;
}
