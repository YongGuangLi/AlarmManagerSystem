#include "databasehelper.h"

DataBaseHelper * DataBaseHelper::dbHelp_ = NULL;

#define SQL_USERINFO_TABLE     "create table userinfo(username varchar(20) primary key, password varchar(20), level int)"

#define SQL_GET_USERINFO       "select id,username,password,groupname from userinfo"

#define SQL_INSERT_USERINFO    "insert into userinfo(username,password,groupname) value('%1','%2','%3')"
#define SQL_UPDATE_USERINFO    "update userinfo set password = '%1' where username = '%2'"
#define SQL_DELETE_USERINFO    "delete from userinfo where username = '%1'"

#define SQL_GET_GROUPINFO      "select id,groupname from groupinfo"
#define SQL_INSERT_GROUPINFO   "insert into groupinfo(id,groupname) value(%1,'%2')"
#define SQL_DELETE_GROUPINFO   "delete from groupinfo where groupname = '%1'"

#define SQL_GET_RIGHTINFO      "select id,rightname,groupflag from rightinfo"
#define SQL_UPDATE_RIGHTINFO   "update rightinfo set groupflag = %1 where id = %2"

#define SQL_GET_IAALARMDATA    "select id,level,time,filename,text,confirmed from iaalarmdata order by time desc limit %1"           //获取智能告警数据
#define SQL_GET_OAALARMDATA    "select id,level,lasTtime,device,msg,confirmed from dtxy_alarm_%1 order by lastTime desc limit %2"     //获取原始告警数据

#define SQL_GET_HISIAALARMDATA    "select id,level,time,filename,text,confirmed from iaalarmdata where time between '%1' and '%2'"                      //获取智能告警历史数据
#define SQL_GET_HISOAALARMDATA    "select id,level,lasTtime,device,msg,confirmed from dtxy_alarm_%1 where lasTtime between '%2' and '%3'"     //获取原始告警历史数据

#define SQL_MODIFY_IACONFIRM   "update iaalarmdata set confirmed = 1 where id = %1"                                                 //修改智能告警状态
#define SQL_MODIFY_OACONFIRM   "update dtxy_alarm_%1 set confirmed = '1' where id = %2"                                                 //修改原始告警状态


#define SQL_GET_IAALARMDATACNT "select COUNT(0) from iaalarmdata where time between '%1' and '%2' and level = %3"
#define SQL_GET_OAALARMDATACNT "select COUNT(0) from dtxy_alarm_%1 where lasTtime between '%2' and '%3' and level = %4"

DataBaseHelper::DataBaseHelper(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<stAlarmData>("stAlarmData");
}

bool DataBaseHelper::open(QString ip, int port, QString dbName, QString user, QString passwd)
{
    sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    sqlDatabase.setHostName(ip);
    sqlDatabase.setPort(port);
    sqlDatabase.setDatabaseName(dbName);
    sqlDatabase.setUserName(user);
    sqlDatabase.setPassword(passwd);
    ip_ = ip;
    port_ = port;
    dbName_ = dbName;
    user_ = user;
    passwd_ = passwd;
    return sqlDatabase.open();
}

bool DataBaseHelper::isOpen()
{
    bool result = sqlDatabase.isOpen();

    if(result == false)
    {
        result = open(ip_, port_, dbName_, user_, passwd_);
        if(result == true)
        {
            qDebug()<<QString::fromUtf8("数据库重新连接成功");
        }
    }

    return result;
}

bool DataBaseHelper::execSqlStr(QString sqlStr)
{
    bool result = false;

    QSqlQuery query(sqlStr);
    if(query.lastError().isValid())
        qDebug()<<query.lastError();
    else
        result = true;

    return result;
}


//读所有的用户信息
void DataBaseHelper::readAllUserInfo(QMap<QString, User_Info> &allUsersMap)
{
    allUsersMap.clear();
    QSqlQuery query(SQL_QUERY_USERS_INFO);
    while(query.next())
    {
        User_Info info;

        info.userId = query.value("Id").toString(); //用户ID
        info.userName = query.value("User_Name").toString(); //用户名称
        info.realName = query.value("Real_Name").toString(); //用户姓名
        info.password = query.value("Password").toString(); //密码
        info.isSystem = query.value("Is_Built_In").toString(); //系统内置
        info.isEnable = query.value("Is_Enabled").toString(); //是否启用
        info.remark = query.value("Remark").toString(); //备注
        info.activeTime = query.value("Active_Time").toDateTime(); //有效时间
        info.roleId = query.value("Role_Id").toString(); //角色ID
        info.roleName = query.value("Role_Name").toString(); //角色名称
        info.loginFailedNum = query.value("LoginFailedNum").toInt(); //登录失败次数
        info.lockEndTime = query.value("LockEndTime").toDateTime(); //账户锁定结束时间
        info.loginStatus = query.value("LoginStatus").toString(); //当前登录状态yes-已登录no-未登录
        info.roleType = query.value("Role_Type").toString(); //角色类型

        allUsersMap.insert(info.userId, info);
    }
}

