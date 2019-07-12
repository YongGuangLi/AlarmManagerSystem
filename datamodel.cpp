#include "datamodel.h"

DataModel* DataModel::m_instance = new DataModel;
//获取实例
DataModel::DataModel(QObject *parent) : QObject(parent)
{
    m_curDatabaseStatus = false;
}

QMap<QString, stAlarmConfig> DataModel::getMapAlarmConfig() const
{
    return mapAlarmConfig;
}

stAlarmConfig DataModel::getAlarmConfig(QString alarmLevel) const
{
    return mapAlarmConfig.value(alarmLevel);
}

void DataModel::setMapAlarmConfig(const QMap<QString, stAlarmConfig> &value)
{
    mapAlarmConfig = value;
}

//获取实例
DataModel *DataModel::getInstance()
{
    return DataModel::m_instance;
}

//设置当前登录的用户
void DataModel::setCurrentLoginUser(User_Info info)
{
    m_currentLoginUser = info;
}

//获取当前登录的用户
User_Info DataModel::getCurrentLoginUser()
{
    return  m_currentLoginUser;
}

//设置当前用户功能读写权限
void DataModel::setAllUserFuncRwMap(QMap<QString, QMap<QString, RoleMenu_Info> > funcReadWriteMap)
{
    m_roleMenuFuncInfoMap = funcReadWriteMap;
}

//获取用户功能读写权限根据角色ID
QMap<QString, RoleMenu_Info> DataModel::getUserFuncRwMapByRoleId(QString roleId)
{
    QMap<QString, RoleMenu_Info> map;
    if(m_roleMenuFuncInfoMap.contains(roleId))
    {
        map = m_roleMenuFuncInfoMap[roleId];
    }
    return map;
}

//设置所有的用户信息
void DataModel::setAllUserInfoMap(QMap<QString, User_Info> queryMap_Users)
{
    m_queryMap_Users = queryMap_Users;
}

//根据名称找用户
bool DataModel::findUserInfoByName(QString name, User_Info &info)
{
    bool ret = false;

    QMapIterator<QString, User_Info> it(m_queryMap_Users);
    while(it.hasNext())
    {
        it.next();
        User_Info tmpInfo = it.value();
        if(tmpInfo.userName.compare(name, Qt::CaseSensitive) == 0)
        {
            info = tmpInfo;
            ret = true;
            break;
        }
    }

    return ret;
}

//统计已经登录的用户个数
int DataModel::countHaveLoginUsers()
{
    int count = 0;
    QMapIterator<QString, User_Info> it(m_queryMap_Users);
    while(it.hasNext())
    {
        it.next();
        User_Info tmpInfo = it.value();
        if(tmpInfo.loginStatus.compare("yes", Qt::CaseInsensitive) == 0)
        {
            count++; //已登录
        }
    }

    return count;
}

//设置当前系统自定义配置信息
void DataModel::setCommonConfInfo(Data_CommonConfig data_CommonConfig)
{
    m_data_CommonConfig = data_CommonConfig;

    //登录地址
    m_loginAddrLimitList.clear();
    QMapIterator<QString, Login_Addr> it(m_data_CommonConfig.loginAddrMap);
    while(it.hasNext())
    {
        it.next();
        Login_Addr info = it.value();
        m_loginAddrLimitList.append(info.ipAddr);
    }

    //重要操作
    m_importantOperFuncIdList.clear();
    QMapIterator<QString, Important_Oper> it_oper(m_data_CommonConfig.importantOperMap);
    while(it_oper.hasNext())
    {
        it_oper.next();
        Important_Oper info = it_oper.value();
        if(info.isImportant.compare("yes", Qt::CaseInsensitive) == 0)
        {
            m_importantOperFuncIdList.append(info.funcId); //功能ID
        }
    }

}

//获取当前系统自定义配置信息
Data_CommonConfig DataModel::getCommonConfInfo()
{
    return m_data_CommonConfig;
}

//获取密码有效时间(秒)
qint64 DataModel::getPwdActiveTimeLimitCur()
{
    qint64 value = 0;

    QString key = "pwdActiveTimeLimit";
    if(m_data_CommonConfig.myConfInfoMap.contains(key))
    {
        MyConf_Info info = m_data_CommonConfig.myConfInfoMap[key];
        if(info.currentUnit.compare("天") == 0)
        {
            value = info.currentVal * 24 * 60 * 60;
        }
        else if(info.currentUnit.compare("时") == 0)
        {
            value = info.currentVal * 60 * 60;
        }
        else if(info.currentUnit.compare("分") == 0)
        {
            value = info.currentVal * 60;
        }
        else if(info.currentUnit.compare("秒") == 0)
        {
            value = info.currentVal;
        }

    }

    return  value;
}

