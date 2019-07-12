#ifndef COMMONOBJECT_H
#define COMMONOBJECT_H

//工程版本号
#define VERSION "Version20190116"
//远程维护redis通道
#define CHANNEL_REMOTE "RemoteHelper"
#define CHANNEL_MYSELF "Config"
//kafka通道
#define CHANNEL_KAFKA "DataTransmission"
//系统日志一页条数
#define PAGE_COUNT 500
//数据库查询定时器
#define TIMER_QUERY_DATA 5*60*1000
//查询系统日期时间定时器
#define TIMER_QUERY_SYSDATE 1000

//重启代码
#define RETCODE_RESTART 773

#include <QDebug>
#include <QList>
#include <QStringList>
#include <QDateTime>

//查询装置当前时间
#define SQL_QUERY_SYSDATE "SELECT SYSDATE()"

//查询功能菜单
#define SQL_ALL_MENU_FUNC "SELECT * FROM menu_func"
//<------------网卡信息设置------------->
//查询网卡信息
#define SQL_QUERY_NETWORK_INFO "SELECT * FROM device_network_card_config"
//更新网卡信息
#define SQL_UPDATE_NETWORK_INFO "UPDATE device_network_card_config a SET a.IP_Address = '%1', a.Subnet_Mask = '%2', a.SafeArea = '%3', a.NetFlag = '%4' WHERE a.Id = '%5'"
//插入网卡信息
#define SQL_INSERT_NETWORK_INFO "INSERT INTO device_network_card_config VALUES('%1','%2','%3','%4','%5','%6')"
//删除网卡信息
#define SQL_DELETE_NETWORK_INFO "DELETE FROM device_network_card_config WHERE Id = '%1'"

//<-------------路由信息设置------------->
//查询路由信息
#define SQL_QUERY_ROUTE_INFO "SELECT * FROM device_route_config"
//插入路由信息
#define SQL_INSERT_ROUTE_INFO "INSERT INTO device_route_config VALUES('%1','%2','%3','%4')"
//删除路由信息
#define SQL_DELETE_ROUTE_INFO "DELETE FROM device_route_config WHERE Id = '%1'"

//<------------子接口参数信息设置------------->
//查询网卡子接口情况
#define SQL_QUERY_SUBNET_MAIN_INFO "SELECT * FROM sub_interface_main"
//更新网卡子接口情况
#define SQL_UPDATE_SUBNET_MAIN_INFO "UPDATE sub_interface_main a SET a.Interface_Num = '%1', a.Update_Time = '%2' WHERE a.Interface_Name = '%3'"
//查询子接口参数
#define SQL_QUERY_SUBNET_INFO "SELECT * FROM sub_interface_config"
//更新子接口参数
#define SQL_UPDATE_SUBNET_INFO "UPDATE sub_interface_config a SET a.IP_Address = '%1', a.Subnet_Mask = '%2' WHERE a.Id = '%3'"
//插入子接口参数
#define SQL_INSERT_SUBNET_INFO "INSERT INTO sub_interface_config VALUES('%1','%2','%3','%4','%5','%6')"
//删除子接口参数
#define SQL_DELETE_SUBNET_INFO "DELETE FROM sub_interface_config WHERE Id = '%1'"

//<------------角色配置------------->
//查询所有的角色信息
#define SQL_QUERY_ROLE_INFO "SELECT * FROM role"
//查询角色已分配的用户
#define SQL_QUERY_ROLE_USERS_INFO "SELECT GROUP_CONCAT(a.User_Name) FROM users a WHERE a.Role_Id = '%1'"
//查询角色的功能
#define SQL_QUERY_FUNC_INFO "SELECT * FROM role_menu"
//查询一个角色的功能
#define SQL_QUERY_ONEROLE_FUNC_INFO "SELECT * FROM role_menu a WHERE a.Role_Id = '%1'"
//更新角色的功能
#define SQL_UPDATE_FUNC_INFO "UPDATE role_menu a SET a.Is_Read = '%1', a.Is_Write = '%2' WHERE a.Role_Id = '%3' AND a.Menu_Func_Id = '%4'"
//删除角色的功能
#define SQL_DELETE_FUNC_INFO "DELETE FROM role_menu WHERE Role_Id = '%1'"
//插入角色的功能
#define SQL_INSERT_FUNC_INFO "INSERT INTO role_menu VALUES('%1','%2','%3','%4')"
//更新角色信息
#define SQL_UPDATE_ROLE_INFO "UPDATE role a SET a.Role_Name = '%1', a.Remark = '%2', a.Role_Type = '%3' WHERE a.Id = '%4'"
//插入角色信息
#define SQL_INSERT_ROLE_INFO "INSERT INTO role VALUES('%1','%2','%3','%4','%5')"
//删除角色信息
#define SQL_DELETE_ROLE_INFO "DELETE FROM role WHERE Id = '%1'"
//删除角色下所有用户
#define SQL_DELETE_ROLE_USERS_INFO "DELETE FROM users WHERE Role_Id = '%1'"

