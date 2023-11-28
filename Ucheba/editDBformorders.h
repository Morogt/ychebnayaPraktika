#ifndef EDITDBFORMORDERS_H
#define EDITDBFORMORDERS_H

#include <QMainWindow>
#include "Global.h"
#include <QSqlQuery>

namespace Ui {
class EditFormOrders;
}

class EditFormOrders : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditFormOrders(QString, QWidget *parent = nullptr);
    ~EditFormOrders();
    void currentIDOrder(QString);
public slots:
    void updateDataInDB();
    void cancel();
private:
    Ui::EditFormOrders *ui;
    void loadInfoFromDB(QString);
    QString id = "";
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","updat");

};

#endif // EDITDBFORMORDERS_H
