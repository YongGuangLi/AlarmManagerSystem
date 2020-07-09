#include "confighelper.h"


ConfigHelper* ConfigHelper::configHelper = NULL;

ConfigHelper *ConfigHelper::GetInstance()
{
    if(configHelper == NULL)
    {
        configHelper = new ConfigHelper();
    }
    return configHelper;
}

QString ConfigHelper::channel()
{
    return qApp->applicationName();
}


ConfigHelper::ConfigHelper(QObject *parent) : QObject(parent)
{
}

ConfigHelper::~ConfigHelper()
{
    settings_->deleteLater();
}

QString ConfigHelper::getAlarmReportPath() const
{
    return alarmReportPath_;
}

void ConfigHelper::setAlarmReportPath(const QString &alarmReportPath)
{
    alarmReportPath_ = alarmReportPath;
}

QString ConfigHelper::getHistoryAlarmExportPath() const
{
    return historyAlarmExportPath_;
}

void ConfigHelper::setHistoryAlarmExportPath(const QString &historyAlarmExportPath)
{
    historyAlarmExportPath_ = historyAlarmExportPath;
}

QString ConfigHelper::getPasswdRedis() const
{
    return passwdRedis_;
}

void ConfigHelper::setPasswdRedis(const QString &passwdRedis)
{
    passwdRedis_ = passwdRedis;
}

void ConfigHelper::databaseConfigWriteToFile()
{
    settings_->beginGroup("MYSQL");
    settings_->setValue("ip", ipMySql_);
    settings_->setValue("port", portMySql_);
    settings_->setValue("user", userMySql_);
    settings_->setValue("passwd", passwdMySql_);
    settings_->endGroup();

    settings_->beginGroup("REDIS");
    settings_->setValue("ip", ipRedis_);
    settings_->setValue("port", portRedis);
    settings_->setValue("passwd", passwdRedis_);
    settings_->endGroup();

    settings_->sync();
}

void ConfigHelper::setPortRedis(int value)
{
    portRedis = value;
}

void ConfigHelper::setIpRedis(const QString &ipRedis)
{
    ipRedis_ = ipRedis;
}

void ConfigHelper::setPasswdMySql(const QString &passwdMySql)
{
    passwdMySql_ = passwdMySql;
}

void ConfigHelper::setUserMySql(const QString &userMySql)
{
    userMySql_ = userMySql;
}

void ConfigHelper::setPortMySql(int portMySql)
{
    portMySql_ = portMySql;
}

void ConfigHelper::setIpMySql(const QString &ipMySql)
{
    ipMySql_ = ipMySql;
}


bool ConfigHelper::initConfigHelper(QString filepath)
{
    if(!QFile::exists(filepath))
        qDebug()<<filepath + " not exist";

    settings_ = new QSettings(filepath,QSettings::IniFormat);

    settings_->beginGroup("MYSQL");
    ipMySql_ =  settings_->value("ip").toString();
    portMySql_ =  settings_->value("port").toInt();
    userMySql_ =  settings_->value("user").toString();
    passwdMySql_ =  settings_->value("passwd").toString();
    dbName_ = settings_->value("dbName").toString();
    settings_->endGroup();

    settings_->beginGroup("REDIS");
    ipRedis_ = settings_->value("ip").toString();
    portRedis = settings_->value("port").toInt();
    passwdRedis_ = settings_->value("passwd").toString();
    settings_->endGroup();

    settings_->beginGroup("PATH");
    alarmReportPath_ = QString::fromUtf8(settings_->value("IASReport").toString().toLatin1());
    settings_->endGroup();

    settings_->beginGroup("SIZE");
    iMaxSize_ = settings_->value("max").toInt();
    settings_->endGroup();

    settings_->beginGroup("COLOR");
    QStringList allAlarmLevelColor = settings_->allKeys();
    for(int i = 0; i < allAlarmLevelColor.size(); ++i)
    {
        QString colorName = settings_->value(allAlarmLevelColor.at(i)).toString();
        mapAlarmLevelColor_.insert(allAlarmLevelColor.at(i).toInt(), QColor(colorName));
    }
    settings_->endGroup();

    settings_->beginGroup("FLAG");
    QStringList allAlarmLevelFlag = settings_->allKeys();
    for(int i = 0; i < allAlarmLevelFlag.size(); ++i)
    {
        int flag = settings_->value(allAlarmLevelFlag.at(i)).toInt();
        mapAlarmLevelFlag_.insert(allAlarmLevelFlag.at(i).toInt(), flag);
    }
    settings_->endGroup();

    settings_->beginGroup("STATION");
    station_ = QString::fromUtf8(settings_->value("station").toString().toLatin1());
    settings_->endGroup();

    return true;
}

