#ifndef ADDNEWORDER_H
#define ADDNEWORDER_H

#include <QMainWindow>
#include "Global.h"
#include <QPushButton>
#include <QSqlQuery>
#include <QDate>

namespace Ui {
class AddNewOrder;
}

class AddNewOrder : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddNewOrder(QWidget *parent = nullptr);
    ~AddNewOrder();
public slots:
    void cancelCreateNewSale();
    void addNewOrder();
    void addNewTourGroup();
    void startCreateNewTourGroup();
    void cancelCreateNewTourGroup();
private:
    Ui::AddNewOrder *ui;
    void updateTourGroupCMBB();
    void updateCMBB();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","addnew");
    const int x_real = 13,y_real =57, x_hide = 14, y_hide = 57;
};

#endif // ADDNEWORDER_H
