#include "realtimealarm.h"
#include "ui_realtimealarm.h"

RealTimeAlarm::RealTimeAlarm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RealTimeAlarm)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("智能告警实时展示"));

    initWidgetDisp();

    //隐藏最小化窗口
    this->setWindowFlags(this->windowFlags() &~ Qt::WindowMinimizeButtonHint);
    this->setWindowIcon(QIcon(":Resources/Images/images/aboutlogo.png"));

    //接收并显示告警数据
    connect(SingletonAlarmData, SIGNAL(receiverSmartAlarmData(int,stAlarmData)), this, SLOT(dispSmartAlarmData(int, stAlarmData)));
    connect(SingletonAlarmData, SIGNAL(receiverOriginalAlarmData(int,stAlarmData)), this, SLOT(dispOriginalAlarmData(int, stAlarmData)));

    //遥控过程
//    remoteControlWidget = new RemoteControlWidget();
    //告警详情
//    alarmDetail = new AlarmDetail();
//    alarmDetail->addRemoteControlProcess(remoteControlWidget);

    //间隔过滤
    connect(ui->btnSmartBayFilter, SIGNAL(clicked()), this, SLOT(selectFilterBay()));
    connect(ui->btnSmartBayShield, SIGNAL(clicked()), this, SLOT(selectFilterBay()));

    connect(ui->btnOriginalBayFilter, SIGNAL(clicked()), this, SLOT(selectFilterBay()));
    connect(ui->btnOriginalBayShield, SIGNAL(clicked()), this, SLOT(selectFilterBay()));

    curSelectIaAlarmLevel = LEVEL_REALTIME;
    curSelectOrAlarmLevel = LEVEL_REALTIME;

    filterBay = new FilterBay(this);
    filterBay->hide();
    connect(filterBay, SIGNAL(sendSmartConfirmCmd()), this, SLOT(replaySmartAlarmData()));
    connect(filterBay, SIGNAL(sendOriginalConfirmCmd()), this, SLOT(replayOriginalAlarmData()));

    ui->btnhistoryQuery->hide();

    connect(ui->btnSmartPause, SIGNAL(clicked(bool)), this, SLOT(pauseSmartAlarmData(bool)));
    connect(ui->btnOriginalPause, SIGNAL(clicked(bool)), this, SLOT(pauseOriginalAlarmData(bool)));

    ui->btnSmartPause->setCheckable(true);
    ui->btnOriginalPause->setCheckable(true);
}

RealTimeAlarm::~RealTimeAlarm()
{
    delete ui;
}

