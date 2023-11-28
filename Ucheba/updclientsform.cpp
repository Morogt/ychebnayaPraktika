#include "updclientsform.h"
#include "ui_updclientsform.h"

UpdClientForm::UpdClientForm(QString id, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UpdClientForm)
{
    ui->setupUi(this);
    loadInfoFromDB(id);
    connect(ui->Btn_Update,&QPushButton::clicked,this,&UpdClientForm::updateDataInDB);
    connect(ui->Btn_cancelUpdate,&QPushButton::clicked,this,&UpdClientForm::cancel);
}

UpdClientForm::~UpdClientForm()
{
    delete ui;
}

void UpdClientForm::currentIDClient(QString value)
{
    id = value;
}

void UpdClientForm::updateDataInDB()
{
db = QSqlDatabase::database("upd");
    QSqlQuery query(db);
    if (getDBConnection(db))
    {
        bool isValid = true;
        QString TourGroupName = "", ClientsName = "";
        QString idTourGroup = "",idClients = "";
        QSqlQuery checkTourGroup(db),checkClients(db);

        checkTourGroup.prepare("select id from tour_groups where Groupname = ?;");
        TourGroupName = ui->CMBB_TourGroupNameUpd->currentText();
        checkTourGroup.addBindValue(TourGroupName);
        checkTourGroup.exec();
        if(checkTourGroup.next()){
            idTourGroup = checkTourGroup.value(0).toString();
        }

        checkClients.prepare("select id from tourists where concat(tourists.surname,' ',tourists.name,' ',tourists.lastname) = ?;");
        ClientsName = ui->CMBB_FIOClientsUpd->currentText();
        checkClients.addBindValue(ClientsName);
        checkClients.exec();
        if(checkClients.next()){
            idClients = checkClients.value(0).toString();
        }

        int cost = ui->TB_TicketCostUpd->text().remove(" ").toInt(&isValid);
        if (!isValid || cost <= 0) {
            showMessage("Введите корректную общую стоимость", "Ошибка!", QMessageBox::Warning, QMessageBox::Ok);
            return;
        }

        query.prepare("UPDATE tours.group_composition SET group_id = ?, tourist_id =?, ticket_price =? WHERE id = ?;");
        query.addBindValue(idTourGroup);
        query.addBindValue(idClients);
        query.addBindValue(cost);
        query.addBindValue(id);
        query.exec();
        showMessage("Клиент успешно изменен!", "Успех!", QMessageBox::Information, QMessageBox::Ok);
        this->close();
    } else {
        showMessage("Отсутствует подключение к БД !", "Ошибка!", QMessageBox::Warning, QMessageBox::Ok);
    }
}

void UpdClientForm::cancel()
{
    ui->TB_TicketCostUpd->clear();
    this->close();
}

void UpdClientForm::loadInfoFromDB(QString value)
{
   id = value;
    db = QSqlDatabase::database("upd");
    if(getDBConnection(db)){
        QSqlQuery query(db);
        QSqlQuery queryAdd(db);
        queryAdd.exec("SELECT Groupname FROM tours.tour_groups;");
        while (queryAdd.next())
        {
            ui->CMBB_TourGroupNameUpd->addItem(queryAdd.value(0).toString());
        }
        queryAdd.exec("select concat(tourists.surname,' ',tourists.name,' ',tourists.lastname) as clientInfo from tourists;");
        while (queryAdd.next())
        {
            ui->CMBB_FIOClientsUpd->addItem(queryAdd.value(0).toString());
        }
        query.prepare("SELECT * FROM tours.group_composition_view where id = ?;");
        query.addBindValue(id);
        query.exec();
        if(query.next()){
            ui->LB_IDClient->setText(query.value(0).toString());
            ui->LB_Date->setText(query.value(1).toString());
            ui->CMBB_TourGroupNameUpd->setCurrentText(query.value(2).toString());
            ui->CMBB_FIOClientsUpd->setCurrentText(query.value(3).toString());
            ui->TB_TicketCostUpd->setText(query.value(4).toString());
        }
        else
        {
        showMessage("Не найден клиент","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
        }
    }
    else{
        showMessage("Отсутствует подключение к БД !","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
    }
}
