#ifndef REMOTECONTROLWIDGET_H
#define REMOTECONTROLWIDGET_H


#include "alarmdata.h"
#include "RtdbMessage.pb.h"

#include <QPaintEvent>
#include <QWidget>
#include <math.h>
#include <QPainter>


#define INTERVAL 20

namespace Ui {
class RemoteControlWidget;
}

class RemoteControlWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit RemoteControlWidget(QWidget *parent = 0);
    ~RemoteControlWidget();
    
    void drawLine(QPainter& painter,QPointF startPoint, QPointF endPoint);

    void calcVertexes(double start_x, double start_y, double end_x, double end_y, double &x1, double &y1, double &x2, double &y2);

    void paintEvent(QPaintEvent *);

    QString getCtrlResult(int ctrlResult);

    void showRemoteControl(QString fileName);


private:
    Ui::RemoteControlWidget *ui;
    QStringList listPointSrcIp;
    QStringList listPointName;
    QList<int> listCtrlResult;
    QStringList listPointTime;
};

#endif // REMOTECONTROLWIDGET_H
