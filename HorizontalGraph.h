#ifndef HORIZONTALGRAPH_H
#define HORIZONTALGRAPH_H
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QScrollBar>
#include <QFrame>
#include <QList>
#include <QFontMetrics>
#include <QDebug>
#include <QDateTime>
#include <QMenu>
#include <QInputDialog>
#include <QLineEdit>
#include <QDir>

#include "DataSturct.h"


class HorizontalGraph : public QWidget
{
	Q_OBJECT

public:
	HorizontalGraph(QWidget* parent = NULL);
	~HorizontalGraph();

	bool InitData(QList<HorizontalData*>& listData, QString strFileName);
	void ReleaseData();
	QPixmap* GetPixmap(){ return m_pPixmap; }

protected:
	void paintEvent(QPaintEvent * event);
	void resizeEvent(QResizeEvent * event);
	void wheelEvent ( QWheelEvent * event );
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
		for (int i = 0; i < m_listData.size(); i++)
		{
			HorizontalData* pVal = m_listData.at(i);
			nWidth = fm.width(pVal->GetPointName());
			if ( nWidth > nMaxVal )
			{
				nMaxVal = nWidth;
			}
		}

		return nMaxVal;
	}

	int CalMaxTimeStringMetricsWidth()
	{
		int nWidth = 0;
		int nMaxVal = 0;
		QFontMetrics fm = this->fontMetrics();
		for (int i = 0; i < m_listData.size(); i++)
		{
			HorizontalData* pVal = m_listData.at(i);
			nWidth = fm.width(pVal->GetStringDateTime());
			if ( nWidth > nMaxVal )
			{
				nMaxVal = nWidth;
			}
		}

		return nMaxVal;
	}

	int CalGraphWidth()
	{
		return  m_nTextSpace + CalMaxTimeStringMetricsWidth() + CalMaxTextMetricsWidth();
	}

	int CalGraphHeigth()
	{
		int nTextHeight = 0;
		int nTextLineCount = 0;
		QSet<QString> setKey;
		for(int nIndexGraphData = 0; nIndexGraphData < m_listData.size(); ++nIndexGraphData )
		{
			HorizontalData* pVal = m_listData.at(nIndexGraphData);
			if ( setKey.contains(pVal->GetStringDateTime()))
			{
				continue;
			}
			setKey.insert(pVal->GetStringDateTime());

			QString strPointName = pVal->GetPointName();
			QFontMetrics fm = this->fontMetrics();
			
			int nSingleTextHeight = this->fontMetrics().height()/m_fFontMetricsRatio;
			nTextLineCount = m_hashMapData.count(pVal->GetStringDateTime());
			nTextHeight += (nSingleTextHeight*nTextLineCount + m_nConstTextHeight);
		}

		return nTextHeight;
	}

	void SaveDataToHash()
	{
		for(int i = 0; i < m_listData.size(); ++i )
		{
			HorizontalData* pVal = m_listData.at(i);
			m_hashMapData.insertMulti(pVal->GetStringDateTime(), pVal->GetPointName());
		}

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
	int m_nConstTextHeight;

	int m_nTextPosX;
	int m_nTextPosY;
	int m_nAxisPosX;
	int m_nAxisPosY;
	int m_nFixedAxisPosX;
	int m_nFixedAxisPosY;
	int m_nLeftSpace;

	QList<HorizontalData*> m_listData;
	QHash<QString, QString> m_hashMapData;

	QMenu* m_contextMenu;
	QAction* m_exportPixmapAction;
	QString m_strFileName;
};

#endif