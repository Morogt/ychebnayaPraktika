#include "editDBformorders.h"
#include "ui_editDBformorders.h"

EditFormOrders::EditFormOrders(QString id, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditFormOrders)
{
    ui->setupUi(this);
    loadInfoFromDB(id);
    connect(ui->Btn_Update,&QPushButton::clicked,this,&EditFormOrders::updateDataInDB);
    connect(ui->Btn_Cancel,&QPushButton::clicked,this,&EditFormOrders::cancel);
}

EditFormOrders::~EditFormOrders()
{
    delete ui; 
}

void EditFormOrders::currentIDOrder(QString value)
{
    id = value;
}

void EditFormOrders::updateDataInDB()
{
   db = QSqlDatabase::database("updat");
    QSqlQuery query(db);
    if (getDBConnection(db))
    {
        bool isValid = true;
        QString TourGroupName = "", HotelsName = "";
        QString idTourGroup = "",idHotels = "";
        QSqlQuery checkTourGroup(db),checkHotels(db);

        checkTourGroup.prepare("select id from tour_groups where Groupname = ?;");
        TourGroupName = ui->CMBB_TourGroupUpd->currentText();
        checkTourGroup.addBindValue(TourGroupName);
        checkTourGroup.exec();
        if(checkTourGroup.next()){
            idTourGroup = checkTourGroup.value(0).toString();
        }

        checkHotels.prepare("select id from hotels where hotel_name = ?;");
        HotelsName = ui->CMBB_HotelUpd->currentText();
        checkHotels.addBindValue(HotelsName);
        checkHotels.exec();
        if(checkHotels.next()){
            idHotels = checkHotels.value(0).toString();
        }

        int cost = ui->TB_FullCostUpd->text().remove(" ").toInt(&isValid);
        if (!isValid || cost <= 0) {
            showMessage("Введите корректную общую стоимость", "Ошибка!", QMessageBox::Warning, QMessageBox::Ok);
            return;
        }

        query.prepare("UPDATE tours.sales_ledger SET tour_group_id = ?, hotel_id =?, total_cost =? WHERE id = ?;");
        query.addBindValue(idTourGroup);
        query.addBindValue(idHotels);
        query.addBindValue(cost);
        query.addBindValue(id);
        query.exec();
        showMessage("Тур успешно изменен!", "Успех!", QMessageBox::Information, QMessageBox::Ok);
        this->close();
    } else {
        showMessage("Отсутствует подключение к БД !", "Ошибка!", QMessageBox::Warning, QMessageBox::Ok);
    }
}

void EditFormOrders::cancel()
{
    ui->TB_FullCostUpd->clear();
    this->close();
}

void EditFormOrders::loadInfoFromDB(QString value)
{
   id = value;
    db = QSqlDatabase::database("updat");
    if(getDBConnection(db)){
        QSqlQuery query(db);
        QSqlQuery queryAdd(db);
        queryAdd.exec("SELECT Groupname FROM tours.tour_groups;");
        while (queryAdd.next())
        {
            ui->CMBB_TourGroupUpd->addItem(queryAdd.value(0).toString());
        }
        queryAdd.exec("SELECT hotel_name FROM tours.hotels;");
        while (queryAdd.next())
        {
            ui->CMBB_HotelUpd->addItem(queryAdd.value(0).toString());
        }
        query.prepare("SELECT * FROM tours.sales_ledger_view where id = ?;");
        query.addBindValue(id);
        query.exec();
        if(query.next()){
            ui->LB_IDOrder->setText(query.value(0).toString());
            ui->LB_Date->setText(query.value(1).toString());
            ui->CMBB_TourGroupUpd->setCurrentText(query.value(2).toString());
            ui->CMBB_HotelUpd->setCurrentText(query.value(3).toString());
            ui->TB_FullCostUpd->setText(query.value(4).toString());
        }
        else
        {
        showMessage("Не найдена продажа","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
        }
    }
    else{
        showMessage("Отсутствует подключение к БД !","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
    }
}


