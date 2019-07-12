#include "userconfig.h"
#include "ui_userconfig.h"

UserConfig::UserConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserConfig)
{
    ui->setupUi(this);
    ui->pBtn_add->setIcon(QIcon(":Resources/Image/add.png"));
    ui->pBtn_modify->setIcon(QIcon(":Resources/Image/modify.png"));
    ui->pBtn_delete->setIcon(QIcon(":Resources/Image/delete.png"));

    initTable(ui->tableWidget); //初始化表格

    m_nameExp = QRegExp("^[a-zA-Z][a-zA-Z0-9_]{4,15}$"); //用户名正则表达式
    //m_pwdExp = QRegExp("^(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z])(?=.*[~!@#$%^&*?]).{8,}$"); //密码正则表达式
    m_pwdExp = QRegExp("^(?=.*[0-9])(?=.*[a-zA-Z])(?=.*[_~!@#$%^&*?]).{8,}$"); //密码正则表达式
}

UserConfig::~UserConfig()
{
    delete ui;
}

//初始化表格
void UserConfig::initTable(QTableWidget *tableWidget)
{
    QStringList header;
    header.clear();
    header<<QString("序号")<<QString("唯一标识")<<QString("用户名称")<<QString("角色名称")<<QString("用户姓名")
         <<QString("系统内置")<<QString("是否启用")<<QString("备注")<<QString("有效时间")<<QString("密码");

    tableWidget->setColumnCount(10);
    tableWidget->verticalHeader()->setHidden(true); //隐藏垂直表头
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tableWidget->setAlternatingRowColors(true);
    //tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget->setCurrentCell(-1, -1);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //单选
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidget->horizontalHeader()->setFixedHeight(25);
    tableWidget->horizontalHeader()->setHighlightSections(false); //表头塌陷
    //将唯一标识列隐藏
    tableWidget->setColumnHidden(1, true);

    tableWidget->setColumnHidden(6, true);

    tableWidget->setColumnHidden(8, true);
    //将密码列隐藏
    tableWidget->setColumnHidden(9, true);
}

//表格添加一行
void UserConfig::addRowOnTable(QTableWidget *tableWidget, User_Info info)
{
    int curRow = tableWidget->rowCount();
    tableWidget->insertRow(curRow); //插入一行
    tableWidget->setRowHeight(curRow, 25);

    tableWidget->setItem(curRow, 0, new QTableWidgetItem(QString::number(curRow + 1)));
    tableWidget->setItem(curRow, 1, new QTableWidgetItem(info.userId));
    tableWidget->setItem(curRow, 2, new QTableWidgetItem(info.userName));
    tableWidget->setItem(curRow, 3, new QTableWidgetItem(info.roleName));
    tableWidget->setItem(curRow, 4, new QTableWidgetItem(info.realName));
    //系统内置
    if(info.isSystem.compare("yes", Qt::CaseInsensitive) == 0)
        tableWidget->setItem(curRow, 5, new QTableWidgetItem("是"));
    else
        tableWidget->setItem(curRow, 5, new QTableWidgetItem("否"));

    //是否启用
    if(info.isEnable.compare("yes", Qt::CaseInsensitive) == 0)
        tableWidget->setItem(curRow, 6, new QTableWidgetItem("是"));
    else
        tableWidget->setItem(curRow, 6, new QTableWidgetItem("否"));

    qint64 value = DataModel::getInstance()->getPwdActiveTimeLimitCur();
    info.activeTime = info.activeTime.addSecs(value); //加上有效时间

    tableWidget->setItem(curRow, 7, new QTableWidgetItem(info.remark));
    tableWidget->setItem(curRow, 8, new QTableWidgetItem(info.activeTime.toString("yyyy-MM-dd hh:mm:ss")));
    tableWidget->setItem(curRow, 9, new QTableWidgetItem(info.password));

    tableWidget->item(curRow, 0)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 1)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 2)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 3)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 4)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 5)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 6)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 7)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 8)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 9)->setTextAlignment(Qt::AlignCenter);

    if(info.activeTime <= DataModel::getInstance()->getCurrentSysDateTime())
        tableWidget->item(curRow, 8)->setTextColor(QColor(255, 0, 0)); //有效时间到期显示红
}

