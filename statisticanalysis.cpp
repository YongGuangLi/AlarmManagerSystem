#include "statisticanalysis.h"
#include "ui_statisticanalysis.h"

StatisticAnalysis::StatisticAnalysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticAnalysis)
{
    ui->setupUi(this);

    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->dateTimeEdit->setCalendarPopup(true);

    connect(ui->btnquery, SIGNAL(clicked()), this, SLOT(slot_btnQuery_Clicked()));

    ui->horizontalLayout->setAlignment(Qt::AlignLeft);

    QStringList listHeader;
    listHeader<<QString::fromLocal8Bit("时间")<<QString::fromLocal8Bit("实际事故")<<QString::fromLocal8Bit("智能告警事故推理")<<QString::fromLocal8Bit("正确推理")
             <<QString::fromLocal8Bit("事故误判")<<QString::fromLocal8Bit("事故漏判")<<QString::fromLocal8Bit("推理异常")<<QString::fromLocal8Bit("事故推理正确率");

    ui->tableWidget->setColumnCount(listHeader.size());
    ui->tableWidget->setHorizontalHeaderLabels(listHeader);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->setColumnHidden(ui->tableWidget->columnCount() - 1 , true);
}

StatisticAnalysis::~StatisticAnalysis()
{
    delete ui;
}

void StatisticAnalysis::showEvent(QShowEvent *)
{
    ui->tableWidget->setRowCount(0);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

void StatisticAnalysis::slot_btnQuery_Clicked()
{
    QString beginDateTime;
    QString endDateTime;

    QDateTime queryDateTime = ui->dateTimeEdit->dateTime();
    if(ui->comboBox->currentText() == QString::fromLocal8Bit("日报"))
    {
        QDate queryDate = queryDateTime.date();
        beginDateTime = queryDate.toString("yyyy-MM-dd") + " 00:00:00";
        endDateTime = queryDate.toString("yyyy-MM-dd") + " 23:59:59";
    }
    else if(ui->comboBox->currentText() == QString::fromLocal8Bit("周报"))
    {
        QDate queryDate = queryDateTime.date();
        int dayOfWeek = queryDate.dayOfWeek();

        beginDateTime = QString("%1-%02-%3 00:00:00").arg(queryDate.toString("yyyy")).arg(queryDate.addDays(dayOfWeek - 1).month()).arg(queryDate.addDays(dayOfWeek - 1).day());
        endDateTime = QString("%1-%02-%3 23:59:59").arg(queryDate.toString("yyyy")).arg(queryDate.addDays(7 - dayOfWeek).month()).arg(queryDate.addDays(7 - dayOfWeek).day());
    }
    else if(ui->comboBox->currentText() == QString::fromLocal8Bit("月报"))
    {
        QDate queryDate = queryDateTime.date();
        beginDateTime = queryDate.toString("yyyy-MM") + "-01 00:00:00";
        endDateTime = QString("%1-%2 23:59:59").arg(queryDate.toString("yyyy-MM")).arg(QString::number(queryDate.daysInMonth()));
    }
    else if(ui->comboBox->currentText() == QString::fromLocal8Bit("年报"))
    {
        QDate queryDate = queryDateTime.date();
        beginDateTime = queryDate.toString("yyyy") + "01-01 00:00:00";
        endDateTime = QString("%1-12-%2 23:59:59").arg(queryDate.toString("yyyy")).arg(QString::number(queryDate.daysInMonth()));
    }

    int smartDataCnt = SingletonDBHelper->querySmartDataCnt(beginDateTime, endDateTime, LEVEL_EVENT);
    int originalDataCnt = SingletonDBHelper->queryOriginalDataCnt(beginDateTime, endDateTime, LEVEL_EVENT);

    ui->tableWidget->insertRow(0);
    QTableWidgetItem *item0 = new QTableWidgetItem(beginDateTime + "  " + endDateTime);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(originalDataCnt));
    item1->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(smartDataCnt));
    item2->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item3 = new QTableWidgetItem(QString::number(smartDataCnt));
    item3->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item4 = new QTableWidgetItem("0");
    item4->setTextAlignment(Qt::AlignCenter);

    int loss = originalDataCnt - smartDataCnt ? originalDataCnt - smartDataCnt : 0;
    QTableWidgetItem *item5 = new QTableWidgetItem(QString::number(loss));
    item5->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item6 = new QTableWidgetItem("0");
    item6->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item7 = new QTableWidgetItem("0");
    item7->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(0, 0, item0);
    ui->tableWidget->setItem(0, 1, item1);
    ui->tableWidget->setItem(0, 2, item2);
    ui->tableWidget->setItem(0, 3, item3);
    ui->tableWidget->setItem(0, 4, item4);
    ui->tableWidget->setItem(0, 5, item5);
    ui->tableWidget->setItem(0, 6, item6);
    ui->tableWidget->setItem(0, 7, item7);
}
