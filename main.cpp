#include "mainwindow.h"
#include "myhelper.h"
#include <QApplication>
#include <QTextCodec>
#include <QFile>

#include "log4qt/propertyconfigurator.h"
#include "log4qt/logmanager.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setFont(QFont("Microsoft Yahei", 9));

    myHelper::SetUTF8Code();
    myHelper::SetStyle(":/Resources/qss/psblack.css");

    Log4Qt::PropertyConfigurator::configure(qApp->applicationDirPath() + QDir::separator() +"log4qt.properties");
    Log4Qt::LogManager::setHandleQtMessages(true);

    MainWindow w;
    w.show();

    int e = a.exec();
    if(e == RETCODE_RESTART)
    {
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        return 0;
    }
    return e;
}


//时间<<厂站<<间隔<<点名<<状态<<模拟量
//遥控时间<<<厂站<<间隔<<遥控点名<<遥控值<<遥控来源<<遥信时间<<遥信点名<<遥信值<<检验结果
//时间<<厂站<<间隔<<告警等级<<推理结论
//起始时间<<结束时间<<<厂站<<间隔<<点名<<遥控结果<<遥控来源
//时间<<当前时间<<厂站<<间隔<<信号名<<是否跳变
//时间<<厂站<<间隔<<信号名<<跳变前值<<跳变后值<<跳变幅度百分比<<跳变幅度阈值
//时间<<当前时间<<厂站<<间隔<<信号名<<信号来源及值<<是否一致
//时间<<厂站<<间隔<<告警等级<<告警内容











