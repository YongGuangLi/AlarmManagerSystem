#ifndef DIALOGGRAPH_H
#define DIALOGGRAPH_H
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

class DialogGraph : public QWidget
{
	Q_OBJECT

public:
	DialogGraph(QWidget* parent = NULL);
	~DialogGraph();

	bool InitData(QList<DialogData*>& listData, QString strFileName);
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
		for (int i = 0; i < m_listXYAxisVal.size(); i++)
		{
			DialogData* pVal = m_listXYAxisVal.at(i);
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
		for (int i = 0; i < m_listXYAxisVal.size(); i++)
		{
			DialogData* pVal = m_listXYAxisVal.at(i);
			nWidth = fm.width(pVal->GetStringDateTime());
			if ( nWidth > nMaxVal )
			{
				nMaxVal = nWidth;
			}
		}
		//qDebug() << "time: " << nMaxVal;
		return nMaxVal;
	}

	int CalGraphWidth()
	{
		//qDebug() <<"----------- " << CalMaxTimeStringMetricsWidth();
		return  m_nTextSpace + CalMaxTextMetricsWidth();
	}

	int CalGraphHeigth()
	{
		int nTextHeight = 0;
		int nSingleTextHeight = this->fontMetrics().height()/m_fFontMetricsRatio;
		for(int nIndexKey = 0; nIndexKey < m_listAllXAxisVal.size(); ++nIndexKey )
		{
			qint64 nKey = m_listAllXAxisVal.at(nIndexKey);
			QList<DialogData*> listData = m_hashData.values(nKey);
			int nDataPointSize = listData.size();

			nTextHeight += (m_nYTimeTopBaseValue + m_nConstTextHeight);
			for(int nIndexPoint = 0; nIndexPoint < nDataPointSize; nIndexPoint++)
			{
				nTextHeight += (nSingleTextHeight *2);
			}

			nTextHeight += (nSingleTextHeight + m_nConstTextHeight);
		}

		return nTextHeight;
	}

	void SaveDataToHash()
	{
		m_listAllXAxisVal.clear();
		for (int i = 0; i < m_listXYAxisVal.size(); i++)
		{
			DialogData* pVal = m_listXYAxisVal.at(i);
			m_hashData.insertMulti(pVal->GetMilliSeconds(), pVal);

			if( !m_listAllXAxisVal.contains(pVal->GetMilliSeconds()) )
				m_listAllXAxisVal.append(pVal->GetMilliSeconds());
			
		}

		qSort(m_listAllXAxisVal.begin(), m_listAllXAxisVal.end());
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
	int m_nXTimeLeftBaseValue;
	int m_nYTimeTopBaseValue;

	int m_nXBaseValue;
	int m_nYBaseValue;
	int m_nMaxTextMetricsWidth;
	int m_nMaxTimeMetricsWidth;
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

	int m_nTimePosX;
	int m_nTimePosY;
	int m_nTextPosX;
	int m_nTextPosY;
	int m_nAxisPosX;
	int m_nAxisPosY;
	int m_nFixedAxisPosX;
	int m_nFixedAxisPosY;

	int m_nConstTextHeight;

	QList<DialogData*> m_listXYAxisVal;
	QList<qint64> m_listAllXAxisVal;
	QHash<qint64, DialogData*> m_hashData;

	QMenu* m_contextMenu;
	QAction* m_exportPixmapAction;
	QString m_strFileName;
};

#endif