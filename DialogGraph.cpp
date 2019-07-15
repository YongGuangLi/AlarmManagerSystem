#include "DialogGraph.h"
#include <QDebug>
#include<ctime>
#include <QTextOption>

DialogGraph::DialogGraph(QWidget* parent /* = NULL */) 
	: QWidget(parent)
{

#if 0 
	srand(time(NULL));
	QDateTime curDateTime = QDateTime::currentDateTime();
	for(int i = 0; i < 16; i++)
	{
		DialogData* pVal = new DialogData;

		QDateTime dt1;
		if( i < 10 )
			dt1 = curDateTime.addSecs(i);
		else
			dt1 = curDateTime;
		QString strDateTime = dt1.toString(GRAPH_DATETIME_FORMAT);
		QString strVal;
		if( i%2 )
			strVal = QString("动作");
		else
			strVal = QString("复归");

		if( !pVal->SetData(strDateTime, QString("110kV开关信号%1").arg(i), strVal) )
			continue;

		m_listXYAxisVal.append(pVal);
	}
#endif

	m_bRedraw = true;
	m_nXTimeLeftBaseValue = 20;
	m_nYTimeTopBaseValue = 0;
	m_nConstTextHeight = 10;

	m_nXBaseValue = 0;
	m_nYBaseValue = 20;
	m_nXAxisMargins = 10;
	m_nYAxisTopMargins = 0;
	m_nYAxisBottomMargins = 0;
	m_nTextSpace = 10;
	m_nYAxisMargins = 10;
	m_nMinPointSpace = 80;
	m_nYAxisHeight = 80;
	m_nFixedTextWidth = 80;
	m_fFontMetricsRatio = 0.8;

	m_nVDelta = 0;
	m_nHDelta = 0;

	/*SaveDataToHash();
	m_nMaxTimeMetricsWidth = CalMaxTimeStringMetricsWidth();
	m_nMaxTextMetricsWidth = CalMaxTextMetricsWidth();
	m_nGraphWidth = CalGraphWidth();
	m_nGraphHeight = CalGraphHeigth();*/

	m_pPixmap = new QPixmap(this->width(), this->height());

	CreateScrollBars();

	CreateMenu();
}

DialogGraph::~DialogGraph()
{

}

void DialogGraph::CreateMenu()
{
    m_contextMenu = new QMenu();
	
    m_exportPixmapAction = new QAction(QString::fromUtf8(EXPORT_PIXMAP_TEXT), this);
	connect(m_exportPixmapAction, SIGNAL(triggered()), this, SLOT(ExportPixmap()));

	m_contextMenu->addAction(m_exportPixmapAction);
}

bool DialogGraph::InitData(QList<DialogData*>& listData, QString strFileName)
{
	ReleaseData();

	m_listXYAxisVal = listData;

	SaveDataToHash();

	m_nMaxTextMetricsWidth = CalMaxTextMetricsWidth();
	m_nGraphWidth = CalGraphWidth();
	m_nGraphHeight = CalGraphHeigth();

	UpdateScrollBars();
	m_bRedraw = true;
	repaint();

	if ( strFileName.isEmpty() || strFileName.isNull() )
	{
		strFileName = QDateTime::currentDateTime().toString(EXPORT_NAME_DATETIME_FORMAT);
	}
	m_strFileName = EXPORT_DEFAULT_PATH + strFileName.trimmed() + DIALOG_GRAPH_FILE_TYPE + EXPORT_PIXMAP_FILE_SUFFIX;

	return true;
}

void DialogGraph::ReleaseData()
{
	qDeleteAll(m_listXYAxisVal.begin(), m_listXYAxisVal.end());
	m_listXYAxisVal.clear();

	m_hashData.clear();
	m_listAllXAxisVal.clear();
}

void DialogGraph::CreateScrollBars()
{
	m_pHScrollBar = new QScrollBar(Qt::Horizontal, this);
	m_pVScrollBar = new QScrollBar(Qt::Vertical, this);
	m_pHScrollBar->hide();
	m_pVScrollBar->hide();
	m_pHScrollBar->setRange(0, 0);
	m_pVScrollBar->setRange(0, 0);
	UpdateScrollBars();

	connect(m_pHScrollBar, SIGNAL(valueChanged(int)), this, SLOT(HScrollBar(int)));
	connect(m_pVScrollBar, SIGNAL(valueChanged(int)), this, SLOT(VScrollBar(int)));
}

