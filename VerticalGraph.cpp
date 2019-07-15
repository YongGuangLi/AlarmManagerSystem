#include "VerticalGraph.h"
#include "SmoothCurveGenerator1.h"
#include <QDebug>
#include<ctime>
#include <QTextOption>

VerticalGraph::VerticalGraph(QWidget* parent /* = NULL */) 
	: QWidget(parent)
{
	m_bRedawCursor = false;
	m_bRedraw = true;
	m_nXBaseValue = 0;
	m_nYBaseValue = 20;
	m_nXAxisMargins = 10;
	m_nYAxisTopMargins = 0;
	m_nYAxisBottomMargins = 40;
	m_nTextSpace = 5;
	m_nYAxisMargins = 10;
	m_nMinPointSpace = 80;
	m_nYAxisHeight = 80;
	m_nFixedTextWidth = 100;
	m_fFontMetricsRatio = 0.8;

	m_nVDelta = 0;
	m_nHDelta = 0;

#if 0 
	srand(time(NULL));
	QDateTime curDateTime = QDateTime::currentDateTime();
	for(int i = 0; i < 20; i++)
	{
		XYAxisTimeValue* pVal = new XYAxisTimeValue;
		pVal->SetPointName( QString("110kV开关aaaaaaaaaa信号%1").arg(i) );
		int nCount = 10;
		for(int j = 0; j < nCount; j++)
		{
			QDateTime dt1 = curDateTime.addSecs(j);
			QString strDateTime = dt1.toString(GRAPH_DATETIME_FORMAT);

			pVal->AppendXYValue(strDateTime, j%2);
		}
		m_listXYAxisVal.append(pVal);
	}

	InitAllXAxisValue();

	m_nMaxTextMetricsWidth = CalMaxTextMetricsWidth();
	m_nGraphWidth = CalGraphWidth();
	m_nGraphHeight = CalGraphHeigth();
#endif

	m_pPixmap = new QPixmap(this->width(), this->height());

	
	CreateScrollBars();

	CreateMenu();
}

VerticalGraph::~VerticalGraph()
{
	
}

void VerticalGraph::CreateMenu()
{
    m_contextMenu = new QMenu();

    m_exportPixmapAction = new QAction(QString::fromUtf8(EXPORT_PIXMAP_TEXT), this);
	connect(m_exportPixmapAction, SIGNAL(triggered()), this, SLOT(ExportPixmap()));

	m_contextMenu->addAction(m_exportPixmapAction);
}


bool VerticalGraph::InitData(QList<XYAxisTimeValue*>& listData, QString strFileName)
{
	ReleaseData();

	m_listXYAxisVal = listData;

	InitAllXAxisValue();
	

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
	m_strFileName = EXPORT_DEFAULT_PATH + strFileName.trimmed() + VERTICAL_GRAPH_FILE_TYPE + EXPORT_PIXMAP_FILE_SUFFIX;

	return true;
}

void VerticalGraph::ReleaseData()
{
	for(int i = 0; i < m_listXYAxisVal.size(); ++i)
	{
		XYAxisTimeValue* pVal = m_listXYAxisVal.at(i);
		pVal->ReleaseData();
	}

	qDeleteAll(m_listXYAxisVal.begin(), m_listXYAxisVal.end());
	m_listXYAxisVal.clear();
}


void VerticalGraph::CreateScrollBars()
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

void VerticalGraph::UpdateScrollBars()
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

