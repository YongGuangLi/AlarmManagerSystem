#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), isRunning(true), redisHelper(NULL),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":Resources/Image/main_logo.png"));

    FramelessHelper *pHelper = new FramelessHelper(this);
    pHelper->setTitleHeight(this->height());
    pHelper->activateOn(this);  //激活当前窗体

    this->initMainWindow();
    this->initLeftMain();
    this->initLeftConfig();
    this->initAlarmSence();

    SingletonConfig->initConfigHelper(qApp->applicationDirPath() + QDir::separator() + "SysConfig.ini");

//    analysisPcapFile = new AnalysisPcapFile(this);
//    ui->verticalLayout_mainWidget->addWidget(analysisPcapFile);
//    QList<QString> listPcapFile;
//    listPcapFile<<qApp->applicationDirPath() + QDir::separator() + "mms_20170907152357.pcap";
//    listPcapFile<<qApp->applicationDirPath() + QDir::separator() + "mms_20170907151920.pcap";
//    listPcapFile<<qApp->applicationDirPath() + QDir::separator() + "mms_20170907152230.pcap";
//    listPcapFile<<qApp->applicationDirPath() + QDir::separator() + "mms_20170907152053.pcap";
//    analysisPcapFile->SetPacpFileToTree(listPcapFile);

    //基本配置
    baseConfig = new BaseConfig(this);
    ui->gridLayout_BaseConfig->addWidget(baseConfig);

    //用户配置
    userConfig = new UserConfig(this);
    ui->gridLayout_UserConfig->addWidget(userConfig);

    //角色配置
    roleConfig = new RoleConfig(this);
    ui->gridLayout_RoleConfig->addWidget(roleConfig);

    //登录窗口
    rotateWidget = new RotateWidget(this);
    rotateWidget->hide();

    //历史告警查询
    historyAlarmQuery = new HistoryAlarmQuery(this);
    ui->verticalLayout_5->addWidget(historyAlarmQuery);

    //关于窗口
    versionDialog = new VersionDialog(this);
    versionDialog->hide();

    //统计数据查询
    statisticAnalysis = new StatisticAnalysis(this);
    ui->verticalLayout_4->addWidget(statisticAnalysis);

    //浮动窗口
    floatingWindow = new FloatingWindow(this);
    floatingWindow->hide();

    //实时告警
    realTimeAlarm = new RealTimeAlarm(this);
    ui->gridLayout->addWidget(realTimeAlarm);

    SettingDialog::getInstance(this)->initMyTitle();
    SettingDialog::getInstance()->move((this->width() -  SettingDialog::getInstance()->width()) / 2, (this->height() -  SettingDialog::getInstance()->height()) / 2);
    SettingDialog::getInstance()->hide();


    connect(ui->btnMenu_Min, SIGNAL(clicked(bool)), this, SLOT(slot_btnMenu_Min_clicked()));
    connect(ui->btnMenu_Max, SIGNAL(clicked(bool)), this, SLOT(slot_btnMenu_Max_clicked()));
    connect(ui->btnMenu_Close, SIGNAL(clicked(bool)), this, SLOT(slot_btnMenu_Close_clicked()));


    //redis订阅
    QtConcurrent::run(this, &MainWindow::slot_redisSubscribe);


    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(SingletonConfig->getIpMySql());
    db.setPort(SingletonConfig->getPortMySql());
    db.setDatabaseName("history");
    db.setUserName(SingletonConfig->getUserMysql());
    db.setPassword(SingletonConfig->getPasswdMysql());
    db.open();
    ui->btnMenu_Max->click();
}

MainWindow::~MainWindow()
{
    isRunning = false;
    if(redisHelper != NULL)
    {
        if(redisHelper->check_connect())
            redisHelper->publish(REDIS_CHANNEL, "exit");
        delete redisHelper;
    }
    delete ui;
}

