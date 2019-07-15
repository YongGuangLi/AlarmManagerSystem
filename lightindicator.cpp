#include "lightindicator.h"
#include "ui_lightindicator.h"

LightIndicator::LightIndicator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LightIndicator)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromUtf8("告警光字牌"));

    color_[0] = Qt::green;
    color_[1] = Qt::red;
    color_[2] = Qt::black;

    centerDisplay();

    baseSignalboxLayout = new QVBoxLayout();
    lineEdit_AlarmConclusion_ = new LineEdit("");

    wholeBoxLayout = new QHBoxLayout(this);
    wholeBoxLayout->setSpacing(50);
    wholeBoxLayout->addLayout(baseSignalboxLayout);
    wholeBoxLayout->addWidget(lineEdit_AlarmConclusion_);
}

LightIndicator::~LightIndicator()
{
    delete ui;
}

void LightIndicator::centerDisplay()
{
    QDesktopWidget *screenResolution = QApplication::desktop();
    int appWindowWidth = this->geometry().width();
    int appWindowHeight = this->geometry().height();
    int center_y = screenResolution->height()/2 - appWindowHeight/2;
    int center_x = screenResolution->width()/2 - appWindowWidth/2;
    setGeometry(center_x, center_y, appWindowWidth,appWindowHeight);
}

void LightIndicator::setMapLogicItems(const QMap<QString, QStringList> &mapLogicItems)
{
    mapLogicItems_ = mapLogicItems;
}

void LightIndicator::setAlarmConclusion(const QString &alarmConclusion)
{
    alarmConclusion_ = alarmConclusion;
}

void LightIndicator::dispLightIndicator()
{
    QMap<LineEdit*,QList<LineEdit*> >::iterator itLineEdit = mapLineEdits_.begin();
    for( ; itLineEdit != mapLineEdits_.end(); ++itLineEdit)
    {
        LineEdit* lineEdit_Base = itLineEdit.key();
        QList<LineEdit*> listLineEdit = itLineEdit.value();
        for(int i = 0; i < listLineEdit.size(); ++i)
        {
            LineEdit *lineEdit_Original = listLineEdit.at(i);
            lineEdit_Original->deleteLater();
            lineEdit_Original = NULL;
        }
        lineEdit_Base->deleteLater();
        lineEdit_Base = NULL;
    }
    mapLineEdits_.clear();

    int classifier_AlarmConclusion = 0;
    QMap<QString , QStringList>::iterator it = mapLogicItems_.begin();
    for( int i = 0 ; it != mapLogicItems_.end(); ++it, ++i)
    {
        int classifier_Base = 0;

        QString BaseSignal= it.key();
        QStringList listLogicItems = it.value();

        if(BaseSignal.contains(QString::fromLocal8Bit("开关")))
        {
            for(int j = 0; j < listLogicItems.size(); ++j)
            {
                QVBoxLayout *OriginalSignalBoxLayout = new QVBoxLayout();     //原始信号垂直布局
                OriginalSignalBoxLayout->setSpacing(10);

                QString OriginalSignal = listLogicItems.at(j);
                //基准信号
                LineEdit *lineEdit_Base = new LineEdit(BaseSignal + " " + OriginalSignal.split(" ").value(1, "") + " " + OriginalSignal.split(" ").value(2, ""));
                int classifier_Original = SingletonConfig->getActionClassifier(mapActionClassifier_[it.key()].at(j));
                classifier_Base |= classifier_Original;

                LineEdit *lineEdit_Original = new LineEdit(OriginalSignal);
                lineEdit_Original->setColor(color_[classifier_Original]);
                OriginalSignalBoxLayout->addWidget(lineEdit_Original);

                mapLineEdits_[lineEdit_Base].push_back(lineEdit_Original);


                classifier_AlarmConclusion |= classifier_Base;

                //单个基准信号垂直布局，包含基准信号及其对应的原始信号
                QHBoxLayout *hBoxLayout = new QHBoxLayout();
                hBoxLayout->addLayout(OriginalSignalBoxLayout);
                hBoxLayout->setSpacing(50);
                lineEdit_Base->setColor(color_[classifier_Base]);
                hBoxLayout->addWidget(lineEdit_Base);

                baseSignalboxLayout->addLayout(hBoxLayout);
                baseSignalboxLayout->setSpacing(20);
            }
        }
        else
        {
            QString lastLogicItems = listLogicItems.last();   //取最后一个原始信号的时间，作为基准信号的时间

            //基准信号
            LineEdit *lineEdit_Base = new LineEdit(BaseSignal + " " + lastLogicItems.split(" ").value(1, "") + " " + lastLogicItems.split(" ").value(2, ""));

            QVBoxLayout *OriginalSignalBoxLayout = new QVBoxLayout();     //原始信号垂直布局
            OriginalSignalBoxLayout->setSpacing(10);
            for(int j = 0; j < listLogicItems.size(); ++j)
            {
                int classifier_Original = SingletonConfig->getActionClassifier(mapActionClassifier_[it.key()].at(j));
                classifier_Base |= classifier_Original;

                LineEdit *lineEdit_Original = new LineEdit(listLogicItems.at(j));

                lineEdit_Original->setColor(color_[classifier_Original]);
                OriginalSignalBoxLayout->addWidget(lineEdit_Original);

                mapLineEdits_[lineEdit_Base].push_back(lineEdit_Original);
            }

            classifier_AlarmConclusion |= classifier_Base;

            //单个基准信号垂直布局，包含基准信号及其对应的原始信号
            QHBoxLayout *hBoxLayout = new QHBoxLayout();
            hBoxLayout->addLayout(OriginalSignalBoxLayout);
            hBoxLayout->setSpacing(50);

            lineEdit_Base->setColor(color_[classifier_Base]);
            hBoxLayout->addWidget(lineEdit_Base);

            baseSignalboxLayout->addLayout(hBoxLayout);
            baseSignalboxLayout->setSpacing(20);
        }
    }

    //告警结论
    lineEdit_AlarmConclusion_->setText(alarmConclusion_);
    lineEdit_AlarmConclusion_->setColor(color_[classifier_AlarmConclusion]);

    update();
}


