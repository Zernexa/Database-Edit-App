#include "viewuserdialog.h"
#include "ui_viewuserdialog.h" // This must include the generated UI header
#include <QDebug>

// // ViewUserDialog::ViewUserDialog(const User& user, QWidget *parent) :
//     QDialog(parent)
//     ///ui(new Ui::ViewUserDialog)
// {
//     ///ui->setupUi(this);
//     qDebug() << "Ceva"
//     // Set the UI labels with user info
//     // ui->id_value->setText(QString::number(user.id));
//     // ui->username_value->setText(user.username);
//     // ui->userlogin_value->setText(user.login);
//     // ui->realname_value->setText(user.realname);
//     // ui->email_value->setText(user.email);
//     // ui->dob_value->setText(user.dob);
//     // ui->country_value->setText(user.country);
//     // ui->accountcreation_value->setText(user.createdAt);
//  }

ViewUserDialog::~ViewUserDialog()
{
    delete ui;
}
