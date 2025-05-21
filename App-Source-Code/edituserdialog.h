#ifndef EDITUSERDIALOG_H
#define EDITUSERDIALOG_H

#include <QDialog>

namespace Ui {
class edituserdialog;
}

class edituserdialog : public QDialog
{
    Q_OBJECT

public:
    explicit edituserdialog(QWidget *parent = nullptr);
    ~edituserdialog();

private:
    Ui::edituserdialog *ui;
};

#endif // EDITUSERDIALOG_H
