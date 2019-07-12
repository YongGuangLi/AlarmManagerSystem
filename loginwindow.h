#ifndef LoginWindow_H
#define LoginWindow_H

#include "basewindow.h"
#include "confighelper.h"
#include "databasehelper.h"
#include "myhelper.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QListWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QMovie>
#include <QHBoxLayout>
#include <QCursor>
#include <QMenu>
#include <QDebug>
#include <QTime>
#include <QPropertyAnimation>
#include <QSignalMapper>

namespace Ui {
	class LoginWindow;
}


class LoginWindow : public BaseWindow
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = 0);
	~LoginWindow();

    QSignalMapper *signalMapper;

protected:
   void showEvent(QShowEvent *);

private:
    // 初始化窗口;
    void initWindow();

signals:
    void rotateWindow();

private slots:
    void slot_btnLogin_clicked();

private:
    Ui::LoginWindow *ui;

    QSqlDatabase db;

	// 翻转;
	QGraphicsScene m_scene;
	QGraphicsWidget *m_graphicsWidget;
	QGraphicsView m_view;

	QPoint m_startMovePos;
};

#endif // LoginWindow_H
