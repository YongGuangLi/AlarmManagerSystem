#ifndef FLOATINGWINDOW_H
#define FLOATINGWINDOW_H

#include "alarmdata.h"
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QDebug>

namespace Ui {
class FloatingWindow;
}

class FloatingWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit FloatingWindow(QWidget *parent = 0);
    ~FloatingWindow();
    
protected:
    bool eventFilter(QObject *obj, QEvent *evt);

private slots:

private:
    Ui::FloatingWindow *ui;
    QTimer *timer;
};

#endif // FLOATINGWINDOW_H
