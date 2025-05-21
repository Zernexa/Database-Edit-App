#ifndef VIEWUSERDIALOG_H
#define VIEWUSERDIALOG_H

#include <QDialog>
#include "database.h" // For User class

// Forward declaration - keep this as is
namespace Ui {
class ViewUserDialog;
}

class ViewUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewUserDialog(const User& user, QWidget *parent = nullptr);
    ~ViewUserDialog();

private:
    Ui::ViewUserDialog *ui;
};

#endif // VIEWUSERDIALOG_H
