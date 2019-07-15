#ifndef VERTICALGRAPH_H
#define VERTICALGRAPH_H
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QScrollBar>
#include <QFrame>
#include <QList>
#include <QFontMetrics>
#include <QDebug>
#include <QMenu>
#include <QInputDialog>
#include <QLineEdit>
#include <QDir>

#include "DataSturct.h"

class VerticalGraph : public QWidget
{
	Q_OBJECT

public:
	VerticalGraph(QWidget* parent = NULL);
	~VerticalGraph();

	bool InitData(QList<XYAxisTimeValue*>& listData, QString strFileName);
	void ReleaseData();
	QPixmap* GetPixmap(){ return m_pPixmap; }

protected:
	void paintEvent(QPaintEvent * event);
	void resizeEvent(QResizeEvent * event);
	void wheelEvent(QWheelEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void contextMenuEvent (QContextMenuEvent * event);
protected slots:
	void ExportPixmap();
protected:
	void CreateScrollBars();
	void UpdateScrollBars();
	
	void CreateMenu();

	void Render();
	
	int GetHScrollBarHeight()
	{
		return m_pHScrollBar->sizeHint().height();
	}

	int GetVScrollBarWidth()
	{
		return m_pVScrollBar->sizeHint().width();
	}

	int CalMaxTextMetricsWidth()
	{
		int nWidth = 0;
		int nMaxVal = 0;
		QFontMetrics fm = this->fontMetrics();
		for (int i = 0; i < m_listXYAxisVal.size(); i++)
		{
			XYAxisTimeValue* pVal = m_listXYAxisVal.at(i);
			nWidth = fm.width(pVal->GetPointName());
			if ( nWidth > nMaxVal )
			{
				nMaxVal = nWidth;
			}
		}

		return nMaxVal;
	}

	int CalGraphWidth()
	{
		return  m_listAllXAxisVal.size()*m_nMinPointSpace + m_nFixedTextWidth;
	}

	int CalGraphHeigth()
	{
		int nTextHeight = 0;
		for(int nIndexGraphData = 0; nIndexGraphData < m_listXYAxisVal.size(); ++nIndexGraphData )
		{
			XYAxisTimeValue* pVal = m_listXYAxisVal.at(nIndexGraphData);

			QString strPointName = pVal->GetPointName();
			
			QFontMetrics fm = this->fontMetrics();
			int nTextWidth = fm.width(strPointName);
			//qDebug() << "nTextWidth: " << nTextWidth;
			double fTextLineCount = (double)nTextWidth/m_nFixedTextWidth;
			int nTextLineCount = fTextLineCount >(int)fTextLineCount ? (int)fTextLineCount + 1:(int)fTextLineCount;
			nTextLineCount++;
			int nSingleTextHeight = this->fontMetrics().height()/m_fFontMetricsRatio;
			nTextHeight += nSingleTextHeight*nTextLineCount + 30;
		}

		return nTextHeight;
	}

	void InitAllXAxisValue()
	{
		m_listAllXAxisVal.clear();
		for (int i = 0; i < m_listXYAxisVal.size(); i++)
		{
			XYAxisTimeValue* pVal = m_listXYAxisVal.at(i);
			QHash<qint64, XYData*> hashData =pVal->GetHashData();
			QList<qint64> listKey = hashData.keys();
			for(int j = 0; j < listKey.size(); ++j)
			{
				qint64 nVal = listKey.at(j);
				if ( !m_listAllXAxisVal.contains(nVal) )
				{
					m_listAllXAxisVal.append(nVal);
				}
			}
		}

		qSort(m_listAllXAxisVal.begin(), m_listAllXAxisVal.end());
	}

	float GetMaxVal(XYAxisTimeValue* pTimeVal)
	{
		Q_ASSERT(pTimeVal);
		float fMaxVal;
		QHash<qint64, XYData*> hashData =pTimeVal->GetHashData();
		QList<XYData*> listData = hashData.values();
		for(int j = 0; j < listData.size(); ++j)
		{
			XYData* pData = listData.at(j);
			if (j==0)
			{
				fMaxVal = pData->fValue;
				continue;
			}

			if ( pData->fValue > fMaxVal )
			{
				fMaxVal = pData->fValue;
			}

		}

		return fMaxVal;
	}

	float GetMinVal(XYAxisTimeValue* pTimeVal)
	{
		Q_ASSERT(pTimeVal);
		float fMinVal;
		QHash<qint64, XYData*> hashData =pTimeVal->GetHashData();
		QList<XYData*> listData = hashData.values();
		for(int j = 0; j < listData.size(); ++j)
		{
			XYData* pData = listData.at(j);
			if (j==0)
			{
				fMinVal = pData->fValue;
				continue;
			}

			if (fMinVal >  pData->fValue)
			{
				fMinVal = pData->fValue;
			}

		}

		return fMinVal;
	}
protected slots:
	void HScrollBar(int nValue);
	void VScrollBar(int nValue);

private:
	QPixmap* m_pPixmap;
	QScrollBar* m_pHScrollBar;
	QScrollBar* m_pVScrollBar;
	int m_nVDelta;
	int m_nHDelta;

	bool m_bRedraw;
	bool m_bRedawCursor;
	int m_nXBaseValue;
	int m_nYBaseValue;
	int m_nMaxTextMetricsWidth;
	int m_nGraphWidth;
	int m_nGraphHeight;
	int m_nXAxisMargins;
	int m_nYAxisMargins;
	int m_nYAxisTopMargins;
	int m_nYAxisBottomMargins;
	int m_nMinPointSpace;
	int m_nYAxisHeight;
	int m_nTextSpace;
	int m_nFixedTextWidth;
	float m_fFontMetricsRatio;
	
	int m_nTextPosX;
	int m_nTextPosY;
	int m_nAxisPosX;
	int m_nAxisPosY;
	int m_nFixedAxisPosX;
	int m_nFixedAxisPosY;
	
	QPoint m_pointMouse;

	QList<XYAxisTimeValue*> m_listXYAxisVal;
	QList<qint64> m_listAllXAxisVal;

	QMenu* m_contextMenu;
	QAction* m_exportPixmapAction;
	QString m_strFileName;
};

#endif