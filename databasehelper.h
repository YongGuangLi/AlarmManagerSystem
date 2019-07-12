#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include "confighelper.h"
#include "alarmdata.h"
#include "RtdbMessage.pb.h"
#include "CommonObject.h"
#include "datamodel.h"
#include <QObject>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <QString>

#define SingletonDBHelper DataBaseHelper::GetInstance()

class DataBaseHelper : public QObject
{
    Q_OBJECT
public:
    static DataBaseHelper *GetInstance();

    bool open(QString ip,int port, QString dbName, QString user, QString passwd);
    bool isOpen();

    bool execSqlStr(QString sqlStr);

    //获取所有用户信息
    void readAllUserInfo(QMap<QString, User_Info> &allUsersMap);

    void readAllRoleInfo(QMap<QString, Role_Info> &allRoleInfoMap);

    void queryUserConfigInfo(Data_UserConfig &data_UserConfig);

    void readRoleMenuReadWrite(QMap<QString, QMap<QString, RoleMenu_Info> > &roleMenuReadWriteMap);

    void queryRoleConfigInfo(Data_RoleConfig &data_RoleConfig);

    void queryMenuFunc(QMultiMap<QString, MenuFunc_Info> &allMenuFuncInfoMap);

    void queryUserLoginInfo(Data_UserLogin &data_UserLogin);

    QMap<QString, stAlarmConfig>  queryAlarmConfig();

    //读取智能告警数据
    void readIaAlarmDataFromDB();
    //读取原始告警数据
    void readOaAlarmDataFromDB();


    //查询历史告警
    void queryHistoryData(QString beginTime, QString endTime, int alarmLevel, int alarmType);   //alarmType 0:智能告警   1:原始告警

    //查询统计信息
    int querySmartDataCnt(QString beginTime, QString endTime, int alarmLevel);
    int queryOriginalDataCnt(QString beginTime, QString endTime, int alarmLevel);

    QString getError();

    bool modifyIaAlarmStatus(int id);
    bool modifyOaAlarmStatus(QString alarmTime, int id);

protected:
    void timerEvent(QTimerEvent *event);
private:

    QDateTime queryCurrentDateTime();
signals:
    void sendAlarmData(int, stAlarmData);

private:
    explicit DataBaseHelper(QObject *parent = 0);
    static DataBaseHelper * dbHelp_;

    QSqlDatabase sqlDatabase;
    QString ip_;
    int port_;
    QString dbName_;
    QString user_;
    QString passwd_;

    QTimer *timer;
signals:

public slots:
};

#endif // DATABASEHELPER_H
