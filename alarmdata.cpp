#include "alarmdata.h"

AlarmData* AlarmData::alarmData = NULL;

AlarmData::AlarmData(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<stAlarmData>("stAlarmData");

}



void AlarmData::insertSmartAlarmData(stAlarmData alarmData)
{
    if(mapSmartAlarmData_.size() > SingletonConfig->getMaxSize())
    {
        mapSmartAlarmData_.erase(mapSmartAlarmData_.begin());      //如果超过设置大小，删除最早的告警
    }
    mapSmartAlarmData_[alarmData.id_] =  alarmData;

    emit receiverSmartAlarmData(alarmData.id_,alarmData);


    AlarmReportParse alarmReportParse;
    stAlarmReport alarmReport = alarmReportParse.loadReportFile(alarmData.filename_);

    if(alarmReport.scene_ == QString::fromUtf8("保护动作"))
    {
        emit receiveSingleBayProtectAction(alarmReport);
    }
    else if(alarmReport.scene_ == QString::fromUtf8("一次设备异常"))
    {
        emit receivePrimaryEquipmentAbnormal(alarmReport);
    }
    else if(alarmReport.scene_ == QString::fromUtf8("二次设备异常"))
    {
        emit receiveSecondaryEquipmentAbnormal(alarmReport);
    }
    else if(alarmReport.scene_ == QString::fromUtf8("遥信状态不一致监视"))
    {
        emit receiveRemoteSignalling(alarmReport);
    }
    else if(alarmReport.scene_ == QString::fromUtf8("开关分合闸状态检验"))
    {
        emit receiveSwitchStatusCheck(alarmReport);
    }
    else if(alarmReport.scene_ == QString::fromUtf8("开关分合动作时间检验"))
    {
        emit receiveSwitchActionTimeCheck(alarmReport);
    }
    else if(alarmReport.scene_ == QString::fromUtf8("电源系统异常"))
    {
        emit receivePowerSystemAbnormal(alarmReport);
    }
    else if(alarmReport.scene_ == QString::fromUtf8("电气运行环境监测"))
    {
        emit receiveElectricRunCheck(alarmReport);
    }
    else if(alarmReport.scene_ == QString::fromUtf8("辅助电源状态监测"))
    {
        emit receiveAuxiliaryPowerCheck(alarmReport);
    }
    else if(alarmReport.scene_ == QString::fromUtf8("全站时间同步状态监测"))
    {
        emit receiveStationTimeSyncAbnormol(alarmReport);
    }
    else if(alarmReport.scene_ == QString::fromUtf8("装置通信中断"))
    {
        emit receiveDeviceCommInterrupt(alarmReport);
    }else if(alarmReport.scene_ == QString::fromUtf8("SOE时标跳变"))
    {
        emit receiveSoeTimestampChange(alarmReport);
    }else if(alarmReport.scene_ == QString::fromUtf8("遥测跳变"))
    {
        emit receiveRemoteMeasuring(alarmReport);
    }else if(alarmReport.scene_ == QString::fromUtf8("遥控过程分析"))
    {
        emit receiveRemoteControl(alarmReport);
    }
    else
    {
        qDebug()<<"Sence"<<alarmReport.scene_;
    }
}

void AlarmData::insertOriginalAlarmData(stAlarmData alarmData)
{
    if(mapOriginalAlarmData_.size() > SingletonConfig->getMaxSize())
    {
        mapOriginalAlarmData_.erase(mapOriginalAlarmData_.begin());          //如果超过设置大小，删除最早的告警
    }
    mapOriginalAlarmData_[alarmData.id_] =  alarmData;

    emit receiverOriginalAlarmData(alarmData.id_,alarmData);
}


stAlarmData AlarmData::getSmartAlarmDataByID(int id)
{
    stAlarmData alarmData = mapSmartAlarmData_.value(id);
    return alarmData;
}

stAlarmData AlarmData::getOriginalAlarmDataByID(int id)
{
    stAlarmData alarmData = mapOriginalAlarmData_.value(id);
    return alarmData;
}


QMap<int, stAlarmData> AlarmData::getSmartAlarmData() const
{
    return mapSmartAlarmData_;
}

QMap<int, stAlarmData> AlarmData::getOriginalAlarmData() const
{
    return mapOriginalAlarmData_;
}


//void AlarmData::modifySmartAlarmStatus(int id)
//{
//    if(mapSmartAlarmData_.count(id))
//    {
//        mapSmartAlarmData_[id].confirmed_ = 1;
//        qDebug()<<QString::fromLocal8Bit("用户:%1 确认智能告警ID:%2").arg(SingletonConfig->getCurUser()).arg(id);
//    }
//}


//void AlarmData::modifyOriginalAlarmStatus(int id)
//{
//    if(mapOriginalAlarmData_.count(id))
//    {
//        mapOriginalAlarmData_[id].confirmed_ = 1;
//        qDebug()<<QString::fromLocal8Bit("用户:%1 确认原始告警ID:%2").arg(SingletonConfig->getCurUser()).arg(id);
//    }
//}


bool AlarmData::removeSmartAlarmByID(int id)
{
    if(mapSmartAlarmData_.count(id))
    {
        mapSmartAlarmData_.remove(id);
        return true;
    }
    else
    {
        return false;
    }
}

bool AlarmData::removeOriginalAlarmByID(int id)
{
    if(mapOriginalAlarmData_.count(id))
    {
        mapOriginalAlarmData_.remove(id);
        return true;
    }
    else
    {
        return false;
    }
}

QString AlarmData::getSmartAlarmFileNameByID(int id)
{
    return mapSmartAlarmData_[id].filename_;
}

AlarmData *AlarmData::GetInstance()
{
    if(alarmData == NULL)
    {
        alarmData = new AlarmData();
    }
    return alarmData;
}
