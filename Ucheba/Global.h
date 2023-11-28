#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QMessageBox>
#include <QSqlDatabase>


int showMessage(QString,QString,QMessageBox::Icon,QMessageBox::StandardButton);
bool getDBConnection(QSqlDatabase db);
#endif // GLOBAL_H
