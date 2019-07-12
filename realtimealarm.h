#ifndef REALTIMEALARM_H
#define REALTIMEALARM_H

#include "databasehelper.h"
#include "alarmreportparse.h"
#include "filterbay.h"
#include "datamodel.h"
#include "iconhelper.h"


#include <QWidget>
#include <QPoint>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QMenu>
#include <QMutexLocker>
#include <QtCore>
#include <QButtonGroup>
#include <QDateTime>
#include <QShowEvent>

namespace Ui {
class RealTimeAlarm;
}

class RealTimeAlarm : public QWidget
{
    Q_OBJECT
    
public:
    explicit RealTimeAlarm(QWidget *parent = 0);
    ~RealTimeAlarm();

    void initWidgetDisp();

protected:
    void showEvent(QShowEvent *event);

private slots:
    //接收并显示智能告警数据
    void dispSmartAlarmData(int, stAlarmData);
    //接收并显示原始告警数据
    void dispOriginalAlarmData(int, stAlarmData);

    //告警表格右击菜单
    void showCustomMenu(QPoint);

    //告警表右击菜单槽函数
    void confirmAlarm();                  //确认告警
    void showAlarmDetail();               //告警详情
    void deleteAlarm();                   //删除告警

    //选择间隔过滤
    void selectFilterBay();

    //修改当前智能告警过滤等级
    void changeCurIaAlarmLevel();
    //修改当前原始告警过滤等级
    void changeCurOrAlarmLevel();

    //暂停智能告警数据显示
    void pauseSmartAlarmData(bool);
    //暂停原始告警数据显示
    void pauseOriginalAlarmData(bool);

    //由于告警颜色改变，间隔过滤，间隔屏蔽，修改过滤告警等级，重新显示
    void replaySmartAlarmData();
    //由于告警颜色改变，间隔过滤，间隔屏蔽，修改过滤告警等级，重新显示
    void replayOriginalAlarmData();

    //间隔过滤,屏蔽stackWidget切换
    void alarmTabWidgetChanged(int);

private:
    Ui::RealTimeAlarm *ui;

    int curSelectIaAlarmLevel;
    int curSelectOrAlarmLevel;

    QMenu *smartAlarmMenu;
    QMenu *originalAlarmMenu;

    //用与修改告警表显示和插入之间保证数据同步
    QMutex mutex;

    bool isSmartAlarmPause;
    bool isOriginalAlarmPause;

    QButtonGroup smartButtonGroup;
    QButtonGroup originalButtonGroup;

    QList<QToolButton *> listSmartButton;

    QList<QToolButton *> listOriginalButton;

    FilterBay *filterBay;
};

#endif // REALTIMEALARM_H
