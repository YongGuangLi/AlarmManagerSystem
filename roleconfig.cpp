#include "roleconfig.h"
#include "ui_roleconfig.h"

RoleConfig::RoleConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoleConfig)
{
    ui->setupUi(this);

    ui->pBtn_add->setIcon(QIcon(":Resources/Image/add.png"));
    ui->pBtn_modify->setIcon(QIcon(":Resources/Image/modify.png"));
    ui->pBtn_delete->setIcon(QIcon(":Resources/Image/delete.png"));

    initTable(ui->tableWidget); //初始化表格
}

RoleConfig::~RoleConfig()
{
    delete ui;
}

//初始化表格
void RoleConfig::initTable(QTableWidget *tableWidget)
{
    QStringList header;
    header.clear();
    header<<QString("序号")<<QString("唯一标识")<<QString("角色名称")<<QString("角色类型")<<QString("已分配用户")<<QString("系统内置")<<QString("备注");

    tableWidget->setColumnCount(7);
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

    tableWidget->setColumnHidden(1, true); //将唯一标识隐藏
}

//表格添加一行
void RoleConfig::addRowOnTable(QTableWidget *tableWidget, Role_Info info)
{
    int curRow = tableWidget->rowCount();
    tableWidget->insertRow(curRow); //插入一行
    tableWidget->setRowHeight(curRow, 25);

    tableWidget->setItem(curRow, 0, new QTableWidgetItem(QString::number(curRow + 1)));
    tableWidget->setItem(curRow, 1, new QTableWidgetItem(info.id));
    tableWidget->setItem(curRow, 2, new QTableWidgetItem(info.roleName));

    //角色类型
    if(info.roleType.compare("1", Qt::CaseInsensitive) == 0)
        tableWidget->setItem(curRow, 3, new QTableWidgetItem("系统审计员"));
    else if(info.roleType.compare("2", Qt::CaseInsensitive) == 0)
        tableWidget->setItem(curRow, 3, new QTableWidgetItem("系统操作员"));
    else
        tableWidget->setItem(curRow, 3, new QTableWidgetItem("系统管理员"));


    tableWidget->setItem(curRow, 4, new QTableWidgetItem(info.userNames));

    //系统内置
    if(info.isSystem.compare("yes", Qt::CaseInsensitive) == 0)
        tableWidget->setItem(curRow, 5, new QTableWidgetItem("是"));
    else
        tableWidget->setItem(curRow, 5, new QTableWidgetItem("否"));

    tableWidget->setItem(curRow, 6, new QTableWidgetItem(info.remark));


    tableWidget->item(curRow, 0)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 1)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 2)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 3)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 4)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 5)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 6)->setTextAlignment(Qt::AlignCenter);
}

//设置功能ID
void RoleConfig::setFuncId(QString id)
{
    m_myFuncId = id;
}

//验证角色是否已存在
bool RoleConfig::isExistThisRoleName(QString name)
{
    bool isExist = false;
    QMapIterator<QString, Role_Info> it(m_data_RoleConfig.allRoleInfoMap);
    while(it.hasNext())
    {
        it.next();
        Role_Info roleInfo = it.value();
        //用户名称
        if(roleInfo.roleName.compare(name, Qt::CaseSensitive) == 0)
        {
            isExist = true;
            break;
        }
    }

    return isExist;
}

void RoleConfig::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    Data_RoleConfig data_RoleConfig;   //角色配置页面数据
    SingletonDBHelper->queryRoleConfigInfo(data_RoleConfig);
    recvMsg(data_RoleConfig);
}

//观察者从订阅主题接收的消息
void RoleConfig::recvMsg(Data_RoleConfig data_RoleConfig)
{
    //数据库查询结果
    ui->tableWidget->setRowCount(0);
    m_data_RoleConfig = data_RoleConfig; //角色配置页面数据

    QMapIterator<QString, Role_Info> it(m_data_RoleConfig.allRoleInfoMap);
    while(it.hasNext())
    {
        it.next();
        addRowOnTable(ui->tableWidget, it.value()); //表格添加一行
    }
}

