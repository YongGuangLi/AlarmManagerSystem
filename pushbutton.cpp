#include "pushbutton.h"


PushButton::PushButton(const QString &text, QWidget *parent):
    QPushButton(text, parent)
{
    eventCnt = 0;
    abnormalCnt = 0;
    outoflimitCnt = 0;
    changeCnt = 0;
    informCnt = 0;
    text_ = text;
}

void PushButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);


    QPainter painter(this);

    QRectF target(20.0, 10.0, 60.0, 60.0);
    QRectF source(0.0, 0.0, 72.0, 72.0);
    QImage image(icon_);
    painter.drawImage(target, image, source,Qt::MonoOnly);

    QRectF position(60.0, 0, 25.0, 25.0);
    painter.setBrush(Qt::red);
    painter.setPen(Qt::red);
    int iCnt = eventCnt + abnormalCnt + outoflimitCnt +changeCnt+informCnt;
    if(iCnt > 0 && iCnt < 10)
    {
        painter.drawEllipse(position);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 15));
        painter.drawText(position, Qt::AlignCenter,QString::number(iCnt));
    }
    else if(iCnt > 9 && iCnt < 100)
    {
        painter.drawEllipse(position);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 13));
        painter.drawText(position, Qt::AlignCenter,QString::number(iCnt));
    }
    else if(iCnt > 99)
    {
        painter.drawEllipse(position);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 10));
        painter.drawText(position, Qt::AlignCenter,QString::number(iCnt));
    }

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 11));
    QRectF rectangle(0,72,100,40);
    painter.drawText(rectangle , Qt::AlignHCenter | Qt::AlignTop, text_);



    //使用PaintEvent之后setStyleSheet不生效的解决办法
    //QPushButton::paintEvent(event);
}

void PushButton::setIcon(QString icon)
{
    icon_ = icon;
}


void PushButton::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);

    emit clicked();
}


void PushButton::increase(int level)
{
    switch(level)
    {
    case LEVEL_EVENT:
        eventCnt++;
        break;
    case LEVEL_ABNORMAL:
        abnormalCnt++;
        break;
    case LEVEL_OUTOFLIMIT:
        outoflimitCnt++;
        break;
    case LEVEL_CHANGE:
        changeCnt++;
        break;
    case LEVEL_INFORM:
        informCnt++;
        break;
    dafult:
        break;
    }
}

