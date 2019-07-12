#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog* SettingDialog::m_instance = Q_NULLPTR;
SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint); //只显示关闭按钮

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(QString("确定"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(QString("取消"));
    connect(ui->btnalarmColor, SIGNAL(clicked(bool)), this, SLOT(slot_modifyAlarmColor()));

    //告警等级配置
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->checkBox_Checked);
    buttonGroup->addButton(ui->checkBox_UnChecked);


    //ip地址框加限制
    QRegExp addressExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    QValidator *validator = new QRegExpValidator(addressExp);

    //路由参数配置界面
    ui->lineEdit_destination_2->setValidator(validator);
    ui->lineEdit_destination_2->setPlaceholderText("000.000.000.000");
    ui->lineEdit_netmask_2->setValidator(validator);
    ui->lineEdit_netmask_2->setPlaceholderText("000.000.000.000");
    ui->lineEdit_nexthop_2->setValidator(validator);
    ui->lineEdit_nexthop_2->setPlaceholderText("000.000.000.000");

    //子接口参数设置
    ui->lineEdit_ip_3->setValidator(validator);
    ui->lineEdit_ip_3->setPlaceholderText("000.000.000.000");
    ui->lineEdit_netmask_3->setValidator(validator);
    ui->lineEdit_netmask_3->setPlaceholderText("000.000.000.000");

    //角色配置
    //ui->treeWidget_4->setHeaderHidden(true);
    QStringList headList;
    headList<<"功能"<<"可读状态"<<"可写状态";
    ui->treeWidget_4->setColumnCount(3);
    ui->treeWidget_4->setHeaderLabels(headList);
    ui->treeWidget_4->setColumnWidth(0, 200);
    ui->comboBox_roleType_4->setView(new QListView());
    ui->lineEdit_roleName_4->setPlaceholderText("请输入角色名称");

    m_dispAllTreeItemFlag = true; //显示所有的树节点，即不可读的也显示

    //用户配置
    ui->comboBox_roleName_5->setView(new QListView());

    ui->lineEdit_userName_5->setPlaceholderText("由字母、数字、下划线组成");
    ui->lineEdit_realName_5->setPlaceholderText("请输入真实姓名");
    ui->lineEdit_pwd_5->setPlaceholderText("密码");
    ui->lineEdit_pwdTwo_5->setPlaceholderText("请再输入一次密码");

    QRegExp nameExp("^[a-zA-Z][a-zA-Z0-9_]{4,15}$"); //名称
    QValidator *nameValidator = new QRegExpValidator(nameExp);
    ui->lineEdit_userName_5->setValidator(nameValidator);

    QRegExp pwdExp("^[a-zA-Z0-9_~!@#$%^&*?]+$"); //密码
    QValidator *pwdValidator = new QRegExpValidator(pwdExp);
    ui->lineEdit_pwd_5->setValidator(pwdValidator);
    ui->lineEdit_pwdTwo_5->setValidator(pwdValidator);

    ui->lineEdit_pwd_5->setEchoMode(QLineEdit::Password);
    ui->lineEdit_pwdTwo_5->setEchoMode(QLineEdit::Password);

    //密码修改
    ui->lineEdit_userName_6->setValidator(nameValidator);
    ui->lineEdit_oldPwd_6->setValidator(pwdValidator);
    ui->lineEdit_newPwd_6->setValidator(pwdValidator);
    ui->lineEdit_pwdTwo_6->setValidator(pwdValidator);
    ui->lineEdit_userName_6->setPlaceholderText("由字母、数字、下划线组成，只能以字母开头(5-16位)");
    ui->lineEdit_oldPwd_6->setPlaceholderText("密码必须包含字母、数字、特殊字符且不小于8位");
    ui->lineEdit_newPwd_6->setPlaceholderText("密码必须包含字母、数字、特殊字符且不小于8位");
    ui->lineEdit_pwdTwo_6->setPlaceholderText("请再输入一次密码");

    ui->lineEdit_oldPwd_6->setEchoMode(QLineEdit::Password);
    ui->lineEdit_newPwd_6->setEchoMode(QLineEdit::Password);
    ui->lineEdit_pwdTwo_6->setEchoMode(QLineEdit::Password);

    //添加登录地址
    ui->lineEdit_ip_7->setValidator(validator);
    ui->lineEdit_ip_7->setPlaceholderText("000.000.000.000");

    //密码验证
    ui->lineEdit_userName_8->setValidator(nameValidator);
    ui->lineEdit_loginPwd_8->setValidator(pwdValidator);
    ui->lineEdit_loginPwd_8->setEchoMode(QLineEdit::Password);

    //主站参数配置
    ui->comboBox_mainFlag_9->setView(new QListView());
    QRegExp portExp("^([0-9]|[1-9]\\d|[1-9]\\d{2}|[1-9]\\d{3}|[1-5]\\d{4}|6[0-4]\\d{3}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])$"); //端口限制
    QValidator *validator_port = new QRegExpValidator(portExp, ui->lineEdit_dataChannelPort_9);
    ui->lineEdit_dataChannelPort_9->setValidator(validator_port);
    QRegExp channelNameExp("^[^\u4e00-\u9fa5]{0,}$");
    QValidator *validator_channelName = new QRegExpValidator(channelNameExp, ui->lineEdit_dataChannelName_9);
    ui->lineEdit_dataChannelName_9->setValidator(validator_channelName);
    ui->lineEdit_ctrlChannelIp_9->setValidator(validator_channelName);
    ui->lineEdit_dataChannelIp_9->setValidator(validator_channelName);

    ui->lineEdit_ctrlChannelIp_9->setPlaceholderText("多个ip请使用英文逗号隔开");
    ui->lineEdit_dataChannelIp_9->setPlaceholderText("多个ip请使用英文逗号隔开");
    ui->lineEdit_dataChannelName_9->setPlaceholderText("多个通道名称请使用英文逗号隔开");
    ui->lineEdit_dataChannelPort_9->setPlaceholderText("0-65535");

    //snmp配置
    ui->comboBox_version_11->setView(new QListView);
    ui->lineEdit_port_11->setValidator(validator_port);
    ui->lineEdit_port_11->setPlaceholderText("0-65535");
    ui->lineEdit_group_11->setValidator(validator_channelName);

    //初始化资产选择表格
    initTable(ui->tableWidget_assetInfo_10);

    //端口详情显示
    //初始化端口详情展示表格
    initPortTable(ui->tableWidget_portInfo_13);

    //密码重置
    ui->lineEdit_userName_14->setValidator(nameValidator);
    ui->lineEdit_userName_14->setPlaceholderText("由字母、数字、下划线组成，只能以字母开头(5-16位)");
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

//获取实例
SettingDialog *SettingDialog::getInstance(QWidget *parent)
{
    if(SettingDialog::m_instance == nullptr)
    {
        SettingDialog::m_instance = new SettingDialog(parent);
    }
    return SettingDialog::m_instance;
}


void SettingDialog::initMyTitle()
{
    m_titleBar = new TitleBar(this);
    m_titleBar->move(0, 0);
    m_titleBar->raise();
    m_titleBar->setBackgroundColor(0, 0, 0, true);
    m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
    m_titleBar->setBackgroundColor(99,99,99);
    m_titleBar->setTitleIcon(":Resources/Image/main_logo.png");
    m_titleBar->setTitleWidth(this->width());

    connect(m_titleBar, SIGNAL(signal_btnClose_Clicked()), this, SLOT(hide()));

    this->installEventFilter(this);
}

//新增告警配置参数
void SettingDialog::addAlarmConfig()
{
    m_titleBar->setTitleContent(QString(" 新增告警配置"));
    ui->stackedWidget->setCurrentIndex(0);

    ui->lineEdit_alarmLevel->clear();
    ui->lineEdit_alarmName->clear();
    ui->checkBox_Checked->setCheckState(Qt::Checked);
    ui->checkBox_UnChecked->setCheckState(Qt::Unchecked);
}

//修改告警配置参数
void SettingDialog::modifyAlarmConfig(stAlarmConfig alarmConfig)
{
    m_titleBar->setTitleContent(QString(" 修改告警配置"));
    ui->stackedWidget->setCurrentIndex(0);

    ui->lineEdit_alarmLevel->setText(alarmConfig.alarmLevel);
    ui->lineEdit_alarmName->setText(alarmConfig.alarmName);

    ui->btnalarmColor->setStyleSheet(QString("background-color:%1;").arg(alarmConfig.alarmColor));
    if(alarmConfig.alarmWindow == 1)
    {
        ui->checkBox_Checked->setCheckState(Qt::Checked);
        ui->checkBox_UnChecked->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->checkBox_Checked->setCheckState(Qt::Unchecked);
        ui->checkBox_UnChecked->setCheckState(Qt::Checked);
    }
}


//获取告警配置设置结果
stAlarmConfig SettingDialog::getAlarmConfigSetResult()
{
    stAlarmConfig alarmConfig;

    alarmConfig.alarmLevel = ui->lineEdit_alarmLevel->text();
    alarmConfig.alarmName = ui->lineEdit_alarmName->text();
    QString styleSheet = ui->btnalarmColor->styleSheet();                                   //"background-color:#55ff7f;"
    alarmConfig.alarmColor = styleSheet.mid(17, 7);

    if(ui->checkBox_Checked->checkState() == Qt::Checked)
        alarmConfig.alarmWindow = 1;
    else
        alarmConfig.alarmWindow = 0;

    return alarmConfig;
}

//路由参数设置
void SettingDialog::routeParamSet(RouteConf_Info info)
{
    m_titleBar->setTitleContent(QString(" 路由配置"));
    ui->stackedWidget->setCurrentIndex(1);

    ui->lineEdit_destination_2->clear();
    ui->lineEdit_netmask_2->clear();
    ui->lineEdit_nexthop_2->clear();

    ui->lineEdit_destination_2->setText(info.segment);
    ui->lineEdit_netmask_2->setText(info.netmask);
    ui->lineEdit_nexthop_2->setText(info.nextHop);

}

//获取路由参数设置结果
RouteConf_Info SettingDialog::getRouteParamSetResult()
{
    RouteConf_Info info;

    info.segment = ui->lineEdit_destination_2->text(); //目标网段
    info.netmask = ui->lineEdit_netmask_2->text(); //子网掩码
    info.nextHop = ui->lineEdit_nexthop_2->text(); //下一跳

    return info;
}

//子接口参数设置
void SettingDialog::subnetParamSet(Subnet_Info info, bool flag)
{
    m_titleBar->setTitleContent(QString(" 子接口配置"));
    ui->stackedWidget->setCurrentIndex(2);

    ui->lineEdit_netName_3->setEnabled(false);
    ui->lineEdit_netName_3->clear();
    ui->lineEdit_subnetName_3->clear();
    ui->lineEdit_subnetName_3->setEnabled(false);
    ui->lineEdit_vlanid_3->clear();
    ui->lineEdit_vlanid_3->setEnabled(flag);
    ui->lineEdit_ip_3->clear();
    ui->lineEdit_netmask_3->clear();

    ui->lineEdit_netName_3->setText(info.netName);
    ui->lineEdit_subnetName_3->setText(info.subnetName);
    ui->lineEdit_vlanid_3->setText(info.vlanId);
    ui->lineEdit_ip_3->setText(info.ipAddr);
    ui->lineEdit_netmask_3->setText(info.netMask);

}

//获取子接口参数设置结果
Subnet_Info SettingDialog::getSubnetParamResult()
{
    Subnet_Info info;

    info.netName = ui->lineEdit_netName_3->text();
    info.subnetName = ui->lineEdit_subnetName_3->text();
    info.vlanId = ui->lineEdit_vlanid_3->text();
    info.ipAddr = ui->lineEdit_ip_3->text();
    info.netMask = ui->lineEdit_netmask_3->text();

    return info;
}

//新增角色信息
void SettingDialog::addRoleInfo(QMap<QString, QMap<QString, RoleMenu_Info> > roleMenuFuncInfoMap)
{
    m_titleBar->setTitleContent(QString(" 新增角色"));
    ui->stackedWidget->setCurrentIndex(3);

    ui->tabWidget_roleSet_4->setCurrentIndex(0);
    ui->comboBox_roleType_4->setCurrentIndex(0);
    ui->lineEdit_roleName_4->clear();
    ui->lineEdit_remark_4->clear();

    ui->comboBox_roleType_4->setEnabled(true); //角色类型


    //功能菜单
    m_roleMenuFuncInfoMap = roleMenuFuncInfoMap; //功能读写权限
    if(m_roleMenuFuncInfoMap.contains("1"))
    {
        m_curFuncMenuMap = m_roleMenuFuncInfoMap["1"]; //系统审计员的功能读写权限
    }
    clearMenuFuncTree();
    showMenuFuncTree("Root", nullptr); //显示功能树
    //更新树节点的选择状态
    updateTreeItemCheckState();
}

//修改角色信息
void SettingDialog::modifyRoleInfo(Role_Info info, QMap<QString, RoleMenu_Info> funcInfoMap)
{
    m_titleBar->setTitleContent(QString(" 修改角色"));
    ui->stackedWidget->setCurrentIndex(3);

    ui->tabWidget_roleSet_4->setCurrentIndex(0);
    ui->comboBox_roleType_4->setCurrentIndex(0);
    ui->lineEdit_roleName_4->clear();
    ui->lineEdit_remark_4->clear();

    ui->comboBox_roleType_4->setEnabled(false); //角色类型

    //基本信息
    ui->lineEdit_roleName_4->setText(info.roleName);
    ui->lineEdit_remark_4->setText(info.remark);
    ui->comboBox_roleType_4->setCurrentText(info.roleType);

    m_curFuncMenuMap = funcInfoMap; //当前角色的功能读写权限

       qDebug()<<funcInfoMap.keys();

    clearMenuFuncTree();
    showMenuFuncTree("Root", nullptr); //显示功能树
    //更新树节点的选择状态
    updateTreeItemCheckState();
}

//获取角色配置结果
Role_Info SettingDialog::getRoleInfoResult()
{
    Role_Info info;

    info.roleName = ui->lineEdit_roleName_4->text(); //角色名称
    info.isSystem = "no"; //系统内置
    info.remark = ui->lineEdit_remark_4->text(); //备注
    info.roleType = QString::number(ui->comboBox_roleType_4->currentIndex() + 1); //角色类型
    info.userNames = "";

    return info;
}

//获取角色功能的读写权限
QMap<QString, RoleMenu_Info> SettingDialog::getRoleMenuFuncResult()
{

    QTreeWidgetItemIterator it(ui->treeWidget_4); //遍历树节点
    while(*it)
    {
        QTreeWidgetItem* myItem = *it;
        QString funcId = myItem->data(0, Qt::UserRole).toString(); //功能ID
        RoleMenu_Info roleInfo = m_curFuncMenuMap[funcId];
        //读权限
        roleInfo.isRead = 1;
        if(myItem->checkState(1) == Qt::Unchecked)
        {
            roleInfo.isRead = 0;
        }
        //写权限
        roleInfo.isWrite = 1;
        if(myItem->checkState(2) == Qt::Unchecked)
        {
            roleInfo.isWrite = 0;
        }

        m_curFuncMenuMap[roleInfo.funcId] = roleInfo; //更新下修改的内容

        it++;
    }

    return m_curFuncMenuMap;
}

//设置所有的功能菜单
void SettingDialog::setAllMenuFuncInfoMap(QMultiMap<QString, MenuFunc_Info> allMenuFuncInfoMap)
{
    m_allMenuFuncInfoMap = allMenuFuncInfoMap;
}

//菜单排序
void SettingDialog::menuFuncSort(QList<MenuFunc_Info> &infoList)
{
    QMap<int, MenuFunc_Info> tmpMap;
    for(int i = 0; i < infoList.size(); i++)
    {
        MenuFunc_Info info = infoList.at(i);
        tmpMap.insert(info.orderNo, info);
    }
    infoList = tmpMap.values();
}

//展示功能菜单树
void SettingDialog::showMenuFuncTree(QString parentId, QTreeWidgetItem *parentItem)
{
    if(m_allMenuFuncInfoMap.contains(parentId))
    {
        QList<MenuFunc_Info> infoList = m_allMenuFuncInfoMap.values(parentId); //该父功能ID的所有子节点
        menuFuncSort(infoList); //菜单排序
        for(int i = 0; i < infoList.count(); i++)
        {
            MenuFunc_Info info = infoList.at(i);
            //读写权限
            RoleMenu_Info roleMenuInfo;
            roleMenuInfo.isRead = false;
            roleMenuInfo.isWrite = false;
            if(m_curFuncMenuMap.contains(info.funcId))
            {
                roleMenuInfo = m_curFuncMenuMap[info.funcId];
            }
            else
            {
                continue;
            }

            if(!m_dispAllTreeItemFlag && !roleMenuInfo.isRead)
            {
                continue; //不可读的就不显示
            }

            //新建一个树节点
            QStringList list;
            list.append(info.funcTitle);
            QTreeWidgetItem *item = nullptr;
            if(parentId == "Root" || parentItem == nullptr)
            {
                item = new QTreeWidgetItem(ui->treeWidget_4, list);
                            //item->setIcon(0, QIcon(":Resources/Images/dir.png"));
                item->setData(0, Qt::UserRole, QVariant(info.funcId)); //将功能ID作为用户数据与树节点绑定
            }
            else
            {
                item = new QTreeWidgetItem(parentItem, list);
                item->setData(0, Qt::UserRole, QVariant(info.funcId));
            }

            //读权限
            if(roleMenuInfo.isRead)
            {
                item->setCheckState(1, Qt::Checked);
            }
            else
            {
                item->setCheckState(1, Qt::Unchecked);
            }
            //写权限
            if(roleMenuInfo.isWrite)
            {
                item->setCheckState(2, Qt::Checked);
            }
            else
            {
                item->setCheckState(2, Qt::Unchecked);
            }

            showMenuFuncTree(info.funcId, item); //递归
        }
    }
}

//清除功能菜单树
void SettingDialog::clearMenuFuncTree()
{
    while(ui->treeWidget_4->topLevelItemCount())
    {
        QTreeWidgetItem* item = ui->treeWidget_4->takeTopLevelItem(0);
        delete item;
        item = nullptr;
    }
    ui->treeWidget_4->clear();
}

//更新树节点的选择状态
void SettingDialog::updateTreeItemCheckState()
{
    QTreeWidgetItemIterator it(ui->treeWidget_4);
    while(*it)
    {
        updateParentItemCheckState(*it, 1); //更新父节点状态
        updateParentItemCheckState(*it, 2); //更新父节点状态
        it++;
    }
}

//更新父节点状态
void SettingDialog::updateParentItemCheckState(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem *parentItem = item->parent();
    if(parentItem == Q_NULLPTR)
    {
        return;
    }

    int checkedCount = 0; //已经选中的个数
    int childCount = parentItem->childCount();
    for(int i = 0; i < childCount; i++)
    {
        if(parentItem->child(i)->checkState(column) == Qt::Checked)
        {
            checkedCount++;
        }
    }

    if(checkedCount == 0 && childCount != 0)
    {
        parentItem->setCheckState(column, Qt::Unchecked);
    }
    else if(checkedCount == childCount && childCount != 0)
    {
        parentItem->setCheckState(column, Qt::Checked);
    }
    else if(checkedCount > 0 && checkedCount < childCount)
    {
        parentItem->setCheckState(column, Qt::PartiallyChecked); //部分选中
    }

}

//更新子节点状态
void SettingDialog::updateSubItemCheckState(QTreeWidgetItem *item, int column)
{
    if(item->checkState(column) == Qt::Checked)
    {
        //如果选择了可写，则自动选上可读
        if(column == 2)
        {
            item->setCheckState(1, Qt::Checked);
        }
    }
    else if(item->checkState(column) == Qt::Unchecked)
    {
        //如果取消可读，自动取消可写
        if(column == 1)
        {
            item->setCheckState(2, Qt::Unchecked);
        }
    }

    //遍历所有的子节点
    int count = item->childCount();
    for(int i = 0; i < count; i++)
    {
        QTreeWidgetItem *subItem = item->child(i);
        subItem->setCheckState(column, item->checkState(column));
        updateSubItemCheckState(subItem, column);
    }
}

//新增用户
void SettingDialog::addUserInfo(QMap<QString, Role_Info> allRoleInfoMap)
{
    m_titleBar->setTitleContent(QString(" 新增用户"));
    ui->stackedWidget->setCurrentIndex(4);

    m_comboxRoleInfoMap.clear();
    ui->comboBox_roleName_5->clear();
    ui->lineEdit_userName_5->clear();
    ui->lineEdit_realName_5->clear();
    ui->lineEdit_pwd_5->clear();
    ui->lineEdit_pwdTwo_5->clear();
    ui->lineEdit_remark_5->clear();

    //显示密码框
    ui->label_pwd_5->show();
    ui->label_pwdTwo_5->show();
    ui->lineEdit_pwd_5->show();
    ui->lineEdit_pwdTwo_5->show();

    int i = 0;
    QMapIterator<QString, Role_Info> it(allRoleInfoMap);
    while(it.hasNext())
    {
        it.next();
        Role_Info roleInfo = it.value();
        ui->comboBox_roleName_5->addItem(roleInfo.roleName); //角色名称
        m_comboxRoleInfoMap.insert(i++, roleInfo); //记录每一行对应的角色信息
    }

}

//修改用户
void SettingDialog::modifyUserInfo(User_Info info)
{
    m_titleBar->setTitleContent(QString(" 修改用户"));
    ui->stackedWidget->setCurrentIndex(4);

    m_comboxRoleInfoMap.clear();
    ui->comboBox_roleName_5->clear();
    ui->lineEdit_userName_5->clear();
    ui->lineEdit_realName_5->clear();
    ui->lineEdit_pwd_5->clear();
    ui->lineEdit_pwdTwo_5->clear();

    //隐藏密码框
    ui->label_pwd_5->hide();
    ui->label_pwdTwo_5->hide();
    ui->lineEdit_pwd_5->hide();
    ui->lineEdit_pwdTwo_5->hide();

    ui->comboBox_roleName_5->addItem(info.roleName);
    ui->comboBox_roleName_5->setCurrentIndex(0);
    ui->lineEdit_userName_5->setText(info.userName);
    ui->lineEdit_realName_5->setText(info.realName);
    ui->lineEdit_pwd_5->setText(info.password);
    ui->lineEdit_pwdTwo_5->setText(info.pwdConfirm);
    ui->lineEdit_remark_5->setText(info.remark);

}

//获取用户配置结果
User_Info SettingDialog::getUserConfResult()
{
    User_Info userInfo;
    int index = ui->comboBox_roleName_5->currentIndex();
    if(m_comboxRoleInfoMap.contains(index))
    {
        Role_Info roleInfo = m_comboxRoleInfoMap[index];
        userInfo.roleId = roleInfo.id; //角色唯一标识
        userInfo.roleName = roleInfo.roleName; //角色名称
    }

    userInfo.userName = ui->lineEdit_userName_5->text(); //用户名称
    userInfo.realName = ui->lineEdit_realName_5->text(); //用户姓名
    userInfo.password = ui->lineEdit_pwd_5->text(); //密码
    userInfo.pwdConfirm = ui->lineEdit_pwdTwo_5->text(); //密码确认
    userInfo.remark = ui->lineEdit_remark_5->text(); //备注

    return userInfo;
}

//用户密码修改
void SettingDialog::modifyUserPwd(QString name, bool flag)
{
    m_titleBar->setTitleContent(QString(" 修改密码"));
    ui->stackedWidget->setCurrentIndex(5);

    ui->lineEdit_userName_6->setText(name);
    ui->lineEdit_oldPwd_6->clear();
    ui->lineEdit_newPwd_6->clear();
    ui->lineEdit_pwdTwo_6->clear();

    ui->lineEdit_userName_6->setEnabled(flag);

}

//获取修改密码结果
QStringList SettingDialog::getModifyUserPwdResult()
{
    QStringList paramList;

    paramList.append(ui->lineEdit_userName_6->text());
    paramList.append(ui->lineEdit_oldPwd_6->text());
    paramList.append(ui->lineEdit_newPwd_6->text());
    paramList.append(ui->lineEdit_pwdTwo_6->text());

    return paramList;
}

//设置登录地址
void SettingDialog::setLoginAddr()
{
    m_titleBar->setTitleContent(QString(" 添加地址"));
    ui->stackedWidget->setCurrentIndex(6);

    ui->lineEdit_ip_7->clear();
}

//获取登录地址
QString SettingDialog::getLoginAddr()
{
    return ui->lineEdit_ip_7->text();
}

//身份信息校验
void SettingDialog::setPasswordConfirm(QString userName)
{
    m_titleBar->setTitleContent(QString(" 身份校验"));
    ui->stackedWidget->setCurrentIndex(7);

    ui->lineEdit_userName_8->setText(userName);
    ui->lineEdit_loginPwd_8->clear();
}

//获取身份校验结果
User_Info SettingDialog::getPasswordConfirmResult()
{
    User_Info info;

    info.userName = ui->lineEdit_userName_8->text();
    info.password = ui->lineEdit_loginPwd_8->text();

    return info;
}

//新增主站参数配置
void SettingDialog::addMainStationInfo()
{
    m_titleBar->setTitleContent(QString(" 新增主站参数"));
    ui->stackedWidget->setCurrentIndex(8);

    ui->lineEdit_stationName_9->clear();
    ui->lineEdit_ctrlChannelIp_9->clear();
    ui->lineEdit_dataChannelIp_9->clear();
    ui->lineEdit_dataChannelName_9->clear();
    ui->lineEdit_dataChannelPort_9->clear();
    ui->comboBox_mainFlag_9->setCurrentIndex(0);

}

//修改主站参数配置
void SettingDialog::modifyMainStationInfo(MainStation_Info info)
{
    m_titleBar->setTitleContent(QString(" 修改主站参数"));
    ui->stackedWidget->setCurrentIndex(8);

    ui->lineEdit_stationName_9->setText(info.stationName);
    ui->lineEdit_ctrlChannelIp_9->setText(info.ctrlChannelIp);
    ui->lineEdit_dataChannelIp_9->setText(info.dataChannelIp);
    ui->lineEdit_dataChannelName_9->setText(info.dataChannelName);
    ui->lineEdit_dataChannelPort_9->setText(info.dataChannelPort);
    ui->comboBox_mainFlag_9->setCurrentIndex(info.mainFlag);

}

//获取主站参数配置结果
MainStation_Info SettingDialog::getMainStationInfoResult()
{
    MainStation_Info info;

    info.stationName = ui->lineEdit_stationName_9->text();
    info.ctrlChannelIp = ui->lineEdit_ctrlChannelIp_9->text();
    info.dataChannelIp = ui->lineEdit_dataChannelIp_9->text();
    info.dataChannelName = ui->lineEdit_dataChannelName_9->text();
    info.dataChannelPort = ui->lineEdit_dataChannelPort_9->text();
    info.mainFlag = ui->comboBox_mainFlag_9->currentIndex();

    return info;
}

//初始化表格
void SettingDialog::initTable(QTableWidget *tableWidget)
{
    QStringList header;
    header.clear();
    header<<QString("序号")<<QString("资产唯一标识")<<QString("资产名称")<<QString("资产类型")<<QString("资产IP");

    tableWidget->setColumnCount(5);
    tableWidget->verticalHeader()->setHidden(true); //隐藏垂直表头
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tableWidget->setAlternatingRowColors(true);
    //tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget->setCurrentCell(-1, -1);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::MultiSelection); //多选
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    //562
    tableWidget->setColumnWidth(0, 100); //第1列宽度
    tableWidget->setColumnWidth(1, 200); //第2列宽度
    tableWidget->setColumnWidth(2, 150); //第3列宽度
    tableWidget->setColumnWidth(3, 150); //第4列宽度
    tableWidget->setColumnWidth(4, 150); //第5列宽度

    tableWidget->horizontalHeader()->setFixedHeight(25);
    tableWidget->horizontalHeader()->setHighlightSections(false); //表头塌陷

    //设置唯一标识列隐藏
    tableWidget->setColumnHidden(1, true);

}

//表格添加一行
void SettingDialog::addRowOnTable(QTableWidget *tableWidget, Asset_Info assetInfo)
{
    int curRow = tableWidget->rowCount();
    tableWidget->insertRow(curRow); //插入一行
    tableWidget->setRowHeight(curRow, 25);

    tableWidget->setItem(curRow, 0, new QTableWidgetItem(QString::number(curRow + 1)));
    tableWidget->setItem(curRow, 1, new QTableWidgetItem(assetInfo.assetId));
    tableWidget->setItem(curRow, 2, new QTableWidgetItem(assetInfo.deviceName));
    tableWidget->setItem(curRow, 3, new QTableWidgetItem(assetInfo.typeDesc));
    tableWidget->setItem(curRow, 4, new QTableWidgetItem(assetInfo.assetIp));

    tableWidget->item(curRow, 0)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 1)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 2)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 3)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 4)->setTextAlignment(Qt::AlignCenter);

}

