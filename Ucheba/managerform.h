#ifndef MANAGERFORM_H
#define MANAGERFORM_H

#include <QMainWindow>
#include <qsqlquery.h>
#include "Global.h"
#include "addclientform.h"
#include "updclientsform.h"
#include <QMenu>

namespace Ui {
class ManagerForm;
}

class ManagerForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerForm(QWidget *parent = nullptr);
    ~ManagerForm();
    void loadCurrentIDOperarot(QString,QString);
public slots:
    void updateClientsTable();
    void deleteFromClientDB(int);
    void updateClientDB(int);
    void openAddClientForm();
private:
    Ui::ManagerForm *ui;
    QString id = "";
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","Manager");
    QString currentIDOperator = "";
    void addDataTW(QSqlQuery);
    void clearClientsInfoPanel();
private slots:
    void loadClientsInfo(QString);
};

#endif // MANAGERFORM_H
