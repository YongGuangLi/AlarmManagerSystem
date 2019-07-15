#include "remotecontrolwidget.h"
#include "ui_remotecontrolwidget.h"

RemoteControlWidget::RemoteControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoteControlWidget)
{
    ui->setupUi(this);
    ui->label_104->setWordWrap(true);
    ui->label_master->setWordWrap(true);
    ui->label_61850->setWordWrap(true);
}

RemoteControlWidget::~RemoteControlWidget()
{
    delete ui;
}

void RemoteControlWidget::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::magenta, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

    if(ui->label_104->isHidden())
    {
        QPointF startPoint(ui->label_master->pos().x() + ui->label_master->width(), ui->label_master->pos().y() + INTERVAL);
        QPointF endPoint(ui->label_61850->pos().x() + 3, ui->label_61850->pos().y() + INTERVAL);
        drawLine(painter,startPoint, endPoint);

        startPoint += QPointF(150, -3);
        painter.drawText(startPoint, listPointTime.value(0, ""));
        startPoint += QPointF(0, + 15);
        painter.drawText(startPoint, QString::fromLocal8Bit("IEC104遥控选择请求"));


        startPoint = QPointF(ui->label_61850->pos().x() + 3, ui->label_61850->pos().y() + 4 * INTERVAL);
        endPoint = QPointF(ui->label_master->pos().x() + ui->label_master->width(), ui->label_master->pos().y() + 4 * INTERVAL);
        drawLine(painter,startPoint, endPoint);

        endPoint += QPointF(150, -3);
        painter.drawText(endPoint, listPointTime.value(1, ""));
        endPoint += QPointF(0, + 15);
        painter.drawText(endPoint, QString::fromLocal8Bit("IEC104遥控选择应答 (%1应答)").arg(getCtrlResult(listCtrlResult.value(1, -1))));


        startPoint = QPointF(ui->label_master->pos().x() + ui->label_master->width(), ui->label_master->pos().y() + 8 * INTERVAL);
        endPoint = QPointF(ui->label_61850->pos().x() + 3, ui->label_61850->pos().y() + 8 * INTERVAL);
        drawLine(painter,startPoint, endPoint);

        startPoint += QPointF(150, -3);
        painter.drawText(startPoint, listPointTime.value(2, ""));
        startPoint += QPointF(0, + 15);
        painter.drawText(startPoint, QString::fromLocal8Bit("IEC104遥控执行请求"));


        startPoint = QPointF(ui->label_61850->pos().x() + 3, ui->label_61850->pos().y() + 12 * INTERVAL);
        endPoint = QPointF(ui->label_master->pos().x() + ui->label_master->width(), ui->label_master->pos().y() + 12 * INTERVAL);
        drawLine(painter,startPoint, endPoint);

        endPoint += QPointF(150, -3);
        painter.drawText(endPoint, listPointTime.value(3, ""));
        endPoint += QPointF(0, + 15);
        painter.drawText(endPoint, QString::fromLocal8Bit("IEC104遥控执行应答 (%1应答)").arg(getCtrlResult(listCtrlResult.value(3, -1))));
    }
    else
    {
        QPointF startPoint(ui->label_master->pos().x() + ui->label_master->width(), ui->label_master->pos().y() + INTERVAL);
        QPointF endPoint(ui->label_104->pos().x() + 3, ui->label_104->pos().y() + INTERVAL);
        drawLine(painter,startPoint, endPoint);

        startPoint += QPointF(30, -3);
        painter.drawText(startPoint, listPointTime.value(0, ""));
        startPoint += QPointF(0, + 15);
        painter.drawText(startPoint, QString::fromLocal8Bit("IEC104遥控选择请求"));

        startPoint = QPointF(ui->label_104->pos().x() + ui->label_104->width(), ui->label_104->pos().y() + INTERVAL);
        endPoint = QPointF(ui->label_61850->pos().x() + 3, ui->label_61850->pos().y() + INTERVAL);
        drawLine(painter,startPoint, endPoint);

        startPoint += QPointF(30, -3);
        painter.drawText(startPoint, listPointTime.value(1, ""));
        startPoint += QPointF(0, + 15);
        painter.drawText(startPoint, QString::fromLocal8Bit("Mms遥控带值选择请求"));


        startPoint = QPointF(ui->label_61850->pos().x() + 3, ui->label_61850->pos().y() + 4 * INTERVAL);
        endPoint = QPointF(ui->label_104->pos().x() + ui->label_104->width(), ui->label_104->pos().y() + 4 * INTERVAL);
        drawLine(painter,startPoint, endPoint);

        endPoint += QPointF(30, -3);
        painter.drawText(endPoint, listPointTime.value(2, ""));
        endPoint += QPointF(0, + 15);
        painter.drawText(endPoint, QString::fromLocal8Bit("Mms遥控带值选择应答 (%1应答)").arg(getCtrlResult(listCtrlResult.value(2, -1))));


        startPoint = QPointF(ui->label_104->pos().x() + 3, ui->label_104->pos().y() + 4 * INTERVAL);
        endPoint = QPointF(ui->label_master->pos().x() + ui->label_master->width(), ui->label_master->pos().y() + 4 * INTERVAL);
        drawLine(painter,startPoint, endPoint);

        endPoint += QPointF(30, -3);
        painter.drawText(endPoint, listPointTime.value(3, ""));
        endPoint += QPointF(0, + 15);
        painter.drawText(endPoint, QString::fromLocal8Bit("IEC104遥控选择应答 (%1应答)").arg(getCtrlResult(listCtrlResult.value(3, -1))));




        startPoint = QPointF(ui->label_master->pos().x() + ui->label_master->width(), ui->label_master->pos().y() + 8 * INTERVAL);
        endPoint = QPointF(ui->label_104->pos().x() + 3, ui->label_104->pos().y() + 8 * INTERVAL);
        drawLine(painter,startPoint, endPoint);

        startPoint += QPointF(30, -3);
        painter.drawText(startPoint, listPointTime.value(4, ""));
        startPoint += QPointF(0, + 15);
        painter.drawText(startPoint, QString::fromLocal8Bit("IEC104遥控执行请求"));

        startPoint = QPointF(ui->label_104->pos().x() + ui->label_104->width(), ui->label_104->pos().y() + 8 * INTERVAL);
        endPoint = QPointF(ui->label_61850->pos().x() + 3, ui->label_61850->pos().y() + 8 * INTERVAL);
        drawLine(painter,startPoint, endPoint);

        startPoint += QPointF(30, -3);
        painter.drawText(startPoint, listPointTime.value(5, ""));
        startPoint += QPointF(0, + 15);
        painter.drawText(startPoint, QString::fromLocal8Bit("Mms遥控带值执行请求"));

        startPoint = QPointF(ui->label_61850->pos().x() + 3, ui->label_61850->pos().y() + 12 * INTERVAL);
        endPoint = QPointF(ui->label_104->pos().x() + ui->label_104->width(), ui->label_104->pos().y() + 12 * INTERVAL);
        drawLine(painter,startPoint, endPoint);

        endPoint += QPointF(30, -3);
        painter.drawText(endPoint, listPointTime.value(6, ""));
        endPoint += QPointF(0, + 15);
        painter.drawText(endPoint, QString::fromLocal8Bit("Mms遥控带值执行应答 (%1应答)").arg(getCtrlResult(listCtrlResult.value(6, -1))));


        startPoint = QPointF(ui->label_104->pos().x() + 3, ui->label_104->pos().y() + 12 * INTERVAL);
        endPoint = QPointF(ui->label_master->pos().x() + ui->label_master->width(), ui->label_master->pos().y() + 12 * INTERVAL);
        drawLine(painter,startPoint, endPoint);

        endPoint += QPointF(30, -3);
        painter.drawText(endPoint, listPointTime.value(7, ""));
        endPoint += QPointF(0, + 15);
        painter.drawText(endPoint, QString::fromLocal8Bit("IEC104遥控执行应答 (%1应答)").arg(getCtrlResult(listCtrlResult.value(7, -1))));
    }
}

