#include "alarmreportparse.h"

AlarmReportParse::AlarmReportParse()
{
    qRegisterMetaType<stAlarmReport>("stAlarmReport");
    level_ = 0;
}


stAlarmReport AlarmReportParse::loadReportFile(QString filename)
{
    filename = filename.replace("txt","xml");
    filename_ = filename;
    alarmReport.filename_ = filename;
    QString filePath = SingletonConfig->getAlarmReportPath() + "/" + filename;
    if(!QFile::exists(filePath))
        filePath = SingletonConfig->getAlarmReportPath() + "His/" + filename;

    QDomDocument doc("mydocument");
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<filePath <<" open failed";
        return alarmReport;
    }
    if (!doc.setContent(&file))
    {
        qDebug()<<filePath<<"setContent failed";
        file.close();
        return alarmReport;
    }
    file.close();

    QDomElement rootElem = doc.documentElement();

    if(rootElem.hasAttribute("code"))
    {
        code_ = rootElem.attribute("code");
    }
    if(rootElem.hasAttribute("scene"))
    {
        scene_ = rootElem.attribute("scene");
        alarmReport.scene_ = scene_;
    }
    if(rootElem.hasAttribute("priority"))
    {
        level_ = rootElem.attribute("priority").toInt();
        alarmReport.level_ = level_;
    }
    if(rootElem.hasAttribute("direction"))
    {
        direction_ = rootElem.attribute("direction");
        alarmReport.direction_ = direction_;
    }
    if(rootElem.hasAttribute("validTime"))
    {
        alarmReport.validTime_ = rootElem.attribute("validTime");
    }
    QDomNode nodeEle = rootElem.firstChild();
    while(!nodeEle.isNull())
    {
        QDomElement element = nodeEle.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("conclusion"))
            {
                alarmConclusion_ = element.text();
                QStringList listString = alarmConclusion_.split(" ");
                alarmReport.time_ = listString.value(0, "") + " " + listString.value(1, "");
                alarmReport.substation_ = listString.value(2, "");
                alarmReport.bay_ = listString.value(3, "");
                alarmReport.conclusion_ = listString.value(4, "");
            }
            else if(element.tagName() == "cause")
            {
                cause_ = element.text();
            }
            else if(element.tagName() == "advice")
            {
                advice_ = element.text();
            }
            else if(element.tagName() == QString("logic"))
            {
                parseLogicItem(element);
            }
            else if(element.tagName() == QString("info"))
            {
                parseInfoItem(element);
            }
            else if(element.tagName() == QString("soe"))
            {
                parseSoeItem(element);
            }
            else if(element.tagName() == QString("pcapfile"))
            {
                parsePcapFile(element);
            }
            else if(element.tagName() == QString("deviceName"))
            {
                if(element.hasAttribute("sourDev"))
                    alarmReport.srcDevice_ = element.attribute("sourDev");

                if(element.hasAttribute("destDev"))
                    alarmReport.dstDevice_ = element.attribute("destDev");
            }

        }
        nodeEle = nodeEle.nextSibling();
    }
    return alarmReport;
}

void AlarmReportParse::parseLogicItem(QDomElement element)
{
    QDomNode n = element.firstChild();
    while(!n.isNull())
    {
        QDomElement element = n.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("item"))
            {
                listLogicItems_.push_back(element.text());
                QStringList listElementMem = element.text().split(" ");
                if(element.hasAttribute("type"))
                    alarmReport.listPointType_.push_back(element.attribute("type"));

                alarmReport.listPointTime_.push_back(listElementMem.value(0, "") + " " + listElementMem.value(1, ""));
                alarmReport.listPointName_.push_back(listElementMem.value(4, ""));
                alarmReport.listPointValue_.push_back(listElementMem.value(5, ""));


                if(element.hasAttribute("sourIp"))
                    alarmReport.listPointSrcIp_.push_back(element.attribute("sourIp"));

                if(element.hasAttribute("destIp"))
                    alarmReport.listPointdstIp_.push_back(element.attribute("destIp"));

                if(element.hasAttribute("cmdType"))
                    alarmReport.listCmdType_.push_back(element.attribute("cmdType").toInt());

                if(element.hasAttribute("result"))
                    alarmReport.listCtrlResult_.push_back(element.attribute("result").toInt());
            }
        }
        n = n.nextSibling();
    }
}

void AlarmReportParse::parseInfoItem(QDomElement element)
{
    QDomNode n = element.firstChild();
    while(!n.isNull())
    {
        QDomElement element = n.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("item"))
            {
                listInfoItems_.push_back(element.text());
            }
        }
        n = n.nextSibling();
    }
}

void AlarmReportParse::parseSoeItem(QDomElement element)
{
    QDomNode n = element.firstChild();
    while(!n.isNull())
    {
        QDomElement element = n.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("item"))
            {
                listSoeItems_.push_back(element.text());
            }
        }
        n = n.nextSibling();
    }
}

void AlarmReportParse::parsePcapFile(QDomElement element)
{
    QDomNode n = element.firstChild();
    while(!n.isNull())
    {
        QDomElement element = n.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("item"))
            {
                listPcapFile_.push_back(element.text());
            }
        }
        n = n.nextSibling();
    }
}



QString AlarmReportParse::getFile()
{
    return filename_;
}

QString AlarmReportParse::getAlarmConclusion() const
{
    return alarmConclusion_;
}

QString AlarmReportParse::getCode() const
{
    return code_;
}

QString AlarmReportParse::getScene() const
{
    return scene_;
}

QString AlarmReportParse::getCause() const
{
    return cause_;
}


QString AlarmReportParse::getAdvice() const
{
    return advice_;
}


QStringList AlarmReportParse::getLogicItems() const
{
    return listLogicItems_;
}

QStringList AlarmReportParse::getInfoItems() const
{
    return listInfoItems_;
}

QStringList AlarmReportParse::getSoeItems() const
{
    return listSoeItems_;
}

QStringList AlarmReportParse::getPcapFile() const
{
    return listPcapFile_;
}