//<------------用户配置------------->
//查询所有的用户
#define SQL_QUERY_USERS_INFO "SELECT a.*, b.Role_Name, b.Role_Type FROM users a , role b WHERE a.Role_Id = b.Id"
//查询一条用户信息
#define SQL_QUERY_ONE_USER_INFO "SELECT a.*, b.Role_Name FROM users a , role b WHERE a.Role_Id = b.Id AND a.User_Name = '%1'"
//插入用户信息
#define SQL_INSERT_USER_INFO "INSERT INTO users VALUES('%1','%2','%3','%4','%5','%6','%7','%8','%9', '0', '%10', 'no')"
//更新用户信息
#define SQL_UPDATE_USER_INFO "UPDATE users a SET a.User_Name = '%1', a.Real_Name = '%2', a.Remark = '%3' WHERE a.Id = '%4'"
//删除用户信息
#define SQL_DELETE_USER_INFO "DELETE FROM users WHERE Id = '%1'"

//<------------审核管理------------->
//查询所有的事项
#define SQL_QUERY_EVENT_INFO "SELECT a.*,b.User_Name FROM schedule_info a, users b WHERE a.Send_User = b.Id ORDER BY a.Start_Time DESC"
//插入待办事项
#define SQL_INSERT_EVENT_INFO "INSERT INTO schedule_info VALUES('%1', '%2', '%3', '%4', null, '%5', null, '%6', '%7')"
//更新待办事项
#define SQL_UPDATE_EVENT_INFO "UPDATE schedule_info a SET a.End_User = '%1', a.End_Time = '%2', a.Is_End = '%3' WHERE a.Id = '%4'"
//更新用户启用状态
#define SQL_UPDATE_USER_ENABLE "UPDATE users a SET a.Is_Enabled = 'yes' WHERE a.Id = '%1'"
//更新用户登录密码(审核之后更新)
#define SQL_UPDATE_USER_PWD "UPDATE users a SET a.`Password` = '%1', a.Active_Time = '%2' WHERE a.Id = '%3'"

//<------------用户登录------------->
//更新用户登录状态
#define SQL_UPDATE_LOGIN_STATUS "UPDATE users a SET a.LoginStatus = '%1' WHERE a.Id = '%2'"
//更新用户登录失败次数
#define SQL_UPDATE_LOGINFAILEDNUM "UPDATE users a SET a.LoginFailedNum = '%1' WHERE a.Id = '%2'"
//更新用户登录锁定时间
#define SQL_UPDATE_LOGINLOCKTIME "UPDATE users a SET a.LockEndTime = '%1' WHERE a.Id = '%2'"

//<------------常用配置------------->
//查询所有的自定义配置
#define SQL_QUERY_CUSTOM_CONFIG "SELECT * FROM custom_config"
//更新自定义配置的当前值
#define SQL_UPDATE_CUSTOM_CONFIG "UPDATE custom_config a SET a.Current_Val = '%1', a.Current_Unit = '%2' WHERE a.Param_Name = '%3'"
//查询登录地址限制
#define SQL_QUERY_LOGIN_ADDR "SELECT * FROM login_addr"
//插入登录地址限制
#define SQL_INSERT_LOGIN_ADDR "INSERT INTO login_addr VALUES('%1', '%2')"
//删除登录地址限制
#define SQL_DELETE_LOGIN_ADDR "DELETE FROM login_addr WHERE Id = '%1'"
//查询重要操作
#define SQL_QUERY_IMPORTANT_OPER "SELECT * FROM important_oper"
//更新重要操作
#define SQL_UPDATE_IMPORTANT_OPER "UPDATE important_oper a SET a.Is_Important = '%1' WHERE a.Id = '%2'"

//<------------系统日志------------->
//查询日志
#define SQL_QUERY_SYSTEM_LOG "SELECT * FROM system_log a WHERE a.Log_Time BETWEEN '%1' AND '%2' ORDER BY a.Log_Time DESC LIMIT %3, %4"
//查询日志条数
#define SQL_QUERY_LOG_COUNT "SELECT COUNT(*) FROM system_log a WHERE a.Log_Time BETWEEN '%1' AND '%2' ORDER BY a.Log_Time DESC"
//插入一条日志
#define SQL_INSERT_SYSTEM_LOG "INSERT INTO system_log VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10')"

//<------------装置自检------------->
//查询装置ID
#define SQL_QUERY_MYDEVICE_INFO "SELECT a.Id FROM asset a WHERE a.Device_Sub_Type = 'DCD' AND a.Is_Delete = 'no'"
//查询装置cpu和内存
#define SQL_QUERY_MYDEVICE_CPUMEM "SELECT * FROM asset_cpu_mem a WHERE a.Asset_Id = '%1'"
//查询装置磁盘使用
#define SQL_QUERY_MYDEVICE_STORAGE "SELECT * FROM asset_storage a WHERE a.Asset_Id = '%1'"
//查询进程信息
#define SQL_QUERY_PROCESS "SELECT * FROM process_config"

//<------------主站参数设置------------->
//查询主站参数信息
#define SQL_QUERY_MAINSTATION_INFO "SELECT * FROM station_connect_config"
//插入主站参数信息
#define SQL_INSERT_MAINSTATION_INFO "INSERT INTO station_connect_config VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10')"
//更新主站参数信息
#define SQL_UPDATE_MAINSTATION_INFO "UPDATE station_connect_config a SET a.StationName='%1',a.CtrlChannelIp='%2',a.CtrlChannelStatus='1',a.DataChannelIp='%3',a.DataChannelName='%4',a.DataChannelPort='%5',a.DataChannelStatus='1',a.MainFlag='%6' WHERE a.Id = '%7'"
//删除主站参数信息
#define SQL_DELETE_MAINSTATION_INFO "DELETE FROM station_connect_config WHERE Id = '%1'"

//<------------装置注册页面------------->
//查询装置注册信息
#define SQL_QUERY_DEVICEREGISTER_INFO "SELECT * FROM device"

