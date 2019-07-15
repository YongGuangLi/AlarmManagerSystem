#ifndef ALARMDETAIL_H
#define ALARMDETAIL_H


#include "alarmreportparse.h"
#include "lightindicator.h"
#include "HorizontalGraph.h"
#include "VerticalGraph.h"
#include "DialogGraph.h"
#include "analysispcapfile.h"

#include <QWidget>
#include <QDesktopWidget>
#include <QDebug>

namespace Ui {
class AlarmDetail;
}

class AlarmDetail : public QWidget
{
    Q_OBJECT
    
public:
    explicit AlarmDetail(QWidget *parent = 0);
    ~AlarmDetail();
    void centerDisplay();
    /**
    * @date      %{CurrentDate:yyyy-MM-dd}
    * @param
    * @return
    * @brief     摘要
    */
    AlarmReportParse showAlarmDetail(QString fileName);

    void dispstAlarmReport(AlarmReportParse);

    void dispLightIndicator(AlarmReportParse);

    void setstAlarmReport(QString);

    void setLightIndicator(QWidget *widget);

    void addRemoteControlProcess(QWidget *widget);

    void showOtherPage(bool);

    void showRemoteControlProcess(bool);

    void showSequenceDiagram(bool);
private:
    Ui::AlarmDetail *ui;

    LightIndicator *lightIndicator_;

    HorizontalGraph *horizontalGraph;

    VerticalGraph *verticalGraph;

    DialogGraph *dialogGraph;

    AnalysisPcapFile *analysisPcapFile;
};

#endif // ALARMDETAIL_H
