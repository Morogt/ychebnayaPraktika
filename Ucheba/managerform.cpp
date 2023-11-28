#include "managerform.h"
#include "ui_managerform.h"

ManagerForm::ManagerForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagerForm)
{
    ui->setupUi(this);
    ui->TW_ClientTable->horizontalHeader()->setMinimumHeight(50);
    ui->TW_ClientTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment) Qt::TextWordWrap);
    ui->TW_ClientTable->setColumnWidth(0,50);
    ui->TW_ClientTable->setColumnWidth(1,95);
    ui->TW_ClientTable->setColumnWidth(2,350);
    ui->TW_ClientTable->setColumnWidth(3,125);
    ui->TW_ClientTable->setColumnWidth(4,50);
    updateClientsTable();
    connect(ui->menu_Upd, &QMenu::aboutToHide, [this]{ManagerForm::updateClientDB(ui->TW_ClientTable->currentRow());});
    connect(ui->menu_DelClient,&QMenu::aboutToHide,[this]{ManagerForm::deleteFromClientDB(ui->TW_ClientTable->currentRow());});
    connect(ui->menu_AddClient, &QMenu::aboutToHide,[this]{ManagerForm::openAddClientForm();});
    connect(ui->TB_Search, &QLineEdit::textChanged,[this]{ManagerForm::updateClientsTable();});
    connect(ui->CMBB_Search, &QComboBox::currentIndexChanged,[this]{ManagerForm::updateClientsTable();});
    connect(ui->CMBB_Sort, &QComboBox::currentIndexChanged,[this]{ManagerForm::updateClientsTable();});
    connect(ui->TW_ClientTable, &QTableWidget::cellClicked, [this]{ManagerForm::loadClientsInfo(ui->TW_ClientTable->item(ui->TW_ClientTable->currentRow(),0)->text());});
}

ManagerForm::~ManagerForm()
{
    delete ui;
}

void ManagerForm::loadCurrentIDOperarot(QString value, QString id)
{
    currentIDOperator = id;
    ui->LB_CurrentOperator->setText("Менеджер: " + value);
}

void ManagerForm::updateClientsTable()
{
    clearClientsInfoPanel();
    int rowsCount = ui->TW_ClientTable->rowCount();
    for(int i =0;i<rowsCount;i++)
    {
        ui->TW_ClientTable->removeRow(0);
    }
    db = QSqlDatabase::database("Manager");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        QString mainCommand = "SELECT * FROM tours.group_composition_view", modCommandOrder = "";
        switch(ui->CMBB_Sort->currentIndex()){
        case 0:
            modCommandOrder += " order by ticket_price asc;";
            break;
        case 1:
            modCommandOrder += " order by ticket_price desc;";
            break;
        }
        mainCommand += modCommandOrder;
        query.exec(mainCommand);
        while(query.next())
        {
            QString search = "";
            if(ui->TB_Search->text().length() > 0)
            {
                switch(ui->CMBB_Search->currentIndex()){
                case 0:
                    search = query.value(0).toString();
                    break;
                case 1:
                    search = query.value(3).toString();
                    break;
                }
                if(search.toLower().contains(ui->TB_Search->text().toLower()))
                {
                    addDataTW(query);
                }
                else
                {
                    continue;
                }
            }
            else
            {
                addDataTW(query);
            }
        }
        ui->TW_ClientTable->resizeRowsToContents();
    }
    else{
        showMessage("Отсутствует подключение к БД !","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
    }
}

