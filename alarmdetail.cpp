#include "alarmdetail.h"
#include "ui_alarmdetail.h"

AlarmDetail::AlarmDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlarmDetail)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromUtf8("告警详情"));

    lightIndicator_ = new LightIndicator();
    ui->gridLayout_4->addWidget(lightIndicator_);

    horizontalGraph = new HorizontalGraph();
    ui->gridLayout_5->addWidget(horizontalGraph);

    verticalGraph = new VerticalGraph();
    ui->gridLayout_6->addWidget(verticalGraph);

    dialogGraph = new DialogGraph();
    ui->gridLayout_7->addWidget(dialogGraph);


    showOtherPage(false);
}

AlarmDetail::~AlarmDetail()
{
    delete ui;
}


AlarmReportParse AlarmDetail::showAlarmDetail(QString fileName)
{
    AlarmReportParse alarmReportParse;
    stAlarmReport alarmReport = alarmReportParse.loadReportFile(fileName);

    dispstAlarmReport(alarmReportParse);       //展示告警简报
    dispLightIndicator(alarmReportParse);      //光字牌


    QList<HorizontalData*> listHorizontalData;
    QList<XYAxisTimeValue*> listXYAxisTimeValue;
    QList<DialogData*> listDialogData;

    QStringList listPointName = alarmReport.listPointName_;
    for(int i = 0; i < listPointName.size(); ++i)
    {
        QString pointName = listPointName.at(i);

        QString time = alarmReport.listPointTime_.at(i);
        QString actionName = alarmReport.listPointValue_.at(i);

        HorizontalData *horizontalData = new HorizontalData();
        horizontalData->SetData(time, pointName + " " + actionName);
        listHorizontalData.push_back(horizontalData);

        int j = 0;
        for( ; j < listXYAxisTimeValue.size(); ++j)
        {
             XYAxisTimeValue* xyAxisTimeValue = listXYAxisTimeValue.at(j);
             if(xyAxisTimeValue->GetPointName() == pointName)
             {
                xyAxisTimeValue->AppendXYValue(time, SingletonConfig->getActionClassifier(actionName));
                break;
             }
        }
        if(j == listXYAxisTimeValue.size())
        {
            XYAxisTimeValue* xyAxisTimeValue = new XYAxisTimeValue();
            xyAxisTimeValue->SetPointName(pointName);

            if(alarmReport.listPointType_.at(i) == QString::fromLocal8Bit(POINT_TYPE_ANA))
                xyAxisTimeValue->SetPointType(QString::fromLocal8Bit(POINT_TYPE_ANA));


            xyAxisTimeValue->AppendXYValue(time, SingletonConfig->getActionClassifier(actionName));
            listXYAxisTimeValue.push_back(xyAxisTimeValue);
        }

        DialogData* dialogData = new DialogData();
        dialogData->SetData(time, pointName, actionName);
        listDialogData.push_back(dialogData);
    }

    //去掉文件名后缀
    fileName.chop(4);

    horizontalGraph->InitData(listHorizontalData, fileName);
    verticalGraph->InitData(listXYAxisTimeValue, fileName);
    dialogGraph->InitData(listDialogData, fileName);


    ui->tabWidget->setCurrentIndex(0);

    show();

    return alarmReportParse;
}


void AlarmDetail::dispstAlarmReport(AlarmReportParse alarmReportParse)
{
    QString alarmConclusion = QString::fromUtf8("[智能告警]\n") + alarmReportParse.getAlarmConclusion();
    QString alarmLogic = QString::fromUtf8("\n\n[推理逻辑]\n") + SingletonConfig->getLogicByCode(alarmReportParse.getCode());
    QString LogicItems = QString::fromUtf8("\n\n[推理依据]\n") + alarmReportParse.getLogicItems().join("\n");
    QString InfoItems =  QString::fromUtf8("\n\n[关联信号]\n") + alarmReportParse.getInfoItems().join("\n");
    QString cause =  QString::fromUtf8("\n\n[原因分析]\n") + alarmReportParse.getCause();
    QString advice =  QString::fromUtf8("\n\n[处理措施]\n") + alarmReportParse.getAdvice();

    ui->textBrowser->setText(alarmConclusion + alarmLogic + LogicItems + InfoItems + cause + advice);
}

void AlarmDetail::dispLightIndicator(AlarmReportParse alarmReportParse)
{
    QString alarmConclusion =alarmReportParse.getAlarmConclusion();
    QStringList listLogicItems = alarmReportParse.getLogicItems();
    QMap<QString , QStringList> mapLogicItems;
    QMap<QString , QStringList> mapActionClassifier;

    for(int i = 0; i < listLogicItems.size(); ++i)
    {
         //<item type="遥控">2018-09-05 13:59:49 线路零序保护 茂名变电站 装置上电 动作</item>
         QStringList LogicItem = listLogicItems.at(i).split(QRegExp("[\\s]+"));
         mapLogicItems[LogicItem.value(2, "")].push_back(LogicItem.value(4, "") + " " + LogicItem.value(0, "") + " " + LogicItem.value(1, ""));
         mapActionClassifier[LogicItem.value(2, "")].push_back(LogicItem.value(5, ""));
    }

    lightIndicator_->setMapLogicItems(mapLogicItems);
    lightIndicator_->setMapActionClassifier(mapActionClassifier);
    lightIndicator_->setAlarmConclusion(alarmConclusion);
    lightIndicator_->dispLightIndicator();
}




void AlarmDetail::addRemoteControlProcess(QWidget *remoteControl)
{
    ui->tabWidget->addTab(remoteControl, QString::fromLocal8Bit("遥控过程"));
}

void AlarmDetail::showOtherPage(bool enabel)
{
    ui->tabWidget->setTabEnabled(2, enabel);
    ui->tabWidget->setTabEnabled(3, enabel);
    ui->tabWidget->setTabEnabled(4, enabel);
    ui->tabWidget->setTabEnabled(5, enabel);
    ui->tabWidget->setTabEnabled(6, enabel);
}

void AlarmDetail::showRemoteControlProcess(bool enabel)
{
    ui->tabWidget->setTabEnabled(6, enabel);
}

void AlarmDetail::showSequenceDiagram(bool enabel)
{
    ui->tabWidget->setTabEnabled(3, enabel);
    ui->tabWidget->setTabEnabled(4, enabel);
    ui->tabWidget->setTabEnabled(5, enabel);
}