//新增按钮
void RoleConfig::on_pBtn_add_clicked()
{
    SettingDialog::getInstance()->addRoleInfo(m_data_RoleConfig.roleMenuReadWriteMap); //设置界面
    if(SettingDialog::getInstance()->exec() == QDialog::Accepted)
    {
        Role_Info roleInfo = SettingDialog::getInstance()->getRoleInfoResult(); //获取配置结果
        QMap<QString, RoleMenu_Info> menuFuncResult = SettingDialog::getInstance()->getRoleMenuFuncResult(); //功能读写
        QString uuid = QUuid::createUuid().toString();
        uuid = uuid.mid(1, uuid.length() - 2); //去掉大括号
        roleInfo.id = uuid;
        if(roleInfo.roleName.isEmpty())
        {
            myHelper::ShowMessageBoxInfo("新增失败，角色名称不合法！");
            return;
        }
        if(isExistThisRoleName(roleInfo.roleName))
        {
            myHelper::ShowMessageBoxInfo("新增失败，该角色名称已存在！");
            return;
        }

        //拼接数据库新增语句
        //新增角色
        QString sqlStr = QString(SQL_INSERT_ROLE_INFO)
                .arg(uuid)
                .arg(roleInfo.roleName)
                .arg("no")
                .arg(roleInfo.remark)
                .arg(roleInfo.roleType);

        if(SingletonDBHelper->execSqlStr(sqlStr))
            qDebug()<<QString("新增角色:%1").arg(roleInfo.roleName);

        //新增角色功能
        QMapIterator<QString, RoleMenu_Info> it(menuFuncResult);
        while(it.hasNext())
        {
           it.next();
           RoleMenu_Info tmpInfo = it.value();
           QString funcStr = QString(SQL_INSERT_FUNC_INFO)
                   .arg(uuid)
                   .arg(tmpInfo.funcId)
                   .arg(tmpInfo.isRead)
                   .arg(tmpInfo.isWrite);

           SingletonDBHelper->execSqlStr(funcStr);
        }

        showEvent(NULL);
    }
}

//修改按钮
void RoleConfig::on_pBtn_modify_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(row < 0 || row >= ui->tableWidget->rowCount())
        return;

    Role_Info oldRoleInfo; //旧的角色
    oldRoleInfo.id = ui->tableWidget->item(row, 1)->text(); //角色ID
    oldRoleInfo.roleName = ui->tableWidget->item(row, 3)->text(); //角色名称
    oldRoleInfo.roleType = ui->tableWidget->item(row, 4)->text(); //角色类型
    oldRoleInfo.remark = ui->tableWidget->item(row, 6)->text(); //备注
    QMap<QString, RoleMenu_Info> funcMenuMap = m_data_RoleConfig.roleMenuReadWriteMap[oldRoleInfo.id]; //角色的功能菜单读写权限

    SettingDialog::getInstance()->modifyRoleInfo(oldRoleInfo, funcMenuMap); //设置界面
    if(SettingDialog::getInstance()->exec() == QDialog::Accepted)
    {
        Role_Info newRoleInfo = SettingDialog::getInstance()->getRoleInfoResult(); //获取配置结果
        QMap<QString, RoleMenu_Info> menuFuncResult = SettingDialog::getInstance()->getRoleMenuFuncResult(); //功能读写
        if(newRoleInfo.roleName.isEmpty())
        {
            myHelper::ShowMessageBoxInfo("修改失败，角色名称不合法！");
            return;
        }
        if(isExistThisRoleName(newRoleInfo.roleName) && oldRoleInfo.roleName.compare(newRoleInfo.roleName) != 0)
        {
            myHelper::ShowMessageBoxInfo("修改失败，该角色名称已存在！"); //名称发生了变化
            return;
        }

        //拼接数据库修改语句
        //修改角色
        QString sqlStr = QString(SQL_UPDATE_ROLE_INFO)
                .arg(newRoleInfo.roleName)
                .arg(newRoleInfo.remark)
                .arg(newRoleInfo.roleType)
                .arg(oldRoleInfo.id);

        if(SingletonDBHelper->execSqlStr(sqlStr))
            qDebug()<<QString("修改角色:%1").arg(oldRoleInfo.roleName);

        //修改角色功能
         QMapIterator<QString, RoleMenu_Info> it(menuFuncResult);
         while(it.hasNext())
         {
             it.next();
             RoleMenu_Info tmpInfo = it.value();
             QString funcStr = QString(SQL_UPDATE_FUNC_INFO)
                     .arg(tmpInfo.isRead)
                     .arg(tmpInfo.isWrite)
                     .arg(oldRoleInfo.id)
                     .arg(tmpInfo.funcId);

             SingletonDBHelper->execSqlStr(funcStr);
         }

         showEvent(NULL);
    }
}

//删除按钮
void RoleConfig::on_pBtn_delete_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(row < 0 || row >= ui->tableWidget->rowCount())
        return;

    if(ui->tableWidget->item(row, 5)->text().compare("是") == 0)
    {
        myHelper::ShowMessageBoxInfo("删除失败，系统内置角色不允许删除！");
        return;
    }

    int ret = myHelper::ShowMessageBoxInfo("确定删除这个角色吗？角色下分配的用户也将删除!");
    if(ret == QMessageBox::Ok)
    {
        QString id = ui->tableWidget->item(row, 1)->text(); //唯一标识
        QString name = ui->tableWidget->item(row, 2)->text(); //角色名称

        QString sqlStr = QString(SQL_DELETE_ROLE_INFO).arg(id); //删除角色
        SingletonDBHelper->execSqlStr(sqlStr);
        sqlStr = QString(SQL_DELETE_ROLE_USERS_INFO).arg(id); //删除角色下所有用户
        SingletonDBHelper->execSqlStr(sqlStr);
        sqlStr = QString(SQL_DELETE_FUNC_INFO).arg(id); //删除角色的功能权限配置
        if(SingletonDBHelper->execSqlStr(sqlStr))
            qDebug()<<QString("删除角色:%1").arg(name);

        showEvent(NULL);
    }
}