//<------------资产在线页面------------->
//查询所有的已知资产信息
#define SQL_QUERY_KNOWN_ASEETINFO "SELECT * FROM asset a WHERE a.Is_Known = 'yes' AND a.Is_Delete = 'no'"
//查询所有的资产信息
#define SQL_QUERY_ALL_ASEETINFO "SELECT * FROM asset a WHERE a.Is_Delete = 'no'"

//<------------范式化规则文件页面------------->
#define SQL_QUERY_FORMALIZEDFILE_INFO "SELECT * FROM formalized_file"

//<------------关键文件清单页面------------->
#define SQL_QUERY_KEYFILE_INFO "SELECT a.*, b.Device_Name, b.Device_Type, b.Device_Sub_Type, b.Asset_Ip FROM key_document a, asset b WHERE a.Is_Delete = 'no' AND a.Asset_Id = b.Id"

//<------------ICMP参数页面------------->
#define SQL_QUERY_ICMPPARAM_INFO "SELECT * FROM config_scan_info"

//<------------SNMP参数页面------------->
//查询所有的snmp配置
#define SQL_QUERY_SNMPPARAM_INFO "SELECT * FROM snmp_parameter_config"
//更新默认配置
#define SQL_UPDATE_DEFAULT_SNMPCONFIG "UPDATE snmp_parameter_config a SET a.IP_Address_Begin = '%1', a.IP_Address_End = '%2', a.SNMP_Port_Number = '%3', a.SNMP_Group = '%4', a.SNMP_Version = '%5' WHERE a.Is_Default = 'yes'"
//插入资产个性化配置
#define SQL_INSERT_ASSET_SNMPCONFIG "INSERT INTO snmp_parameter_config VALUES('%1', '%2', '', '', '%3', '%4', '%5', 'no')"
//更新资产个性化配置
#define SQL_UPDATE_ASSET_SNMPCONFIG "UPDATE snmp_parameter_config a SET a.SNMP_Port_Number = '%1', a.SNMP_Group = '%2', a.SNMP_Version = '%3' WHERE a.Id = '%4'"
//删除资产个性化配置
#define SQL_DELETE_ASSET_SNMPCONFIG "DELETE FROM snmp_parameter_config WHERE Id = '%1'"

//<------------采集对象页面------------->
//查询资产当天的事件个数
#define SQL_QUERY_ASSET_EVENTNUM_DAY "SELECT COUNT(*) FROM formalized_log a WHERE a.Asset_Id = '%1' AND TO_DAYS(a.Alarm_Time) = TO_DAYS(NOW())"

//<------------范式化日志页面------------->
//查询所有的范式化日志个数
#define SQL_QUERY_ALL_EVENTNUM "SELECT COUNT(*) FROM formalized_log a WHERE a.Alarm_Time BETWEEN '%1' AND '%2' ORDER BY a.Alarm_Time DESC"
//查询所有的范式化日志
#define SQL_QUERY_ALL_FROMALIZEDLOG "SELECT a.*,b.Device_Name,b.Asset_Ip FROM formalized_log a,asset b WHERE a.Asset_Id = b.Id AND a.Alarm_Time BETWEEN '%1' AND '%2' ORDER BY a.Alarm_Time DESC LIMIT %3, %4"
//查询资产的范式化日志个数
#define SQL_QUERY_ASSET_EVENTNUM "SELECT COUNT(*) FROM formalized_log a WHERE a.Asset_Id = '%1' AND a.Alarm_Time BETWEEN '%2' AND '%3' ORDER BY a.Alarm_Time DESC"
//查询资产的范式化日志
#define SQL_QUERY_ASSET_FROMALIZEDLOG "SELECT a.*,b.Device_Name,b.Asset_Ip FROM formalized_log a,asset b WHERE a.Asset_Id = b.Id AND a.Asset_Id = '%1' AND a.Alarm_Time BETWEEN '%2' AND '%3' ORDER BY a.Alarm_Time DESC LIMIT %4, %5"

//<------------原始日志------------->
//查询原始日志条数,SELECT COUNT(*) FROM original_log a WHERE a.Collect_Time BETWEEN '%1' AND '%2' ORDER BY a.Collect_Time DESC
#define SQL_QUERY_ORIGINALLOG_NUM "SELECT COUNT(*) FROM original_log a,asset b WHERE a.Asset_Id = b.Id AND a.Collect_Time BETWEEN '%1' AND '%2' ORDER BY a.Collect_Time DESC"
//查询原始日志
#define SQL_QUERY_ORIGINAL_LOG "SELECT a.*,b.Device_Name,b.Asset_Ip,b.Device_Type,b.Device_Sub_Type FROM original_log a,asset b WHERE a.Asset_Id = b.Id AND a.Collect_Time BETWEEN '%1' AND '%2' ORDER BY a.Collect_Time DESC LIMIT %3, %4"

//<------------资产详情显示页面------------->
//查询一条资产信息
#define SQL_QUERY_ONE_ASEETINFO "SELECT * FROM asset a WHERE a.Id = '%1'"

//<------------事件解析字段------------->
//查询事件解析字段信息
#define SQL_QUERY_RESOLVE_FIELD "SELECT * FROM formalized_field"
//更新事件解析字段信息
#define SQL_UPDATE_RESOLVE_FIELD "UPDATE formalized_field a SET a.Field_Name = '%1', a.Field_Alias = '%2', a.Field_Type = '%3' WHERE a.Id = '%4'"
//插入事件解析字段信息
#define SQL_INSERT_RESOLVE_FIELD "INSERT INTO formalized_field VALUES('%1','%2','%3','%4')"
//删除事件解析字段信息
#define SQL_DELETE_RESOLVE_FIELD "DELETE FROM formalized_field WHERE Id = '%1'"