//获取登录失败次数限制
int DataModel::getLoginFailedLimitCur()
{
    int value = 0;

    QString key = "loginFailedLimit";
    if(m_data_CommonConfig.myConfInfoMap.contains(key))
    {
        MyConf_Info info = m_data_CommonConfig.myConfInfoMap[key];
        value = info.currentVal;
    }

    return  value;
}

//获取账户锁定时间(秒)
int DataModel::getUserLockTimeLimitCur()
{
    int value = 0;

    QString key = "userLockTimeLimit";
    if(m_data_CommonConfig.myConfInfoMap.contains(key))
    {
        MyConf_Info info = m_data_CommonConfig.myConfInfoMap[key];
        if(info.currentUnit.compare("天") == 0)
        {
            value = info.currentVal * 24 * 60 * 60;
        }
        else if(info.currentUnit.compare("时") == 0)
        {
            value = info.currentVal * 60 * 60;
        }
        else if(info.currentUnit.compare("分") == 0)
        {
            value = info.currentVal * 60;
        }
        else if(info.currentUnit.compare("秒") == 0)
        {
            value = info.currentVal;
        }

    }

    return  value;
}

//获取登录连接个数限制
int DataModel::getLoginUsersLimitCur()
{
    int value = 0;

    QString key = "loginUsersLimit";
    if(m_data_CommonConfig.myConfInfoMap.contains(key))
    {
        MyConf_Info info = m_data_CommonConfig.myConfInfoMap[key];
        value = info.currentVal;
    }

    return  value;
}

//获取登录超时时间，没有操作时间(秒)
int DataModel::getLoginNoOperTimeLimitCur()
{
    int value = 0;

    QString key = "loginNoOperTimeLimit";
    if(m_data_CommonConfig.myConfInfoMap.contains(key))
    {
        MyConf_Info info = m_data_CommonConfig.myConfInfoMap[key];
        if(info.currentUnit.compare("天") == 0)
        {
            value = info.currentVal * 24 * 60 * 60;
        }
        else if(info.currentUnit.compare("时") == 0)
        {
            value = info.currentVal * 60 * 60;
        }
        else if(info.currentUnit.compare("分") == 0)
        {
            value = info.currentVal * 60;
        }
        else if(info.currentUnit.compare("秒") == 0)
        {
            value = info.currentVal;
        }

    }

    return  value;
}

//获取装置cpu使用率阈值
int DataModel::getDeviceCpuLimit()
{
    int value = 0;

    QString key = "deviceCpuLimit";
    if(m_data_CommonConfig.myConfInfoMap.contains(key))
    {
        MyConf_Info info = m_data_CommonConfig.myConfInfoMap[key];
        value = info.currentVal;
    }

    return  value;
}

//获取装置内存使用率阈值
int DataModel::getDeviceMemLimit()
{
    int value = 0;

    QString key = "deviceMemLimit";
    if(m_data_CommonConfig.myConfInfoMap.contains(key))
    {
        MyConf_Info info = m_data_CommonConfig.myConfInfoMap[key];
        value = info.currentVal;
    }

    return  value;
}

//获取装置磁盘使用率阈值
int DataModel::getDeviceStorageLimit()
{
    int value = 0;

    QString key = "deviceStorageLimit";
    if(m_data_CommonConfig.myConfInfoMap.contains(key))
    {
        MyConf_Info info = m_data_CommonConfig.myConfInfoMap[key];
        value = info.currentVal;
    }

    return  value;
}

//更新表格行号
void DataModel::updateTableRowNo(QTableWidget *tableWidget)
{
    int rowCount = tableWidget->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        tableWidget->item(i, 0)->setText(QString::number(i + 1));
    }
}

//判断是否是重要操作
bool DataModel::isImportantOper(QString funcId)
{
    bool flag = false;
    if(m_importantOperFuncIdList.contains(funcId))
    {
        flag = true;
    }
    return flag;
}