void MainWindow::initMainWindow()
{   
    this->setProperty("form", true);
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);

    IconHelper::Instance()->setIcon(ui->btnMenu_Min, QChar(0xf068));
    IconHelper::Instance()->setIcon(ui->btnMenu_Max, QChar(0xf067));
    IconHelper::Instance()->setIcon(ui->btnMenu_Close, QChar(0xf00d));

    ui->widgetTitle->setProperty("form", "title");
    ui->widgetTop->setProperty("nav", "top");
    ui->labTitle->setText("全景数据集成及智能告警系统");
    ui->labTitle->setFont(QFont("Microsoft Yahei", 20));
    this->setWindowTitle(ui->labTitle->text());

    QSize icoSize(32, 32);
    int icoWidth = 85;

    //设置顶部导航按钮
    QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns) {
        btn->setIconSize(icoSize);
        btn->setMinimumWidth(icoWidth);
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked()), this, SLOT(slot_btnTitle_Clicked()));
    }

    ui->btnMain->click();

    ui->widgetLeftMain->setProperty("flag", "left");
    ui->widgetLeftConfig->setProperty("flag", "left");
    ui->page1->setStyleSheet(QString("QWidget[flag=\"left\"] QAbstractButton{min-height:%1px;max-height:%1px;}").arg(60));
    ui->page2->setStyleSheet(QString("QWidget[flag=\"left\"] QAbstractButton{min-height:%1px;max-height:%1px;}").arg(20));
}

void MainWindow::slot_btnTitle_Clicked()
{
    QToolButton *b = static_cast<QToolButton *>(sender());
    QString name = b->text();

    QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns) {
        if (btn == b) {
            btn->setChecked(true);
        } else {
            btn->setChecked(false);
        }
    }

    if(name == "主界面") {
        ui->stackedWidget->setCurrentIndex(0);
    }else if (name == "系统配置") {
        ui->stackedWidget->setCurrentIndex(1);
    }else if (name == "告警查询") {
        ui->stackedWidget->setCurrentIndex(2);
    }else if (name == "统计分析") {
        ui->stackedWidget->setCurrentIndex(3);
    }else if (name == "实时告警") {
        ui->stackedWidget->setCurrentIndex(4);
    }else if (name == "关于") {
        versionDialog->show();
        versionDialog->move((this->width() - versionDialog->width()) / 2, (this->height() - versionDialog->height()) / 2);
    }else if (name == "用户登录") {
        rotateWidget->show();
        rotateWidget->move((this->width() - rotateWidget->width()) / 2, (this->height() - rotateWidget->height()) / 2);
    }else if (name == "用户退出") {
        qApp->exit(RETCODE_RESTART);
    }
}

void MainWindow::initLeftMain()
{
    QList<int> pixCharMain;
    pixCharMain << 0xf030 << 0xf03e << 0xf247;
    btnsMain << ui->tbtnMain1 << ui->tbtnMain2 << ui->tbtnMain3;

    int count = btnsMain.count();
    for (int i = 0; i < count; i++) {
        btnsMain.at(i)->setCheckable(true);
        btnsMain.at(i)->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        connect(btnsMain.at(i), SIGNAL(clicked(bool)), this, SLOT(slot_btnLeftMain_Clicked()));
    }

    IconHelper::Instance()->setStyle(ui->widgetLeftMain, btnsMain, pixCharMain, 15, 35, 25, "left", 4);

    ui->tbtnMain1->click();
}

void MainWindow::initLeftConfig()
{
    QList<int> pixCharConfig;
    pixCharConfig << 0xf031 << 0xf036 << 0xf249 << 0xf055 << 0xf05a;// << 0xf249;
    btnsConfig << ui->btnBaseConfig << ui->btnUserConfig << ui->btnRoleConfig << ui->tbtnConfig2 << ui->tbtnConfig5;

    int count = btnsConfig.count();
    for (int i = 0; i < count; i++) {
        btnsConfig.at(i)->setCheckable(true);
        btnsConfig.at(i)->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        connect(btnsConfig.at(i), SIGNAL(clicked(bool)), this, SLOT(slot_btnLeftConfig_Clicked()));
    }

    IconHelper::Instance()->setStyle(ui->widgetLeftConfig, btnsConfig, pixCharConfig, 10, 20, 15, "left", 4);

    ui->btnBaseConfig->click();
}

