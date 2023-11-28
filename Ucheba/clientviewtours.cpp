#include "clientviewtours.h"
#include "ui_clientviewtours.h"

ClientViewTours::ClientViewTours(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientViewTours)
{
    ui->setupUi(this);
    ui->TW_ClientViewTours->horizontalHeader()->setMinimumHeight(50);
    ui->TW_ClientViewTours->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment) Qt::TextWordWrap);
    ui->TW_ClientViewTours->setColumnWidth(0,50);
    ui->TW_ClientViewTours->setColumnWidth(1,95);
    ui->TW_ClientViewTours->setColumnWidth(2,90);
    ui->TW_ClientViewTours->setColumnWidth(3,80);
    ui->TW_ClientViewTours->setColumnWidth(4,200);
    ui->TW_ClientViewTours->setColumnWidth(5,125);
    ui->TW_ClientViewTours->setColumnWidth(6,125);
    ui->TW_ClientViewTours->setColumnWidth(7,125);
    updateOrderTable();
    connect(ui->TB_Search, &QLineEdit::textChanged,[this]{ClientViewTours::updateOrderTable();});
    connect(ui->CMBB_Search, &QComboBox::currentIndexChanged,[this]{ClientViewTours::updateOrderTable();});
    connect(ui->CMBB_Sort, &QComboBox::currentIndexChanged,[this]{ClientViewTours::updateOrderTable();});
    connect(ui->TW_ClientViewTours, &QTableWidget::cellClicked, [this]{ClientViewTours::loadOrderInfo(ui->TW_ClientViewTours->item(ui->TW_ClientViewTours->currentRow(),0)->text());});
}

ClientViewTours::~ClientViewTours()
{
    delete ui;
}

void ClientViewTours::loadCurrentIDOperatop(QString value, QString id)
{
    currentIDOperator = id;
    ui->LB_CurrentOperator->setText("Клиент: " + value);
}

void ClientViewTours::updateOrderTable()
{
    clearOrderInfoPanel();
    int rowsCount = ui->TW_ClientViewTours->rowCount();
    for(int i =0;i<rowsCount;i++)
    {
        ui->TW_ClientViewTours->removeRow(0);
    }
    db = QSqlDatabase::database("Client");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        QString mainCommand = "SELECT * FROM tours.tours", modCommandOrder = "";
        switch(ui->CMBB_Sort->currentIndex()){
        case 0:
            modCommandOrder += " order by tour_price asc;";
            break;
        case 1:
            modCommandOrder += " order by tour_price desc;";
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
                    search = query.value(1).toString();
                    break;
                case 1:
                    search = query.value(2).toString();
                    break;
                case 2:
                    search = query.value(7).toString();
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
        ui->TW_ClientViewTours->resizeRowsToContents();
    }
    else{
        showMessage("Отсутствует подключение к БД !","Ошибка!",QMessageBox::Warning,QMessageBox::Ok);
    }
}

void ClientViewTours::addDataTW(QSqlQuery query)
{
    ui->TW_ClientViewTours->insertRow(ui->TW_ClientViewTours->rowCount());
    ui->TW_ClientViewTours->setItem(ui->TW_ClientViewTours->rowCount()-1,0,new QTableWidgetItem(query.value(0).toString()));
    ui->TW_ClientViewTours->setItem(ui->TW_ClientViewTours->rowCount()-1,1,new QTableWidgetItem(query.value(1).toString()));
    ui->TW_ClientViewTours->setItem(ui->TW_ClientViewTours->rowCount()-1,2,new QTableWidgetItem(query.value(2).toString()));
    ui->TW_ClientViewTours->setItem(ui->TW_ClientViewTours->rowCount()-1,3,new QTableWidgetItem(query.value(3).toString()));
    ui->TW_ClientViewTours->setItem(ui->TW_ClientViewTours->rowCount()-1,4,new QTableWidgetItem(query.value(4).toString()));
    ui->TW_ClientViewTours->setItem(ui->TW_ClientViewTours->rowCount()-1,5,new QTableWidgetItem(query.value(5).toString()));
    ui->TW_ClientViewTours->setItem(ui->TW_ClientViewTours->rowCount()-1,6,new QTableWidgetItem(query.value(6).toString()));
    ui->TW_ClientViewTours->setItem(ui->TW_ClientViewTours->rowCount()-1,7,new QTableWidgetItem(query.value(7).toString()));
}

void ClientViewTours::clearOrderInfoPanel()
{
    ui->LB_IDTour->clear();
    ui->LB_NameTour->clear();
    ui->LB_Country->clear();
    ui->LB_City->clear();
    ui->LB_TransportationType->clear();
    ui->LB_MealType->clear();
    ui->LB_CostTour->clear();
    ui->LB_TypeAccommodation->clear();
}

void ClientViewTours::loadOrderInfo(QString id)
{
    clearOrderInfoPanel();
    db = QSqlDatabase::database("Client");
    if(getDBConnection(db)){
        QSqlQuery query(db);
        query.prepare("SELECT * FROM tours.tours where id = ?;");
        query.addBindValue(id);
        query.exec();
        if(query.next())
        {
            ui->LB_IDTour->setText(query.value(0).toString());
            ui->LB_NameTour->setText(query.value(1).toString());
            ui->LB_Country->setText(query.value(2).toString());
            ui->LB_City->setText(query.value(3).toString());
            ui->LB_TransportationType->setText(query.value(4).toString());
            ui->LB_MealType->setText(query.value(5).toString());
            ui->LB_CostTour->setText(query.value(6).toString());
            ui->LB_TypeAccommodation->setText(query.value(7).toString());
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
