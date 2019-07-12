#ifndef ROLECONFIG_H
#define ROLECONFIG_H

/************************************************************************
描述： 角色配置界面
作者： WANGXL
时间： 2019-01-08
************************************************************************/

#include <QWidget>
#include <QTableWidget>
#include <QUuid>
#include <QMessageBox>

#include "databasehelper.h"
#include "settingdialog.h"
#include "datamodel.h"
#include "myhelper.h"

#pragma execution_character_set("utf-8")

namespace Ui {
class RoleConfig;
}

class RoleConfig : public QWidget
{
    Q_OBJECT

public:
    explicit RoleConfig(QWidget *parent = nullptr);
    ~RoleConfig();
    /***************************************************************************
    *功能：初始化表格
    *输入：无
    *输出：无
    *时间：2018-12-24
    *作者：WANGXL
    ***************************************************************************/
    void initTable(QTableWidget* tableWidget);

    /***************************************************************************
    *功能：表格添加一行
    *输入：无
    *输出：无
    *时间：2018-12-24
    *作者：WANGXL
    ***************************************************************************/
    void addRowOnTable(QTableWidget* tableWidget, Role_Info info);

    /***************************************************************************
    *功能：设置功能ID
    *输入：无
    *输出：无
    *时间：2018-12-24
    *作者：WANGXL
    ***************************************************************************/
    void setFuncId(QString id);

    /***************************************************************************
    *功能：验证角色是否已存在
    *输入：无
    *输出：无
    *时间：2019-01-08
    *作者：WANGXL
    ***************************************************************************/
    bool isExistThisRoleName(QString name);

protected:
    void showEvent(QShowEvent *event);
public slots:
    /***************************************************************************
    *功能：观察者从订阅主题接收的消息
    *输入：message:消息体
    *输出：无
    *时间：2018-12-24
    *作者：WANGXL
    ***************************************************************************/
    void recvMsg(Data_RoleConfig data_RoleConfig);
private slots:
    void on_pBtn_add_clicked();  //新增
    void on_pBtn_modify_clicked(); //修改
    void on_pBtn_delete_clicked(); //删除

private:
    Ui::RoleConfig *ui;
    QString m_myFuncId; //功能ID
    Data_RoleConfig m_data_RoleConfig; //页面数据
};

#endif // ROLECONFIG_H