void VerticalGraph::Render()
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
	
	//qDebug() << "this->fontMetrics().height(): " << this->fontMetrics().height();
	m_nTextPosX = m_nXBaseValue - m_nHDelta;
	m_nTextPosY = m_nYAxisTopMargins - m_nVDelta;

	m_nAxisPosX = m_nTextPosX  + m_nFixedTextWidth + 10 ;
	m_nAxisPosY = m_nTextPosY;

	
	bool bPaintXAxis =false;
	int nXAxisCanvasWidth;
	int nDataPointSize;
	int nKx;
	for(int nIndexGraphData = 0; nIndexGraphData < m_listXYAxisVal.size(); ++nIndexGraphData )
	{
		QPen penAxis;
		penAxis.setColor(Qt::black);
		penAxis.setWidth(1);
		painter.setPen(penAxis);

		XYAxisTimeValue* pVal = m_listXYAxisVal.at(nIndexGraphData);

		QString strPointName = pVal->GetPointName();
		QTextOption textOption(Qt::AlignLeft);
		textOption.setWrapMode(QTextOption::WordWrap);
		QFontMetrics fm = this->fontMetrics();
		int nTextWidth = fm.width(strPointName);
		double fTextLineCount = (double)nTextWidth/m_nFixedTextWidth;
		int nTextLineCount = fTextLineCount >(int)fTextLineCount ? (int)fTextLineCount + 1:(int)fTextLineCount;
		nTextLineCount++;
		//qDebug() << nTextWidth << " ntextLinecount: " << nTextLineCount << " fTextLineCount: " << fTextLineCount << " strPointName: " << strPointName;
		int nSingleTextHeight = this->fontMetrics().height()/m_fFontMetricsRatio;
		int nTextHeight = nSingleTextHeight*nTextLineCount +30;

		int nMaxPixmapHeight = this->height() - m_nYAxisBottomMargins;
		if(m_pHScrollBar->isVisible()) nMaxPixmapHeight -= GetHScrollBarHeight();
		if( m_nTextPosY+nTextHeight <= nMaxPixmapHeight )
			painter.drawText(QRect(m_nTextPosX, m_nTextPosY+15, m_nFixedTextWidth, nTextHeight), strPointName, textOption);
	
		nXAxisCanvasWidth = m_pPixmap->width();
		if(m_pVScrollBar->isVisible()) nXAxisCanvasWidth -= GetVScrollBarWidth();
		
		if( m_nAxisPosY+nTextHeight <= nMaxPixmapHeight )
		{
			painter.drawLine(m_nAxisPosX, m_nAxisPosY+ nTextHeight -10, nXAxisCanvasWidth, m_nAxisPosY+ nTextHeight-10);
			painter.drawText(m_nAxisPosX-10, m_nAxisPosY+ nTextHeight - 10, QString("0"));
		}

		if( m_nAxisPosY+nTextHeight <= nMaxPixmapHeight )
		{
			painter.drawLine(m_nAxisPosX, m_nAxisPosY, m_nAxisPosX, m_nAxisPosY + nTextHeight -10 );
			painter.drawText(m_nAxisPosX-10, m_nAxisPosY+ nTextHeight/2 - 10, QString("1"));
		}
		else
			painter.drawLine(m_nAxisPosX, m_nAxisPosY, m_nAxisPosX, nMaxPixmapHeight  );

		m_nTextPosY += nTextHeight;
		m_nAxisPosY = m_nTextPosY;

		int nLineCanvasWidth = m_pPixmap->width() - m_nAxisPosX;
		if(m_pVScrollBar->isVisible()) nLineCanvasWidth -= GetVScrollBarWidth();


		QHash<qint64, XYData*> hashData =pVal->GetHashData();
		nDataPointSize = hashData.size();
		if ( nDataPointSize < 1 )
		{
			//m_nTextPosY = m_nTextPosY + m_nYAxisHeight + m_nYAxisMargins;
			//m_nAxisPosY = m_nAxisPosY + m_nYAxisHeight + m_nYAxisMargins;
			continue;
		}

		QList<qint64> listKey = hashData.keys();
		qSort(listKey.begin(), listKey.end());
		int nDistance = listKey.last() - listKey.first() + 1;
		
		nKx = (int)nLineCanvasWidth/(m_listAllXAxisVal.size());
		if ( nKx < m_nMinPointSpace ) nKx = m_nMinPointSpace;
		
		QPen penLine;
		penLine.setColor(Qt::red);
		penLine.setWidth(3);
		painter.setPen(penLine);
		
		if( pVal->GetPointType() == POINT_TYPE_SIGNAL )
		{
			float nKy = nTextHeight/2;
			//qint64 nXVal;
			int nIndexVal;
			int nIndexNextVal;
			float fYVal;
			for(int nIndexPoint = 0; nIndexPoint < nDataPointSize - 1; nIndexPoint++)
			{
				//nXVal = listKey.at(nIndexPoint) - m_listAllXAxisVal.at(0);
				nIndexVal = m_listAllXAxisVal.indexOf(listKey.at(nIndexPoint));
				nIndexNextVal = m_listAllXAxisVal.indexOf(listKey.at(nIndexPoint+1));
				XYData* pData = hashData.value(listKey.at(nIndexPoint));
				fYVal = pData->fValue;

				if(  m_nAxisPosY - nKy * fYVal <= nMaxPixmapHeight )
					painter.drawLine(m_nAxisPosX + nIndexVal*nKx, m_nAxisPosY - nKy * fYVal - 10, m_nAxisPosX + nKx * nIndexNextVal, m_nAxisPosY - nKy * fYVal - 10);

				float fNextVal = hashData.value(listKey.at(nIndexPoint+1))->fValue;
				if( fYVal != fNextVal && ( (m_nAxisPosY  - nKy * fYVal) <= nMaxPixmapHeight && (m_nAxisPosY - nKy * fNextVal) <= nMaxPixmapHeight ) )
				{
					painter.drawLine(m_nAxisPosX + nIndexNextVal*nKx, m_nAxisPosY  - nKy * fYVal - 10, m_nAxisPosX + nKx* nIndexNextVal, m_nAxisPosY - nKy * fNextVal - 10);
				}
			}

			fYVal = hashData.value(listKey.at(nDataPointSize-1))->fValue;
			nIndexVal = m_listAllXAxisVal.indexOf(listKey.at(nDataPointSize-1));
			if( (m_nAxisPosY - nKy * fYVal) <= nMaxPixmapHeight  )
				painter.drawLine(m_nAxisPosX + nIndexVal*nKx, m_nAxisPosY - nKy * fYVal - 10, m_nAxisPosX + nKx* (nIndexVal+1), m_nAxisPosY - nKy * fYVal - 10);
			//qDebug() << strPointName << "\t" << m_nAxisPosX + nXVal*nKx * (nDataPointSize);
		}
        else if( pVal->GetPointType() == QString::fromLocal8Bit(POINT_TYPE_ANA))
		{
			QHash<qint64, XYData*> hashData =pVal->GetHashData();
			nDataPointSize = hashData.size();
			if ( nDataPointSize < 1 )
			{
				continue;
			}

			QList<qint64> listKey = hashData.keys();
			qSort(listKey.begin(), listKey.end());
			int nDistance = listKey.last() - listKey.first() + 1;

			nKx = (int)nLineCanvasWidth/(m_listAllXAxisVal.size());
			if ( nKx < m_nMinPointSpace ) nKx = m_nMinPointSpace;

			QPen penLine;
			penLine.setColor(Qt::red);
			penLine.setWidth(2);
			painter.setPen(penLine);

			float fMax = GetMaxVal(pVal);
			float fMin = GetMinVal(pVal);
			float fDiv = fMax;
			float fSub = 0;
			if(GetMinVal(pVal) < 0)
			{
				fDiv -= fMin;
				fSub = fMin;
			}

			float fKy = (nTextHeight-20)/fDiv;
			qint64 nXVal;
			float fYVal;
			float fNextVal;
			QList<QPointF> listPoints;
			for(int nIndexPoint = 0; nIndexPoint < nDataPointSize; nIndexPoint++)
			{
				nXVal = listKey.at(nIndexPoint) - listKey.at(0);
				XYData* pData = hashData.value(listKey.at(nIndexPoint));
				fYVal = pData->fValue - fSub;
				if(  m_nAxisPosY - fKy * fYVal <= nMaxPixmapHeight )
				{
					listPoints << QPointF(m_nAxisPosX + nKx * nIndexPoint, m_nAxisPosY - fKy * fYVal - 10);
					//qDebug() << m_nAxisPosY - fKy * fYVal << "\t" << m_nAxisPosY << "\t" << fKy << "\t" << fYVal;
					//painter.drawPoint(QPointF(m_nAxisPosX + nKx * nIndexPoint, m_nAxisPosY - fKy * fYVal));

					QPen penMaxMin;
					penMaxMin.setColor(Qt::darkGreen);
					painter.setPen(penMaxMin);
					if ( pData->fValue == fMax )
					{
						painter.drawText(m_nAxisPosX + nKx * nIndexPoint, m_nAxisPosY - fKy * fYVal-5, QString("Max:")+QString::number(fMax));
						//painter.drawText(pointx+kx*maxpos-kx,pointy-a[maxpos]*ky-5,QString("Max")+QString::number(_ma));
					}
					else if ( pData->fValue == fMin )
					{
						painter.drawText(m_nAxisPosX + nKx * nIndexPoint, m_nAxisPosY - fKy * fYVal-5, QString("Min:")+QString::number(fMin));
					}

					//painter.drawText(pointx+kx*minpos-kx,pointy-a[minpos]*ky+15,"Min"+QString::number(_mi));
				}
			}

			penLine.setColor(Qt::black);
			penLine.setWidth(2);
			painter.setPen(penLine);
			QPainterPath smoothCurve2 = SmoothCurveGenerator1::generateSmoothCurve(listPoints);
			painter.drawPath(smoothCurve2);
		}
	}
	

		

	
	QPen penText;
	penText.setColor(Qt::black);
	painter.setPen(penText);
	m_nFixedAxisPosX = m_nAxisPosX;
	m_nFixedAxisPosY = m_pPixmap->height() - m_nYAxisBottomMargins;
	if(m_pHScrollBar->maximum()) m_nFixedAxisPosY -= GetHScrollBarHeight();
	painter.drawLine(0, m_nFixedAxisPosY, nXAxisCanvasWidth, m_nFixedAxisPosY);
	int nXAxisDotCount = m_listAllXAxisVal.size();
	//qDebug() << "nXAxisDotCount: " << nXAxisDotCount;

	for(int nIndexPoint = 0; nIndexPoint < nXAxisDotCount; nIndexPoint++)
	{
		QDateTime dtTime = QDateTime::fromMSecsSinceEpoch(m_listAllXAxisVal.at(nIndexPoint));
		QString strXValue = dtTime.date().toString(GRAPH_DATE_FORMAT);
		strXValue += "\n";
		strXValue += dtTime.time().toString(GRAPH_TIME_FORMAT);
		QFontMetrics fm = painter.fontMetrics();
		int nTextWidth = fm.width(strXValue);

		painter.drawText( QRect(m_nFixedAxisPosX + nKx * nIndexPoint, m_nFixedAxisPosY, nTextWidth, fm.height()*2 ), strXValue);
	}
	

	painter.restore();
}