void RealTimeAlarm::initWidgetDisp()
{
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(alarmTabWidgetChanged(int)));

    ui->tableWidget_SmartAlarm->verticalHeader()->hide();
    ui->tableWidget_SmartAlarm->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置内容不可更改
    ui->tableWidget_SmartAlarm->setContextMenuPolicy(Qt::CustomContextMenu);           //允许右击菜单
    ui->tableWidget_SmartAlarm->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_SmartAlarm->setColumnHidden(4, true);
    ui->tableWidget_SmartAlarm->setSelectionBehavior(QAbstractItemView::SelectRows);   //单击选择一行
    ui->tableWidget_SmartAlarm->setShowGrid(true);

    ui->tableWidget_OriginalAlarm->verticalHeader()->hide();
    ui->tableWidget_OriginalAlarm->setEditTriggers(QAbstractItemView::NoEditTriggers);       //设置内容不可更改
    ui->tableWidget_OriginalAlarm->setContextMenuPolicy(Qt::CustomContextMenu);              //允许右击菜单
    ui->tableWidget_OriginalAlarm->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_OriginalAlarm->setColumnHidden(4, true);
    ui->tableWidget_OriginalAlarm->setSelectionBehavior(QAbstractItemView::SelectRows);      //单击选择一行
    ui->tableWidget_OriginalAlarm->setShowGrid(true);

    ui->btnSmartBayFilter->setIcon(QIcon(":Resources/Images/images/RealTimeAlarm/filter.png"));
    ui->btnSmartBayShield->setIcon(QIcon(":Resources/Images/images/RealTimeAlarm/shield.png"));
    ui->btnSmartPause->setIcon(QIcon(":Resources/Images/images/RealTimeAlarm/pause.png"));

    ui->btnOriginalBayFilter->setIcon(QIcon(":Resources/Images/images/RealTimeAlarm/filter.png"));
    ui->btnOriginalBayShield->setIcon(QIcon(":Resources/Images/images/RealTimeAlarm/shield.png"));
    ui->btnOriginalPause->setIcon(QIcon(":Resources/Images/images/RealTimeAlarm/pause.png"));

    ui->btnhistoryQuery->setIcon(QIcon(":Resources/Images/images/RealTimeAlarm/query.png"));

    //告警表右击菜单
    connect(ui->tableWidget_SmartAlarm, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showCustomMenu(QPoint)));
    connect(ui->tableWidget_OriginalAlarm, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showCustomMenu(QPoint)));


    //智能告警表右击菜单列表
    smartAlarmMenu = new QMenu(ui->tableWidget_SmartAlarm);
    //修改告警状态
    QAction *iaAlarmStatusAction = new QAction(QString::fromLocal8Bit("确认"), ui->tableWidget_SmartAlarm);
    connect(iaAlarmStatusAction,SIGNAL(triggered(bool)),this,SLOT(confirmAlarm()));
    smartAlarmMenu->addAction(iaAlarmStatusAction);
    //查看告警详情
    QAction *alarmDetailAction = new QAction(QString::fromUtf8("详情"),ui->tableWidget_SmartAlarm);
    connect(alarmDetailAction,SIGNAL(triggered(bool)),this,SLOT(showAlarmDetail()));
    smartAlarmMenu->addAction(alarmDetailAction);
    //删除告警
    QAction *iaDeleteAlarmAction = new QAction(QString::fromUtf8("删除告警"),ui->tableWidget_SmartAlarm);
    connect(iaDeleteAlarmAction,SIGNAL(triggered(bool)),this,SLOT(deleteAlarm()));
    smartAlarmMenu->addAction(iaDeleteAlarmAction);

    //原始告警表右击菜单
    originalAlarmMenu = new QMenu(ui->tableWidget_OriginalAlarm);
    QAction *oaAlarmStatusAction = new QAction(QString::fromLocal8Bit("确认"), ui->tableWidget_OriginalAlarm);
    connect(oaAlarmStatusAction,SIGNAL(triggered(bool)),this,SLOT(confirmAlarm()));
    originalAlarmMenu->addAction(oaAlarmStatusAction);

    QAction *oaDeleteAlarmAction = new QAction(QString::fromUtf8("删除告警"), ui->tableWidget_OriginalAlarm);
    connect(oaDeleteAlarmAction,SIGNAL(triggered(bool)),this,SLOT(deleteAlarm()));
    originalAlarmMenu->addAction(oaDeleteAlarmAction);
}

