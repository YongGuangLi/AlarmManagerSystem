#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H


#include "alarmdata.h"
#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QStyleOption>

class PushButton : public QPushButton
{
    Q_OBJECT         // 添加Q_OBJECT 要重新qmake下
public:
    PushButton ( const QString & text, QWidget * parent = 0 );
    void paintEvent(QPaintEvent *);
    void setIcon(QString);
    void mousePressEvent(QMouseEvent *e);
signals:
    void clicked();

public slots:
    void increase(int);
private:
    int eventCnt;
    int abnormalCnt;
    int outoflimitCnt;
    int changeCnt;
    int informCnt;
    QString text_;
    QString icon_;

};

#endif // PUSHBUTTON_H
