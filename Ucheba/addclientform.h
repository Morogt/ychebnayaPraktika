#ifndef ADDCLIENTFORM_H
#define ADDCLIENTFORM_H

#include "Global.h"
#include <QMainWindow>
#include <QPushButton>
#include <QSqlQuery>
#include <QDate>

namespace Ui {
class addClientform;
}

class addClientform : public QMainWindow
{
    Q_OBJECT

public:
    explicit addClientform(QWidget *parent = nullptr);
    ~addClientform();
public slots:
    void cancelCreateClientGroup();
    void addNewClientGroup();
    void addNewClients();
    void startCreateNewClient();
    void cancelCreateNewClient();
private:
    Ui::addClientform *ui;
    void updateClientCMBB();
    void updateOtherCMBB();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","addnewTourGroup");
    const int x_real = 13,y_real =45, x_hide = 14, y_hide = 46;
};

#endif // ADDCLIENTFORM_H