void RealTimeAlarm::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    QMap<QString, stAlarmConfig> mapAlarmConfig = SingletonDBHelper->queryAlarmConfig();
    DataModel::getInstance()->setMapAlarmConfig(mapAlarmConfig);

    qDeleteAll(listSmartButton.begin(), listSmartButton.end());
    listSmartButton.clear();

    qDeleteAll(listOriginalButton.begin(), listOriginalButton.end());
    listOriginalButton.clear();


    QToolButton *btnSmartAlarm = new QToolButton(this);
    btnSmartAlarm->setText("所有信号");
    btnSmartAlarm->setCheckable(true);
    btnSmartAlarm->setFixedSize(80, 30);
    btnSmartAlarm->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(btnSmartAlarm, SIGNAL(clicked(bool)), this, SLOT(changeCurIaAlarmLevel()));
    listSmartButton.push_back(btnSmartAlarm);
    ui->verticalLayout_SmartAlarm->addWidget(btnSmartAlarm);

    QToolButton *btnOrignalAlarm = new QToolButton(this);
    btnOrignalAlarm->setText("所有信号");
    btnOrignalAlarm->setCheckable(true);
    btnOrignalAlarm->setFixedSize(80, 30);
    btnOrignalAlarm->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(btnOrignalAlarm, SIGNAL(clicked(bool)), this, SLOT(changeCurOrAlarmLevel()));
    listOriginalButton.push_back(btnOrignalAlarm);
    ui->verticalLayout_OrignalAlarm->addWidget(btnOrignalAlarm);


    for(QMap<QString, stAlarmConfig>::iterator it = mapAlarmConfig.begin(); it != mapAlarmConfig.end(); ++it)
    {
        stAlarmConfig alarmConfig = it.value();

        QToolButton *btnSmartAlarm = new QToolButton(this);
        btnSmartAlarm->setText(alarmConfig.alarmName);
        btnSmartAlarm->setCheckable(true);
        btnSmartAlarm->setFixedSize(80, 30);
        btnSmartAlarm->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        connect(btnSmartAlarm, SIGNAL(clicked(bool)), this, SLOT(changeCurIaAlarmLevel()));
        listSmartButton.push_back(btnSmartAlarm);
        ui->verticalLayout_SmartAlarm->addWidget(btnSmartAlarm);

        QToolButton *btnOrignalAlarm = new QToolButton(this);
        btnOrignalAlarm->setText(alarmConfig.alarmName);
        btnOrignalAlarm->setCheckable(true);
        btnOrignalAlarm->setFixedSize(80, 30);
        btnOrignalAlarm->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        connect(btnOrignalAlarm, SIGNAL(clicked(bool)), this, SLOT(changeCurOrAlarmLevel()));
        listOriginalButton.push_back(btnOrignalAlarm);
        ui->verticalLayout_OrignalAlarm->addWidget(btnOrignalAlarm);
    }


    IconHelper::Instance()->setStyle(ui->tab_1);
    IconHelper::Instance()->setStyle(ui->tab_2);

    listSmartButton.at(0)->click();
    listOriginalButton.at(0)->click();
}


//由于告警颜色改变，间隔过滤，间隔屏蔽，修改过滤告警等级，重新显示
void RealTimeAlarm::replaySmartAlarmData()
{
    ui->tableWidget_SmartAlarm->setRowCount(0);

    QMap<int, stAlarmData> mapSmartAlarmData = SingletonAlarmData->getSmartAlarmData();
    QMapIterator<int, stAlarmData> itSmartAlarmData(mapSmartAlarmData);
    while(itSmartAlarmData.hasNext())
    {
        itSmartAlarmData.next();
        dispSmartAlarmData(itSmartAlarmData.key(), itSmartAlarmData.value());
    }

    //根据告警时间排序
    ui->tableWidget_SmartAlarm->sortByColumn(1, Qt::DescendingOrder);
}

//由于告警颜色改变，间隔过滤，间隔屏蔽，修改过滤告警等级，重新显示
void RealTimeAlarm::replayOriginalAlarmData()
{
    ui->tableWidget_OriginalAlarm->setRowCount(0);

    QMap<int, stAlarmData> mapOriginalAlarmData = SingletonAlarmData->getOriginalAlarmData();
    QMapIterator<int, stAlarmData> itOriginalAlarmData(mapOriginalAlarmData);
    while(itOriginalAlarmData.hasNext())
    {
        itOriginalAlarmData.next();
        dispOriginalAlarmData(itOriginalAlarmData.key(), itOriginalAlarmData.value());
    }
    //根据告警时间排序
    ui->tableWidget_OriginalAlarm->sortByColumn(1, Qt::DescendingOrder);
}

void RealTimeAlarm::alarmTabWidgetChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}


/*************************************
Description:  显示智能告警数据
Input:
Output:
Return:
Other:
 **************************************/