//设置功能ID
void UserConfig::setFuncId(QString id)
{
    m_myFuncId = id;
}

//设置写权限
void UserConfig::setWriteAccessFlag(bool flag)
{

}

//验证用户是否已存在
bool UserConfig::isExistThisUserName(QString name)
{
    bool isExist = false;
    QMapIterator<QString, User_Info> it(m_data_UserConfig.allUsersMap);
    while(it.hasNext())
    {
        it.next();
        User_Info userInfo = it.value();
        //用户名称
        if(userInfo.userName.compare(name, Qt::CaseSensitive) == 0)
        {
            isExist = true;
            break;
        }
    }

    return isExist;
}

void UserConfig::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    QMultiMap<QString, MenuFunc_Info> allMenuFuncInfoMa;
    SingletonDBHelper->queryMenuFunc(allMenuFuncInfoMa);
    SettingDialog::getInstance()->setAllMenuFuncInfoMap(allMenuFuncInfoMa); //所有的功能菜单

    Data_UserConfig data_UserConfig;   //用户配置页面数据
    SingletonDBHelper->queryUserConfigInfo(data_UserConfig);
    recvMsg(data_UserConfig);
}

//观察者从订阅主题接收的消息
void UserConfig::recvMsg(Data_UserConfig data_UserConfig)
{
    //数据库查询结果
    ui->tableWidget->setRowCount(0);
    m_data_UserConfig = data_UserConfig; //用户配置页面数据
    QMapIterator<QString, User_Info> it(m_data_UserConfig.allUsersMap);
    while(it.hasNext())
    {
        it.next();
        addRowOnTable(ui->tableWidget, it.value()); //表格添加一行
    }
}

//新增按钮
void UserConfig::on_pBtn_add_clicked()
{
    SettingDialog::getInstance()->addUserInfo(m_data_UserConfig.allRoleInfoMap); //设置界面
    if(SettingDialog::getInstance()->exec() == QDialog::Accepted)
    {
        User_Info userInfo = SettingDialog::getInstance()->getUserConfResult();
        QString uuid = QUuid::createUuid().toString();
        uuid = uuid.mid(1, uuid.length() - 2); //去掉大括号

        if(userInfo.userName.isEmpty())   //!m_nameExp.exactMatch(userInfo.userName) ||
        {
            myHelper::ShowMessageBoxInfo("新增失败，用户名称格式不正确！");
            return;
        }
        if(isExistThisUserName(userInfo.userName))
        {
            myHelper::ShowMessageBoxInfo("新增失败，该用户名称已存在！");
            return;
        }
        if(userInfo.realName.isEmpty())
        {
            myHelper::ShowMessageBoxInfo("新增失败，请填写用户姓名！");
            return;
        }
        if(userInfo.password.isEmpty())
        {
            myHelper::ShowMessageBoxInfo("新增失败，登录密码格式不正确！");
            return;
        }
        if(userInfo.password.compare(userInfo.pwdConfirm) != 0)
        {
            myHelper::ShowMessageBoxInfo("新增失败，两次密码输入不一致！");
            return;
        }
        if(userInfo.userName.compare(userInfo.password) == 0)
        {
            myHelper::ShowMessageBoxInfo("新增失败，用户名和密码不允许相同！");
            return;
        }

        qint64 value = DataModel::getInstance()->getPwdActiveTimeLimitCur();
        QDateTime activeTime = DataModel::getInstance()->getCurrentSysDateTime(); //有效时间

        userInfo.userId = uuid; //用户唯一标识
        userInfo.isSystem = "no"; //系统内置
        userInfo.isEnable = "yes"; //是否启用
        userInfo.activeTime = activeTime; //有效时间

        //拼接数据库新增语句
        QString sqlStr = QString(SQL_INSERT_USER_INFO)
                .arg(userInfo.userId)
                .arg(userInfo.userName)
                .arg(userInfo.realName)
                .arg(userInfo.password)
                .arg(userInfo.isSystem)
                .arg(userInfo.isEnable)
                .arg(userInfo.remark)
                .arg(userInfo.activeTime.toString("yyyy-MM-dd hh:mm:ss"))
                .arg(userInfo.roleId)
                .arg(DataModel::getInstance()->getCurrentSysDateTime().toString("yyyy-MM-dd hh:mm:ss"));

        if(SingletonDBHelper->execSqlStr(sqlStr))
            qDebug()<<QString("新增用户:%1").arg(userInfo.userName);

        showEvent(NULL);
    }
}

