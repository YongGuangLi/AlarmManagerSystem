#include "filterbay.h"
#include "ui_filterbay.h"

FilterBay::FilterBay(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::FilterBay)
{
    ui->setupUi(this);
    initTitleBar(this, " 间隔过滤", ":Resources/Image/main_logo.png", QColor(99,99,99), false);

    selectType = 0;

    //显示所有间隔表
    ui->tableWidget_ListBay->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置内容不可更改
    ui->tableWidget_ListBay->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_ListBay->horizontalHeader()->setEnabled(false);
    ui->tableWidget_ListBay->setShowGrid(false);

    //显示所有已选间隔表
    ui->tableWidget_SelectBay->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置内容不可更改
    ui->tableWidget_SelectBay->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_SelectBay->horizontalHeader()->setEnabled(false);
    ui->tableWidget_SelectBay->setShowGrid(false);

    ui->btnconfirm->setIcon(QIcon(":Resources/Images/images/confirm.png"));

    connect(ui->tableWidget_ListBay,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(addBayName(QTableWidgetItem*)));
    connect(ui->tableWidget_SelectBay,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(removeBayName(QTableWidgetItem*)));

    connect(this, SIGNAL(signal_btnClose_Clicked()), this, SLOT(hide()));
}

FilterBay::~FilterBay()
{
    delete ui;
}


void FilterBay::showSelectBay(int type)
{
    ui->tableWidget_ListBay->setRowCount(0);
    for(int i = 0 ; i < SingletonConfig->getListBay().size(); ++i)
    {
        ui->tableWidget_ListBay->insertRow(i);
        QTableWidgetItem *item = new QTableWidgetItem(SingletonConfig->getListBay().at(i));
        ui->tableWidget_ListBay->setItem(i , 0, item);
    }

    selectType = type;
    ui->tableWidget_SelectBay->setRowCount(0);
    switch(type)
    {
    case 0: this->setWindowTitle(QString::fromUtf8("智能告警间隔过滤"));
        for(int i = 0; i < listSmartBayFilter_.size(); ++i)
        {
            ui->tableWidget_SelectBay->insertRow(i);
            QTableWidgetItem *item = new QTableWidgetItem(listSmartBayFilter_.at(i));
            ui->tableWidget_SelectBay->setItem(i , 0, item);
        }
        break;
    case 1: this->setWindowTitle(QString::fromUtf8("智能告警间隔屏蔽"));
        for(int i = 0; i < listSmartBayHide_.size(); ++i)
        {
            ui->tableWidget_SelectBay->insertRow(i);
            QTableWidgetItem *item = new QTableWidgetItem(listSmartBayHide_.at(i));
            ui->tableWidget_SelectBay->setItem(i , 0, item);
        }
        break;
    case 2: this->setWindowTitle(QString::fromUtf8("原始告警间隔过滤"));
        for(int i = 0; i < listOriginalBayFilter_.size(); ++i)
        {
            ui->tableWidget_SelectBay->insertRow(i);
            QTableWidgetItem *item = new QTableWidgetItem(listOriginalBayFilter_.at(i));
            ui->tableWidget_SelectBay->setItem(i , 0, item);
        }
        break;
    case 3: this->setWindowTitle(QString::fromUtf8("原始告警间隔屏蔽"));
        for(int i = 0; i < listOriginalBayHide_.size(); ++i)
        {
            ui->tableWidget_SelectBay->insertRow(i);
            QTableWidgetItem *item = new QTableWidgetItem(listOriginalBayHide_.at(i));
            ui->tableWidget_SelectBay->setItem(i , 0, item);
        }
        break;
    default:
        break;
    }

    show();
}


void FilterBay::addBayName(QTableWidgetItem *item)
{
    QTableWidget *table = dynamic_cast< QTableWidget *>(sender());
    if(table == ui->tableWidget_ListBay)
    {
        //判断过滤间隔是否已经添加
        if(ui->tableWidget_SelectBay->findItems(item->text(),Qt::MatchExactly).size() == 0)
        {
            int row = ui->tableWidget_SelectBay->rowCount();
            ui->tableWidget_SelectBay->insertRow(row);

            QTableWidgetItem *tmpItem = new QTableWidgetItem(item->text());
            ui->tableWidget_SelectBay->setItem(row , 0, tmpItem);
        }
    }
}


void FilterBay::removeBayName(QTableWidgetItem *item)
{
    QTableWidget *table = dynamic_cast< QTableWidget *>(sender());
    if(table == ui->tableWidget_SelectBay)
    {
        ui->tableWidget_SelectBay->removeRow(item->row());
    }
}



bool FilterBay::judgeBayValid(QString bayName, int alarmType)
{
    bool isValid = true;
    if(alarmType == 0)       //智能告警
    {
        if(listSmartBayFilter_.size() != 0 && !listSmartBayFilter_.contains(bayName))
            isValid = false;

        if(listSmartBayHide_.size() != 0 && listSmartBayHide_.contains(bayName))
            isValid = false;
    }
    else if(alarmType == 1)   //原始告警
    {
        if(listOriginalBayFilter_.size() != 0 && !listOriginalBayFilter_.contains(bayName))
            isValid = false;

        if(listOriginalBayHide_.size() != 0 && listOriginalBayHide_.contains(bayName))
            isValid = false;
    }

    return isValid;
}


//间隔过滤和间隔屏蔽同时只能存在一种，选择过滤，屏蔽就清空
void FilterBay::on_btnconfirm_clicked()
{
    switch(selectType)
    {
    case 0:
        listSmartBayHide_.clear();

        listSmartBayFilter_.clear();
        for(int i = 0; i < ui->tableWidget_SelectBay->rowCount(); ++i)
        {
            listSmartBayFilter_.push_back(ui->tableWidget_SelectBay->item(i, 0)->text());
        }
        emit sendSmartConfirmCmd();
        break;
    case 1:
        listSmartBayFilter_.clear();

        listSmartBayHide_.clear();
        for(int i = 0; i < ui->tableWidget_SelectBay->rowCount(); ++i)
        {
           listSmartBayHide_.push_back(ui->tableWidget_SelectBay->item(i, 0)->text());
        }
        emit sendSmartConfirmCmd();
        break;
    case 2:
        listOriginalBayHide_.clear();

        listOriginalBayFilter_.clear();
        for(int i = 0; i < ui->tableWidget_SelectBay->rowCount(); ++i)
        {
            listOriginalBayFilter_.push_back(ui->tableWidget_SelectBay->item(i, 0)->text());
        }
        emit sendOriginalConfirmCmd();
        break;
    case 3:
        listOriginalBayFilter_.clear();

        listOriginalBayHide_.clear();
        for(int i = 0; i < ui->tableWidget_SelectBay->rowCount(); ++i)
        {
            listOriginalBayHide_.push_back(ui->tableWidget_SelectBay->item(i, 0)->text());
        }
        emit sendOriginalConfirmCmd();
        break;
    default:
        break;
    }

    hide();
}