void MainWindow::initAlarmSence()
{
    ui->gridLayout_3->setAlignment(Qt::AlignTop);

    QLabel *label1 = new QLabel(QString::fromLocal8Bit("一次设备智能告警"));
    label1->setStyleSheet("color: white; font-size: 20px;");
    label1->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    ui->gridLayout_3->addWidget(label1, 0, 0, 1, 3);

    pushButton_SwitchStatusCheck = new PushButton(QString::fromUtf8("开关分合\n状态检查"));
    pushButton_SwitchStatusCheck->setIcon(":/Resources/Image/Scene/switchstatuscheck.png");
    pushButton_SwitchStatusCheck->setFixedSize(100,110);
    connect(pushButton_SwitchStatusCheck, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_SwitchStatusCheck, 1, 0);

    pushButton_SwitchActionTimeCheck  = new PushButton(QString::fromUtf8("开关分合动\n作时间检验"));
    pushButton_SwitchActionTimeCheck->setIcon(":/Resources/Image/Scene/switchactiontimecheck.png");
    pushButton_SwitchActionTimeCheck->setFixedSize(100,110);
    connect(pushButton_SwitchActionTimeCheck, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_SwitchActionTimeCheck, 1, 1);

    pushButton_PrimaryEquipmentAbnormal = new PushButton(QString::fromUtf8("一次设备异常"));
    pushButton_PrimaryEquipmentAbnormal->setIcon(":/Resources/Image/Scene/primaryequipmentabnormal.png");
    pushButton_PrimaryEquipmentAbnormal->setFixedSize(100,110);
    connect(pushButton_PrimaryEquipmentAbnormal, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_PrimaryEquipmentAbnormal, 1, 2);


    QLabel *label2 = new QLabel(QString::fromLocal8Bit("二次设备智能告警"));
    label2->setStyleSheet("color: white; font-size: 20px;");
    label2->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    label2->setMinimumHeight(50);
    ui->gridLayout_3->addWidget(label2, 2, 0, 1, 3);

    pushButton_SecondaryEquipmentAbnormal = new PushButton(QString::fromUtf8("二次设备异常"));
    pushButton_SecondaryEquipmentAbnormal->setIcon(":/Resources/Image/Scene/secondaryequipmentabnormal.png");
    pushButton_SecondaryEquipmentAbnormal->setFixedSize(100,110);
    connect(pushButton_SecondaryEquipmentAbnormal, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_SecondaryEquipmentAbnormal, 3, 0);

    pushButton_SingleBayProtectAction = new PushButton(QString::fromUtf8("保护动作"));
    pushButton_SingleBayProtectAction->setIcon(":/Resources/Image/Scene/singlebayprotectaction.png");
    pushButton_SingleBayProtectAction->setFixedSize(100,110);
    connect(pushButton_SingleBayProtectAction, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_SingleBayProtectAction, 3, 1);

    pushButton_RemoteControlProcess = new PushButton(QString::fromUtf8("遥控过程分析"));
    pushButton_RemoteControlProcess->setIcon(":/Resources/Image/Scene/remotecontrolprocess.png");
    pushButton_RemoteControlProcess->setFixedSize(100,110);
    connect(pushButton_RemoteControlProcess, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_RemoteControlProcess, 3, 2);

    pushButton_StationTimeSyncAbnormol = new PushButton(QString::fromUtf8("全站时间\n同步异常"));
    pushButton_StationTimeSyncAbnormol->setIcon(":/Resources/Image/Scene/stationtimesyncabnormol.png");
    pushButton_StationTimeSyncAbnormol->setFixedSize(100,110);
    connect(pushButton_StationTimeSyncAbnormol, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_StationTimeSyncAbnormol, 4, 0);

    pushButton_DeviceCommInterrupt = new PushButton(QString::fromUtf8("通信异常"));
    pushButton_DeviceCommInterrupt->setIcon(":/Resources/Image/Scene/devicecomminterrupt.png");
    pushButton_DeviceCommInterrupt->setFixedSize(100,110);
    connect(pushButton_DeviceCommInterrupt, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_DeviceCommInterrupt, 4, 1);

    pushButton_SoeTimestampChange = new PushButton(QString::fromUtf8("SOE时标跳变"));
    pushButton_SoeTimestampChange->setIcon(":/Resources/Image/Scene/soetimestampchange.png");
    pushButton_SoeTimestampChange->setFixedSize(100,110);
    connect(pushButton_SoeTimestampChange, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_SoeTimestampChange, 4, 2);

    pushButton_RemoteMeasuring = new PushButton(QString::fromUtf8("遥测跳变"));
    pushButton_RemoteMeasuring->setIcon(":/Resources/Image/Scene/remotemeasuring.png");
    pushButton_RemoteMeasuring->setFixedSize(100,110);
    connect(pushButton_RemoteMeasuring, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_RemoteMeasuring, 5, 0);

    pushButton_RemoteSignalling = new PushButton(QString::fromUtf8("遥信状态\n不一致"));
    pushButton_RemoteSignalling->setIcon(":/Resources/Image/Scene/remotesignalling.png");
    pushButton_RemoteSignalling->setFixedSize(100,110);
    connect(pushButton_RemoteSignalling, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_RemoteSignalling, 5, 1);

    QLabel *label3 = new QLabel(QString::fromLocal8Bit("辅助系统智能告警"));
    label3->setStyleSheet("color: white; font-size: 20px;");
    label3->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    label3->setMinimumHeight(50);
    ui->gridLayout_3->addWidget(label3, 6, 0, 1, 3);

    pushButton_AuxiliaryPowerCheck = new PushButton(QString::fromUtf8("辅助电源\n状态检测"));
    pushButton_AuxiliaryPowerCheck->setIcon(":/Resources/Image/Scene/auxiliarypowercheck.png");
    pushButton_AuxiliaryPowerCheck->setFixedSize(100,110);
    connect(pushButton_AuxiliaryPowerCheck, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_AuxiliaryPowerCheck, 7, 0);

    pushButton_ElectricRunCheck = new PushButton(QString::fromUtf8("电气运行\n环境检测"));
    pushButton_ElectricRunCheck->setIcon(":/Resources/Image/Scene/electricruncheck.png");
    pushButton_ElectricRunCheck->setFixedSize(100,110);
    connect(pushButton_ElectricRunCheck, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_ElectricRunCheck, 7, 1);

    pushButton_PowerSystemAbnormal = new PushButton(QString::fromUtf8("电源系统异常"));
    pushButton_PowerSystemAbnormal->setIcon(":/Resources/Image/Scene/powersystemabnormal.png");
    pushButton_PowerSystemAbnormal->setFixedSize(100,110);
    connect(pushButton_PowerSystemAbnormal, SIGNAL(clicked()), this, SLOT(slot_changeAlarmSence()));
    ui->gridLayout_3->addWidget(pushButton_PowerSystemAbnormal, 7, 2);
}