//<------------事件解析规则------------->
//查询事件解析规则信息
#define SQL_QUERY_RESOLVE_RULE "SELECT * FROM formalized_rule"
//更新事件解析规则信息
#define SQL_UPDATE_RESOLVE_RULE "UPDATE formalized_rule a SET a.Rule_Name='%1',a.Rule_Status='%2',a.Modify_Time='%3',a.Rule_Regex='%4',a.Log_Type='%5',a.Log_Sub_Type='%6',a.Rule_Content='%7',a.Remark='%8' WHERE a.Id = '%9'"
//插入事件解析规则信息
#define SQL_INSERT_RESOLVE_RULE "INSERT INTO formalized_rule VALUES('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11')"
//删除事件解析规则信息
#define SQL_DELETE_RESOLVE_RULE "DELETE FROM formalized_rule WHERE Id = '%1'"

//<------------事件解析------------->
//查询功能匹配事件类型
#define SQL_QUERY_ITEM_INFO "SELECT * FROM item"

//<------------事件展示------------->
//按日志类型，日志子类型查询日志事件('日志类型_日志子类型','日志类型_日志子类型')
#define SQL_QUERY_LOG_BYTYPE "SELECT a.*,b.Device_Name,b.Asset_Ip FROM formalized_log a,asset b WHERE a.Asset_Id = b.Id AND CONCAT_WS('_',Log_Type, Log_Sub_Type) in (%1) AND a.Alarm_Time BETWEEN '%2' AND '%3' ORDER BY Alarm_Time DESC"

//<------------逻辑拓扑关系------------->
//查询逻辑拓扑关系
#define SQL_QUERY_LOGICTOP_INFO "SELECT * FROM topology"


//<------------告警配置------------->
#define SQL_INSERT_ALARMCONFIG "insert into alarmconfig value('%1','%2','%3',%4)"
#define SQL_MODIFY_ALARMCONFIG "update alarmconfig set alarmName = '%1',alarmColor = '%2', alarmWindow=%3 where alarmLevel = '%4'"
#define SQL_DELETE_ALARMCONFIG "delete from alarmconfig where alarmLevel = '%1'"
#define SQL_QUERY_ALARMCONFIG "select * from alarmconfig"

//消息类型
/********************************************************
 *  MSG_START:程序启动
 *  MSG_DATABASE_START:数据库启动
 *  MSG_DATABASE_OK: 数据库启动成功
 *  MSG_SAVE_SYSLOG: 保存系统日志
 *  MSG_LOGIN_SUCCESS:登录成功
 *  MSG_QUERY_DATA:查询页面数据
 *  MSG_QUERY_RESULT:页面数据查询结果
 *  MSG_INSERT_UPDATE_DEL:插入数据库，或者更新数据库,删除
 *  MSG_SEND_REDIS:往redis发送数据
 *  MSG_RECV_REDIS:从redis接收数据
 *  MSG_WRITE_ACCESS:设置写权限
 *  MSG_RESTART_LOGIN:重新登录
 *  MSG_USER_EXIT_LOGIN:用户退出登录
 *  MSG_USER_EXIT_SYSTEM:用户退出系统
 *  MSG_SWITCH_PAGE:切换页面
 *  MSG_ASSETDETAIL_DISP:展示资产详情页面
 *  MSG_CLOSE_MYSQL_PORT:关闭mysql端口
 ********************************************************/
enum MsgDataType
{
    MSG_START, MSG_DATABASE_START, MSG_DATABASE_OK, MSG_SAVE_SYSLOG, MSG_LOGIN_SUCCESS, MSG_QUERY_DATA,
    MSG_QUERY_RESULT, MSG_INSERT_UPDATE_DEL, MSG_SEND_REDIS, MSG_RECV_REDIS, MSG_WRITE_ACCESS,
    MSG_RESTART_LOGIN, MSG_USER_EXIT_LOGIN, MSG_USER_EXIT_SYSTEM, MSG_SWITCH_PAGE, MSG_ASSETDETAIL_DISP, MSG_CLOSE_MYSQL_PORT
};

//系统配置
typedef struct Sys_Config_
{
    //redis
    QString redisHost; //redis的ip
    int redisPort; //redis端口
    QString redisPwd; //redis的密码

    //mysql
    QString sql_Ip; //Ip地址
    int sql_Port; //端口号
    QString sql_user; //用户名
    QString sql_Pwd; //密码
    QString sql_DbName; //数据库名称

    //sftp
    QString sftp_ip;
    QString sftp_user;
    QString sftp_pwd;
    quint16 sftp_port;
    int sftp_timeout;

    //启用Mysql端口
    int mysqlPortEnable;
}Sys_Config;


//自定义的配置
//qint64 pwdActiveTimeLimit; //密码有效时间(秒)
//int loginFailedLimit; //登录失败次数限制
//int userLockTimeLimit; //账户锁定时间(秒)
//int loginUsersLimit; //登录连接个数限制
//int loginNoOperTimeLimit; //登录超时时间，没有操作时间(秒)
//int deviceCpuLimit	//装置cpu使用率阈值
//int deviceMemLimit	//装置内存使用率阈值
//int deviceStorageLimit	//装置磁盘使用率阈值
typedef struct MyConf_Info_
{
    QString id; //唯一标识
    QString paramName; //参数名称
    QString paramDesc; //参数描述
    int defaultVal; //默认值
    QString defaultUnit; //默认值单位，天、时、分、秒
    int currentVal; //当前值
    QString currentUnit; //当前值单位
}MyConf_Info;