void LightIndicator::calcVertexes(double start_x, double start_y, double end_x, double end_y, double &x1, double &y1, double &x2, double &y2)
{
    double arrow_lenght_ = 10;//箭头长度，一般固定
    double arrow_degrees_ = 0.35;//箭头角度，一般固定

    double angle = atan2(end_y - start_y, end_x - start_x) + 3.1415926;//

    x1 = end_x + arrow_lenght_ * cos(angle - arrow_degrees_);//求得箭头点1坐标
    y1 = end_y + arrow_lenght_ * sin(angle - arrow_degrees_);
    x2 = end_x + arrow_lenght_ * cos(angle + arrow_degrees_);//求得箭头点2坐标
    y2 = end_y + arrow_lenght_ * sin(angle + arrow_degrees_);
}

void LightIndicator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 2));

    QMap<LineEdit*,QList<LineEdit*> >::iterator it = mapLineEdits_.begin();
    for( ; it != mapLineEdits_.end(); ++it)
    {
        LineEdit* baseLineEdit = it.key();
        QList<LineEdit*> listPointLineEdit = it.value();     //所有原始测点的LineEdit

        if(listPointLineEdit.size() > 1)
        {
            QPoint point(baseLineEdit->x() - 30, baseLineEdit->y() + 18);
            painter.drawText(point, QString::fromLocal8Bit("或"));
        }

        QPoint endPoint(baseLineEdit->x(), baseLineEdit->y() + baseLineEdit->height() / 2);
        for(int i = 0; i < it.value().size(); ++i)
        {
            QPoint startPoint(listPointLineEdit.at(i)->x() + baseLineEdit->width(), listPointLineEdit.at(i)->y() + listPointLineEdit.at(i)->height() / 2);
            double x1, y1, x2, y2; //箭头的两点坐标
            //求得箭头两点坐标
            calcVertexes(startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y(), x1, y1, x2, y2);

            painter.drawLine(startPoint, endPoint);//绘制线段
            painter.drawLine(endPoint.x(), endPoint.y(), x1, y1);//绘制箭头一半
            painter.drawLine(endPoint.x(), endPoint.y(), x2, y2);//绘制箭头另一半
        }

        double x1, y1, x2, y2; //箭头的两点坐标
        //求得箭头两点坐标
        calcVertexes(baseLineEdit->x() + baseLineEdit->width(), baseLineEdit->y() + it.key()->height() / 2, lineEdit_AlarmConclusion_->x(), lineEdit_AlarmConclusion_->y() + lineEdit_AlarmConclusion_->height()/ 2, x1, y1, x2, y2);

        painter.drawLine(baseLineEdit->x() + baseLineEdit->width(),
                         baseLineEdit->y() + it.key()->height() / 2,
                         lineEdit_AlarmConclusion_->x(),
                         lineEdit_AlarmConclusion_->y() + lineEdit_AlarmConclusion_->height()/ 2);

        painter.drawLine(lineEdit_AlarmConclusion_->x(), lineEdit_AlarmConclusion_->y() + lineEdit_AlarmConclusion_->height()/ 2, x1, y1);//绘制箭头一半
        painter.drawLine(lineEdit_AlarmConclusion_->x(), lineEdit_AlarmConclusion_->y() + lineEdit_AlarmConclusion_->height()/ 2, x2, y2);//绘制箭头另一半
    }

    if(mapLineEdits_.size() > 1)
    {
        QPoint point(lineEdit_AlarmConclusion_->x() - 30, lineEdit_AlarmConclusion_->y() + 18);
        painter.drawText(point, QString::fromLocal8Bit("与"));
    }


    QMap<LineEdit*,QList<LineEdit*> >::iterator itLineEdit = mapLineEdits_.begin();
    for( ; itLineEdit != mapLineEdits_.end(); ++itLineEdit)
    {
        LineEdit* baseLineEdit = itLineEdit.key();
        QList<LineEdit*> listPointLineEdit = itLineEdit.value();     //所有原始测点的LineEdit
        LineEdit* pointLineEdit = listPointLineEdit.value(0, NULL);

        if(pointLineEdit != NULL)
            painter.drawText(QPoint(pointLineEdit->x(), pointLineEdit->y() - 5), QString::fromLocal8Bit("原始信号"));

        painter.drawText(QPoint(baseLineEdit->x(), baseLineEdit->y() - 5), QString::fromLocal8Bit("基准信号"));

        painter.drawText(QPoint(lineEdit_AlarmConclusion_->x(), lineEdit_AlarmConclusion_->y() - 13), QString::fromLocal8Bit("结论"));
    }
}


void LightIndicator::setMapActionClassifier(const QMap<QString, QStringList> &mapActionClassifier)
{
    mapActionClassifier_ = mapActionClassifier;
}