void DialogGraph::UpdateScrollBars()
{
	int nTimeWidth;
	if ( m_pVScrollBar->maximum() > 0 )
		nTimeWidth = this->width()/2 + m_nMaxTimeMetricsWidth + GetVScrollBarWidth() ;
	else
		nTimeWidth = this->width()/2 + m_nMaxTimeMetricsWidth;

	if ( nTimeWidth > this->width() )
	{
		m_nGraphWidth = nTimeWidth;
	}
	else
	{
		m_nGraphWidth = CalGraphWidth();
	}
	int nDifWidth = m_nGraphWidth - width();
	//qDebug() << "nTimeWidth: " << nTimeWidth;
	//qDebug() << "m_nGraphWidth: " << m_nGraphWidth << ", width: " << width() << ", dif: " << nDifWidth;
	if ( nDifWidth > 0 )
	{
		m_pHScrollBar->setRange(0, nDifWidth);
	}
	else
	{
		m_pHScrollBar->setRange(0, 0);
	}
	m_pHScrollBar->setVisible(m_pHScrollBar->maximum() > 0);

	int nDifHeight = m_nGraphHeight - height() + m_nYAxisBottomMargins;
	if ( nDifHeight > 0 )
	{
		if (m_pHScrollBar->maximum() > 0) nDifHeight += GetHScrollBarHeight();
		m_pVScrollBar->setRange(0, nDifHeight);

		int nPageStep = m_nGraphHeight/this->height();
		if( nPageStep )
			m_pVScrollBar->setPageStep(m_nGraphHeight/nPageStep);
		//qDebug() << "npagestep: " <<m_nGraphHeight/this->height();
		//qDebug() << "m_nGraphHeight/nPageStep: " << m_nGraphHeight/nPageStep;
	}
	else
	{
		m_pVScrollBar->setRange(0, 0);
	}
	m_pVScrollBar->setVisible(m_pVScrollBar->maximum() > 0);

	int nHeight = height();
	int nWidth = width();
	if (m_pHScrollBar->maximum() > 0)
	{
		nHeight -= GetHScrollBarHeight();

		if (m_pVScrollBar->maximum() > 0) 
		{
			nDifWidth += GetVScrollBarWidth();
			m_pHScrollBar->setRange(0, nDifWidth);
		}

		int nPageStep = m_nGraphWidth/this->width();
		if( nPageStep )
			m_pHScrollBar->setPageStep(m_nGraphWidth/nPageStep);
	}

	if (m_pVScrollBar->maximum() > 0)
		nWidth -= GetVScrollBarWidth();

	m_pHScrollBar->setGeometry(0, height() - GetHScrollBarHeight(), nWidth, GetHScrollBarHeight());
	m_pVScrollBar->setGeometry(width() - GetVScrollBarWidth(), 0, GetVScrollBarWidth(), nHeight-m_nYAxisBottomMargins);

}

void DialogGraph::Render()
{
	m_bRedraw = false;

	m_pPixmap->fill(this, this->rect().topLeft());
	*m_pPixmap = m_pPixmap->scaled(this->width(), this->height());

	if (m_listXYAxisVal.size() <= 0)
	{
		return;
	}

	QPainter painter(m_pPixmap);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.save();

	m_nTimePosX = m_pPixmap->width()/2 - m_nHDelta;
	m_nTimePosY = m_nYTimeTopBaseValue - m_nVDelta;

	m_nTextPosX = m_nXBaseValue - m_nHDelta;
	m_nTextPosY = m_nTimePosY;//m_nYAxisTopMargins - m_nVDelta;

	m_nAxisPosX = m_nTextPosX  + m_nFixedTextWidth ;
	m_nAxisPosY = m_nTextPosY;

	bool bPaintXAxis =false;
	int nXAxisCanvasWidth;
	int nDataPointSize;
	int nKx;
	for(int nIndexKey = 0; nIndexKey < m_listAllXAxisVal.size(); ++nIndexKey )
	{
		QPen penAxis;
		penAxis.setColor(Qt::black);
		penAxis.setWidth(1);
		painter.setPen(penAxis);

		qint64 nKey = m_listAllXAxisVal.at(nIndexKey);
		QList<DialogData*> listData = m_hashData.values(nKey);

		QString strTime = listData.first()->GetStringDateTime();
		QFontMetrics fm = this->fontMetrics();
		int nTextTimeWidth = fm.width(strTime);
		int nTextTimeHeight = fm.height()/m_fFontMetricsRatio;
		//painter.fillRect(QRect(m_nTimePosX, m_nTimePosY, nTextTimeWidth, nTextTimeHeight), Qt::red);
		//painter.setBrush(QBrush(Qt::red));
		painter.drawText(QRect(m_nTimePosX, m_nTimePosY, nTextTimeWidth, nTextTimeHeight), strTime);
		
		//qDebug() << QRect(m_nTimePosX, m_nTimePosY, nTextTimeWidth, nTextTimeHeight) << "\t" << strTime;
		nDataPointSize = listData.size();
		DialogData* pVal;
		
		int nTextPosX;
		

		m_nTextPosY = m_nTimePosY + m_nConstTextHeight;
		for(int nIndexPoint = 0; nIndexPoint < nDataPointSize; nIndexPoint++)
		{
			pVal = listData.at(nIndexPoint);
			QString strPointName = pVal->GetPointName();
			QString strVal = pVal->GetVal();

			int nPointNameWidth = this->fontMetrics().width(strPointName) + 20;
			if( nIndexKey % 2 == 0)
				nTextPosX = m_nTextPosX;
			else
			{
				nTextPosX = this->width() - nPointNameWidth;
				nTextPosX = m_pVScrollBar->maximum() ? nTextPosX - GetVScrollBarWidth() : nTextPosX;
			}

			m_nTextPosY += nTextTimeHeight;
			int nTextNameWidth = fm.width(strPointName) + 20;
			painter.fillRect(QRect(nTextPosX, m_nTextPosY, nTextNameWidth, nTextTimeHeight), Qt::cyan);
			painter.drawText(QRect(nTextPosX, m_nTextPosY, nTextNameWidth, nTextTimeHeight), strPointName);
			
			m_nTextPosY += nTextTimeHeight;
			int nTextValWidth = fm.width(strVal);
			painter.fillRect(QRect(nTextPosX, m_nTextPosY, nTextNameWidth, nTextTimeHeight), Qt::cyan);
			painter.drawText(QRect(nTextPosX + 10, m_nTextPosY, nTextValWidth, nTextTimeHeight), strVal);
		}

		m_nTimePosY = m_nTextPosY + nTextTimeHeight + m_nConstTextHeight;
		
	}
	
	painter.restore();
}

