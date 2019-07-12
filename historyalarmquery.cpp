#include "historyalarmquery.h"
#include "ui_historyalarmquery.h"

HistoryAlarmQuery::HistoryAlarmQuery(QWidget *parent) : QWidget(parent), ui(new Ui::HistoryAlarmQuery)
{
    ui->setupUi(this);
    this->initForm();
}

HistoryAlarmQuery::~HistoryAlarmQuery()
{
    delete ui;
}


void HistoryAlarmQuery::initForm()
{
    ui->tableMain->verticalHeader()->setDefaultSectionSize(25);

    ui->dateTimeEdit_begin->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->dateTimeEdit_begin->setCalendarPopup(true);

    ui->dateTimeEdit_end->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->dateTimeEdit_end->setCalendarPopup(true);
    columnNames.clear();
    columnWidths.clear();

    tableName = "dtxy_alarm_2018_10_10";
    countName = "id";

    columnNames.append("防区号");
    columnNames.append("防区名称");
    columnNames.append("设备IP");
    columnNames.append("日志类型");
    columnNames.append("事件内容");
    columnNames.append("触发时间");
    columnNames.append("告警详情");
    columnNames.append("告警数据");
    columnNames.append("告警图像");

    columnWidths.append(70);
    columnWidths.append(120);
    columnWidths.append(120);
    columnWidths.append(80);
    columnWidths.append(150);
    columnWidths.append(160);
    columnWidths.append(160);
    columnWidths.append(160);
    columnWidths.append(160);

    //设置需要显示数据的表格和翻页的按钮
    dbPage = new DbPage(this);
    //设置所有列居中显示
    dbPage->setAllCenter(true);

    ui->tableMain->horizontalHeader()->setStretchLastSection(true);

    connect(ui->btnSelect, SIGNAL(clicked(bool)), this, SLOT(slot_btnSelect_clicked()));
    connect(ui->btnExport, SIGNAL(clicked(bool)), this, SLOT(slot_btnExport_clicked()));
}


void HistoryAlarmQuery::showEvent(QShowEvent *e)
{
    Q_UNUSED(e);

    ui->dateTimeEdit_begin->setDate(QDateTime::currentDateTime().date());
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());
}

void HistoryAlarmQuery::slot_btnSelect_clicked()
{
    ui->labResult->setText("正在查询...");

    dbPage->setControl(ui->tableMain, ui->labPageCount, ui->labPageCurrent, ui->labResultCount, ui->labResultCurrent, ui->labResult, 0,
                       ui->btnFirst, ui->btnPre, ui->btnNext, ui->btnLast, countName);
    //绑定数据到表格
    QString sql = "where 1=1";
    dbPage->setTableName(tableName);
    dbPage->setOrderSql(QString("%1 %2").arg(countName).arg("asc"));
    dbPage->setWhereSql(sql);
    dbPage->setResultCurrent(20);
    dbPage->setColumnNames(columnNames);
    dbPage->setColumnWidths(columnWidths);
    dbPage->select();
}

void HistoryAlarmQuery::slot_btnExport_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save File"),  "/home/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".csv", tr("(*.csv)"));
    qDebug()<<fileName;

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        file.close();
    }
}