//读所有的角色信息
void DataBaseHelper::readAllRoleInfo(QMap<QString, Role_Info> &allRoleInfoMap)
{
    allRoleInfoMap.clear();
    //先查询所有的角色信息
    QSqlQuery query(SQL_QUERY_ROLE_INFO);
    while(query.next())
    {
        Role_Info roleInfo;
        roleInfo.id = query.value("Id").toString(); //唯一标识
        roleInfo.roleName = query.value("Role_Name").toString(); //角色名称
        roleInfo.isSystem = query.value("Is_Built_In").toString(); //系统内置
        roleInfo.remark = query.value("Remark").toString(); //备注
        roleInfo.roleType = query.value("Role_Type").toString(); //角色类型

        allRoleInfoMap.insert(roleInfo.id, roleInfo);
    }

    //查询角色已分配的用户
    QMapIterator<QString, Role_Info> it(allRoleInfoMap);
    while(it.hasNext())
    {
        it.next();
        Role_Info roleInfo = it.value();
        QString sqlStr = QString(SQL_QUERY_ROLE_USERS_INFO).arg(roleInfo.id);

        QSqlQuery queryUsers(sqlStr);

        QString usersInfo;
        while(queryUsers.next())
        {
            usersInfo = queryUsers.value(0).toString();
        }
        roleInfo.userNames = usersInfo;
        allRoleInfoMap[roleInfo.id] = roleInfo;

        queryUsers.clear();
    }
}

/*************************************
Description: 读取数据库中的用户信息
Input:
Output:
Return:
Other:
 **************************************/
void DataBaseHelper::queryUserConfigInfo(Data_UserConfig &data_UserConfig)
{
    //读所有的用户信息
    readAllUserInfo(data_UserConfig.allUsersMap);

    //读所有的角色信息
    readAllRoleInfo(data_UserConfig.allRoleInfoMap);
}



//读所有的角色的功能读写权限
void DataBaseHelper::readRoleMenuReadWrite(QMap<QString, QMap<QString, RoleMenu_Info> > &roleMenuReadWriteMap)
{
    //查询角色的功能
    roleMenuReadWriteMap.clear();

    QSqlQuery query(SQL_QUERY_FUNC_INFO);
    while(query.next())
    {
        RoleMenu_Info info;
        info.roleId = query.value("Role_Id").toString(); //角色ID
        info.funcId = query.value("Menu_Func_Id").toString(); //功能ID
        info.isRead = query.value("Is_Read").toBool(); //是否可读
        info.isWrite = query.value("Is_Write").toBool(); //是否可写

        QMap<QString, RoleMenu_Info> roleMenuInfoMap = roleMenuReadWriteMap[info.roleId]; //角色的读写权限
        roleMenuInfoMap.insert(info.funcId, info);
        roleMenuReadWriteMap[info.roleId] = roleMenuInfoMap; //新增一条
    }
}

//查询角色配置信息
void DataBaseHelper::queryRoleConfigInfo(Data_RoleConfig &data_RoleConfig)
{
    //读所有的角色信息
    readAllRoleInfo(data_RoleConfig.allRoleInfoMap);

    //读所有的角色的功能读写权限
    readRoleMenuReadWrite(data_RoleConfig.roleMenuReadWriteMap);
}

void DataBaseHelper::queryMenuFunc(QMultiMap<QString, MenuFunc_Info> &allMenuFuncInfoMap)
{
    QSqlQuery query(SQL_ALL_MENU_FUNC);

    //查询已知资产信息
    while(query.next())
    {
        MenuFunc_Info info;
        info.id = query.value(0).toString(); //唯一标识
        info.parentId = query.value(1).toString(); //父节点ID
        info.funcId = query.value(2).toString(); //功能ID
        info.funcTitle = query.value(3).toString(); //功能标题
        info.funcType = query.value(4).toString(); //功能类型
        info.orderNo = query.value(5).toInt(); //序号

        if(!info.parentId.isEmpty())
        {
            allMenuFuncInfoMap.insert(info.parentId, info);
        }
    }
}