bool ConfigHelper::initActionClassifier(QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<file.fileName() << " open failed";
    }
    QDomDocument doc("myDocument");
    if (!doc.setContent(&file))
    {
        qDebug()<<file.fileName()<<" setContent failed";
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNode actions = docElem.firstChild();
    while(!actions.isNull())
    {
        QDomElement element = actions.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("actions"))
            {
                parseActions(element);
            }
        }
        actions = actions.nextSibling();
    }
    return true;
}


void ConfigHelper::parseActions(QDomElement elem)
{
    QDomNode action = elem.firstChild();
    while(!action.isNull())
    {
        QDomElement element = action.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("action"))
            {
                parseItem(element);
            }
        }
        action = action.nextSibling();
    }
}

void ConfigHelper::parseItem(QDomElement elem)
{
    QDomNode item = elem.firstChild();
    while(!item.isNull())
    {
        QDomElement element = item.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("item"))
            {
                mapActionClassifier_[element.attribute("desc")] = element.attribute("value").toInt();
            }
        }
        item = item.nextSibling();
    }
}

float ConfigHelper::getActionClassifier(QString action)
{
    if(mapActionClassifier_.count(action))
    {
         return mapActionClassifier_[action];
    }
    else
    {
        qDebug()<<action;
        return action.toFloat();
    }
}

bool ConfigHelper::initIABaySignal(QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<file.fileName() << " open failed";
    }
    QDomDocument doc("myDocument");
    if (!doc.setContent(&file))
    {
        qDebug()<<file.fileName()<<" setContent failed";
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNode stations = docElem.firstChild();
    while(!stations.isNull())
    {
        QDomElement element = stations.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("station"))
            {
                parseStation(element);
            }
        }
        stations = stations.nextSibling();
    }
    return true;
}

void ConfigHelper::parseStation(QDomElement elem)
{
    QDomNode voltage = elem.firstChild();
    while(!voltage.isNull())
    {
        QDomElement element = voltage.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("voltage"))
            {
                parseVoltage(element);
            }
        }
        voltage = voltage.nextSibling();
    }
}

void ConfigHelper::parseVoltage(QDomElement elem)
{
    QDomNode item = elem.firstChild();
    while(!item.isNull())
    {
        QDomElement element = item.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("item"))
            {
                parseVoltageItem(element);

                if(element.hasAttribute("name"))
                {
                    QString bayName = element.attribute("name");
                    SingletonConfig->getBay(bayName);
                }
            }
        }
        item = item.nextSibling();
    }
}

void ConfigHelper::parseVoltageItem(QDomElement elem)
{
    QDomNode device = elem.firstChild();
    while(!device.isNull())
    {
        QDomElement element = device.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("device"))
            {
                parseDevice(element);
            }
        }
        device = device.nextSibling();
    }
}

void ConfigHelper::parseDevice(QDomElement elem)
{
    QDomNode dots = elem.firstChild();
    while(!dots.isNull())
    {
        QDomElement element = dots.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("dots"))
            {
                parseDots(element);
            }
        }
        dots = dots.nextSibling();
    }
}

void ConfigHelper::parseDots(QDomElement elem)
{
    QDomNode dot = elem.firstChild();
    while(!dot.isNull())
    {
        QDomElement element = dot.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("dot"))
            {
                if(element.attribute("priority") == "0")
                    listEventSignal_.push_back(element.attribute("redisAddress"));
            }
        }
        dot = dot.nextSibling();
    }
}

bool ConfigHelper::initIAFRule(QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<file.fileName() << " open failed";
    }
    QDomDocument doc("myDocument");
    if (!doc.setContent(&file))
    {
        qDebug()<<file.fileName()<<" setContent failed";
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNode IAFRule = docElem.firstChild();
    while(!IAFRule.isNull())
    {
        QDomElement element = IAFRule.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("item"))
            {
                parseIAFRuleItem(element);
            }
        }
        IAFRule = IAFRule.nextSibling();
    }
    return true;
}

