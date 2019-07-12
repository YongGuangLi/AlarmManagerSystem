#ifndef FILTERBAY_H
#define FILTERBAY_H

#include "confighelper.h"
#include "basewindow.h"
#include <QTableWidgetItem>
#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>

namespace Ui {
class FilterBay;
}

class FilterBay : public BaseWindow
{
    Q_OBJECT

public:
    explicit FilterBay(QWidget *parent = 0);
    ~FilterBay();

    void showSelectBay(int type);

    bool judgeBayValid(QString bayName, int alarmType);
private slots:
    void addBayName(QTableWidgetItem*);

    void removeBayName(QTableWidgetItem*);

    void on_btnconfirm_clicked();

signals:
    void sendSmartConfirmCmd();

    void sendOriginalConfirmCmd();
private:
    Ui::FilterBay *ui;
    QStringList listSmartBayFilter_;
    QStringList listSmartBayHide_;

    QStringList listOriginalBayFilter_;
    QStringList listOriginalBayHide_;

    QStringList listFilterBay_;
    QStringList listHideBay_;


    int selectType;
};

#endif // FILTERBAY_H
