#ifndef BASECONFIG_H
#define BASECONFIG_H

#include "confighelper.h"
#include "settingdialog.h"
#include "databasehelper.h"

#include <QWidget>
#include <QShowEvent>
#include <QCheckBox>
#include <QColorDialog>

namespace Ui {
class BaseConfig;
}

class BaseConfig : public QWidget
{
    Q_OBJECT

public:
    explicit BaseConfig(QWidget *parent = 0);
    ~BaseConfig();

    void showAlarmConfig(stAlarmConfig alarmConfig);

protected:
    void showEvent(QShowEvent *e);

private slots:
    void slot_addAlarmConfig();
    void slot_modifyAlarmConfig();
    void slot_deleteAlarmConfig();
private:
    Ui::BaseConfig *ui;
};

#endif // BASECONFIG_H
