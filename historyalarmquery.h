#ifndef HistoryAlarmQuery_H
#define HistoryAlarmQuery_H

#include "dbpage.h"
#include <QWidget>
#include <QShowEvent>
#include <QFileDialog>

namespace Ui {
class HistoryAlarmQuery;
}

class HistoryAlarmQuery : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryAlarmQuery(QWidget *parent = 0);
    ~HistoryAlarmQuery();

private:
    void initForm();

protected:
    void showEvent(QShowEvent *e);

private slots:
    void slot_btnSelect_clicked();
    void slot_btnExport_clicked();

private:
    Ui::HistoryAlarmQuery *ui;

    QList<QString> columnNames; //字段名集合
    QList<int> columnWidths;    //字段宽度集合
    DbPage *dbPage;             //数据库翻页类

    QString tableName;          //表名称
    QString countName;          //统计行数字段名称
};

#endif // HistoryAlarmQuery_H