//资产选择
void SettingDialog::assetSelect(QMap<QString, Asset_Info> assetInfoMap)
{
    m_titleBar->setTitleContent(QString(" 资产选择"));
    ui->stackedWidget->setCurrentIndex(9);
    ui->tableWidget_assetInfo_10->setRowCount(0);
    QMapIterator<QString, Asset_Info> it(assetInfoMap);
    while(it.hasNext())
    {
        it.next();
        addRowOnTable(ui->tableWidget_assetInfo_10, it.value()); //表格添加一行
    }

}

//获取资产选择结果
QStringList SettingDialog::getAssetSelectResult()
{
    QStringList assetIdList;
    QList<QTableWidgetSelectionRange> list = ui->tableWidget_assetInfo_10->selectedRanges();

    for(int i = 0; i < list.count(); i++)
    {
        QTableWidgetSelectionRange range = list.at(i);
        int topRow = range.topRow();
        int bottomRow = range.bottomRow();
        for(int j = topRow; j <= bottomRow; j++)
        {
            QString assetId = ui->tableWidget_assetInfo_10->item(j, 1)->text();
            assetIdList.append(assetId);
        }
    }

    return assetIdList;
}

//添加snmp配置
void SettingDialog::addSnmpConfig()
{
    m_titleBar->setTitleContent(QString(" 新增SNMP配置"));
    ui->stackedWidget->setCurrentIndex(10);

    ui->lineEdit_port_11->clear();
    ui->lineEdit_group_11->clear();
    ui->comboBox_version_11->setCurrentIndex(1); //默认v2版本
}

