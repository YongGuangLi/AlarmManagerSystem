#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>
#include <QDebug>
#include <QColor>

enum ButtonType
{
	MIN_BUTTON = 0,			// 最小化和关闭按钮;
	MIN_MAX_BUTTON ,		// 最小化、最大化和关闭按钮;
	ONLY_CLOSE_BUTTON		// 只有关闭按钮;
};

class TitleBar : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent);
	~TitleBar();

	// 设置标题栏背景色;
	void setBackgroundColor(int r, int g, int b , bool isTransparent = false);
	// 设置标题栏图标;
	void setTitleIcon(QString filePath);
	// 设置标题内容;
    void setTitleContent(QString titleContent , int titleFontSize = 10);
	// 设置标题栏长度;
	void setTitleWidth(int width);
	// 设置标题栏上按钮类型;
	void setButtonType(ButtonType buttonType);
	// 设置标题栏中的标题是否会滚动;具体可以看效果;
	void setTitleRoll();
	// 设置窗口边框宽度;
	void setWindowBorderWidth(int borderWidth);

	// 保存/获取 最大化前窗口的位置及大小;
	void saveRestoreInfo(const QPoint point, const QSize size);
	void getRestoreInfo(QPoint& point, QSize& size);

	// 设置是否通过标题栏移动窗口;
	void setMoveParentWindowFlag(bool isMoveParentWindow);

private:
	void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

	// 初始化控件;
	void initControl();
	// 信号槽的绑定;
	void initConnections();
	// 加载样式文件;
	void loadStyleSheet(const QString &sheetName);

signals:
	// 按钮触发的信号;
    void signal_btnMin_Clicked();
    void signal_btnRestore_Clicked();
    void signal_btnMax_Clicked();
    void signal_btnClose_Clicked();

private slots:
	// 按钮触发的槽;
    void slot_btnMin_Clicked();
    void slot_btnRestore_Clicked();
    void slot_btnMax_Clicked();
    void slot_btnClose_Clicked();
	void onRollTitle();

private:
	QLabel* m_pIcon;					// 标题栏图标;
	QLabel* m_pTitleContent;			// 标题栏内容;
    QPushButton* m_btnMin;			// 最小化按钮;
    QPushButton* m_btnRestore;		// 最大化还原按钮;
    QPushButton* m_btnMax;			// 最大化按钮;
    QPushButton* m_btnClose;		// 关闭按钮;
	
	// 标题栏背景色;
	int m_colorR;
	int m_colorG;
	int m_colorB;

	// 最大化，最小化变量;
	QPoint m_restorePos;
    QSize m_restoreSize;
    // 移动窗口的变量;
    QPoint m_startMovePos;
	// 标题栏跑马灯效果时钟;
	QTimer m_titleRollTimer;
	// 标题栏内容;
	QString m_titleContent;
	// 按钮类型;
	ButtonType m_buttonType;
	// 窗口边框宽度;
	int m_windowBorderWidth;
	// 标题栏是否透明;
	bool m_isTransparent;
	// 是否通过标题栏移动窗口;
	bool m_isMoveParentWindow;
};

#endif // MYTITLEBAR_H