void RealTimeAlarm::dispSmartAlarmData(int id, stAlarmData alarmData)
{
    if(curSelectIaAlarmLevel != LEVEL_REALTIME  && curSelectIaAlarmLevel != alarmData.level_)                 //显示所选告警等级
        return;

    if(alarmData.text_.split(".").size() <= 2 )               //数据错误，间隔获取失败
        return;
    QString tmp = alarmData.text_.split(".").at(2);
    QString bayName = tmp.left(tmp.indexOf(" "));
//    if(!pFilterBay->judgeBayValid(bayName, 0))
//        return;

    QString status = alarmData.confirmed_ == 1 ? QString::fromUtf8("已确认") : QString::fromUtf8("未确认");
    QColor statusColor =  alarmData.confirmed_ == 1 ? Qt::cyan : Qt::red;

    //超过最大设置行数，需要删除
    if(ui->tableWidget_SmartAlarm->rowCount() > SingletonConfig->getMaxSize())
    {
        ui->tableWidget_SmartAlarm->removeRow(SingletonConfig->getMaxSize());
    }

    ui->tableWidget_SmartAlarm->insertRow(0); //插入一个空行

    QTableWidgetItem *item0 = new QTableWidgetItem(status);
    item0->setTextColor(statusColor);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmData.time_);
    item1->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item1->setTextAlignment(Qt::AlignCenter);

//    QTableWidgetItem *item2 = new QTableWidgetItem(SingletonAlarmData->getDescByAlarmLevel(alarmData.level_));
//    item2->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
//    item2->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item3 = new QTableWidgetItem(alarmData.text_);
    item3->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item3->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(id));

    ui->tableWidget_SmartAlarm->setItem(0,0,item0);
    ui->tableWidget_SmartAlarm->setItem(0,1,item1);
//    ui->tableWidget_SmartAlarm->setItem(0,2,item2);
    ui->tableWidget_SmartAlarm->setItem(0,3,item3);
    ui->tableWidget_SmartAlarm->setItem(0,4,item4);

    //显示第一条
    ui->tableWidget_SmartAlarm->scrollToTop();
}


/*************************************
Description:  显示原始告警数据
Input:
Output:
Return:
Other:
 **************************************/
void RealTimeAlarm::dispOriginalAlarmData(int id, stAlarmData alarmData)
{
    if(curSelectOrAlarmLevel != LEVEL_REALTIME  && curSelectOrAlarmLevel != alarmData.level_)                 //显示所选告警等级
        return;

    //alarmData.text_ 茂名电网.广场变/10kV.F05馈线.SL10049/B02GOOSE报警
    if(alarmData.text_.split(".").size() <= 2 )               //数据错误，间隔获取失败
        return;
    QString bayName = alarmData.text_.split(".").at(2);
//    if(!pFilterBay->judgeBayValid(bayName, 1))
//        return;

    QString status = alarmData.confirmed_ == 1 ? QString::fromUtf8("已确认") : QString::fromUtf8("未确认");
    QColor statusColor =  alarmData.confirmed_ == 1 ? Qt::cyan : Qt::red;

    //超过最大设置行数，需要删除
    if(ui->tableWidget_OriginalAlarm->rowCount() > SingletonConfig->getMaxSize())
    {
        ui->tableWidget_OriginalAlarm->removeRow(SingletonConfig->getMaxSize());
    }

    ui->tableWidget_OriginalAlarm->insertRow(0); //插入一个空行

    QTableWidgetItem *item0 = new QTableWidgetItem(status);
    item0->setTextColor(statusColor);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmData.time_);
    item1->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item1->setTextAlignment(Qt::AlignCenter);

//    QTableWidgetItem *item2 = new QTableWidgetItem(SingletonAlarmData->getDescByAlarmLevel(alarmData.level_));
//    item2->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
//    item2->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item3 = new QTableWidgetItem(alarmData.text_);
    item3->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item3->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(id));

    ui->tableWidget_OriginalAlarm->setItem(0,0,item0);
    ui->tableWidget_OriginalAlarm->setItem(0,1,item1);
//    ui->tableWidget_OriginalAlarm->setItem(0,2,item2);
    ui->tableWidget_OriginalAlarm->setItem(0,3,item3);
    ui->tableWidget_OriginalAlarm->setItem(0,4,item4);

    //显示第一条
    ui->tableWidget_OriginalAlarm->scrollToTop();
}

