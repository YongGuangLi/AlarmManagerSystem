#ifndef RELATEDREPORT_H
#define RELATEDREPORT_H

#include "confighelper.h"

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QDomDocument>
#include <QStringList>



typedef struct
{
    int level_;
    QString time_;
    QString substation_;
    QString bay_;
    QString conclusion_;
    QString filename_;
    QString text_;
    QString scene_;
    QString direction_;
    QString srcDevice_;
    QString dstDevice_;
    QString validTime_;
    QStringList listPointTime_;
    QStringList listPointType_;
    QStringList listPointName_;
    QStringList listPointValue_;
    QStringList listPointSrcIp_;
    QStringList listPointdstIp_;
    QList<int> listCmdType_;
    QList<int> listCtrlResult_;
}stAlarmReport;


class AlarmReportParse
{
public:
    AlarmReportParse();
    stAlarmReport loadReportFile(QString);
    void parseLogicItem(QDomElement);
    void parseInfoItem(QDomElement);
    void parseSoeItem(QDomElement);
    void parsePcapFile(QDomElement);

    QString getFile();
    QString getAlarmConclusion() const;
    QString getCode() const;

    QString getScene() const;
    QString getCause() const;
    QString getAdvice() const;
    QStringList getLogicItems() const;
    QStringList getInfoItems() const;
    QStringList getSoeItems() const;
    QStringList getPcapFile() const;

private:
    QString filename_;
    QString alarmConclusion_;  //"2018-09-11 10:59:37.742 广场变 #1曲折变 非电量告警"
    QStringList listLogicItems_;
    QStringList listInfoItems_;
    QStringList listSoeItems_;
    QStringList listPcapFile_;

    QString cause_;
    QString advice_;
    QString code_;
    QString scene_;
    QString  direction_;
    int level_;

    stAlarmReport alarmReport;
};

#endif // RELATEDREPORT_H