QString RemoteControlWidget::getCtrlResult(int ctrlResult)
{
    QString stCtrlResult;
    switch(ctrlResult)
    {
    case CTYPE_ACTIVE:
        break;
    case CTYPE_ACTIVESUCCESS:
        stCtrlResult = QString::fromLocal8Bit("肯定");
        break;
    case CTYPE_ACTIVEFAIL:
        stCtrlResult = QString::fromLocal8Bit("否定");
        break;
    default:
        stCtrlResult = QString::fromLocal8Bit("未知");
        break;
    }
    return stCtrlResult;
}



void RemoteControlWidget::showRemoteControl(QString fileName)
{
    AlarmReportParse alarmReportParse;
    stAlarmReport alarmReport = alarmReportParse.loadReportFile(fileName);


    listPointSrcIp = alarmReport.listPointSrcIp_;
    listPointName = alarmReport.listPointName_;
    listCtrlResult = alarmReport.listCtrlResult_;
    listPointTime = alarmReport.listPointTime_;

    if(listPointSrcIp.size() == 8)
    {
        ui->label_masterIp->setText(listPointSrcIp.at(0));
        ui->label_104SrcIp->setText(listPointSrcIp.at(3));
        ui->label_104DstIp->setText(listPointSrcIp.at(1));
        ui->label_61850Ip->setText(listPointSrcIp.at(2));

        ui->label_masterPoint->setText(listPointName.at(0));
        ui->label_104SrcPoint->setText(listPointName.at(3));
        ui->label_104DstPoint->setText(listPointName.at(1));
        ui->label_61850Point->setText(listPointName.at(2));


        ui->label_104->show();
    }
    else if(listPointSrcIp.size() == 4)
    {

        ui->label_masterIp->setText(listPointSrcIp.at(0));
        ui->label_104SrcIp->setText("");
        ui->label_104DstIp->setText("");
        ui->label_61850Ip->setText(listPointSrcIp.at(1));

        ui->label_masterPoint->setText(listPointName.at(0));
        ui->label_104SrcPoint->setText("");
        ui->label_104DstPoint->setText("");
        ui->label_61850Point->setText(listPointName.at(1));

        ui->label_104->hide();
    }

    ui->label_61850->setText(alarmReport.bay_);
}


