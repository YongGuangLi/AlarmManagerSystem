#include "titlebar.h"

#define BUTTON_HEIGHT 30		// 按钮高度;
#define BUTTON_WIDTH 30			// 按钮宽度;
#define TITLE_HEIGHT 30			// 标题栏高度;

TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent)
	, m_colorR(153)
	, m_colorG(153)
    , m_colorB(153)
    , m_buttonType(MIN_MAX_BUTTON)
	, m_windowBorderWidth(0)
	, m_isTransparent(false)
	, m_isMoveParentWindow(true)
{
	// 初始化;
	initControl();
	initConnections();
    loadStyleSheet(":/Resources/qss/MyTitle.css");

    // 这里需要设置成false，不允许通过标题栏拖动来移动窗口位置,否则会造成窗口位置错误;
    this->setMoveParentWindowFlag(false);
}

TitleBar::~TitleBar()
{

}

// 初始化控件;
void TitleBar::initControl()
{
	m_pIcon = new QLabel;
	m_pTitleContent = new QLabel;

    m_btnMin = new QPushButton;
    m_btnRestore = new QPushButton;
    m_btnMax = new QPushButton;
    m_btnClose = new QPushButton;

    m_btnMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_btnRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_btnMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_btnClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	m_pTitleContent->setObjectName("TitleContent");
    m_btnMin->setObjectName("ButtonMin");
    m_btnRestore->setObjectName("ButtonRestore");
    m_btnMax->setObjectName("ButtonMax");
    m_btnClose->setObjectName("ButtonClose");

	QHBoxLayout* mylayout = new QHBoxLayout(this);
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);

    mylayout->addWidget(m_btnMin);
    mylayout->addWidget(m_btnRestore);
    mylayout->addWidget(m_btnMax);
    mylayout->addWidget(m_btnClose);

    mylayout->setContentsMargins(5, 0, 0, 0);
    mylayout->setSpacing(0);

	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	this->setFixedHeight(TITLE_HEIGHT);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

// 信号槽的绑定;
void TitleBar::initConnections()
{
    connect(m_btnMin, SIGNAL(clicked()), this, SLOT(slot_btnMin_Clicked()));
    connect(m_btnRestore, SIGNAL(clicked()), this, SLOT(slot_btnRestore_Clicked()));
    connect(m_btnMax, SIGNAL(clicked()), this, SLOT(slot_btnMax_Clicked()));
    connect(m_btnClose, SIGNAL(clicked()), this, SLOT(slot_btnClose_Clicked()));
}

// 设置标题栏背景色,在paintEvent事件中进行绘制标题栏背景色;
//在构造函数中给了默认值，可以外部设置颜色值改变标题栏背景色;
void TitleBar::setBackgroundColor(int r, int g, int b, bool isTransparent)
{
	m_colorR = r;
	m_colorG = g;
	m_colorB = b;
	m_isTransparent = isTransparent;
	// 重新绘制（调用paintEvent事件）;
	update();
}

// 设置标题栏图标;
void TitleBar::setTitleIcon(QString filePath)
{
	QPixmap titleIcon(filePath);
    m_pIcon->setPixmap(titleIcon.scaled(18 , 18));
}

// 设置标题内容;
void TitleBar::setTitleContent(QString titleContent, int titleFontSize)
{
	// 设置标题字体大小;
	QFont font = m_pTitleContent->font();
	font.setPointSize(titleFontSize);
	m_pTitleContent->setFont(font);
	// 设置标题内容;
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;
}

// 设置标题栏长度;
void TitleBar::setTitleWidth(int width)
{
	this->setFixedWidth(width);
}

