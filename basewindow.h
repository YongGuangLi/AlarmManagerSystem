#ifndef BASEWINDOW_H
#define BASEWINDOW_H


#include "titlebar.h"

#include <QDialog>
#include <QMouseEvent>

class BaseWindow : public QDialog
{
	Q_OBJECT

public:
	BaseWindow(QWidget *parent = 0);
    ~BaseWindow();

    void initTitleBar(QWidget *parent = 0, QString titleContent = "", QString titleICon = "", QColor color = (0,0,0), bool isTransparent = true);

    void setMoveParentWindow(bool isMoveParentWindow);

    void loadStyleSheet(const QString &sheetName);

    void setTitleContent(QString titleContent);
protected:
	void paintEvent(QPaintEvent *event);

    bool eventFilter(QObject *obj, QEvent *evt);

signals:
    void signal_btnClose_Clicked();

private slots:
    void slot_btnMin_Clicked();
    void slot_btnRestore_Clicked();
    void slot_btnMax_Clicked();

protected:
	TitleBar* m_titleBar;
    bool m_isMoveParentWindow;
	
};

#endif // BASEWINDOW_H