//修改snmp配置
void SettingDialog::modifySnmpConfig(Snmp_Info info)
{
    m_titleBar->setTitleContent(QString(" 修改SNMP配置"));
    ui->stackedWidget->setCurrentIndex(10);

    ui->lineEdit_port_11->setText(info.snmpPort);
    ui->lineEdit_group_11->setText(info.snmpGroup);
    int versionIndex = 0;
    if(info.snmpVersion == "v2")
    {
        versionIndex = 1;
    }
    else if(info.snmpVersion == "v3")
    {
        versionIndex = 2;
    }

    ui->comboBox_version_11->setCurrentIndex(versionIndex);
}

//获取snmp配置结果
Snmp_Info SettingDialog::getSnmpConfigResult()
{
    Snmp_Info info;

    info.snmpPort = ui->lineEdit_port_11->text();
    info.snmpGroup = ui->lineEdit_group_11->text();
    info.snmpVersion = QString::number(ui->comboBox_version_11->currentIndex());

    return info;
}

//添加事件解析字段
void SettingDialog::addResolveFieldConfig()
{
    m_titleBar->setTitleContent(QString(" 新增字段"));
    ui->stackedWidget->setCurrentIndex(11);

    ui->lineEdit_name_12->clear();
    ui->lineEdit_alias_12->clear();
    ui->lineEdit_type_12->clear();
}