// 设置标题栏上按钮类型;
// 由于不同窗口标题栏上的按钮都不一样，所以可以自定义标题栏中的按钮;
// 这里提供了四个按钮，分别为最小化、还原、最大化、关闭按钮，如果需要其他按钮可自行添加设置;
void TitleBar::setButtonType(ButtonType buttonType)
{
	m_buttonType = buttonType;

	switch (buttonType)
	{
	case MIN_BUTTON:
		{
            m_btnRestore->setVisible(false);
            m_btnMax->setVisible(false);
		}
		break;
	case MIN_MAX_BUTTON:
		{
            m_btnRestore->setVisible(false);
		}
		break;
	case ONLY_CLOSE_BUTTON:
		{
            m_btnMin->setVisible(false);
            m_btnRestore->setVisible(false);
            m_btnMax->setVisible(false);
		}
		break;
	default:
		break;
	}
}

// 设置标题栏中的标题是否会自动滚动，跑马灯的效果;
// 一般情况下标题栏中的标题内容是不滚动的，但是既然自定义就看自己需要嘛，想怎么设计就怎么搞O(∩_∩)O！
void TitleBar::setTitleRoll()
{
	connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(onRollTitle()));
	m_titleRollTimer.start(200);
}

void TitleBar::setWindowBorderWidth(int borderWidth)
{
	m_windowBorderWidth = borderWidth;
}

// 保存窗口最大化前窗口的位置以及大小;
void TitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
	m_restorePos = point;
	m_restoreSize = size;
}

// 获取窗口最大化前窗口的位置以及大小;
void TitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
	point = m_restorePos;
	size = m_restoreSize;
}

// 设置是否通过标题栏移动窗口;
void TitleBar::setMoveParentWindowFlag(bool isMoveParentWindow)
{
	m_isMoveParentWindow = isMoveParentWindow;
}

// 绘制标题栏背景色;
void TitleBar::paintEvent(QPaintEvent *event)
{
	if (!m_isTransparent)
	{
		//设置背景色;
		QPainter painter(this);
		QPainterPath pathBack;
		pathBack.setFillRule(Qt::WindingFill);
        pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 0, 0);
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.fillPath(pathBack, QBrush(QColor(m_colorR, m_colorG, m_colorB)));
	}	

	// 当窗口最大化或者还原后，窗口长度变了，标题栏的长度应当一起改变;
	if (this->width() != (this->parentWidget()->width() - m_windowBorderWidth))
	{
		this->setFixedWidth(this->parentWidget()->width() - m_windowBorderWidth);
	}

    QWidget::paintEvent(event);
}

// 双击响应事件，主要是实现双击标题栏进行最大化和最小化操作;
void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
	// 只有存在最大化、还原按钮时双击才有效;
	if (m_buttonType == MIN_MAX_BUTTON)
	{
		// 通过最大化按钮的状态判断当前窗口是处于最大化还是原始大小状态;
		// 或者通过单独设置变量来表示当前窗口状态;
        if (m_btnMax->isVisible())
		{
            slot_btnMax_Clicked();
		}
		else
		{
            slot_btnRestore_Clicked();
		}
	}	

	return QWidget::mouseDoubleClickEvent(event);
}


// 加载本地样式文件;
// 可以将样式直接写在文件中，程序运行时直接加载进来;
void TitleBar::loadStyleSheet(const QString &sheetName)
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

// 以下为按钮操作响应的槽;
void TitleBar::slot_btnMin_Clicked()
{
    emit signal_btnMin_Clicked();
}

void TitleBar::slot_btnRestore_Clicked()
{
    m_btnRestore->setVisible(false);
    m_btnMax->setVisible(true);
    emit signal_btnRestore_Clicked();
}

void TitleBar::slot_btnMax_Clicked()
{
    m_btnMax->setVisible(false);
    m_btnRestore->setVisible(true);
    emit signal_btnMax_Clicked();
}

void TitleBar::slot_btnClose_Clicked()
{
    emit signal_btnClose_Clicked();
}

// 该方法主要是让标题栏中的标题显示为滚动的效果;
void TitleBar::onRollTitle()
{
	static int nPos = 0;
	QString titleContent = m_titleContent;
	// 当截取的位置比字符串长时，从头开始;
	if (nPos > titleContent.length())
		nPos = 0;

	m_pTitleContent->setText(titleContent.mid(nPos));
	nPos++;
}
