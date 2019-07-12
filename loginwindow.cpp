#include "loginwindow.h"
#include "ui_loginwindow.h"

#define TRANSLATE_WIDTH 80

LoginWindow::LoginWindow(QWidget *parent)
	: BaseWindow(parent)
    , ui(new Ui::LoginWindow)
    , m_view(&m_scene)
	, m_graphicsWidget(NULL)
{
	ui->setupUi(this);
    initWindow();
    // 初始化标题栏;
    initTitleBar(this);
    ui->btnRotate->raise();
    this->loadStyleSheet(":/Resources/LoginWindow/LoginWindow.css");
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::showEvent(QShowEvent *)
{
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    ui->usernameEdit->setFocus();
}

// 初始化窗口;
void LoginWindow::initWindow()
{
    //背景GIG图;
    QMovie *movie = new QMovie(this);
    movie->setFileName(":/Resources/LoginWindow/back.gif");
    movie->start();

    QLabel* pBack = new QLabel(this);
    pBack->setMovie(movie);

    //暗注释;
    ui->usernameEdit->setPlaceholderText(QStringLiteral("用户名"));

    ui->passwordEdit->setPlaceholderText(QStringLiteral("密码"));

    ui->userHead->setPixmap(QPixmap(":/Resources/LoginWindow/HeadImage.png"));

    connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(slot_btnLogin_clicked()));


    signalMapper = new QSignalMapper(this);
    connect(ui->btnRotate, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->btnRotate, 1);

    connect(ui->btnModifyPassword, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->btnModifyPassword, 2);
}


void LoginWindow::slot_btnLogin_clicked()
{
    if(!db.isOpen())
    {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName(SingletonConfig->getIpMySql());
        db.setPort(SingletonConfig->getPortMySql());
        db.setDatabaseName("history");
        db.setUserName(SingletonConfig->getUserMysql());
        db.setPassword(SingletonConfig->getPasswdMysql());
        if(db.open())
        {
            qDebug()<<QString("Mysql 连接成功:%1:%2").arg( SingletonConfig->getIpMySql()).arg(SingletonConfig->getPortMySql());
        }
        else
        {
            qDebug()<<db.lastError().text();

            myHelper::ShowMessageBoxInfo(db.lastError().text());
            return;
        }
    }

    Data_UserLogin data_UserLogin;
    SingletonDBHelper->queryUserLoginInfo(data_UserLogin);
    DataModel::getInstance()->setAllUserInfoMap(data_UserLogin.allUsersMap); //用户信息
    DataModel::getInstance()->setAllUserFuncRwMap(data_UserLogin.roleMenuReadWriteMap); //设置所有用户的读写权限

    QString userName = ui->usernameEdit->text(); //用户名
    QString userPwd = ui->passwordEdit->text(); //密码

    if(userName.isEmpty() || userPwd.isEmpty())
    {
        myHelper::ShowMessageBoxInfo("登录失败，用户名/密码不能为空！");
        ui->passwordEdit->clear();
        return;
    }

    User_Info info;
    if(DataModel::getInstance()->findUserInfoByName(userName, info))
    {
       if(info.isEnable.compare("yes", Qt::CaseInsensitive) != 0)
       {
           myHelper::ShowMessageBoxInfo("登录失败，用户没有审核！");
           ui->passwordEdit->clear();
           return;
       }

       if(info.loginStatus.compare("yes", Qt::CaseInsensitive) == 0)
       {
           myHelper::ShowMessageBoxInfo("登录失败，该用户已在其他地方登录！");
           ui->passwordEdit->clear();
           return;
       }

       if(info.password.compare(userPwd, Qt::CaseSensitive) != 0)
       {
           myHelper::ShowMessageBoxInfo("登录失败，登录密码不正确！");
           ui->passwordEdit->clear();
           return;
       }
    }
    else
    {
           myHelper::ShowMessageBoxInfo("登录失败，该用户不存在，请重新输入！");
           ui->passwordEdit->clear();
           return;
    }

    qDebug()<<QString("用户登录成功:%1").arg(userName);

    QMap<QString, stAlarmConfig> mapAlarmConfig = SingletonDBHelper->queryAlarmConfig();
    DataModel::getInstance()->setMapAlarmConfig(mapAlarmConfig);

    emit signal_btnClose_Clicked();
}


