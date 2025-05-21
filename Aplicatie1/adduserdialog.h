#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>

namespace Ui {
class adduserdialog;
}

class adduserdialog : public QDialog
{
    Q_OBJECT

public:
    explicit adduserdialog(QWidget *parent = nullptr);
    ~adduserdialog();

private:
    Ui::adduserdialog *ui;
};

#endif // ADDUSERDIALOG_H