//登录地址限制
typedef struct Login_Addr_
{
    QString id; //唯一标识
    QString ipAddr; //ip地址
}Login_Addr;

//重要操作
typedef struct Important_Oper_
{
    QString id; //唯一标识
    QString funcId; //功能标识
    QString operDesc; //操作描述
    QString isImportant; //是否重要操作yes-是 no-不是
}Important_Oper;


//功能菜单
typedef struct MenuFunc_Info_
{
    QString id; //唯一标识
    QString parentId; //父节点ID
    QString funcId; //功能ID
    QString funcTitle; //功能标题
    QString funcType; //功能类型
    int orderNo; //功能序号

}MenuFunc_Info;

//角色信息
typedef struct Role_Info_
{
    QString id; //唯一标识
    QString roleName; //角色名称
    QString isSystem; //系统内置
    QString remark; //备注
    QString roleType; //角色类型
    QString userNames; //已分配的用户
}Role_Info;

//角色的功能
typedef struct RoleMenu_Info_
{
    QString roleId; //角色ID
    QString funcId; //功能ID
    bool isRead; //是否可读
    bool isWrite; //是否可写
}RoleMenu_Info;

//用户信息
typedef struct User_Info_
{
    QString userId; //用户ID
    QString roleName; //角色名称
    QString userName; //用户名称
    QString realName; //用户姓名
    QString isSystem; //系统内置
    QString isEnable; //是否启用
    QString remark; //备注
    QDateTime activeTime; //有效时间
    QString roleId; //角色ID
    QString password; //密码
    QString pwdConfirm; //密码确认
    int loginFailedNum; //登录失败次数
    QDateTime lockEndTime; //账户锁定结束时间
    QString loginStatus; //当前登录状态yes-已登录no-未登录
    QString roleType; //角色类型
}User_Info;

//审核事件信息
typedef struct Event_Info_
{
    QString id;//唯一标识
    QString task_Name; //任务名称
    QString send_User; //发起人ID
    QString desc; //描述
    QString end_User; //审核人
    QString start_Time; //发起时间
    QString end_Time; //审核时间
    QString is_End; //是否结束
    QString password; //密码
    QString send_Name; //发起人用户名
}Event_Info;

//网卡参数
typedef struct Netcard_Info_
{
    QString id; //唯一标识
    QString name; //接口名称
    QString ipAddr; //ip地址
    QString netmask; //子网掩码
    QString safeArea; //安全区域
    QString netFlag; //分网标识
}Netcard_Info;

//路由参数配置
typedef struct RouteConf_Info_
{
    QString id; //唯一标识
    QString segment; //目标网段
    QString netmask; //子网掩码
    QString nextHop; //下一跳
}RouteConf_Info;

//网卡子接口情况
typedef struct Netcard_Subnet_Info_
{
    QString mainId; //唯一标识
    QString netName; //网卡名称
    QString subNum; //子接口数量
    QString updateTime; //配置更新时间
}Netcard_Subnet_Info;

//子接口信息
typedef struct Subnet_Info_
{
    QString id; //唯一标识
    QString netName; //网卡名称
    QString subnetName; //子接口名称
    QString vlanId; //vlanid
    QString ipAddr; //ip地址
    QString netMask; //子网掩码
}Subnet_Info;

//系统日志
typedef struct SystemLog_Info_
{
    QString id; //唯一标识
    QString userName; //用户账户
    QString roleName; //用户类型
    QString logLevel; //日志等级
    QString logType; //日志类型
    QString logDetail; //日志详情
    QString logResult; //操作成功标志,yes-成功，no-失败
    QString failedCause; //失败原因
    QString logTime; //日志时间
    QString loginAddr; //登录地址
}SystemLog_Info;

//磁盘使用信息
typedef struct Storage_UsedInfo_
{
    QString id; //唯一标识
    QString desc; //描述
    int total; //总共的
    int used; //已使用
}Storage_UsedInfo;

//进程信息
typedef struct Process_RunInfo_
{
    QString id; //唯一标识
    QString processPid; //进程ID
    QString processName; //进程名称
    QString desc; //描述
    QString cpuUsed; //cpu使用率
    QString memUsed; //内存使用率
    QString startTime; //启动时间
    QString heartTime; //心跳时间
    QString runStatus; //运行状态
}Process_RunInfo;

//主站连接信息
typedef struct MainStation_Info_
{
     QString id; //唯一标识
     QString stationName; //主站名称
     QString ctrlChannelIp; //控制通道ip地址
     QString ctrlChannelPort; //控制通道端口
     QString ctrlChannelStatus; //控制通道状态0-已断开1-连接正常
     QString dataChannelIp; //数据通道ip地址
     QString dataChannelPort; //数据通道端口
     QString dataChannelName;//数据通道名称
     QString dataChannelStatus; //数据通道状态0-已断开1-连接正常
     int mainFlag; //0-主主站 1-附属主站
}MainStation_Info;

//装置注册信息
typedef struct DeviceRegister_Info_
{
    QString siteName; //站点名称
    QString partition; //安全分区
    QString deviceName; //装置名称
    QString factoryId; //厂家名称
    QString guid; //装置GUID
    QString delongedMajor; //归属单位
    QString busnessSystemId; //调度单位
    QString status; //注册状态
    QString time; //注册时间
}DeviceRegister_Info;