//判断是否是可以登录的地址
bool DataModel::isCanLoginAddr()
{
    bool flag = false;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for(int i = 0; i < list.count(); i++)
    {
        QHostAddress addr = list.at(i);
        QString ip = addr.toString();
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
        {
            if(m_loginAddrLimitList.contains(ip))
            {
                flag = true;
                break;
            }
        }
    }

    return flag;
}

//根据设备类型，设备子类型获取资产设备类型描述
QString DataModel::getAssetDeviceTypeDesc(QString type, QString subType)
{
    Q_UNUSED(type)

    QString typeDesc = "未知类型";

    if(subType.compare("FW", Qt::CaseInsensitive) == 0)
    {
        typeDesc = QString("防火墙");
    }
    else if(subType.compare("FID", Qt::CaseInsensitive) == 0)
    {
        typeDesc = QString("横向正向隔离装置");
    }
    else if(subType.compare("BID", Qt::CaseInsensitive) == 0)
    {
        typeDesc = QString("横向反向隔离装置");
    }
    else if(subType.compare("SVR", Qt::CaseInsensitive) == 0)
    {
        typeDesc = QString("服务器");
    }
    else if(subType.compare("SVROT", Qt::CaseInsensitive) == 0)
    {
        typeDesc = QString("通用主机其它");
    }
    else if(subType.compare("SW", Qt::CaseInsensitive) == 0)
    {
        typeDesc = QString("交换机");
    }
    else if(subType.compare("RT", Qt::CaseInsensitive) == 0)
    {
        typeDesc = QString("路由器");
    }
    else if(subType.compare("SWOT", Qt::CaseInsensitive) == 0)
    {
        typeDesc = QString("网络设备其它");
    }
    else if(subType.compare("VEAD", Qt::CaseInsensitive) == 0)
    {
        typeDesc = QString("纵向加密认证装置");
    }
    else if(subType.compare("SUOT", Qt::CaseInsensitive) == 0)
    {
        typeDesc = QString("安全设备其它");
    }
    else if(subType.compare("CTLOT", Qt::CaseInsensitive) == 0)
    {
        typeDesc = QString("嵌入式主机其它");
    }
    else if(subType.compare("DCD", Qt::CaseInsensitive) == 0)
    {
        typeDesc = QString("态势感知采集装置");
    }

    return typeDesc;
}

//设置事件解析相关数据
void DataModel::setData_EventDistribute(Data_EventDistribute data_EventDistribute)
{
    m_data_EventDistribute = data_EventDistribute;
}

//获取当前功能ID对应的日志类型串('日志类型_日志子类型','日志类型_日志子类型')
QString DataModel::getLogTypeStrByFuncId(QString funcId)
{
    QString logTypeStr = "";
    QMap<QString, FuncMatch_EventType> funcMatchEventMap; //功能匹配事件类型
    if(m_data_EventDistribute.funcMatchEventMap.contains(funcId))
    {
        FuncMatch_EventType matchInfo = m_data_EventDistribute.funcMatchEventMap[funcId];

        QStringList logSubtype = matchInfo.logSubType;
        for(int i = 0; i < logSubtype.count(); i++)
        {
            logTypeStr += QString("'%1_%2'").arg(matchInfo.logType).arg(logSubtype.at(i));
            if(i != logSubtype.count() - 1)
            {
                logTypeStr += ",";
            }
        }

    }

    return logTypeStr;
}

//根据功能ID获取表格表头
QStringList DataModel::getHeadersByFuncId(QString funcId)
{
    QStringList headers;

    if(m_data_EventDistribute.funcMatchEventMap.contains(funcId))
    {
        FuncMatch_EventType matchInfo = m_data_EventDistribute.funcMatchEventMap[funcId];

        QMap<QString, Resolve_FieldInfo> fieldInfoMap; //事件解析字段

        //事件解析规则
        QMapIterator<QString, Resolve_RuleInfo> it(m_data_EventDistribute.ruleInfoMap);
        while(it.hasNext())
        {
            it.next();
            Resolve_RuleInfo ruleInfo = it.value();
            if(ruleInfo.logType == matchInfo.logType && matchInfo.logSubType.contains(ruleInfo.logSubType))
            {
                QStringList contentlist = ruleInfo.ruleContent.split(";", QString::SkipEmptyParts);
                for(int i = 0; i < contentlist.count(); i++)
                {
                    QStringList paramList = contentlist.at(i).split(",", QString::SkipEmptyParts);
                    if(m_data_EventDistribute.fieldInfoMap.contains(paramList.last()))
                    {
                        headers.append(m_data_EventDistribute.fieldInfoMap[paramList.last()].fieldAlias);
                    }
                }
                break;
            }
        }

    }

    return headers;
}

