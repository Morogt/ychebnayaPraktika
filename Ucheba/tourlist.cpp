#include "tourlist.h"
#include "ui_tourlist.h"
#include "addneworder.h"
#include <QMenu>

TourList::TourList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TourList)
{
    ui->setupUi(this);
    ui->TW_OrderTable->horizontalHeader()->setMinimumHeight(50);
    ui->TW_OrderTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment) Qt::TextWordWrap);
    ui->TW_OrderTable->setColumnWidth(0,50);
    ui->TW_OrderTable->setColumnWidth(1,95);
    ui->TW_OrderTable->setColumnWidth(2,90);
    ui->TW_OrderTable->setColumnWidth(3,80);
    ui->TW_OrderTable->setColumnWidth(4,50);
    connect(ui->menu_DeleteOrder,&QMenu::aboutToHide,[this]{TourList::deleteFromDB(ui->TW_OrderTable->currentRow());});
    connect(ui->menu_AddNewOrder, &QMenu::aboutToHide,[this]{TourList::openAddOrderForm();});
    connect(ui->menu_Edit, &QMenu::aboutToHide, [this]{TourList::updateDB(ui->TW_OrderTable->currentRow());});
    updateOrderTable();
    connect(ui->TB_Search, &QLineEdit::textChanged,[this]{TourList::updateOrderTable();});
    connect(ui->CMBB_Find, &QComboBox::currentIndexChanged,[this]{TourList::updateOrderTable();});
    connect(ui->CMBB_Sort, &QComboBox::currentIndexChanged,[this]{TourList::updateOrderTable();});
    connect(ui->TW_OrderTable, &QTableWidget::cellClicked, [this]{TourList::loadOrderInfo(ui->TW_OrderTable->item(ui->TW_OrderTable->currentRow(),0)->text());});

    // сделать оставшиеся модули для менеджера и клиента
}

TourList::~TourList()
{
    delete ui;
}

void TourList::loadCurrentIDOperarot(QString value, QString id)
{
    currentIDOperator = id;
    ui->LB_CurrentOperator->setText("Оператор: " + value);
}

void TourList::updateOrderTable()
{
    clearOrderInfoPanel();
    int rowsCount = ui->TW_OrderTable->rowCount();
    for(int i =0;i<rowsCount;i++)
    {
        ui->TW_OrderTable->removeRow(0);
    }
    db = QSqlDatabase::database("Orders");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        QString mainCommand = "SELECT * FROM tours.sales_ledger_view", modCommandOrder = "";
        switch(ui->CMBB_Sort->currentIndex()){
        case 0:
            modCommandOrder += " order by total_cost asc;";
            break;
        case 1:
            modCommandOrder += " order by total_cost desc;";
            break;
        }
        mainCommand += modCommandOrder;
        query.exec(mainCommand);
        while(query.next())
        {
            QString search = "";
            if(ui->TB_Search->text().length() > 0)
            {
                switch(ui->CMBB_Find->currentIndex()){
                case 0:
                    search = query.value(0).toString();
                    break;
                case 1:
                    search = query.value(2).toString();
                    break;
                case 2:
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
        ui->TW_OrderTable->resizeRowsToContents();
    }
    else{
        showMessage("Отсутствует подключение к БД !","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
    }
}

void TourList::deleteFromDB(int row)
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

        id = ui->TW_OrderTable->item(row, 0)->text(); // Предполагаем, что ID находится в первом столбце

        db = QSqlDatabase::database("Orders");

        if (getDBConnection(db))
        {
            if (msg.exec() == QMessageBox::Button::Ok)
            {
                QSqlQuery query(db);
                query.prepare("DELETE FROM sales_ledger WHERE id = ?;");
                query.addBindValue(id);
                query.exec();
                updateOrderTable();
            }
            else
            {
                showMessage("Заказ остается в базе данных.", "Информация", QMessageBox::Information, QMessageBox::Ok);
                return;
            }
        }
        else
        {
            showMessage("Отсутствует подключение к БД !", "Ошибка!", QMessageBox::Warning, QMessageBox::Ok);
        }
    }
}

void TourList::updateDB(int row)
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

        id = ui->TW_OrderTable->item(row, 0)->text();

        db = QSqlDatabase::database("Orders");
        EditFormOrders *formupd = new EditFormOrders(id);
        this->hide();
        formupd->show();
        while(formupd->isVisible())
            QApplication::processEvents();
        this->show();
        updateOrderTable();
    }
}

void TourList::openAddOrderForm()
{
    AddNewOrder *newOrder = new AddNewOrder;
    //newOrder->loadCurrentIDOperator(currentIDOperator);
    this->hide();
    newOrder->show();
    while(newOrder->isVisible())
        QApplication::processEvents();
    this->show();
    db.close();
    updateOrderTable();
}


void TourList::addDataTW(QSqlQuery query)
{
    ui->TW_OrderTable->insertRow(ui->TW_OrderTable->rowCount());
    ui->TW_OrderTable->setItem(ui->TW_OrderTable->rowCount()-1,0,new QTableWidgetItem(query.value(0).toString()));
    ui->TW_OrderTable->setItem(ui->TW_OrderTable->rowCount()-1,1,new QTableWidgetItem(query.value(1).toString()));
    ui->TW_OrderTable->setItem(ui->TW_OrderTable->rowCount()-1,2,new QTableWidgetItem(query.value(2).toString()));
    ui->TW_OrderTable->setItem(ui->TW_OrderTable->rowCount()-1,3,new QTableWidgetItem(query.value(3).toString()));
    ui->TW_OrderTable->setItem(ui->TW_OrderTable->rowCount()-1,4,new QTableWidgetItem(query.value(4).toString()));
}

void TourList::clearOrderInfoPanel()
{
    ui->LB_IDOrder->clear();
    ui->LB_DateStart->clear();
    ui->LB_TouristGroup->clear();
    ui->LB_NameHotel->clear();
    ui->LB_CostOfTicket->clear();
}

void TourList::loadOrderInfo(QString id)
{
    clearOrderInfoPanel();
    db = QSqlDatabase::database("Orders");
    if(getDBConnection(db)){
        QSqlQuery query(db);
        query.prepare("SELECT * FROM tours.sales_ledger_view where id = ?;");
        query.addBindValue(id);
        query.exec();
        if(query.next())
        {
            ui->LB_IDOrder->setText(query.value(0).toString());
            ui->LB_DateStart->setText(query.value(1).toString());
            ui->LB_TouristGroup->setText(query.value(2).toString());
            ui->LB_NameHotel->setText(query.value(3).toString());
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