//资产信息
typedef struct Asset_Info_
{
    QString assetId; //资产唯一标识
    QString assetIp; //资产ip地址
    QString deviceName; //设备名称
    QString deviceType; //设备类型
    QString deviceSubType; //设备子类型
    QString typeDesc; //设备类型描述
    QString isOnLine; //在线状态up,down
    QString isKnown; //是否已知yes,no
    QString factory; //产商
    QString model; //型号
    QString createTime; //创建时间
    QString modifyTime; //修改时间
    QString eventTotalNum; //事件总数

    QString safePatition; //安全分区
    QString schedulPartition; //调度分区
    QString osVersion; //操作系统版本
    QString macAddr; //mac地址
    QString openServer; //开放服务
}Asset_Info;

//范式化规则文件
typedef struct FormalizedFile_Info_
{
    QString id; //唯一标识
    QString fileName; //文件名称
    QString fileVersion; //文件版本
    QString factory; //厂家
    QString model; //型号
    QString desc; //描述
    QString updateTime; //更新时间
}FormalizedFile_Info;

//关键文件清单
typedef struct KeyFile_Info_
{
    QString id; //唯一标识
    QString assetId; //资产唯一标识
    QString assetIp; //资产Ip地址
    QString assetType; //资产类型
    QString assetSubType; //资产子类型
    QString assetTypeDesc; //资产类型描述
    QString assetName; //资产名称
    QString path; //文件路径
}KeyFile_Info;

//icmp参数
typedef struct Icmp_Info_
{
    QString id; //唯一标识
    QString ipSegment; //ip子网
    QString netmask; //子网掩码
    QString vlanId; //vlan号
    QString safeArea; //安全分区
    QString subnet; //分网标识
    QString period; //采集周期
}Icmp_Info;

//snmp采集参数
typedef struct Snmp_Info_
{
    QString id; //唯一标识
    QString assetId; //资产唯一标识
    QString beginIpAddr; //开始ip地址
    QString endIpAddr; //结束ip地址
    QString snmpPort; //snmp端口
    QString snmpGroup; //snmp团体名
    QString snmpVersion; //snmp版本
    QString isDefault; //no-资产个性化值yes-默认值
}Snmp_Info;

//范式化日志
typedef struct Fromalized_LogInfo_
{
    QString id; //唯一标识
    QString assetId; //资产唯一标识
    QString assetIp; //资产IP地址
    QString assetName; //资产名称
    QString alarmTime; //告警时间
    QString alarmLevel; //告警等级
    QString assetType; //资产类型
    QString assetSubType; //资产子类型
    QString assetTypeDesc; //资产类型描述
    QString logType; //日志类型
    QString logSubType; //日志子类型
    QString logDetail; //日志详情
}Fromalized_LogInfo;

//原始日志
typedef struct Original_LogInfo_
{
    QString id; //唯一标识
    QString assetId; //资产唯一标识
    QString assetIp; //资产IP地址
    QString assetName; //资产名称
    QString assetType; //资产类型
    QString assetSubType; //资产子类型
    QString assetTypeDesc; //资产类型描述
    QString alarmTime; //告警时间
    QString logDetail; //日志详情
}Original_LogInfo;

//事件解析字段
typedef struct Resolve_FieldInfo_
{
    QString id; //唯一标识
    QString fieldName; //字段名称
    QString fieldAlias; //字段别名
    QString fieldType; //字段类型
}Resolve_FieldInfo;

//事件解析规则
typedef struct Resolve_RuleInfo_
{
    QString id; //唯一标识
    QString ruleName; //规则名称
    QString ruleGroup; //规则组
    QString ruleStatus; //规则状态
    QString createTime; //创建时间
    QString modifyTime; //修改时间
    QString ruleRegex; //规则正则表达式
    QString logType; //日志类型
    QString logSubType; //日志子类型
    QString ruleContent; //规则内容
    QString remark; //备注

}Resolve_RuleInfo;

//功能节点匹配事件类型
typedef struct FuncMatch_EventType_
{
    QString itemName; //节点名称
    QString funcId; //功能ID
    QString logType; //日志类型
    QStringList logSubType; //日志子类型

}FuncMatch_EventType;

//逻辑拓扑表信息
typedef struct LogicTop_Info_
{
    QString id; //唯一标识
    QString patition; //调度分区
    QString subnet; //IP子网
    QString vlanId; //vLan号
    QString srcDeviceId; //源设备唯一标识
    QString srcDeviceMac; //源设备mac
    QString srcDeviceIP; //源设备ip
    QString srcDevicestate; //源设备状态
    QString desDeviceId; //目的设备唯一标识
    QString desDeviceMac; //目的设备mac
    QString desDeviceIp; //目的设备ip
    QString desDeviceState; //目的设备状态

}LogicTop_Info;

//数据库执行语句
typedef struct SqlExec_Info_
{
    QString sqlStr; //数据库执行语句
    QString sqlDesc; //数据库语句描述
    QString sqlLogType; //日志类型
}SqlExec_Info;


//页面需要的数据
//<------------主配置页面------------->
typedef struct Data_Configurator_
{

}Data_Configurator;