void RealTimeAlarm::showCustomMenu(QPoint mousePos)
{
    QTableWidget *tableWidget = static_cast<QTableWidget*>(sender());
    if(tableWidget == ui->tableWidget_SmartAlarm)
    {
        QTableWidgetItem *item = tableWidget->itemAt(QPoint(mousePos.x(),mousePos.y()));
        if(item == NULL)
             return;

        smartAlarmMenu->move (cursor().pos ());
        smartAlarmMenu->show ();
    }
    else if(tableWidget == ui->tableWidget_OriginalAlarm)
    {
        QTableWidgetItem *item = tableWidget->itemAt(QPoint(mousePos.x(),mousePos.y()));
        if(item == NULL)
             return;

        originalAlarmMenu->move (cursor().pos ());
        originalAlarmMenu->show ();
    }
}

void RealTimeAlarm::confirmAlarm()
{
    QAction *AlarmStatus = dynamic_cast<QAction *>(sender());
    if(AlarmStatus->parent() == ui->tableWidget_SmartAlarm)                    //确认智能告警
    {

        int row = ui->tableWidget_SmartAlarm->currentRow();
        QString id =  ui->tableWidget_SmartAlarm->item(row, 4)->text();        //获取当前行的告警序号

        qDebug()<<QDateTime::currentDateTime();
        if(SingletonDBHelper->modifyIaAlarmStatus(id.toInt()))
        {
              qDebug()<<QDateTime::currentDateTime();
            //SingletonAlarmData->modifySmartAlarmStatus(id.toInt());
            ui->tableWidget_SmartAlarm->item(row,0)->setTextColor(Qt::cyan);
            ui->tableWidget_SmartAlarm->item(row,0)->setText(QString::fromLocal8Bit("已确认"));
        }
    }
    else if(AlarmStatus->parent() == ui->tableWidget_OriginalAlarm)            //确认原始告警
    {
        int row = ui->tableWidget_OriginalAlarm->currentRow();
        QString alarmTime = ui->tableWidget_OriginalAlarm->item(row, 1)->text();      //获取当前行的告警时间
        QString id = ui->tableWidget_OriginalAlarm->item(row, 4)->text();             //获取当前行的告警序号
        if(SingletonDBHelper->modifyOaAlarmStatus(alarmTime ,id.toInt()))
        {
            //SingletonAlarmData->modifyOriginalAlarmStatus(id.toInt());
            ui->tableWidget_OriginalAlarm->item(row,0)->setTextColor(Qt::cyan);
            ui->tableWidget_OriginalAlarm->item(row,0)->setText(QString::fromLocal8Bit("已确认"));
        }
    }
}

void RealTimeAlarm::showAlarmDetail()
{
    int row = ui->tableWidget_SmartAlarm->currentRow();        //鼠标左击时，可能当前行被删除了
    if(row >= 0)
    {
        QString id = ui->tableWidget_SmartAlarm->item(row, 4)->text();
        QString fileName = SingletonAlarmData->getSmartAlarmFileNameByID(id.toInt());

//        AlarmReportParse alarmReportParse = alarmDetail->showAlarmDetail(fileName);
//        bool enable = alarmReportParse.getScene() == QString::fromLocal8Bit("遥控过程分析") ? true : false;
//        alarmDetail->showRemoteControlProcess(enable);
    }
}




void RealTimeAlarm::deleteAlarm()
{
    QAction *action = dynamic_cast<QAction*>(sender());
    if(action->parent() == ui->tableWidget_SmartAlarm)
    {
        int row = ui->tableWidget_SmartAlarm->currentRow();
        QString id = ui->tableWidget_SmartAlarm->item(row, 4)->text();

        if(SingletonAlarmData->removeSmartAlarmByID(id.toInt()))
        {
            ui->tableWidget_SmartAlarm->removeRow(row);
//            qDebug()<<QString::fromUtf8("用户:%1 删除智能告警，告警ID:%2").arg(SingletonConfig->getCurUser()).arg(id);
        }
    }
    else if(action->parent() == ui->tableWidget_OriginalAlarm)
    {
        int row = ui->tableWidget_OriginalAlarm->currentRow();
        QString id = ui->tableWidget_OriginalAlarm->item(row, 4)->text();

        if(SingletonAlarmData->removeOriginalAlarmByID(id.toInt()))
        {
            ui->tableWidget_OriginalAlarm->removeRow(row);
//            qDebug()<<QString::fromUtf8("用户:%1 删除原始告警，告警ID:%2").arg(SingletonConfig->getCurUser()).arg(id);
        }
    }
}

