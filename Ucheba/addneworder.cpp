#include "addneworder.h"
#include "ui_addneworder.h"


AddNewOrder::AddNewOrder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddNewOrder)
{
    ui->setupUi(this);
    updateTourGroupCMBB();
    updateCMBB();
    ui->PNL_TourGroupAdd->setVisible(false);
    ui->PNL_TourGroupAdd->setEnabled(false);
    connect(ui->Btn_NewTourGroup, &QPushButton::clicked,this,&AddNewOrder::startCreateNewTourGroup);
    connect(ui->Btn_CancelTourGroup, &QPushButton::clicked,this,&AddNewOrder::cancelCreateNewTourGroup);
    connect(ui->Btn_AddTourGroup, &QPushButton::clicked,this, &AddNewOrder::addNewTourGroup);
    connect(ui->Btn_Cancel, &QPushButton::clicked,this,&AddNewOrder::cancelCreateNewSale); //lambda not included
    connect(ui->Btn_Add, &QPushButton::clicked,this,&AddNewOrder::addNewOrder);
}
AddNewOrder::~AddNewOrder()
{
    delete ui;
}


void AddNewOrder::cancelCreateNewSale()
{
    ui->LE_FullCost->clear();
    this->close();
}

void AddNewOrder::addNewOrder()
{
    if(ui->LE_FullCost->text().remove(" ").length()>0)
    {
        if(ui->CMBB_TourGroup->currentIndex()>-1)
        {
            if(ui->CMBB_Hotel->currentIndex()>-1)
            {
                bool isValid;
                double Cost = ui->LE_FullCost->text().remove(" ").toDouble(&isValid);
                if (!isValid || Cost <= 0)
                {
                    showMessage("Введите корректную общую стоимость(число)", "Ошибка!", QMessageBox::Warning, QMessageBox::Ok);
                    return;
                }
                db = QSqlDatabase::database("addnew");
                if(getDBConnection(db))
                {
                    QString  ID_TourGroup = "", ID_Hotel = "";
                    QSqlQuery query(db);
                    query.prepare("select id from tours.tour_groups where Groupname = ?;");
                    query.addBindValue(ui->CMBB_TourGroup->currentText());
                    query.exec();
                    if(query.next())
                      ID_TourGroup = query.value(0).toString();
                    else
                    {
                        showMessage("Пустое или неверное поле тур группы", "Ошибка", QMessageBox::Warning, QMessageBox::FirstButton);
                        return;
                    }
                    query.prepare("select id from tours.hotels where hotel_name = ?;");
                    query.addBindValue(ui->CMBB_Hotel->currentText());
                    query.exec();
                    if(query.next())
                      ID_Hotel = query.value(0).toString();
                    else
                    {
                        showMessage("Пустое или неверное поле гостиниц", "Ошибка", QMessageBox::Warning, QMessageBox::FirstButton);
                        return;
                    }
                    QDate date = QDate::currentDate();
                    query.prepare("insert into sales_ledger(date, tour_group_id, hotel_id, total_cost) values (?,?,?,?);");
                    query.addBindValue(date.toString("yyyy-MM-dd"));
                    query.addBindValue(ID_TourGroup);
                    query.addBindValue(ID_Hotel);
                    query.addBindValue(ui->LE_FullCost->text());
                    query.exec();
                    showMessage("Продажа успешно добавлена","Успех!",QMessageBox::Information,QMessageBox::Ok);
                    this->close();
                }
                else
                {
                    showMessage("Ошибка подключения к бд", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
                }
            }
            else
            {
                showMessage("Гостиницы не найдены", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
            }
        }
        else
        {
            showMessage("Туристическая группа не найдена", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
        }
    }
    else
    {
        showMessage("Заполните общую стоимость", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
    }
}

void AddNewOrder::startCreateNewTourGroup()
{
    ui->PNL_TourGroupAdd->setVisible(true);
    ui->PNL_TourGroupAdd->setEnabled(true);
    ui->PNL_TourGroupAdd->setGeometry(x_real,y_real,ui->PNL_Order->width(),ui->PNL_TourGroupAdd->height());
    ui->PNL_Order->setVisible(false);
    ui->PNL_Order->setEnabled(false);
    ui->LB_TextSetLevel->setText("Добавление новой тур. группы");
}

void AddNewOrder::cancelCreateNewTourGroup()
{
    ui->PNL_TourGroupAdd->setVisible(false);
    ui->PNL_TourGroupAdd->setEnabled(false);
    ui->PNL_Order->setVisible(true);
    ui->PNL_Order->setEnabled(true);
    ui->TB_CountTourist->clear();
    ui->TB_DateArrival->clear();
    ui->TB_DateDeparure->clear();
    ui->TB_NameTourGroup->clear();
    ui->LB_TextSetLevel->setText("Формирование новой продажи");
}

void AddNewOrder::updateTourGroupCMBB()
{
    ui->CMBB_TourGroup->clear();
    db = QSqlDatabase::database("addnew");
    if(getDBConnection(db)){
        QSqlQuery query(db);
        query.exec("select Groupname from tour_groups");
        while (query.next()) {
            ui->CMBB_TourGroup->addItem(query.value(0).toString());
        }
    }
    else
    {
        showMessage("Отсутствует подключение к БД !","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
    }
}

void AddNewOrder::addNewTourGroup()
{
    if(ui->TB_NameTourGroup->text().remove(" ").length()>0)
    {
        if(ui->TB_DateArrival->text().remove(" ").length()>0)
        {
            if(ui->TB_DateDeparure->text().remove(" ").length()>0)
            {
                if(ui->TB_CountTourist->text().remove(" ").length()>0)
                {
                    if(ui->CMBB_tour->currentIndex()>-1)
                    {
                        bool isValid;
                        int Count = ui->TB_CountTourist->text().remove(" ").toInt(&isValid);
                        if (!isValid || Count <= 0 || Count >= 9)
                        {
                            showMessage("Введите корректное количество человек в группе (макс. 8)", "Ошибка!", QMessageBox::Warning, QMessageBox::Ok);
                            return;
                        }


                        db = QSqlDatabase::database("addnew");
                        if(getDBConnection(db))
                        {
                            QString  ID_Tour = "";
                            QSqlQuery query(db);
                            query.prepare("select id from tours.tours where name = ?;");
                            query.addBindValue(ui->CMBB_tour->currentText());
                            query.exec();
                            if(query.next())
                              ID_Tour = query.value(0).toString();
                            else
                            {
                                showMessage("Пустое или неверное поле туров", "Ошибка", QMessageBox::Warning, QMessageBox::FirstButton);
                                return;
                            }
                            // Преобразуем строки с датами в объекты QDate
                            QDate departureDate = QDate::fromString(ui->TB_DateDeparure->text(), "yyyy-MM-dd");
                            QDate arrivalDate = QDate::fromString(ui->TB_DateArrival->text(), "yyyy-MM-dd");
                            // Проверяем правильность формата дат
                            if (!departureDate.isValid() || !arrivalDate.isValid())
                            {
                                showMessage("Неверный формат даты", "Ошибка", QMessageBox::Warning, QMessageBox::FirstButton);
                                return;
                            }
                            query.prepare("insert into tour_groups(Groupname, departure_date, arrival_date, tour_id, count_of_tourists) values (?,?,?,?,?);");
                            query.addBindValue(ui->TB_NameTourGroup->text());
                            query.addBindValue(departureDate.toString("yyyy-MM-dd"));
                            query.addBindValue(arrivalDate.toString("yyyy-MM-dd"));
                            query.addBindValue(ID_Tour);
                            query.addBindValue(ui->TB_CountTourist->text());
                            query.exec();
                            updateTourGroupCMBB();
                            showMessage("Группа добавлена","Успех!",QMessageBox::Information,QMessageBox::Ok);

                        }
                        else
                        {
                            showMessage("Ошибка подключения к бд", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
                        }
                    }
                    else
                    {
                        showMessage("Туры не найдены", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
                    }
                }
                else
                {
                    showMessage("Заполните количество туристов", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
                }
            }
            else
            {
                showMessage("Заполните дату прибытия", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
            }
        }
        else
        {
            showMessage("Заполните дату отправки", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
        }
    }
    else
    {
        showMessage("Заполните название группы", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
    }
}

void AddNewOrder::updateCMBB()
{
    db = QSqlDatabase::database("addnew");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.exec("select hotel_name from hotels");
        while (query.next())
        {
            ui->CMBB_Hotel->addItem(query.value(0).toString());
        }
        query.exec("select name from tours");
        while (query.next())
        {
            ui->CMBB_tour->addItem(query.value(0).toString());
        }
    }
    else
    {
        showMessage("Отсутствует подключение к БД !","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
    }
}

