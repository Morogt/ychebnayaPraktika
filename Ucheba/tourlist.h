#ifndef TOURLIST_H
#define TOURLIST_H

#include <QMainWindow>
#include <qsqlquery.h>
#include "Global.h"
#include "editDBformorders.h"

namespace Ui {
class TourList;
}

class TourList : public QMainWindow
{
    Q_OBJECT

public:
    explicit TourList(QWidget *parent = nullptr);
    ~TourList();
    void loadCurrentIDOperarot(QString,QString);
    //QString getID() { return ID; }
public slots:
    void updateOrderTable();
    void deleteFromDB(int);
    void updateDB(int);
    //void updateOrderInfo();
    void openAddOrderForm();
private:
    Ui::TourList *ui;
    QString id = "";
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","Orders");
    QString currentIDOperator = "";
    void addDataTW(QSqlQuery);
    void clearOrderInfoPanel();
private slots:
    void loadOrderInfo(QString);
};

#endif // TOURLIST_H