void VerticalGraph::paintEvent(QPaintEvent * event)
{
	//qDebug() << "--------------paintEvent event---------------------";
	if ( m_bRedraw )
	{
		Render();
	}
	
	QPainter painter(this);
	painter.drawPixmap(0, 0, *m_pPixmap);

	if ( m_bRedawCursor )
	{
		int nBottomHeight = m_pHScrollBar->maximum() > 0 ? m_nYAxisBottomMargins + GetHScrollBarHeight() : m_nYAxisBottomMargins;
		QPen penLine;
		penLine.setColor(Qt::magenta);
		penLine.setWidth(2);
		penLine.setStyle(Qt::DotLine);
		painter.setPen(penLine);
		painter.drawLine(m_pointMouse.x(), m_nYAxisTopMargins, m_pointMouse.x(), this->height() - nBottomHeight);
	}
	
}

void VerticalGraph::resizeEvent(QResizeEvent * event)
{
	//qDebug() << "--------------resize event---------------------";
	QWidget::resizeEvent(event);
	UpdateScrollBars();

	m_bRedraw = true;
	Render();

}

void VerticalGraph::HScrollBar(int nValue)
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

void VerticalGraph::VScrollBar(int nValue)
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

void VerticalGraph::wheelEvent(QWheelEvent * event)
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

void VerticalGraph::mousePressEvent(QMouseEvent * event)
{
	//qDebug() << "-----------------mousepressevent--------------";

	m_pointMouse = event->pos();
	if ( event->button() == Qt::LeftButton )
	{
		m_bRedawCursor = true;
	}
	else
	{
		m_bRedawCursor = false;
	}

	repaint();
}

void VerticalGraph::contextMenuEvent(QContextMenuEvent * event)
{
	//qDebug() << "------------contextMenuEvent-------------";
	m_contextMenu->exec(QCursor::pos());
}

void VerticalGraph::ExportPixmap()
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
