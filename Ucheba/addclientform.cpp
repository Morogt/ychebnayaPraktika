#include "addclientform.h"
#include "ui_addclientform.h"

addClientform::addClientform(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::addClientform)
{
    ui->setupUi(this);
    updateClientCMBB();
    updateOtherCMBB();
    ui->PNL_AddClient->setVisible(false);
    ui->PNL_AddClient->setEnabled(false);
    connect(ui->Btn_NewClient, &QPushButton::clicked,this,&addClientform::startCreateNewClient);
    connect(ui->Btn_CancelAddClientDB, &QPushButton::clicked,this,&addClientform::cancelCreateNewClient);
    connect(ui->Btn_AddClientDB, &QPushButton::clicked,this, &addClientform::addNewClients);
    connect(ui->Btn_cancelAddClientToTourGroup, &QPushButton::clicked,this,&addClientform::cancelCreateClientGroup); //lambda not included
    connect(ui->Btn_AddClientToTourGroup, &QPushButton::clicked,this,&addClientform::addNewClientGroup);
}

addClientform::~addClientform()
{
    delete ui;
}

void addClientform::cancelCreateClientGroup()
{
    ui->TB_TicketCost->clear();
    this->close();
}

void addClientform::addNewClientGroup()
{
    if(ui->TB_TicketCost->text().remove(" ").length()>0)
    {
        if(ui->CMBB_FIOClients->currentIndex()>-1)
        {
            if(ui->CMBB_TourGroupName->currentIndex()>-1)
            {
                bool isValid;
                double Cost = ui->TB_TicketCost->text().remove(" ").toDouble(&isValid);
                if (!isValid || Cost <= 0)
                {
                    showMessage("Введите корректную цену за билет(число)", "Ошибка!", QMessageBox::Warning, QMessageBox::Ok);
                    return;
                }
                db = QSqlDatabase::database("addnewTourGroup");
                if(getDBConnection(db))
                {
                    QString  ID_TourGroup = "", ID_Client = "";
                    QSqlQuery query(db);
                    query.prepare("select id from tours.tour_groups where Groupname = ?;");
                    query.addBindValue(ui->CMBB_TourGroupName->currentText());
                    query.exec();
                    if(query.next())
                      ID_TourGroup = query.value(0).toString();
                    else
                    {
                        showMessage("Пустое или неверное поле тур группы", "Ошибка", QMessageBox::Warning, QMessageBox::FirstButton);
                        return;
                    }
                    query.prepare("select id from tourists where concat(tourists.surname,' ',tourists.name,' ',tourists.lastname) = ?;");
                    query.addBindValue(ui->CMBB_FIOClients->currentText());
                    query.exec();
                    if(query.next())
                      ID_Client = query.value(0).toString();
                    else
                    {
                        showMessage("Пустое или неверное поле гостиниц", "Ошибка", QMessageBox::Warning, QMessageBox::FirstButton);
                        return;
                    }
                    QDate date = QDate::currentDate();
                    query.prepare("insert into group_composition(sale_date, group_id, tourist_id, ticket_price) values (?,?,?,?);");
                    query.addBindValue(date.toString("yyyy-MM-dd"));
                    query.addBindValue(ID_TourGroup);
                    query.addBindValue(ID_Client);
                    query.addBindValue(ui->TB_TicketCost->text());
                    query.exec();
                    showMessage("Клиент успешно добавлен в тур группу","Успех!",QMessageBox::Information,QMessageBox::Ok);
                    this->close();
                }
                else
                {
                    showMessage("Ошибка подключения к бд", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
                }
            }
            else
            {
                showMessage("Тур группы не найдены", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
            }
        }
        else
        {
            showMessage("Клиент не найден", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
        }
    }
    else
    {
        showMessage("Заполните цену за билет", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
    }
}

void addClientform::addNewClients()
{
    if(ui->TB_Surname->text().remove(" ").length()>0)
    {
        if(ui->TB_Name->text().remove(" ").length()>0)
        {
            if(ui->TB_Lastname->text().remove(" ").length()>0)
            {
                if(ui->TB_DataPassport->text().remove(" ").length()>0)
                {
                    if(ui->TB_Age->text().remove(" ").length()>0)
                    {
                        if(ui->TB_KidsCount->text().remove(" ").length()>0)
                        {
                            if(ui->CMBB_Gender->currentIndex()>-1)
                            {
                                bool isValid;
                                int Count = ui->TB_KidsCount->text().remove(" ").toInt(&isValid);
                                if (!isValid || Count < 0 || Count >= 6)
                                {
                                    showMessage("Максимум можно записать 5 детей", "Ошибка!", QMessageBox::Warning, QMessageBox::Ok);
                                    return;
                                }
                                db = QSqlDatabase::database("addnewTourGroup");
                                if(getDBConnection(db))
                                {
                                    QString  ID_Gender = "";
                                    QSqlQuery query(db);
                                    query.prepare("select idgender from tours.gender_list where NameGender = ?;");
                                    query.addBindValue(ui->CMBB_Gender->currentText());
                                    query.exec();
                                    if(query.next())
                                      ID_Gender = query.value(0).toString();
                                    else
                                    {
                                        showMessage("Пустое или неверное поле гендера", "Ошибка", QMessageBox::Warning, QMessageBox::FirstButton);
                                        return;
                                    }
                                    query.prepare("insert into tourists(surname, name, lastname, passport_data, gender, age, children) values (?, ?, ?, ?, ?, ?, ?);");
                                    query.addBindValue(ui->TB_Surname->text());
                                    query.addBindValue(ui->TB_Name->text());
                                    query.addBindValue(ui->TB_Lastname->text());
                                    query.addBindValue(ui->TB_DataPassport->text());
                                    query.addBindValue(ID_Gender);
                                    query.addBindValue(ui->TB_Age->text());
                                    query.addBindValue(ui->TB_KidsCount->text());
                                    query.exec();
                                    updateClientCMBB();
                                    showMessage("Группа добавлена","Успех!",QMessageBox::Information,QMessageBox::Ok);
                                }
                                else
                                {
                                    showMessage("Ошибка подключения к бд", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
                                }
                            }
                            else
                            {
                                showMessage("Гендер не определен", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
                            }
                        }
                        else
                        {
                            showMessage("Заполните количество детей", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
                        }
                    }
                    else
                    {
                        showMessage("Заполните возраст", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
                    }
                }
                else
                {
                    showMessage("Заполните паспортные данные", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
                }
            }
            else
            {
                showMessage("Заполните отчество", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
            }
        }
        else
        {
            showMessage("Заполните имя", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
        }
    }
    else
    {
        showMessage("Заполните фамилию", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
    }
}

void addClientform::startCreateNewClient()
{
    ui->PNL_AddClient->setVisible(true);
    ui->PNL_AddClient->setEnabled(true);
    ui->PNL_AddClient->setGeometry(x_real,y_real,ui->PNL_AddClientAtGroup->width(),ui->PNL_AddClient->height());
    ui->PNL_AddClientAtGroup->setVisible(false);
    ui->PNL_AddClientAtGroup->setEnabled(false);
    ui->LB_TextSetLevel->setText("Добавление нового клиента");
}

void addClientform::cancelCreateNewClient()
{
    ui->PNL_AddClient->setVisible(false);
    ui->PNL_AddClient->setEnabled(false);
    ui->PNL_AddClientAtGroup->setVisible(true);
    ui->PNL_AddClientAtGroup->setEnabled(true);
    ui->TB_Surname->clear();
    ui->TB_Name->clear();
    ui->TB_Lastname->clear();
    ui->TB_Age->clear();
    ui->TB_KidsCount->clear();
    ui->TB_DataPassport->clear();
    ui->LB_TextSetLevel->setText("Формирование клиента в тур группу");
}

void addClientform::updateClientCMBB()
{
    ui->CMBB_FIOClients->clear();
    db = QSqlDatabase::database("addnewTourGroup");
    if(getDBConnection(db)){
        QSqlQuery query(db);
        query.exec("select concat(tourists.surname,' ',tourists.name,' ',tourists.lastname) as clientInfo from tourists;");
        while (query.next()) {
            ui->CMBB_FIOClients->addItem(query.value(0).toString());
        }
    }
    else
    {
        showMessage("Отсутствует подключение к БД !","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
    }
}

void addClientform::updateOtherCMBB()
{
    db = QSqlDatabase::database("addnewTourGroup");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.exec("select Groupname from tour_groups");
        while (query.next())
        {
            ui->CMBB_TourGroupName->addItem(query.value(0).toString());
        }
        query.exec("select NameGender from gender_list");
        while (query.next())
        {
            ui->CMBB_Gender->addItem(query.value(0).toString());
        }
    }
    else
    {
        showMessage("Отсутствует подключение к БД !","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
    }
}
