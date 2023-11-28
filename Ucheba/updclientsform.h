#ifndef UPDCLIENTSFORM_H
#define UPDCLIENTSFORM_H

#include <QMainWindow>
#include "Global.h"
#include <QSqlQuery>

namespace Ui {
class UpdClientForm;
}

class UpdClientForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit UpdClientForm(QString, QWidget *parent = nullptr);
    ~UpdClientForm();
    void currentIDClient(QString);
public slots:
    void updateDataInDB();
    void cancel();
private:
    Ui::UpdClientForm *ui;
    void loadInfoFromDB(QString);
    QString id = "";
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","upd");
};

#endif // UPDCLIENTSFORM_H