//修改事件解析字段
void SettingDialog::modifyResolveFieldConfig(Resolve_FieldInfo info)
{
    m_titleBar->setTitleContent(QString(" 修改字段"));
    ui->stackedWidget->setCurrentIndex(11);

    ui->lineEdit_name_12->setText(info.fieldName);
    ui->lineEdit_alias_12->setText(info.fieldAlias);
    ui->lineEdit_type_12->setText(info.fieldType);
}

//获取事件解析字段配置结果
Resolve_FieldInfo SettingDialog::getResolveFieldConfigResult()
{
    Resolve_FieldInfo info;

    info.fieldName = ui->lineEdit_name_12->text();
    info.fieldAlias = ui->lineEdit_alias_12->text();
    info.fieldType = ui->lineEdit_type_12->text();

    return info;
}

//初始化端口详情展示表格
void SettingDialog::initPortTable(QTableWidget *tableWidget)
{
    QStringList header;
    header.clear();
    header<<QString("序号")<<QString("开放端口")<<QString("协议名称")<<QString("服务名称");

    tableWidget->setColumnCount(4);
    tableWidget->verticalHeader()->setHidden(true); //隐藏垂直表头
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tableWidget->setAlternatingRowColors(true);
    //tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget->setCurrentCell(-1, -1);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::MultiSelection); //多选
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    //562
    tableWidget->setColumnWidth(0, 100); //第1列宽度
    tableWidget->setColumnWidth(1, 120); //第2列宽度
    tableWidget->setColumnWidth(2, 150); //第3列宽度
    tableWidget->setColumnWidth(3, 150); //第4列宽度

    tableWidget->horizontalHeader()->setFixedHeight(25);
    tableWidget->horizontalHeader()->setHighlightSections(false); //表头塌陷
}