void DialogGraph::paintEvent(QPaintEvent * event)
{
	//qDebug() << "--------------paintEvent event---------------------";
	if ( m_bRedraw )
	{
		Render();
	}

	QPainter painter(this);
	painter.drawPixmap(0, 0, *m_pPixmap);
}

void DialogGraph::resizeEvent(QResizeEvent * event)
{
	//qDebug() << "--------------resize event---------------------";
	QWidget::resizeEvent(event);
	UpdateScrollBars();

	m_bRedraw = true;
	Render();

}

void DialogGraph::HScrollBar(int nValue)
{
	//qDebug() << "---------------------HScrollBar range: " << nValue;
	//qDebug() << m_pHScrollBar->minimum() << " " << m_pHScrollBar->maximum();

	if ( nValue > m_pHScrollBar->maximum())
	{
		nValue = m_pHScrollBar->maximum();
	}
	m_nHDelta = nValue;

	m_bRedraw = true;
	repaint();
}

void DialogGraph::VScrollBar(int nValue)
{
	//qDebug() << "VScrollBar range: " << nValue;
	if ( nValue > m_pVScrollBar->maximum())
	{
		nValue = m_pVScrollBar->maximum();
	}
	m_nVDelta = nValue;

	m_bRedraw = true;
	repaint();

}

void DialogGraph::wheelEvent(QWheelEvent * event)
{
	//qDebug() << "--------------wheelEvent---------------------" << event->delta();
	int nVBarValue = m_pVScrollBar->value();
	int nStepVal = m_pVScrollBar->pageStep()/2;
	if ( event->delta() < 0 )
	{
		m_pVScrollBar->setValue(nVBarValue + nStepVal);
	}
	else
	{
		m_pVScrollBar->setValue(nVBarValue - nStepVal);
	}

}

void DialogGraph::contextMenuEvent(QContextMenuEvent * event)
{
	//qDebug() << "------------contextMenuEvent-------------";
	m_contextMenu->exec(QCursor::pos());
}

void DialogGraph::ExportPixmap()
{
	if ( GetPixmap() == NULL )
	{
		return;
	}

	if ( m_strFileName.contains("/") )
	{
		int nIndex = m_strFileName.lastIndexOf("/");
		QString strPath = m_strFileName.left(nIndex);
		if (!QDir(strPath).exists())
		{
			QDir().mkpath(strPath);
		}
	}

    QString strText = QFileDialog::getSaveFileName(this, QString::fromUtf8(INPUT_DIALOG_TITLE), m_strFileName, tr("Image Files (*.png)"));// QInputDialog::getText(this, QString::fromUtf8(INPUT_DIALOG_TITLE), QString(EXPORT_FILE_PATH_NAME), QLineEdit::Normal, m_strFileName, &bOK);
	if (strText.isEmpty() || strText.isNull())
	{
		return;
	}

	if( !GetPixmap()->save(strText, "PNG") )
		qDebug() << "export pixmap failed: " << strText;
}
