#ifndef CLIENTVIEWTOURS_H
#define CLIENTVIEWTOURS_H

#include <QMainWindow>
#include "Global.h"
#include <qsqlquery.h>

namespace Ui {
class ClientViewTours;
}

class ClientViewTours : public QMainWindow
{
    Q_OBJECT
public:
    explicit ClientViewTours(QWidget *parent = nullptr);
    ~ClientViewTours();
    void loadCurrentIDOperatop(QString,QString);
public slots:
    void updateOrderTable();
private:
    Ui::ClientViewTours *ui;
    void addDataTW(QSqlQuery);
    QString id = "";
    QString currentIDOperator = "";
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","Client");
    void clearOrderInfoPanel();
private slots:
    void loadOrderInfo(QString);
};

#endif // CLIENTVIEWTOURS_H
