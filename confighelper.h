#ifndef ConfigHelper_H
#define ConfigHelper_H

#include <QObject>
#include <QSettings>
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QColor>
#include <QColorDialog>
#include <QToolButton>
#include <QDomDocument>
#include <QMessageBox>


#define SingletonConfig ConfigHelper::GetInstance()

class ConfigHelper : public QObject
{
    Q_OBJECT
public:
    static ConfigHelper *GetInstance();

    //读取Config.ini
    bool initConfigHelper(QString);

    //读取ActionClassifier.xml
    bool initActionClassifier(QString);
    void parseActions(QDomElement);
    void parseItem(QDomElement);

    float getActionClassifier(QString);

    //读取IABaySignal.xml
    bool initIABaySignal(QString);
    void parseStation(QDomElement);
    void parseVoltage(QDomElement);
    void parseVoltageItem(QDomElement);
    void parseDevice(QDomElement);
    void parseDots(QDomElement);


    //读取IAFRule.xml
    bool initIAFRule(QString);
    void parseIAFRuleItem(QDomElement);

    //读取IASRule.xml
    bool initIASRule(QString);
    void parseIASRuleItem(QDomElement);


    //mysql数据
    QString getIpMySql() const;
    int getPortMySql() const;
    QString getDbName() const;
    QString getUserMysql() const;
    QString getPasswdMysql() const;


    void setIpMySql(const QString &ipMySql);
    void setPortMySql(int portMySql);
    void setUserMySql(const QString &userMySql);
    void setPasswdMySql(const QString &passwdMySql);

    //redis数据
    QString getIpRedis() const;
    int getPortRedis() const;
    QString getPasswdRedis() const;

    void setIpRedis(const QString &ipRedis);
    void setPortRedis(int value);
    void setPasswdRedis(const QString &passwdRedis);

    void databaseConfigWriteToFile();


    QString getStation() const;

    //告警报告路径
    QString getAlarmReportPath() const;
    void setAlarmReportPath(const QString &alarmReportPath);

    QString getHistoryAlarmExportPath() const;
    void setHistoryAlarmExportPath(const QString &historyAlarmExportPath);

    //表格显示最大行数
    int getMaxSize() const;

    //告警等级颜色
    QColor getAlarmLevelColor(int level);
    void setAlarmLevelColor(int level, QColor color);
    void writeColorToFile();

    //间隔过滤
    void getBay(QString);
    QStringList getListBay();

    //告警等级弹窗
    int getAlarmLevelFlag(int level);
    void setAlarmLevelFlag(int level, int flag);


    QString getLogicByCode(QString);
private:
    explicit ConfigHelper(QObject *parent = 0);
    ~ConfigHelper();

    static ConfigHelper* configHelper;

    QString ipMySql_;
    int portMySql_;
    QString userMySql_;
    QString passwdMySql_;
    QString dbName_;

    QString ipRedis_;
    int portRedis;
    QString passwdRedis_;

    QString alarmReportPath_;          //告警简报路径

    QString historyAlarmExportPath_;   //历史告警导出路径

    int iMaxSize_;            //显示最大告警个数

    QString station_;

    QStringList listBay_;

    QMap<int,QColor> mapAlarmLevelColor_;      //告警等级对应的颜色  key:告警等级   value:颜色
    QMap<int, int> mapAlarmLevelFlag_;         //告警框自动弹出       key:告警等级   value:0关  1开


    QMap<QString, int> mapActionClassifier_;
    QSettings *settings_;

    QStringList listEventSignal_;

    QMap<QString, QString> mapCodeToLogic_;
};

#endif // ConfigHelper_H
