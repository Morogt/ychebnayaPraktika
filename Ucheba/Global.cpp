#include "Global.h"

int showMessage(QString message, QString title, QMessageBox::Icon icon, QMessageBox::StandardButton btn)
{
    QMessageBox msg;
    msg.setText(message);
    msg.setWindowTitle(title);
    msg.setIcon(icon);
    msg.setStandardButtons(btn);
    return msg.exec();
}

bool getDBConnection(QSqlDatabase db)
{
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("tours");
    db.setUserName("root");
    db.setPassword("root");
    return db.open();
}