//修改按钮
void UserConfig::on_pBtn_modify_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(row < 0 || row >= ui->tableWidget->rowCount())
        return;

    User_Info oldUserInfo;
    oldUserInfo.userId = ui->tableWidget->item(row, 1)->text(); //用户唯一标识
    oldUserInfo.userName = ui->tableWidget->item(row, 2)->text(); //用户名称
    oldUserInfo.roleName = ui->tableWidget->item(row, 3)->text(); //角色名称
    oldUserInfo.realName = ui->tableWidget->item(row, 4)->text(); //用户姓名
    oldUserInfo.isEnable = ui->tableWidget->item(row, 6)->text(); //是否启用,已启用其他人不能修改密码
    oldUserInfo.remark = ui->tableWidget->item(row, 7)->text(); //备注
    oldUserInfo.activeTime = QDateTime::fromString(ui->tableWidget->item(row, 8)->text(), "yyyy-MM-dd hh:mm:ss");  //有效时间
    oldUserInfo.password = ui->tableWidget->item(row, 9)->text(); //用户密码

    SettingDialog::getInstance()->modifyUserInfo(oldUserInfo); //设置界面
    if(SettingDialog::getInstance()->exec() == QDialog::Accepted)
    {
        User_Info newUserInfo = SettingDialog::getInstance()->getUserConfResult();
        if(!m_nameExp.exactMatch(newUserInfo.userName) || newUserInfo.userName.isEmpty())
        {
            myHelper::ShowMessageBoxInfo("修改失败，用户名称格式不正确！");
            return;
        }
        if(isExistThisUserName(newUserInfo.userName) && oldUserInfo.userName.compare(newUserInfo.userName) != 0)
        {
            myHelper::ShowMessageBoxInfo("修改失败，该用户名称已存在！");
            return;
        }
        if(newUserInfo.realName.isEmpty())
        {
            myHelper::ShowMessageBoxInfo("修改失败，请填写用户姓名！");
            return;
        }
        if(newUserInfo.userName.compare(newUserInfo.password) == 0)
        {
            myHelper::ShowMessageBoxInfo("修改失败，用户名和密码不允许相同！");
            return;
        }

        //拼接数据库修改语句
        //修改用户
        QString sqlStr = QString(SQL_UPDATE_USER_INFO)
                .arg(newUserInfo.userName)
                .arg(newUserInfo.realName)
                .arg(newUserInfo.remark)
                .arg(oldUserInfo.userId);

        if(SingletonDBHelper->execSqlStr(sqlStr))
            qDebug()<<QString("修改用户:%1").arg(ui->tableWidget->item(row, 2)->text());

        showEvent(NULL);
    }
}

//删除按钮
void UserConfig::on_pBtn_delete_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(row < 0 || row >= ui->tableWidget->rowCount())
        return;

    if(ui->tableWidget->item(row, 5)->text().compare("是") == 0)
    {
        myHelper::ShowMessageBoxInfo("删除失败，系统内置用户不允许删除！");
        return;
    }

    int ret = myHelper::ShowMessageBoxInfo("确定删除这个用户吗？");
    if(ret == QMessageBox::Ok)
    {
        QString id = ui->tableWidget->item(row, 1)->text(); //唯一标识

        //拼接数据库删除语句
        QString sqlStr = QString(SQL_DELETE_USER_INFO).arg(id); //删除用户
        if(SingletonDBHelper->execSqlStr(sqlStr))
            qDebug()<<QString("删除用户:%1").arg(ui->tableWidget->item(row, 2)->text());

        showEvent(NULL);
    }
}