//暂停智能告警数据显示
void RealTimeAlarm::pauseSmartAlarmData(bool status)
{
    if(status == true)
    {
        ui->btnSmartPause->setText(QString::fromUtf8("恢复"));
        disconnect(SingletonAlarmData, SIGNAL(receiverSmartAlarmData(int,stAlarmData)), this, SLOT(dispSmartAlarmData(int, stAlarmData)));
    }
    else
    {
        ui->btnSmartPause->setText(QString::fromUtf8("暂停"));
        connect(SingletonAlarmData, SIGNAL(receiverSmartAlarmData(int,stAlarmData)), this, SLOT(dispSmartAlarmData(int, stAlarmData)));
        replaySmartAlarmData();
    }
}


//暂停原始告警数据显示
void RealTimeAlarm::pauseOriginalAlarmData(bool status)
{
    if(status == true)
    {
        ui->btnOriginalPause->setText(QString::fromUtf8("恢复"));
        disconnect(SingletonAlarmData, SIGNAL(receiverOriginalAlarmData(int,stAlarmData)), this, SLOT(dispOriginalAlarmData(int, stAlarmData)));
    }
    else
    {
        ui->btnOriginalPause->setText(QString::fromUtf8("暂停"));
        connect(SingletonAlarmData, SIGNAL(receiverOriginalAlarmData(int,stAlarmData)), this, SLOT(dispOriginalAlarmData(int, stAlarmData)));
        replayOriginalAlarmData();
    }
}



void RealTimeAlarm::changeCurIaAlarmLevel()
{
    QToolButton *b = static_cast<QToolButton *>(sender());
    int count = listSmartButton.count();
    for (int i = 0; i < count; i++) {
        if (listSmartButton.at(i) == b) {
            listSmartButton.at(i)->setChecked(true);
            listSmartButton.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(listSmartButton.at(i), false)));

        } else {
            listSmartButton.at(i)->setChecked(false);
            listSmartButton.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(listSmartButton.at(i), true)));
        }
    }
}

void RealTimeAlarm::changeCurOrAlarmLevel()
{
    QToolButton *b = static_cast<QToolButton *>(sender());
    int count = listOriginalButton.count();
    for (int i = 0; i < count; i++) {
        if (listOriginalButton.at(i) == b) {
            listOriginalButton.at(i)->setChecked(true);
            listOriginalButton.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(listOriginalButton.at(i), false)));

        } else {
            listOriginalButton.at(i)->setChecked(false);
            listOriginalButton.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(listOriginalButton.at(i), true)));
        }
    }
}


void RealTimeAlarm::selectFilterBay()
{
    QPushButton *button = dynamic_cast<QPushButton*>(sender());
    if(button == ui->btnSmartBayFilter)
    {
        filterBay->showSelectBay(0);
        filterBay->setTitleContent(" 智能告警间隔过滤");
    }
    else if(button == ui->btnSmartBayShield)
    {
        filterBay->showSelectBay(1);
        filterBay->setTitleContent(" 智能告警间隔屏蔽");
    }
    else if(button == ui->btnOriginalBayFilter)
    {
        filterBay->showSelectBay(2);
        filterBay->setTitleContent(" 原始告警间隔过滤");
    }
    else if(button == ui->btnOriginalBayShield)
    {
        filterBay->showSelectBay(3);
        filterBay->setTitleContent(" 原始告警间隔屏蔽");
    }


    filterBay->move((this->width() - filterBay->width()) / 2, (this->height() - filterBay->height()) / 2);
}