//查询用户登录需要的信息
void DataBaseHelper::queryUserLoginInfo(Data_UserLogin &data_UserLogin)
{
    //读所有的用户信息
    readAllUserInfo(data_UserLogin.allUsersMap);

    //读所有的角色的功能读写权限
    readRoleMenuReadWrite(data_UserLogin.roleMenuReadWriteMap);
}

QMap<QString, stAlarmConfig> DataBaseHelper::queryAlarmConfig()
{
    QMap<QString, stAlarmConfig> mapAlarmConfig;
    QSqlQuery query(SQL_QUERY_ALARMCONFIG);
    while (query.next())
    {
        stAlarmConfig alarmConfig;
        alarmConfig.alarmLevel = query.value(0).toString();
        alarmConfig.alarmName = query.value(1).toString();
        alarmConfig.alarmColor = query.value(2).toString();
        alarmConfig.alarmWindow = query.value(3).toInt();

        mapAlarmConfig.insert(alarmConfig.alarmLevel, alarmConfig);
    }

    return mapAlarmConfig;
}



void DataBaseHelper::readIaAlarmDataFromDB()
{
    QSqlQuery query(QString(SQL_GET_IAALARMDATA).arg(QString::number(SingletonConfig->getMaxSize())));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text();
        return;
    }
    int size = query.size();
    while(size--)
    {
        query.seek(size);
        stAlarmData alarmData;
        alarmData.id_  = query.value(0).toInt();
        alarmData.level_  = query.value(1).toInt();
        alarmData.time_ =  query.value(2).toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        alarmData.filename_  = query.value(3).toString();
        //<0> 2018-09-10 16:39:17 茂名电网.广场变/10kV.#1站用变 保护测控装置异常告警 "T_0013 20180910_163917263_1S.xml 主变"
        alarmData.text_ =  query.value(4).toString().split(" ").at(3) + " " + query.value(4).toString().split(" ").at(4);
        alarmData.confirmed_ =   query.value(5).toInt();

        SingletonAlarmData->insertSmartAlarmData(alarmData);
    }
}


void DataBaseHelper::readOaAlarmDataFromDB()
{
    QSqlQuery query(QString(SQL_GET_OAALARMDATA).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd")).arg(QString::number(SingletonConfig->getMaxSize())));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text()<<query.lastQuery();
        return;
    }
    int size = query.size();

    while(size--)
    {
        query.seek(size);
        stAlarmData alarmData;
        alarmData.id_  = query.value(0).toInt();
        alarmData.level_  = query.value(1).toInt();
        alarmData.time_ =  query.value(2).toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        alarmData.text_ =  query.value(3).toString() + "  " + query.value(4).toString() ;
        alarmData.confirmed_ =   query.value(5).toInt();

        SingletonAlarmData->insertOriginalAlarmData(alarmData);
    }
}



void DataBaseHelper::queryHistoryData(QString beginTime, QString endTime, int alarmLevel, int alarmType)
{
    if(alarmType == 0)          // 0:智能告警
    {
        QSqlQuery query(QString(SQL_GET_HISIAALARMDATA).arg(beginTime).arg(endTime));
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError().text()<<query.lastQuery();
            return;
        }
        int size = query.size();
        while(size--)
        {
            query.seek(size);
            stAlarmData alarmData;
            alarmData.id_  = query.value(0).toInt();
            alarmData.level_  = query.value(1).toInt();
            alarmData.time_ =  query.value(2).toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
            alarmData.filename_  = query.value(3).toString();
            //<0> 2018-09-10 16:39:17 茂名电网.广场变/10kV.#1站用变 保护测控装置异常告警 "T_0013 20180910_163917263_1S.xml 主变"
            alarmData.text_ =  query.value(4).toString().split(" ").at(3) + " " + query.value(4).toString().split(" ").at(4);
            alarmData.confirmed_ =   query.value(5).toInt();

            if(alarmData.level_ == alarmLevel || LEVEL_REALTIME == alarmLevel)
            {
                emit sendAlarmData(alarmData.id_, alarmData);
            }
        }
    }else if(alarmType == 1)   //  1:原始告警
    {
        QDate beginDate = QDateTime::fromString(beginTime, "yyyy-MM-dd hh:mm:ss").date();
        QDate endDate = QDateTime::fromString(endTime, "yyyy-MM-dd hh:mm:ss").date();

        while(beginDate <= endDate)
        {
            QSqlQuery query(QString(SQL_GET_HISOAALARMDATA).arg(beginDate.toString("yyyy_MM_dd")).arg(beginTime).arg(endTime));
            beginDate = beginDate.addDays(1);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError().text()<<query.lastQuery();
                continue;
            }
            int size = query.size();

            while(size--)
            {
                query.seek(size);
                stAlarmData alarmData;
                alarmData.id_  = query.value(0).toInt();
                alarmData.level_  = query.value(1).toInt();
                alarmData.time_ =  query.value(2).toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
                alarmData.text_ =  query.value(3).toString() + "  " + query.value(4).toString() ;
                alarmData.confirmed_ =   query.value(5).toInt();

                if(alarmData.level_ == alarmLevel || LEVEL_REALTIME == alarmLevel)
                {
                    emit sendAlarmData(alarmData.id_, alarmData);
                }
            }
        }
    }
}

