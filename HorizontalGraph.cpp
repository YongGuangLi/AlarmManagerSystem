#include "HorizontalGraph.h"
#include <QDebug>
#include <ctime>
#include <QTextOption>

bool SortLessByTime(HorizontalData* s1, HorizontalData* s2)
{
	return s1->GetMilliSeconds() < s2->GetMilliSeconds();
}


HorizontalGraph::HorizontalGraph(QWidget* parent /* = NULL */) 
	: QWidget(parent)
	, m_nMaxTextMetricsWidth(0)
	, m_nGraphWidth(0)
	, m_nGraphHeight(0)
{

#if 0 
	srand(time(NULL));
	QDateTime curDateTime = QDateTime::currentDateTime();
	for(int i = 0; i < 16; i++)
	{
		HorizontalData* pVal = new HorizontalData;
		QString strDateTime = curDateTime.toString(GRAPH_DATETIME_FORMAT);

		pVal->SetData(strDateTime, QString("110kV开关信号%1").arg(i));
		//if (i==2 || i == 7)
		{
			curDateTime = curDateTime.addSecs(-1);
		}
		qDebug() << strDateTime;
		m_listData.append(pVal);
	}
#endif
	
	m_bRedraw = true;
	m_nXBaseValue = 0;
	m_nYBaseValue = 20;
	m_nXAxisMargins = 10;
	m_nYAxisTopMargins = 0;
	m_nYAxisBottomMargins = 20;
	
	m_nYAxisMargins = 10;
	m_nMinPointSpace = 40;
	m_nYAxisHeight = 80;
	m_nFixedTextWidth = 0;

	m_nTextSpace = 100;
	m_fFontMetricsRatio = 0.8;
	m_nConstTextHeight = 30;
	m_nLeftSpace = 0;

	m_nVDelta = 0;
	m_nHDelta = 0;


	m_pPixmap = new QPixmap(this->width(), this->height());
	CreateScrollBars();

	CreateMenu();
}

HorizontalGraph::~HorizontalGraph()
{
	ReleaseData();
}
#include <QTextCodec>
void HorizontalGraph::CreateMenu()
{
    m_contextMenu = new QMenu();

	
    m_exportPixmapAction = new QAction(QString::fromUtf8(EXPORT_PIXMAP_TEXT), this);
	connect(m_exportPixmapAction, SIGNAL(triggered()), this, SLOT(ExportPixmap()));

	m_contextMenu->addAction(m_exportPixmapAction);
}

bool HorizontalGraph::InitData(QList<HorizontalData*>& listData, QString strFileName)
{
	ReleaseData();

	m_listData = listData;

	qSort(m_listData.begin(), m_listData.end(), SortLessByTime);
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
	m_strFileName = EXPORT_DEFAULT_PATH + strFileName.trimmed() + HORIZONTAL_GRAPH_FILE_TYPE + EXPORT_PIXMAP_FILE_SUFFIX;


	return true;
}

void HorizontalGraph::ReleaseData()
{
	qDeleteAll(m_listData);
	m_hashMapData.clear();
}

void HorizontalGraph::CreateScrollBars()
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