void MainWindow::slot_btnLeftMain_Clicked()
{
    QToolButton *b = static_cast<QToolButton *>(sender());
    QString name = b->text();

    int count = btnsMain.count();
    for (int i = 0; i < count; i++) {
        if (btnsMain.at(i) == b) {
            btnsMain.at(i)->setChecked(true);
            btnsMain.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btnsMain.at(i), false)));
        } else {
            btnsMain.at(i)->setChecked(false);
            btnsMain.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btnsMain.at(i), true)));
        }
    }
}

void MainWindow::slot_btnLeftConfig_Clicked()
{
    QToolButton *b = static_cast<QToolButton *>(sender());

    int count = btnsConfig.count();
    for (int i = 0; i < count; i++) {
        if (btnsConfig.at(i) == b) {
            btnsConfig.at(i)->setChecked(true);
            btnsConfig.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btnsConfig.at(i), false)));

            ui->stackedWidget_SysConfig->setCurrentIndex(i);
        } else {
            btnsConfig.at(i)->setChecked(false);
            btnsConfig.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btnsConfig.at(i), true)));
        }
    }
}

void MainWindow::slot_changeAlarmSence()
{
    PushButton *button = dynamic_cast<PushButton*>(sender());
    qDebug()<<button->text();
}

void MainWindow::slot_redisSubscribe()
{
    redisHelper = new RedisHelper(QString("%1:%2").arg(SingletonConfig->getIpRedis()).arg(SingletonConfig->getPortRedis()).toStdString(), SingletonConfig->getPasswdRedis().toStdString());

    while(isRunning)
    {
        if(!redisHelper->check_connect())
        {
            if(redisHelper->open())
            {
                qDebug()<<QString("Redis 连接成功:%1:%2").arg(SingletonConfig->getIpRedis()).arg(SingletonConfig->getPortRedis());
                if(redisHelper->subscribe(REDIS_CHANNEL, NULL) >= 1)
                    qDebug()<<QString("Redis 订阅成功:%1").arg(REDIS_CHANNEL);
            }else
            {
                qDebug()<<QString("Redis 连接失败:%1:%2").arg(SingletonConfig->getIpRedis()).arg(SingletonConfig->getPortRedis());
                QThread::sleep(1);
                continue;
            }
        }

        string message;
        if(redisHelper->getMessage(message))
        {
            RtdbMessage rtdbMessage;
            if(rtdbMessage.ParseFromString(message))
            {
                switch(rtdbMessage.messagetype())
                {
                case TYPE_ALARMDATAMSG:
                {
                    AlarmDataMsg alarmDataMsg = rtdbMessage.alarmdatamsg();
                    qDebug()<<alarmDataMsg.id().c_str()<<alarmDataMsg.level()<<alarmDataMsg.time().c_str()
                            <<QString::fromUtf8(alarmDataMsg.devicename().c_str())<<QString::fromUtf8(alarmDataMsg.msg().c_str())
                            <<QString::fromUtf8(alarmDataMsg.scene().c_str())<<alarmDataMsg.filename().c_str();               //对方用QString::toStdString，不能用UTF8接受 要用toUtf8

                    if(alarmDataMsg.type() == 1)           //原始告警
                    {
                        stAlarmData alarmData;
                        alarmData.id_ = QString::fromStdString(alarmDataMsg.id()).toInt();
                        alarmData.level_ = alarmDataMsg.level();
                        alarmData.time_ = QString::fromStdString(alarmDataMsg.time());
                        alarmData.text_ = QString::fromUtf8(alarmDataMsg.devicename().c_str()) + " " + QString::fromUtf8(alarmDataMsg.msg().c_str());
                        alarmData.confirmed_ = 0;
                        SingletonAlarmData->insertOriginalAlarmData(alarmData);
                    }
                    else if(alarmDataMsg.type() == 2)    //智能告警
                    {
                        stAlarmData alarmData;
                        string id = alarmDataMsg.id();
                        alarmData.id_ = QString::fromStdString(id).left(id.length() - 1).toInt();
                        alarmData.level_ = alarmDataMsg.level();
                        alarmData.time_ = QString::fromStdString(alarmDataMsg.time());
                        alarmData.text_ = QString::fromUtf8(alarmDataMsg.devicename().c_str()) + " " + QString::fromUtf8(alarmDataMsg.msg().c_str());
                        alarmData.confirmed_ = 0;
                        alarmData.filename_ =  QString(alarmDataMsg.filename().c_str());

                        SingletonAlarmData->insertSmartAlarmData(alarmData);
                    }
                    break;
                }
                case TYPE_REMOTECONTROL:
                    break;
                case TYPE_REALPOINT:
                {
                    QString strPointName = QString::fromStdString(rtdbMessage.realpointvalue().pointaddr());
                    QString strValue = QString::fromStdString(rtdbMessage.realpointvalue().pointvalue());

                    break;
                }
                default:
                    break;
                }
            }
            else if(message != "exit")
            {
                qWarning()<<"ParseFromString Failure";
            }
        }
        QThread::usleep(1000);
    }
}


void MainWindow::slot_btnMenu_Min_clicked()
{
    showMinimized();
}

void MainWindow::slot_btnMenu_Max_clicked()
{
    static bool max = false;
    static QRect location = this->geometry();

    if (max) {
        this->setGeometry(location);
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
    }

    this->setProperty("canMove", max);
    max = !max;
}

void MainWindow::slot_btnMenu_Close_clicked()
{
    close();
}