void ConfigHelper::parseIAFRuleItem(QDomElement elem)
{
    QDomNode logic = elem.firstChild();
    while(!logic.isNull())
    {
        QDomElement element = logic.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("logic"))
            {
                if(elem.hasAttribute("code"))
                    mapCodeToLogic_[elem.attribute("code")] = element.text();
            }
        }
        logic = logic.nextSibling();
    }
}

bool ConfigHelper::initIASRule(QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<file.fileName() << " open failed";
    }
    QDomDocument doc("myDocument");
    if (!doc.setContent(&file))
    {
        qDebug()<<file.fileName()<<" setContent failed";
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNode IASRule = docElem.firstChild();
    while(!IASRule.isNull())
    {
        QDomElement element = IASRule.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("item"))
            {
                parseIASRuleItem(element);
            }
        }
        IASRule = IASRule.nextSibling();
    }
    return true;
}

void ConfigHelper::parseIASRuleItem(QDomElement elem)
{
    QString logic;
    QString type = elem.attribute("type");
    if(type == QString::fromLocal8Bit("延时告警"))
    {
        logic = QString::fromLocal8Bit("类型:延时告警") + QString::fromLocal8Bit("   有效时间:%1秒").arg(elem.attribute("validTime"));
    }else if(type == QString::fromLocal8Bit("抖动监视"))
    {
        logic = QString::fromLocal8Bit("类型:抖动监视") + QString::fromLocal8Bit("   判定时间:%1秒").arg(elem.attribute("judgeTime")) +
                QString::fromLocal8Bit("   判定次数:%1").arg(elem.attribute("count"));
    }

    mapCodeToLogic_[elem.attribute("code")] = logic;
}




void ConfigHelper::writeColorToFile()
{
    QMap<int,QColor>::iterator itColor = mapAlarmLevelColor_.begin();
    for( ; itColor != mapAlarmLevelColor_.end(); ++itColor)
    {
        settings_->setValue("COLOR/" + QString::number(itColor.key()), itColor.value().name());
    }

    QMap<int,int>::iterator itFlag = mapAlarmLevelFlag_.begin();
    for( ; itFlag != mapAlarmLevelFlag_.end(); ++itFlag)
    {
        settings_->setValue("FLAG/" + QString::number(itFlag.key()), QString::number(itFlag.value()));
    }

    settings_->sync();
}

void ConfigHelper::getBay(QString bayName)
{
    if(listBay_.contains(bayName))
        return;

    listBay_.push_back(bayName);
}


QStringList ConfigHelper::getListBay()
{
    return listBay_;
}


int ConfigHelper::getMaxSize() const
{
    return iMaxSize_;
}

QColor ConfigHelper::getAlarmLevelColor(int level)
{
    return mapAlarmLevelColor_[level];
}

void ConfigHelper::setAlarmLevelColor(int level, QColor color)
{
    mapAlarmLevelColor_[level] = color;
}



int ConfigHelper::getAlarmLevelFlag(int level)
{
    return mapAlarmLevelFlag_[level];
}

void ConfigHelper::setAlarmLevelFlag(int level, int flag)
{
    mapAlarmLevelFlag_[level] = flag;
}



QString ConfigHelper::getDbName() const
{
    return dbName_;
}



QString ConfigHelper::getIpMySql() const
{
    return ipMySql_;
}

int ConfigHelper::getPortMySql() const
{
    return portMySql_;
}

QString ConfigHelper::getIpRedis() const
{
    return ipRedis_;
}

int ConfigHelper::getPortRedis() const
{
    return portRedis;
}

QString ConfigHelper::getStation() const
{
    return station_;
}


QString ConfigHelper::getLogicByCode(QString code)
{
    return mapCodeToLogic_.value(code, code + QString::fromLocal8Bit(" 不存在"));
}


QString ConfigHelper::getUserMysql() const
{
    return userMySql_;
}

QString ConfigHelper::getPasswdMysql() const
{
    return passwdMySql_;
}

