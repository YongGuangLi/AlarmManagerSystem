#ifndef USERCONFIG_H
#define USERCONFIG_H

/************************************************************************
描述： 用户配置
作者： WANGXL
时间： 2019-01-08
************************************************************************/

#include <QWidget>
#include <QTableWidget>
#include <QUuid>
#include <QMessageBox>

#include "databasehelper.h"
#include "myhelper.h"
#include "settingdialog.h"
#include "myhelper.h"

#pragma execution_character_set("utf-8")

namespace Ui {
class UserConfig;
}

class UserConfig : public QWidget
{
    Q_OBJECT

public:
    explicit UserConfig(QWidget *parent = nullptr);
    ~UserConfig();
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
    void addRowOnTable(QTableWidget* tableWidget, User_Info info);

    /***************************************************************************
    *功能：设置功能ID
    *输入：无
    *输出：无
    *时间：2018-12-24
    *作者：WANGXL
    ***************************************************************************/
    void setFuncId(QString id);

    /***************************************************************************
    *功能：设置写权限
    *输入：无
    *输出：无
    *时间：2019-01-08
    *作者：WANGXL
    ***************************************************************************/
    void setWriteAccessFlag(bool flag);

    /***************************************************************************
    *功能：验证用户是否已存在
    *输入：无
    *输出：无
    *时间：2019-01-08
    *作者：WANGXL
    ***************************************************************************/
    bool isExistThisUserName(QString name);
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
    void recvMsg(Data_UserConfig data_UserConfig);
private slots:
    void on_pBtn_add_clicked(); //新增按钮
    void on_pBtn_modify_clicked(); //修改按钮
    void on_pBtn_delete_clicked(); //删除按钮

private:
    Ui::UserConfig *ui;
    QString m_myFuncId; //功能ID
    Data_UserConfig m_data_UserConfig; //用户配置页面数据
    QRegExp m_nameExp; //姓名正则表达式
    QRegExp m_pwdExp; //密码正则表达式
};

#endif // USERCONFIG_H