//<------------用户登录页面------------->
typedef struct Data_UserLogin_
{
    QMap<QString, User_Info> allUsersMap; //所有的用户<用户唯一标识，用户信息>
    QMap<QString, QMap<QString, RoleMenu_Info> > roleMenuReadWriteMap; //角色功能菜单读写权限<角色ID, <功能ID，读写标志>>
}Data_UserLogin;

//<------------审核管理页面------------->
typedef struct Data_AuditManager_
{
    QList<Event_Info> allEventInfoList; //所有的事件信息
}Data_AuditManager;

//<------------角色配置页面------------->
typedef struct Data_RoleConfig_
{
    QMap<QString, Role_Info> allRoleInfoMap; //所有角色<角色唯一标识，角色信息>
    QMap<QString, QMap<QString, RoleMenu_Info> > roleMenuReadWriteMap; //角色功能菜单读写权限<角色ID, <功能ID，读写标志>>
}Data_RoleConfig;

//<------------用户配置页面------------->
typedef struct Data_UserConfig_
{
    QMap<QString, User_Info> allUsersMap; //所有的用户<用户唯一标识，用户信息>
    QMap<QString, Role_Info> allRoleInfoMap;  //所有角色<角色唯一标识，角色信息>

}Data_UserConfig;

//<------------网卡配置页面------------->
typedef struct Data_NetworkParamSet_
{
    QMap<QString, Netcard_Info> allNetcardInfoMap; //所有的网卡信息<网卡信息唯一标识，网卡信息>
}Data_NetworkParamSet;

//<------------路由配置页面------------->
typedef struct Data_RouteParamSet_
{
    QMap<QString, RouteConf_Info> allRouteInfoMap; //所有的路由信息<路由信息唯一标识，路由信息>
}Data_RouteParamSet;

//<------------子接口参数配置页面------------->
typedef struct Data_SubnetParamSet_
{
    QList<Netcard_Subnet_Info> netMainInfoList; //网卡子接口情况
    QMultiMap<QString, Subnet_Info> subnetInfoMap; //所有的子接口参数<网卡名称，子接口信息>
}Data_SubnetParamSet;

//<------------常用系统配置页面------------->
typedef struct  Data_CommonConfig_
{
    QMap<QString, MyConf_Info> myConfInfoMap; //系统自定义配置信息<参数名称，参数信息>
    QMap<QString, Login_Addr> loginAddrMap; //登录地址限制表<唯一标识，信息>
    QMap<QString, Important_Oper> importantOperMap; //重要操作表<唯一标识，信息>
}Data_CommonConfig;

//<------------系统日志页面------------->
typedef struct  Data_SystemLog_
{
    QMap<QString, User_Info> allUsersMap; //所有的用户<用户唯一标识，用户信息>
    QMap<QString, Role_Info> allRoleInfoMap;  //所有角色<角色唯一标识，角色信息>
    QList<SystemLog_Info> systemLogInfoList; //系统日志
    int logCount; //日志条数
}Data_SystemLog;

//<------------装置自检状态页面------------->
typedef struct  Data_DeviceCheck_
{
    double cpuUsed; //cpu使用
    double memUsed; //内存使用
    double diskUsed; //磁盘使用率
    QList<Storage_UsedInfo> storageList; //磁盘使用信息
    QList<Process_RunInfo> processInfoList; //进程信息
}Data_DeviceCheck;

//<------------数据通道状态页面------------->
typedef struct  Data_DataChannelStatus_
{
    QList<MainStation_Info> mainStationInfoList;
}Data_DataChannelStatus;

//<------------主站参数配置页面------------->
typedef struct  Data_MainStationParam_
{
    QList<MainStation_Info> mainStationInfoList;
}Data_MainStationParam;

//<------------装置注册页面------------->
typedef struct  Data_DeviceRegister_
{
    DeviceRegister_Info deviceRegisterInfo;
}Data_DeviceRegister;

//<------------资产在线页面------------->
typedef struct  Data_AssetOnlineStatus_
{
    QMap<QString, Asset_Info> assetInfoMap; //<唯一标识，资产信息>
}Data_AssetOnlineStatus;

//<------------范式化规则文件页面------------->
typedef struct  Data_FormalizedRuleFile_
{
    QList<FormalizedFile_Info> fileInfoList; //范式化规则文件
}Data_FormalizedRuleFile;

//<------------关键文件清单页面------------->
typedef struct  Data_KeyFile_
{
    QList<KeyFile_Info> fileInfoList; //关键文件清单
}Data_KeyFile;

//<------------ICMP参数页面------------->
typedef struct  Data_IcmpParam_
{
    QList<Icmp_Info> icmpInfoList; //icmp配置信息
}Data_IcmpParam;

//<------------SNMP参数设置页面------------->
typedef struct  Data_SnmpParamSet_
{
    QMap<QString, Asset_Info> assetInfoMap; //<唯一标识，资产信息>
    QList<Snmp_Info> snmpInfoList; //SNMP参数配置信息
}Data_SnmpParamSet;

//<------------采集对象页面------------->
typedef struct  Data_CollectObject_
{
    QMap<QString, Asset_Info> assetInfoMap; //<唯一标识，资产信息>
}Data_CollectObject;

//<------------范式化日志页面------------->
typedef struct  Data_FormalizedLog_
{
    QList<Fromalized_LogInfo> logInfoList; //范式化日志
    int logCount; //日志条数
}Data_FormalizedLog;


//<------------原始日志页面------------->
typedef struct  Data_OriginalLogDisp_
{
    QList<Original_LogInfo> logInfoList; //原始日志
    int logCount; //日志条数
}Data_OriginalLogDisp;