void RemoteControlWidget::drawLine(QPainter &painter, QPointF startPoint, QPointF endPoint)
{
    double x1, y1, x2, y2; //箭头的两点坐标
    //求得箭头两点坐标
    calcVertexes(startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y(), x1, y1, x2, y2);

    painter.drawLine(startPoint, endPoint);//绘制线段
    painter.drawLine(endPoint.x(), endPoint.y(), x1, y1);//绘制箭头一半
    painter.drawLine(endPoint.x(), endPoint.y(), x2, y2);//绘制箭头另一半
}

void RemoteControlWidget::calcVertexes(double start_x, double start_y, double end_x, double end_y, double &x1, double &y1, double &x2, double &y2)
{
    double arrow_lenght_ = 10;//箭头长度，一般固定
    double arrow_degrees_ = 0.5;//箭头角度，一般固定

    double angle = atan2(end_y - start_y, end_x - start_x) + 3.1415926;//

    x1 = end_x + arrow_lenght_ * cos(angle - arrow_degrees_);//求得箭头点1坐标
    y1 = end_y + arrow_lenght_ * sin(angle - arrow_degrees_);
    x2 = end_x + arrow_lenght_ * cos(angle + arrow_degrees_);//求得箭头点2坐标
    y2 = end_y + arrow_lenght_ * sin(angle + arrow_degrees_);
}

