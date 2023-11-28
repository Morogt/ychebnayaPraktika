#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class Authorization; }
QT_END_NAMESPACE

class Authorization : public QMainWindow
{
    Q_OBJECT

public:
    Authorization(QWidget *parent = nullptr);
    ~Authorization();
public slots:
    void tryAuth();
    void togglePasswordChar(bool);
private:
    Ui::Authorization *ui;
    const int maxWrongAccessTry= 1;
    int currentWrongAccessTry = 0;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","authorization");
    void updateCAPTCHA();
    QString captcha = "";
    void updateWrongAccess();
    void tryAddresToDB();
};
#endif // AUTHORIZATION_H
