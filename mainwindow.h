#ifndef MainWindow_H
#define MainWindow_H


#include "framelesshelper.h"
#include "rotatewidget.h"
#include "iconhelper.h"
#include "historyalarmquery.h"
#include "confighelper.h"
#include "versiondialog.h"
#include "databasehelper.h"
#include "baseconfig.h"
#include "RedisHelper.h"
#include "statisticanalysis.h"
#include "floatingwindow.h"
#include "userconfig.h"
#include "roleconfig.h"
#include "realtimealarm.h"
#include "pushbutton.h"

#include <QWidget>
#include <QLayout>
#include <QMessageBox>
#include <QtConcurrent>
#include <QDesktopWidget>
#include <QThread>
#include <QShowEvent>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void initMainWindow();
    void initLeftMain();
    void initLeftConfig();
    void initAlarmSence();
public slots:
    void slot_btnTitle_Clicked();
    void slot_btnLeftMain_Clicked();
    void slot_btnLeftConfig_Clicked();

    void slot_changeAlarmSence();

    void slot_redisSubscribe();

private slots:
    void slot_btnMenu_Min_clicked();
    void slot_btnMenu_Max_clicked();
    void slot_btnMenu_Close_clicked();
private:
    Ui::MainWindow *ui;

    QList<QToolButton *> btnsMain;

    QList<QToolButton *> btnsConfig;

    RotateWidget *rotateWidget;
    VersionDialog *versionDialog;

    bool isRunning;
};

#endif // MainWindow_H
