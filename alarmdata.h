#ifndef ALARMDATA_H
#define ALARMDATA_H


#include "alarmreportparse.h"
#include "confighelper.h"

#include <QObject>
#include <QMap>
#include <QMutexLocker>

#define SingletonAlarmData AlarmData::GetInstance()

typedef struct
{
     int id_;          //智能告警ID带有字符
     int level_;
     QString time_;
     QString filename_;
     QString text_;
     int confirmed_;
}stAlarmData;

enum AlarmLevel{
    LEVEL_EVENT = 0,
    LEVEL_ABNORMAL,
    LEVEL_OUTOFLIMIT,
    LEVEL_CHANGE,
    LEVEL_INFORM,
    LEVEL_REALTIME
};

class AlarmData : public QObject
{
    Q_OBJECT
public:
    static AlarmData *GetInstance();

    void insertSmartAlarmData(stAlarmData);
    void insertOriginalAlarmData(stAlarmData);

    stAlarmData getSmartAlarmDataByID(int);
    stAlarmData getOriginalAlarmDataByID(int);

    QMap<int, stAlarmData> getSmartAlarmData() const;
    QMap<int, stAlarmData> getOriginalAlarmData() const;

    void modifySmartAlarmStatus(int);
    void modifyOriginalAlarmStatus(int);

    bool removeSmartAlarmByID(int);
    bool removeOriginalAlarmByID(int);

    QString getSmartAlarmFileNameByID(int);          //通过智能告警ID获取告警文件


signals:
    void receiverSmartAlarmData(int,stAlarmData);
    void receiverOriginalAlarmData(int,stAlarmData);

signals:
    void receiveSwitchStatusCheck(stAlarmReport);
    void receiveSwitchActionTimeCheck(stAlarmReport);
    void receiveSingleBayProtectAction(stAlarmReport);
    void receiveRemoteSignalling(stAlarmReport);
    void receivePrimaryEquipmentAbnormal(stAlarmReport);
    void receiveSecondaryEquipmentAbnormal(stAlarmReport);
    void receivePowerSystemAbnormal(stAlarmReport);
    void receiveElectricRunCheck(stAlarmReport);
    void receiveAuxiliaryPowerCheck(stAlarmReport);
    void receiveStationTimeSyncAbnormol(stAlarmReport);
    void receiveDeviceCommInterrupt(stAlarmReport);
    void receiveSoeTimestampChange(stAlarmReport);
    void receiveRemoteMeasuring(stAlarmReport);
    void receiveRemoteControl(stAlarmReport);
private:
    explicit AlarmData(QObject *parent = 0);
    static AlarmData* alarmData;

    QMap<int, stAlarmData> mapSmartAlarmData_;
    QMap<int, stAlarmData> mapOriginalAlarmData_;

public slots:

};

#endif // ALARMDATA_H