int DataBaseHelper::querySmartDataCnt(QString beginTime, QString endTime, int alarmLevel)
{
    int Cnt = 0;
    QSqlQuery query(QString(SQL_GET_IAALARMDATACNT).arg(beginTime).arg(endTime).arg(alarmLevel));

    if(query.lastError().isValid())
        qDebug()<<query.lastError().text()<<query.lastQuery();

    while(query.next())
    {
        Cnt = query.value(0).toInt();
    }
    return Cnt;
}

int DataBaseHelper::queryOriginalDataCnt(QString beginTime, QString endTime, int alarmLevel)
{
    int Cnt = 0;
    QDate beginDate = QDateTime::fromString(beginTime, "yyyy-MM-dd hh:mm:ss").date();
    QDate endDate = QDateTime::fromString(endTime, "yyyy-MM-dd hh:mm:ss").date();

    while(beginDate <= endDate)
    {
        if(!sqlDatabase.tables().contains(QString("dtxy_alarm_%1").arg(beginDate.toString("yyyy_MM_dd"))))
        {
            beginDate = beginDate.addDays(1);
            continue;
        }
        QSqlQuery query(QString(SQL_GET_OAALARMDATACNT).arg(beginDate.toString("yyyy_MM_dd")).arg(beginTime).arg(endTime).arg(alarmLevel));
        beginDate = beginDate.addDays(1);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError().text()<<query.lastQuery();
            continue ;
        }
        while(query.next())
        {
            Cnt += query.value(0).toInt();
        }
    }

    return  Cnt;
}


QString DataBaseHelper::getError()
{
    return sqlDatabase.lastError().text();
}


bool DataBaseHelper::modifyIaAlarmStatus(int id)
{
    bool status = false;

    QSqlQuery query(QString(SQL_MODIFY_IACONFIRM).arg(id));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text();
    }
    else
    {
        status = true;
    }

    return status;
}

bool DataBaseHelper::modifyOaAlarmStatus(QString alarmTime, int id)
{
    bool status = false;
    QDate alarmDate = QDateTime::fromString(alarmTime, "yyyy-MM-dd hh:mm:ss.zzz").date();
    QSqlQuery query(QString(SQL_MODIFY_OACONFIRM).arg(alarmDate.toString("yyyy_MM_dd")).arg(id));
    if(query.lastError().isValid())
    {
       qDebug()<<query.lastError().text();
       qDebug()<<query.lastQuery();
    }
    else
    {
         status = true;
    }

    return status;
}

void DataBaseHelper::timerEvent(QTimerEvent *event)
{

        QDateTime dateTime = queryCurrentDateTime();
        DataModel::getInstance()->setCurrentSysDateTime(dateTime);
}

QDateTime DataBaseHelper::queryCurrentDateTime()
{
    QDateTime dateTime;

    QSqlQuery query(SQL_QUERY_SYSDATE);
    while(query.next())
    {
       dateTime = query.value(0).toDateTime(); //装置日期时间
    }
    return dateTime;
}


DataBaseHelper *DataBaseHelper::GetInstance()
{
    if(dbHelp_ == NULL)
    {
        dbHelp_ = new DataBaseHelper();
    }
    return dbHelp_;
}