void HorizontalGraph::UpdateScrollBars()
{
	int nDifWidth = m_nGraphWidth - width();
	//qDebug() << "m_nGraphWidth: " << m_nGraphWidth << ", width: " << width() << ", dif: " << nDifWidth;
	if ( nDifWidth > 0 )
	{
		m_pHScrollBar->setRange(0, nDifWidth);
	}
	else
	{
		m_pHScrollBar->setRange(0, 0);
		m_nLeftSpace = -nDifWidth/2;
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

void HorizontalGraph::Render()
{
	m_bRedraw = false;
	m_pPixmap->fill(this, this->rect().topLeft());
	*m_pPixmap = m_pPixmap->scaled(this->width(), this->height());

	if ( m_listData.size() <= 0 )
	{
		return;
	}
	
	QPainter painter(m_pPixmap);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.save();
	
	//qDebug() << "this->fontMetrics().height(): " << this->fontMetrics().height();
	m_nTextPosX = m_nXBaseValue - m_nHDelta + m_nLeftSpace;
	m_nTextPosY = m_nYAxisTopMargins - m_nVDelta;

	m_nAxisPosX = m_nFixedTextWidth ;
	m_nAxisPosY = m_nTextPosY;

	bool bPaintXAxis =false;
	int nXAxisCanvasWidth;
	int nDataPointSize;
	int nKx;
	QSet<QString> setKey;
	setKey.clear();

	int nOrgCenterX, nOrgCenterY, nCenterY, nTextHeight;
	for(int nIndexGraphData = 0; nIndexGraphData < m_listData.size(); ++nIndexGraphData )
	{
		QPen penAxis;
		penAxis.setColor(Qt::black);
		penAxis.setWidth(2);
		painter.setPen(penAxis);

		HorizontalData* pVal = m_listData.at(nIndexGraphData);

		QString strPointName = pVal->GetPointName();
		QString strTime = pVal->GetStringDateTime();

		if (setKey.contains(strTime))
		{
			continue;
		}
		setKey.insert(strTime);

		
		QFontMetrics fm = this->fontMetrics();
		int nSingleTextHeight = this->fontMetrics().height()/m_fFontMetricsRatio;
		
		QList<QString> strListVal = m_hashMapData.values(strTime);
		int nTextLineCount = strListVal.size();
		nTextHeight = nSingleTextHeight*nTextLineCount + m_nConstTextHeight;

		int nMaxNameWidth = fm.width(strListVal.at(0));
		QString strMulName  = QString();
		for (int i = 0; i<nTextLineCount-1; i++)
		{
			QString strTempName = strListVal.at(i);
			strMulName += strTempName;
			strMulName += "\n";
			if ( fm.width(strTempName) > nMaxNameWidth )
			{
				nMaxNameWidth = fm.width(strTempName);
			}
		}
		strMulName += strListVal.at(nTextLineCount-1);

		int nTextTimeWidth = fm.width(strTime);
		
		painter.drawText(QRect(m_nTextPosX, m_nTextPosY, nTextTimeWidth, nTextHeight), Qt::AlignCenter, strTime);
		painter.drawText(QRect(m_nTextPosX + m_nTextSpace + nTextTimeWidth, m_nTextPosY, nMaxNameWidth, nTextHeight), Qt::AlignLeft| Qt::AlignVCenter, strMulName);
	
		nXAxisCanvasWidth = m_pPixmap->width();
		if(m_pVScrollBar->isVisible()) nXAxisCanvasWidth -= GetVScrollBarWidth();
		
		
		if( nIndexGraphData == 0)
		{
			nOrgCenterX = m_nTextPosX + nTextTimeWidth + m_nTextSpace/2;
			nOrgCenterY = m_nTextPosY + nTextHeight/2;
			painter.setBrush(QBrush(QColor(Qt::black)));
			painter.drawEllipse(QPoint(nOrgCenterX, nOrgCenterY), nSingleTextHeight/2, nSingleTextHeight/2 );

			nCenterY = nOrgCenterY;
		}
		else
		{
			painter.drawLine(nOrgCenterX, m_nTextPosY+nTextHeight/2 , nOrgCenterX -10,  m_nTextPosY+nTextHeight/2-10);
			painter.drawLine(nOrgCenterX, m_nTextPosY+nTextHeight/2 , nOrgCenterX +10,  m_nTextPosY+nTextHeight/2-10);
		}
		
		m_nTextPosY += nTextHeight;
		m_nAxisPosY = m_nTextPosY;

		painter.drawLine(m_nAxisPosX, m_nAxisPosY, nXAxisCanvasWidth, m_nAxisPosY);
	}

	
	painter.drawLine(nOrgCenterX, nOrgCenterY, nOrgCenterX, m_nTextPosY - nTextHeight/2);

	painter.restore();
}

void HorizontalGraph::paintEvent(QPaintEvent * event)
{
	//qDebug() << "--------------paintEvent event---------------------";
	if ( m_bRedraw )
	{
		Render();
	}
	
	QPainter painter(this);
	painter.drawPixmap(0, 0, *m_pPixmap);
}

void HorizontalGraph::resizeEvent(QResizeEvent * event)
{
	//qDebug() << "--------------resize event---------------------";
	QWidget::resizeEvent(event);
	UpdateScrollBars();

	m_bRedraw = true;
	Render();

}

void HorizontalGraph::HScrollBar(int nValue)
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

void HorizontalGraph::VScrollBar(int nValue)
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

void HorizontalGraph::wheelEvent(QWheelEvent * event)
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

void HorizontalGraph::contextMenuEvent(QContextMenuEvent * event)
{
	//qDebug() << "------------contextMenuEvent-------------";
	m_contextMenu->exec(QCursor::pos());
}

void HorizontalGraph::ExportPixmap()
{
	if ( GetPixmap() == NULL )
	{
		return;
	}

	if ( m_strFileName.contains("/") )
	{
		int nIndex = m_strFileName.lastIndexOf("/");
		QString strPath = m_strFileName.left(nIndex);
		//qDebug() << strPath;
		if (!QDir(strPath).exists())
		{
			QDir().mkpath(strPath);
		}
	}

    QString strText = QFileDialog::getSaveFileName(NULL, QString::fromUtf8(INPUT_DIALOG_TITLE), m_strFileName, tr("Image Files (*.png)"));// QInputDialog::getText(this, QString::fromUtf8(INPUT_DIALOG_TITLE), QString(EXPORT_FILE_PATH_NAME), QLineEdit::Normal, m_strFileName, &bOK);
	if (strText.isEmpty() || strText.isNull())
	{
		return;
	}
	//qDebug() << strText;
	
	/*if( QFile::exists(strText) )
	{
        if( QMessageBox::warning (this, QString::fromUtf8(EXPORT_PIXMAP_TEXT), QString(MESSAGEBOX_WARING_TEXT), QMessageBox::Ok|QMessageBox::Cancel) == QMessageBox::Cancel )
		{
			return;
		}
	}*/

	if( !GetPixmap()->save(strText, "PNG") )
		qDebug() << "export pixmap failed: " << strText;
}