//端口详情表添加一行
void SettingDialog::addRowOnPortTable(QTableWidget *tableWidget, QString portStr, QString protocolStr, QString serverStr)
{
    int curRow = tableWidget->rowCount();
    tableWidget->insertRow(curRow); //插入一行
    tableWidget->setRowHeight(curRow, 25);

    tableWidget->setItem(curRow, 0, new QTableWidgetItem(QString::number(curRow + 1)));
    tableWidget->setItem(curRow, 1, new QTableWidgetItem(portStr));
    tableWidget->setItem(curRow, 2, new QTableWidgetItem(protocolStr));
    tableWidget->setItem(curRow, 3, new QTableWidgetItem(serverStr));

    tableWidget->item(curRow, 0)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 1)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 2)->setTextAlignment(Qt::AlignCenter);
    tableWidget->item(curRow, 3)->setTextAlignment(Qt::AlignCenter);

}

//端口详情展示
void SettingDialog::portDetailDisplay(QString detail)
{
    m_titleBar->setTitleContent(QString(" 端口详情"));
    ui->stackedWidget->setCurrentIndex(12);
    ui->tableWidget_portInfo_13->setRowCount(0);

    QStringList portInfoList = detail.split(",", QString::SkipEmptyParts); //22;tcp;ssh,23;tcp;telnet
    for(int i = 0; i < portInfoList.count(); i++)
    {
        QString portInfo = portInfoList.at(i);
        QStringList detail = portInfo.split(";", QString::SkipEmptyParts);
        if(detail.size() <= 0)
        {
            continue;
        }
        QString portStr = detail.first();
        QString protocolStr = "";
        QString serverStr = "";
        if(detail.size() >= 3)
        {
            protocolStr = detail.at(1);
            serverStr = detail.at(2);
        }

        addRowOnPortTable(ui->tableWidget_portInfo_13, portStr, protocolStr, serverStr);
    }

}

