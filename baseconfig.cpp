#include "baseconfig.h"
#include "ui_baseconfig.h"

BaseConfig::BaseConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseConfig)
{
    ui->setupUi(this);
    ui->btnAddAlarmConfig->setIcon(QIcon(":Resources/Image/add.png"));
    ui->btnModifyAlarmConfig->setIcon(QIcon(":Resources/Image/modify.png"));
    ui->btnDeleteAlarmConfig->setIcon(QIcon(":Resources/Image/delete.png"));


    ui->btnAddStationConfig->setIcon(QIcon(":Resources/Image/add.png"));
    ui->btnModifyStationConfig->setIcon(QIcon(":Resources/Image/modify.png"));
    ui->btnDeleteStationConfig->setIcon(QIcon(":Resources/Image/delete.png"));

    QStringList alarmConfigHeaders;
    alarmConfigHeaders<<"告警等级"<<"告警名称"<<"告警颜色"<<"是否弹窗";

    ui->tableWidget_AlarmConfig->verticalHeader()->hide();
    ui->tableWidget_AlarmConfig->setColumnCount(alarmConfigHeaders.size());
    ui->tableWidget_AlarmConfig->setHorizontalHeaderLabels(alarmConfigHeaders);
    ui->tableWidget_AlarmConfig->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_AlarmConfig->setEditTriggers(QAbstractItemView::NoEditTriggers);


    connect(ui->btnAddAlarmConfig, SIGNAL(clicked(bool)), this, SLOT(slot_addAlarmConfig()));
    connect(ui->btnModifyAlarmConfig, SIGNAL(clicked(bool)), this, SLOT(slot_modifyAlarmConfig()));
    connect(ui->btnDeleteAlarmConfig, SIGNAL(clicked(bool)), this, SLOT(slot_deleteAlarmConfig()));
}

BaseConfig::~BaseConfig()
{
    delete ui;
}

void BaseConfig::showAlarmConfig(stAlarmConfig alarmConfig)
{
    int currentRow= ui->tableWidget_AlarmConfig->rowCount();
    ui->tableWidget_AlarmConfig->insertRow(currentRow);

    QTableWidgetItem *item0 = new QTableWidgetItem(alarmConfig.alarmLevel);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmConfig.alarmName);
    item1->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item2 = new QTableWidgetItem();
    item2->setBackground(QColor(alarmConfig.alarmColor));
    item2->setTextAlignment(Qt::AlignCenter);

    QString state = alarmConfig.alarmWindow == 1 ? "是" : "否";
    QTableWidgetItem *item3 = new QTableWidgetItem(state);
    item3->setTextAlignment(Qt::AlignCenter);


    ui->tableWidget_AlarmConfig->setItem(currentRow, 0,  item0);
    ui->tableWidget_AlarmConfig->setItem(currentRow, 1,  item1);
    ui->tableWidget_AlarmConfig->setItem(currentRow, 2,  item2);
    ui->tableWidget_AlarmConfig->setItem(currentRow, 3,  item3);
}

void BaseConfig::showEvent(QShowEvent *e)
{
    Q_UNUSED(e);
    ui->lineEdit_AlarmReportPath->setText(SingletonConfig->getAlarmReportPath());
    ui->lineEdit_HistoryAlarmExportPath->setText(SingletonConfig->getHistoryAlarmExportPath());

    QMap<QString, stAlarmConfig> mapAlarmConfig = SingletonDBHelper->queryAlarmConfig();
    DataModel::getInstance()->setMapAlarmConfig(mapAlarmConfig);

    ui->tableWidget_AlarmConfig->setRowCount(0);
    for(QMap<QString,stAlarmConfig>::iterator it = mapAlarmConfig.begin(); it != mapAlarmConfig.end(); ++it)
        showAlarmConfig(it.value());
}

void BaseConfig::slot_addAlarmConfig()
{
    SettingDialog::getInstance()->addAlarmConfig();
    if(SettingDialog::getInstance()->exec() == QDialog::Accepted)
    {
        stAlarmConfig alarmConfig = SettingDialog::getInstance()->getAlarmConfigSetResult();

        //拼接数据库新增语句
        QString sqlStr = QString(SQL_INSERT_ALARMCONFIG)
                .arg(alarmConfig.alarmLevel)
                .arg(alarmConfig.alarmName)
                .arg(alarmConfig.alarmColor)
                .arg(alarmConfig.alarmWindow);

        if(SingletonDBHelper->execSqlStr(sqlStr))
            qDebug()<<QString("新增告警配置,告警等级:%1，告警名称:%2").arg(alarmConfig.alarmLevel).arg(alarmConfig.alarmName);

        showEvent(NULL);
    }
}

void BaseConfig::slot_modifyAlarmConfig()
{
    int currentRow = ui->tableWidget_AlarmConfig->currentRow();
    if(currentRow >= 0)
    {
        QString alarmLevel = ui->tableWidget_AlarmConfig->item(currentRow, 0)->text();
        stAlarmConfig alarmConfig = DataModel::getInstance()->getAlarmConfig(alarmLevel);
        SettingDialog::getInstance()->modifyAlarmConfig(alarmConfig);
        if(SettingDialog::getInstance()->exec() == QDialog::Accepted)
        {
            stAlarmConfig alarmConfig = SettingDialog::getInstance()->getAlarmConfigSetResult();

            //拼接数据库新增语句
            QString sqlStr = QString(SQL_MODIFY_ALARMCONFIG)
                    .arg(alarmConfig.alarmName)
                    .arg(alarmConfig.alarmColor)
                    .arg(alarmConfig.alarmWindow)
                    .arg(alarmConfig.alarmLevel);

            if(SingletonDBHelper->execSqlStr(sqlStr))
                qDebug()<<QString("修改告警配置,告警等级:%1，告警名称:%2").arg(alarmConfig.alarmLevel).arg(alarmConfig.alarmName);

            showEvent(NULL);
        }
    }
}

void BaseConfig::slot_deleteAlarmConfig()
{
    int currentRow= ui->tableWidget_AlarmConfig->currentRow();
    if(currentRow >= 0)
    {
        QString alarmLevel = ui->tableWidget_AlarmConfig->item(currentRow, 0)->text();
        //拼接数据库新增语句
        QString sqlStr = QString(SQL_DELETE_ALARMCONFIG).arg(alarmLevel);

        if(SingletonDBHelper->execSqlStr(sqlStr))
            qDebug()<<QString("删除告警配置,告警等级:%1").arg(alarmLevel);

        showEvent(NULL);
    }
}

