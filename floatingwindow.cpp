#include "floatingwindow.h"
#include "ui_floatingwindow.h"

FloatingWindow::FloatingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatingWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->installEventFilter(this);
}

FloatingWindow::~FloatingWindow()
{
    delete ui;
}



bool FloatingWindow::eventFilter(QObject *obj, QEvent *evt)
{
    QWidget *w = (QWidget *)obj;

    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonPress) {
        if (event->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = event->globalPos() - w->pos();
            return true;
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
        return true;
    } else if (event->type() == QEvent::MouseMove) {
        if (mousePressed && (event->buttons() && Qt::LeftButton)) {
            w->move(event->globalPos() - mousePoint);
            return true;
        }
    }

    return QObject::eventFilter(obj, evt);
}