//重置用户密码
void SettingDialog::resetUserPwd(QString userName)
{
    m_titleBar->setTitleContent(QString(" 重置密码"));
    ui->stackedWidget->setCurrentIndex(13);

    ui->lineEdit_userName_14->setText(userName);
}

//获取重置用户名称
QString SettingDialog::getResetUserName()
{
    QString name = ui->lineEdit_userName_14->text();
    return name;
}

//自参数设置界面vlanid改变
void SettingDialog::on_lineEdit_vlanid_3_textChanged(const QString &arg1)
{
    QString subName = ui->lineEdit_netName_3->text() + "." + arg1;
    ui->lineEdit_subnetName_3->setText(subName);
}

//角色类型选择框变化
void SettingDialog::on_comboBox_roleType_4_currentIndexChanged(int index)
{

    //功能菜单
    QString id = QString::number(index + 1);
    if(m_roleMenuFuncInfoMap.contains(id))
    {
        m_curFuncMenuMap = m_roleMenuFuncInfoMap[id]; //系统审计员的功能读写权限
    }
    clearMenuFuncTree();
    showMenuFuncTree("Root", nullptr); //显示功能树
    //更新树节点的选择状态
    updateTreeItemCheckState();

}

//树节点点击
void SettingDialog::on_treeWidget_4_itemClicked(QTreeWidgetItem *item, int column)
{
    if(column != 1 && column != 2)
    {
        return;
    }

    //同步子节点的状态和父节点一致
    updateSubItemCheckState(item, column);

    //更新父节点
    updateParentItemCheckState(item, 1);
    updateParentItemCheckState(item, 2);

}

void SettingDialog::slot_modifyAlarmColor()
{
    QToolButton *button = static_cast<QToolButton*>(sender());
    QColor color = QColorDialog::getColor(Qt::white);
    if (color.isValid())
    {
        button->setStyleSheet(QString("background-color:%1;").arg(color.name()));
    }
}

bool SettingDialog::eventFilter(QObject *obj, QEvent *evt)
{
    QDialog *w = (QDialog *)obj;

    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonPress) {
        if (event->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = event->globalPos() - w->pos();
            return true;
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
        return true;
    } else if (event->type() == QEvent::MouseMove) {
        if (mousePressed && (event->buttons() && Qt::LeftButton)) {
            w->move(event->globalPos() - mousePoint);
            return true;
        }
    }

    return QObject::eventFilter(obj, evt);
}








