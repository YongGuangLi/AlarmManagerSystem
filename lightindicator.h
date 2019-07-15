#ifndef LIGHTINDICATOR_H
#define LIGHTINDICATOR_H


#include "confighelper.h"
#include "lineedit.h"
#include <math.h>
#include <QWidget>
#include <QMap>
#include <QString>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QPoint>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QDesktopWidget>

namespace Ui {
class LightIndicator;
}

class LightIndicator : public QWidget
{
    Q_OBJECT

public:
    explicit LightIndicator(QWidget *parent = 0);
    ~LightIndicator();
    void centerDisplay();

    void setMapLogicItems(const QMap<QString, QStringList> &mapLogicItems);
    void setMapActionClassifier(const QMap<QString, QStringList> &mapActionClassifier);
    void setAlarmConclusion(const QString &alarmConclusion);

    void dispLightIndicator();

    void calcVertexes(double start_x, double start_y, double end_x, double end_y, double &x1, double &y1, double &x2, double &y2);

    void paintEvent(QPaintEvent *);

private:
    Ui::LightIndicator *ui;
    QMap<QString , QStringList> mapLogicItems_;
    QMap<QString , QStringList> mapActionClassifier_;
    QString alarmConclusion_;
    QMap<LineEdit*,QList<LineEdit*> > mapLineEdits_;
    QColor color_[3];

    //结论
    LineEdit *lineEdit_AlarmConclusion_;

    //整体布局，包含告警结论和所有基准信号布局
    QHBoxLayout *wholeBoxLayout;

    //所有基准信号布局
    QVBoxLayout *baseSignalboxLayout;
};

#endif // LIGHTINDICATOR_H
