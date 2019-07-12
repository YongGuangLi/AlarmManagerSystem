#include "basewindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QFile>
#include <QStyleOption>
#include <QGraphicsDropShadowEffect>

BaseWindow::BaseWindow(QWidget *parent)
	: QDialog(parent)
{
	// FramelessWindowHint属性设置窗口去除边框;
	// WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
	//Qt::WindowFlags flag = this->windowFlags();
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	// 设置窗口背景透明;
    //setAttribute(Qt::WA_TranslucentBackground);
	// 关闭窗口时释放资源;
    //setAttribute(Qt::WA_DeleteOnClose);

    this->installEventFilter(this);
    m_isMoveParentWindow = false;
}

BaseWindow::~BaseWindow()
{

}

void BaseWindow::initTitleBar(QWidget* parent,QString titleContent, QString titleICon, QColor color,bool isTransparent)
{
    m_titleBar = new TitleBar(parent);

    m_titleBar->move(0, 0);
    m_titleBar->setBackgroundColor(color.red(), color.green(), color.blue(), isTransparent);
    m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
    m_titleBar->setTitleWidth(parent->width());
    if(!titleICon.isEmpty())
        m_titleBar->setTitleIcon(titleICon);
    m_titleBar->setTitleContent(titleContent);
    m_titleBar->raise();
	
    connect(m_titleBar, SIGNAL(signal_btnMin_Clicked()), this, SLOT(slot_btnMin_Clicked()));
    connect(m_titleBar, SIGNAL(signal_btnRestore_Clicked()), this, SLOT(slot_btnRestore_Clicked()));
    connect(m_titleBar, SIGNAL(signal_btnMax_Clicked()), this, SLOT(slot_btnMax_Clicked()));
    connect(m_titleBar, SIGNAL(signal_btnClose_Clicked()), this, SIGNAL(signal_btnClose_Clicked()));
}

void BaseWindow::setMoveParentWindow(bool isMoveParentWindow)
{
    m_isMoveParentWindow = isMoveParentWindow;
}



bool BaseWindow::eventFilter(QObject *obj, QEvent *evt)
{
    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonPress)
    {
        if (event->button() == Qt::LeftButton)
        {
            mousePressed = true;
            if(m_isMoveParentWindow)
                mousePoint = event->globalPos() - this->parentWidget()->pos();
            else
                mousePoint = event->globalPos() - this->pos();

            return true;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        mousePressed = false;
        return true;
    }
    else if (event->type() == QEvent::MouseMove)
    {
        if (mousePressed && (event->buttons() && Qt::LeftButton))
        {
            if(m_isMoveParentWindow)
                this->parentWidget()->move(event->globalPos() - mousePoint);
            else
                this->move(event->globalPos() - mousePoint);

            return true;
        }
    }

    return QObject::eventFilter(obj, evt);
}


void BaseWindow::paintEvent(QPaintEvent* event)
{
	// blurRadius 阴影清晰度，越小越清晰
	// dx x方向阴影位置和偏移量，正值在右方，值越大偏移越大
    // dy y方向阴影位置和偏移量，正值在下方，值越大偏移越大
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    //设置背景色;
	
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(pathBack, QBrush(QColor(236, 241, 245)));
}

void BaseWindow::loadStyleSheet(const QString &sheetName)
{
	QFile file(sheetName);
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
        file.close();
    }
}

void BaseWindow::setTitleContent(QString titleContent)
{
    m_titleBar->setTitleContent(titleContent);
}

void BaseWindow::slot_btnMin_Clicked()
{
	if (Qt::Tool == (windowFlags() & Qt::Tool))
	{
		hide();    //设置了Qt::Tool 如果调用showMinimized()则窗口就销毁了？？？
	}
	else
	{
		showMinimized();
	}
}


void BaseWindow::slot_btnRestore_Clicked()
{
    QPoint windowPos;
    QSize windowSize;
    m_titleBar->getRestoreInfo(windowPos, windowSize);
    this->setGeometry(QRect(windowPos, windowSize));
}

void BaseWindow::slot_btnMax_Clicked()
{
    m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
    QRect desktopRect = QApplication::desktop()->availableGeometry();
    QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
    setGeometry(FactRect);
}

