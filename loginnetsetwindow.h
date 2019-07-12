#ifndef LOGINNETSETWINDOW_H
#define LOGINNETSETWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QMovie>
#include <QMouseEvent>
#include <QSignalMapper>
#include "basewindow.h"
#include "confighelper.h"
#include "CommonObject.h"

namespace Ui {
    class LoginNetSetWindow;
}

class LoginNetSetWindow : public BaseWindow
{
	Q_OBJECT

public:
	LoginNetSetWindow(QWidget *parent = 0);
	~LoginNetSetWindow();
     QSignalMapper *signalMapper;
signals:
    void rotateWindow(int);

private:
	void initWindow();

protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *);

private slots:
    void slot_btnOk_clicked();
private:
    Ui::LoginNetSetWindow *ui;
	QPoint m_startMovePos;
};

#endif // LOGINNETSETWINDOW_H
