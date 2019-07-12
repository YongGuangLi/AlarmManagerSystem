#ifndef ModifyPasswdWindow_H
#define ModifyPasswdWindow_H

#include <QWidget>
#include <QSignalMapper>
#include <QPainter>
#include <QMovie>
#include <QMouseEvent>

#include "basewindow.h"
#include "confighelper.h"
#include "datamodel.h"
#include "myhelper.h"
#include "databasehelper.h"

namespace Ui {
class ModifyPasswdWindow;
}

class ModifyPasswdWindow : public BaseWindow
{
	Q_OBJECT

public:
	ModifyPasswdWindow(QWidget *parent = 0);
	~ModifyPasswdWindow();
    QSignalMapper *signalMapper;
signals:
    void rotateWindow();

private:
	void initWindow();

protected:
	void paintEvent(QPaintEvent *event);

    void showEvent(QShowEvent *);

private slots:
    void slot_btnOk_clicked();

private:
    Ui::ModifyPasswdWindow *ui;
	QPoint m_startMovePos;
};

#endif // ModifyPasswdWindow_H
