#include "rotatewidget.h"
#include <QPropertyAnimation>

RotateWidget::RotateWidget(QWidget *parent)
	: QStackedWidget(parent)
	, m_isRoratingWindow(false)
	, m_nextPageIndex(0)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

	// 给窗口设置rotateValue属性;
	this->setProperty("rotateValue", 0);
	initRotateWindow();
}

RotateWidget::~RotateWidget()
{
}

// 初始化旋转的窗口;
void RotateWidget::initRotateWindow()
{
    m_loginWindow = new LoginWindow(this);
    m_loginWindow->setMoveParentWindow(true);
    connect(m_loginWindow->signalMapper, SIGNAL(mapped(int)), this, SLOT(onRotateWindow(int)));
    connect(m_loginWindow, SIGNAL(signal_btnClose_Clicked()), this, SLOT(hide()));

    m_loginNetSetWindow = new LoginNetSetWindow(this);
    m_loginNetSetWindow->setMoveParentWindow(true);
    connect(m_loginNetSetWindow->signalMapper, SIGNAL(mapped(int)), this, SLOT(onRotateWindow(int)));
    connect(m_loginNetSetWindow, SIGNAL(signal_btnClose_Clicked()), this, SLOT(hide()));

    m_modifyPasswdWindow = new ModifyPasswdWindow(this);
    m_modifyPasswdWindow->setMoveParentWindow(true);
    connect(m_modifyPasswdWindow->signalMapper, SIGNAL(mapped(int)), this, SLOT(onRotateWindow(int)));
    connect(m_modifyPasswdWindow, SIGNAL(signal_btnClose_Clicked()), this, SLOT(hide()));

	this->addWidget(m_loginWindow);
    this->addWidget(m_loginNetSetWindow);
    this->addWidget(m_modifyPasswdWindow);

    this->setFixedSize(QSize(m_loginWindow->width(), m_loginWindow->height()));
}

// 开始旋转窗口;
void RotateWidget::onRotateWindow(int i)
{
	// 如果窗口正在旋转，直接返回;
    if (m_isRoratingWindow)
		return;

	m_isRoratingWindow = true;
    m_nextPageIndex = i;

    QPropertyAnimation *rotateAnimation = new QPropertyAnimation(this, "rotateValue");
	// 设置旋转持续时间;
    rotateAnimation->setDuration(500);
	// 设置旋转角度变化趋势;
	rotateAnimation->setEasingCurve(QEasingCurve::InCubic);
	// 设置旋转角度范围;
	rotateAnimation->setStartValue(0);
	rotateAnimation->setEndValue(180);
	connect(rotateAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(repaint()));
	connect(rotateAnimation, SIGNAL(finished()), this, SLOT(onRotateFinished()));

	// 隐藏当前窗口，通过不同角度的绘制来达到旋转的效果;
	currentWidget()->hide();
	rotateAnimation->start();
}

// 旋转结束;
void RotateWidget::onRotateFinished()
{
	m_isRoratingWindow = false;
	setCurrentWidget(widget(m_nextPageIndex));
    repaint();
}


// 绘制旋转效果;
void RotateWidget::paintEvent(QPaintEvent* event)
{
    if (m_isRoratingWindow)
    {
        // 小于90度时;
        int rotateValue = this->property("rotateValue").toInt();
        if (rotateValue <= 90)
        {
            QPixmap rotatePixmap(currentWidget()->size());
            currentWidget()->render(&rotatePixmap);
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);
            QTransform transform;
            transform.translate(width() / 2, 0);
            transform.rotate(rotateValue, Qt::YAxis);
            painter.setTransform(transform);
            painter.drawPixmap(-1 * width() / 2, 0, rotatePixmap);
        }
        // 大于90度时
        else
        {
            QPixmap rotatePixmap(widget(m_nextPageIndex)->size());
            widget(m_nextPageIndex)->render(&rotatePixmap);
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);
            QTransform transform;
            transform.translate(width() / 2, 0);
            transform.rotate(rotateValue + 180, Qt::YAxis);
            painter.setTransform(transform);
            painter.drawPixmap(-1 * width() / 2, 0, rotatePixmap);
        }
    }
}