//<------------资产详情展示页面------------->
typedef struct  Data_AssetDetailDialog_
{
    Asset_Info assetInfo;
    QString cpuUsed; //cpu使用率
    QString memUsed; //内存使用率
    QString storageUsed; //磁盘使用率
}Data_AssetDetailDialog;

//<------------事件解析字段页面------------->
typedef struct  Data_EventResolveField_
{
    QMap<QString, Resolve_FieldInfo> fieldInfoMap; //事件解析字段
}Data_EventResolveField;

//<------------事件解析规则页面------------->
typedef struct  Data_EventResolveRule_
{
    QMap<QString, Resolve_FieldInfo> fieldInfoMap; //事件解析字段
    QMap<QString, Resolve_RuleInfo> ruleInfoMap; //事件解析规则
}Data_EventResolveRule;

//事件分类信息
typedef struct Data_EventDistribute_
{
    QMap<QString, Resolve_FieldInfo> fieldInfoMap; //事件解析字段
    QMap<QString, Resolve_RuleInfo> ruleInfoMap; //事件解析规则
    QMap<QString, FuncMatch_EventType> funcMatchEventMap; //功能匹配事件类型
}Data_EventDistribute;

//<------------事件分类展示页面------------->
typedef struct Data_EventDisplay_
{
    QList<Fromalized_LogInfo> logInfoList; //范式化日志
}Data_EventDisplay;

//<------------逻辑拓扑展示页面------------->
typedef struct Data_LogicTopTable_
{
    QList<LogicTop_Info> logicTopInfoList; //逻辑拓扑信息
}Data_LogicTopTable;

//<------------开放服务页面------------->
typedef struct  Data_OpenServer_
{
    QMap<QString, Asset_Info> assetInfoMap; //<唯一标识，资产信息>
}Data_OpenServer;

//消息体
typedef struct Msg_Data_
{
    MsgDataType type; //消息类型
    bool flag;
    Sys_Config config; //系统配置

    //数据库查询结果
    QMultiMap<QString, MenuFunc_Info> allMenuFuncInfoMap; //所有的功能菜单<父节点唯一标识，功能>
    QString funcId; //功能ID
    //日志查询条件
    QString startTime; //日志查询开始时间
    QString endTime; //日志查询结束时间
    int startIndex; //起始序号

    //<------------主配置页面------------->
    Data_Configurator data_Configurator;
    //<------------用户登录页面------------->
    Data_UserLogin data_UserLogin;
    //<------------审核管理页面------------->
    Data_AuditManager data_AuditManager;
    //<------------角色配置页面------------->
    Data_RoleConfig data_RoleConfig;
    //<------------用户配置页面------------->
    Data_UserConfig data_UserConfig;
    //<------------网卡配置页面------------->
    Data_NetworkParamSet data_NetworkParamSet;
    //<------------路由配置页面------------->
    Data_RouteParamSet data_RouteParamSet;
    //<------------子接口参数配置页面------------->
    Data_SubnetParamSet data_SubnetParamSet;
    //<------------常用系统配置页面------------->
    Data_CommonConfig data_CommonConfig;
    //<------------系统日志页面------------->
    Data_SystemLog data_SystemLog;
    //<------------装置自检状态页面------------->
    Data_DeviceCheck data_DeviceCheck;
    //<------------数据通道状态页面------------->
    Data_DataChannelStatus data_DataChannelStatus;
    //<------------主站参数配置页面------------->
    Data_MainStationParam data_MainStationParam;
    //<------------装置注册页面------------->
    Data_DeviceRegister data_DeviceRegister;
    //<------------资产在线页面------------->
    Data_AssetOnlineStatus data_AssetOnlineStatus;
    //<------------范式化规则文件页面------------->
    Data_FormalizedRuleFile data_FormalizedRuleFile;
    //<------------关键文件清单页面------------->
    Data_KeyFile data_KeyFile;
    //<------------ICMP参数页面------------->
    Data_IcmpParam data_IcmpParam;
    //<------------SNMP参数设置页面------------->
    Data_SnmpParamSet data_SnmpParamSet;
    //<------------采集对象页面------------->
    Data_CollectObject data_CollectObject;
    //<------------范式化日志页面------------->
    Data_FormalizedLog data_FormalizedLog;
    //<------------原始日志页面------------->
    Data_OriginalLogDisp data_OriginalLogDisp;

    //<------------资产详情展示页面------------->
    Data_AssetDetailDialog data_AssetDetailDialog;

    //<------------事件解析字段页面------------->
    Data_EventResolveField data_EventResolveField;
    //<------------事件解析规则页面------------->
    Data_EventResolveRule data_EventResolveRule;

    //事件分类信息
    Data_EventDistribute data_EventDistribute;

    //<------------事件分类展示页面------------->
    Data_EventDisplay data_EventDisplay;
    //<------------逻辑拓扑展示页面------------->
    Data_LogicTopTable data_LogicTopTable;
    //<------------开放服务页面------------->
    Data_OpenServer data_OpenServer;

    //数据库增，删，改，执行语句
    QList<SqlExec_Info> sqlExecList;

    //protobuf消息体,发送到redis
    QString sendChannel; //发送频道

    //切换页面
    QString pageFuncId; //页面功能ID
    QString assetId; //资产ID

}Msg_Data;




//LiyG
typedef struct
{
    QString alarmLevel;
    QString alarmName;
    QString alarmColor;
    int alarmWindow;
}stAlarmConfig;





#endif // COMMONOBJECT_H
