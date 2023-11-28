#include "authorization.h"
#include "ui_authorization.h"
#include "Global.h"
#include "tourlist.h"
#include "clientviewtours.h"
#include "managerform.h"
#include <QLineEdit>
#include <ctime>
#include <cstdlib>

Authorization::Authorization(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Authorization)
{
    ui->setupUi(this);
    connect(ui->CHKB_ShowPass, &QCheckBox::clicked, [this]{Authorization::togglePasswordChar(ui->CHKB_ShowPass->isChecked());});
    connect(ui->BTN_Login, &QPushButton::clicked,this, &Authorization::tryAuth);
    ui->FR_Captcha->setVisible(false);
    ui->FR_Captcha->setEnabled(false);
    ui->BTN_Login->setShortcut(QKeySequence(Qt::Key_Return));
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::tryAuth()
{
    if(ui->TB_Login->text() == "")
    {
        showMessage("Введите логин","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
        updateWrongAccess();
    }

    else if(ui->TB_Password->text() == "")
    {
        showMessage("Введите пароль","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
        updateWrongAccess();
    }
    else
    {
        if(ui->FR_Captcha->isVisible() && captcha == ui->TB_Captcha->text())
        {
            tryAddresToDB();
        }
        else if(!ui->FR_Captcha->isVisible())
        {
            tryAddresToDB();
        }
        else
        {
            showMessage("Неверная каптча!","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
            updateWrongAccess();
            ui->TB_Captcha->clear();
        }
    }
}

void Authorization::togglePasswordChar(bool value)
{
    if(value)
    {
        ui->TB_Password->setEchoMode(QLineEdit::EchoMode::Normal);
    }
    else
    {
        ui->TB_Password->setEchoMode(QLineEdit::EchoMode::Password);
    }
}

void Authorization::updateCAPTCHA()
{
    srand(time(NULL));
    captcha.clear();
    QString currentValue = "";
    for (int i = 0; i < 4; i++)
    {
        int randomValue = 9 - rand()%(9-0); //в капче рандомайзер чисел
        currentValue = QString::number(randomValue);
        captcha += currentValue;
        switch(i)
        {
            case 0:
            {
                ui->LB_captcha1->setText(currentValue);
                break;
            }
            case 1:
            {
                ui->LB_captcha2->setText(currentValue);
                break;
            }
            case 2:
            {
                ui->LB_captcha3->setText(currentValue);
                break;
            }
            case 3:
            {
                ui->LB_captcha4->setText(currentValue);
                break;
            }
        }
    }
}

void Authorization::updateWrongAccess()
{
    currentWrongAccessTry++;
    if(currentWrongAccessTry >= maxWrongAccessTry){
        if(!ui->FR_Captcha->isVisible()){
                ui->FR_Captcha->setVisible(true);
                ui->FR_Captcha->setEnabled(true);
        }
        updateCAPTCHA();
    }
}

void Authorization::tryAddresToDB()
{
    db = QSqlDatabase::database("authorization");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("Select concat(users.UserSurname ,' ', users.UserName ,' ', users.UserLastname) as FIO, ID_users, users.ID_role from users inner join authorization on authorization.Auth_ID_Users = users.ID_users where authorization.Auth_Login = ? and authorization.Auth_Password = ?;");
        query.addBindValue(ui->TB_Login->text());
        query.addBindValue(ui->TB_Password->text());
        query.exec();
        if(query.next())
        {
                if(query.value(2).toInt() == 1)
                {
                    showMessage("Добро пожаловать!","Администратор",QMessageBox::Information,QMessageBox::Ok);
                    TourList *OperatorForm = new TourList();
                    OperatorForm->loadCurrentIDOperarot(query.value(0).toString(),query.value(1).toString());
                    this->hide();
                    OperatorForm->show();
                    while(OperatorForm->isVisible())
                        QApplication::processEvents();
                    this->show();
                    db.close();
                    ui->TB_Captcha->clear();
                    ui->FR_Captcha->setVisible(false);
                    ui->FR_Captcha->setEnabled(false);
                    currentWrongAccessTry = 0;
                    ui->TB_Login->clear();
                    ui->TB_Password->clear();
                }
                else if(query.value(2).toInt() == 2)
                {
                    showMessage("Добро пожаловать!","Менеджер",QMessageBox::Information,QMessageBox::Ok);
                    ManagerForm *ManagForm = new ManagerForm();
                    ManagForm->loadCurrentIDOperarot(query.value(0).toString(),query.value(1).toString());
                    this->hide();
                    ManagForm->show();
                    while(ManagForm->isVisible())
                    QApplication::processEvents();
                    this->show();
                    db.close();
                    ui->TB_Captcha->clear();
                    ui->FR_Captcha->setVisible(false);
                    ui->FR_Captcha->setEnabled(false);
                    currentWrongAccessTry = 0;
                    ui->TB_Login->clear();
                    ui->TB_Password->clear();
                }
                else if(query.value(2).toInt() == 3)
                {
                    showMessage("Добро пожаловать!","Клиент",QMessageBox::Information,QMessageBox::Ok);
                    ClientViewTours *ClientForm = new ClientViewTours();
                    ClientForm->loadCurrentIDOperatop(query.value(0).toString(),query.value(1).toString());
                    this->hide();
                    ClientForm->show();
                    while(ClientForm->isVisible())
                    QApplication::processEvents();
                    this->show();
                    db.close();
                    ui->TB_Captcha->clear();
                    ui->FR_Captcha->setVisible(false);
                    ui->FR_Captcha->setEnabled(false);
                    currentWrongAccessTry = 0;
                    ui->TB_Login->clear();
                    ui->TB_Password->clear();
                }
        }
        else
        {
            showMessage("Неудачная авторизация!","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
            updateWrongAccess();
        }

    }
    else
    {
        showMessage("Отсутствует подключение к БД !","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
        updateWrongAccess();
    }
}