//根据功能ID获取日志展示
QStringList DataModel::getLogDispByFuncId(Fromalized_LogInfo logInfo, QString funcId)
{
    QStringList logDisp;
    bool haveRuleFlag = false;
    if(m_data_EventDistribute.funcMatchEventMap.contains(funcId))
    {
        FuncMatch_EventType matchInfo = m_data_EventDistribute.funcMatchEventMap[funcId];

        logDisp.append(logInfo.assetIp); //资产IP地址

        //事件解析规则
        QMapIterator<QString, Resolve_RuleInfo> it(m_data_EventDistribute.ruleInfoMap);
        while(it.hasNext())
        {
            it.next();
            Resolve_RuleInfo ruleInfo = it.value();
            if(ruleInfo.logType == matchInfo.logType && matchInfo.logSubType.contains(ruleInfo.logSubType))
            {
                QMap<QString, QString> paramValueMap; //<字段名称，字段值>
                QStringList paramList = logInfo.logDetail.split(ruleInfo.ruleRegex, QString::SkipEmptyParts);
                QString paramStr;
                for(int i = 0; i < paramList.count(); i++)
                {
                    paramStr += QString("#%1(%2) ").arg(i + 1).arg(paramList.at(i));
                    paramValueMap.insert(QString::number(i + 1), paramList.at(i)); //<字段名称，字段值>
                }
                QStringList contentlist = ruleInfo.ruleContent.split(";", QString::SkipEmptyParts);
                for(int i = 0; i < contentlist.count(); i++)
                {
                    QStringList paramList = contentlist.at(i).split(",", QString::SkipEmptyParts);
                    QString fieldIndex = paramList.first();
                    if(!fieldIndex.isEmpty())
                    {
                        QStringList indexList = fieldIndex.split("#", QString::SkipEmptyParts);
                        QString valueStr = "";
                        for(int j = 0; j < indexList.count(); j++)
                        {
                            if(paramValueMap.contains(indexList.at(j)))
                            {
                                valueStr += paramValueMap[indexList.at(j)];
                            }
                        }
                        logDisp.append(valueStr);
                    }
                }
                haveRuleFlag = true;
                break;
            }
        }

    }
    else
    {
        haveRuleFlag = false;
    }

    //没有规则
    if(!haveRuleFlag)
    {
        logDisp.clear();
        logDisp.append(logInfo.assetName); //资产名称
        logDisp.append(logInfo.assetTypeDesc); //资产类型
        logDisp.append(logInfo.assetIp); //资产IP地址
        logDisp.append(logInfo.alarmTime); //事件时间
        logDisp.append(logInfo.logDetail); //事件内容     
    }

    return logDisp;
}

//设置所有在事件模型页面展示的功能ID
void DataModel::setAllEventDispFuncIdList(QStringList allEventDispFuncIdList)
{
    m_allEventDispFuncIdList = allEventDispFuncIdList;
}

//判断该功能ID是不是在事件展示页面显示
bool DataModel::isEventDispFuncId(QString funcId)
{
    bool flag = false;

    if(m_allEventDispFuncIdList.contains(funcId))
    {
        flag = true;
    }

    return flag;
}

//可以下载文件的功能ID
bool DataModel::isCanDownloadFileFuncId(QString funcId)
{
    bool flag = false;

    if(m_allCanDownloadFileFuncIdList.contains(funcId))
    {
        flag = true;
    }

    return flag;
}

//添加一个可以下载文件的功能ID
void DataModel::appendCanDownloadFileFuncId(QString funcId)
{
    m_allCanDownloadFileFuncIdList.append(funcId);
}

//数据库当前登录状态
void DataModel::setCurDatabaseStatus(bool flag)
{
    m_curDatabaseStatus = flag;
}

//数据库当前登录状态
bool DataModel::getCurDatabaseStatus()
{
    return m_curDatabaseStatus;
}

//设置系统当前时间
void DataModel::setCurrentSysDateTime(QDateTime dateTime)
{
    m_sysDatetime = dateTime;
}

//获取系统当前时间
QDateTime DataModel::getCurrentSysDateTime()
{
    return QDateTime::currentDateTime();
}