void ManagerForm::deleteFromClientDB(int row)
{
    if (row == -1)
    {
        // Если элемент не выбран, выведите сообщение об ошибке или просто вернитесь
        showMessage("Выберите элемент для удаления", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }
    else
    {
        QMessageBox msg;
        QString id = "";

        msg.setText("Вы уверены что хотите удалить данную запись?");
        msg.setWindowTitle("Подтверждение");
        msg.setIcon(QMessageBox::Icon::Question);
        msg.addButton(QMessageBox::Button::Ok);
        msg.addButton(QMessageBox::Button::Cancel);
        msg.setDefaultButton(QMessageBox::Button::Cancel);

        id = ui->TW_ClientTable->item(row, 0)->text(); // Предполагаем, что ID находится в первом столбце

        db = QSqlDatabase::database("Manager");

        if (getDBConnection(db))
        {
            if (msg.exec() == QMessageBox::Button::Ok)
            {
                QSqlQuery query(db);
                query.prepare("DELETE FROM group_composition WHERE id = ?;");
                query.addBindValue(id);
                query.exec();
                updateClientsTable();
            }
            else
            {
                showMessage("Клиент остается в базе данных.", "Информация", QMessageBox::Information, QMessageBox::Ok);
                return;
            }
        }
        else
        {
            showMessage("Отсутствует подключение к БД !", "Ошибка!", QMessageBox::Warning, QMessageBox::Ok);
        }
    }
}

void ManagerForm::updateClientDB(int row)
{
    if (row == -1)
    {
        // Если элемент не выбран, выведите сообщение об ошибке или просто вернитесь
        showMessage("Выберите элемент для редактирования", "Ошибка", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }
    else
    {
        QString id = "";

        id = ui->TW_ClientTable->item(row, 0)->text();

        db = QSqlDatabase::database("Manager");
        UpdClientForm *formupdat = new UpdClientForm(id);
        this->hide();
        formupdat->show();
        while(formupdat->isVisible())
            QApplication::processEvents();
        this->show();
        updateClientsTable();
    }
}

void ManagerForm::openAddClientForm()
{
    addClientform *newClient = new addClientform;
    //newOrder->loadCurrentIDOperator(currentIDOperator);
    this->hide();
    newClient->show();
    while(newClient->isVisible())
        QApplication::processEvents();
    this->show();
    db.close();
    updateClientsTable();
}

void ManagerForm::addDataTW(QSqlQuery query)
{
    ui->TW_ClientTable->insertRow(ui->TW_ClientTable->rowCount());
    ui->TW_ClientTable->setItem(ui->TW_ClientTable->rowCount()-1,0,new QTableWidgetItem(query.value(0).toString()));
    ui->TW_ClientTable->setItem(ui->TW_ClientTable->rowCount()-1,1,new QTableWidgetItem(query.value(1).toString()));
    ui->TW_ClientTable->setItem(ui->TW_ClientTable->rowCount()-1,2,new QTableWidgetItem(query.value(2).toString()));
    ui->TW_ClientTable->setItem(ui->TW_ClientTable->rowCount()-1,3,new QTableWidgetItem(query.value(3).toString()));
    ui->TW_ClientTable->setItem(ui->TW_ClientTable->rowCount()-1,4,new QTableWidgetItem(query.value(4).toString()));
}

void ManagerForm::clearClientsInfoPanel()
{
    ui->LB_IDTourist->clear();
    ui->LB_DateSale->clear();
    ui->LB_FIOTourist->clear();
    ui->LB_TourGroup->clear();
    ui->LB_CostOfTicket->clear();
}

void ManagerForm::loadClientsInfo(QString id)
{
    clearClientsInfoPanel();
    db = QSqlDatabase::database("Manager");
    if(getDBConnection(db)){
        QSqlQuery query(db);
        query.prepare("SELECT * FROM tours.group_composition_view where id = ?;");
        query.addBindValue(id);
        query.exec();
        if(query.next())
        {
            ui->LB_IDTourist->setText(query.value(0).toString());
            ui->LB_DateSale->setText(query.value(1).toString());
            ui->LB_FIOTourist->setText(query.value(2).toString());
            ui->LB_TourGroup->setText(query.value(3).toString());
            ui->LB_CostOfTicket->setText(query.value(4).toString()+" руб.");
        }
        else{
            showMessage("Ошибка внесения данных!","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
            return;
        }
    }
    else{
        showMessage("Отсутствует подключение к БД !","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
    }
}
