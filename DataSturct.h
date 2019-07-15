#ifndef DATASTURCT_H
#define DATASTURCT_H
#include <QString>
#include <QList>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>

#define GRAPH_DATETIME_FORMAT "yyyy-MM-dd hh:mm:ss.zzz"
#define GRAPH_DATE_FORMAT     "yyyy-MM-dd"
#define GRAPH_TIME_FORMAT     "hh:mm:ss.zzz"

#define EXPORT_PIXMAP_FILE_SUFFIX   ".png"
#define EXPORT_DEFAULT_PATH         "/home/IAReportPixmap/"

#define HORIZONTAL_GRAPH_FILE_TYPE  "_H"
#define VERTICAL_GRAPH_FILE_TYPE    "_V"
#define DIALOG_GRAPH_FILE_TYPE      "_D"

#define EXPORT_NAME_DATETIME_FORMAT "yyyyMMdd_hhmmss_zzz"

#define EXPORT_PIXMAP_TEXT   "导出图片"
#define INPUT_DIALOG_TITLE   "保存图片"
#define EXPORT_FILE_PATH_NAME "文件路径及名称："
#define MESSAGEBOX_WARING_TEXT "文件名已经存在，是否覆盖？"

#define POINT_TYPE_SIGNAL    "遥信"
#define POINT_TYPE_ANA       "遥测"


struct XYAxisValue
{
	QString strPointName;
	QList<int> listXAxisValue;
	QList<int> listYAxisValue;
};

struct XYData
{
	QString strDateTime;
	float fValue;
	QString strVal;
	qint64 nMilliSeconds; 
};

class XYAxisTimeValue
{
public:
	XYAxisTimeValue(){ SetPointType(POINT_TYPE_SIGNAL); }
	~XYAxisTimeValue(){}
	
	void SetPointName(QString strName){ m_strPointName = strName; }
	QString GetPointName() { return m_strPointName; }
	void SetPointType(QString strType){ m_strPointType = strType; }
	QString GetPointType() { return m_strPointType; }
	bool AppendXYValue(QString strXTime, double fValue )
	{
		QDateTime dt = QDateTime::fromString(strXTime.trimmed(), GRAPH_DATETIME_FORMAT);
		if ( !dt.isValid() )
		{
			qDebug() << "ERROR: from string to time: " << strXTime << "\t" << __FILE__ << "\t" << __LINE__;
			return false;
		}

		qint64 nKey = dt.toMSecsSinceEpoch();
		if ( m_hashXYAxisData.contains(nKey) )
		{
			qDebug() << "ERROR: dup time " << strXTime << "\t" << __FILE__ << "\t" << __LINE__;
			return false;
		}

		XYData* pData = new XYData;
		pData->strDateTime = strXTime.trimmed();
		pData->fValue = fValue;
		pData->nMilliSeconds = dt.toMSecsSinceEpoch();

	    m_hashXYAxisData.insert(nKey, pData);
		return true;
	}

	QHash<qint64, XYData*>& GetHashData(){ return m_hashXYAxisData; }

	void ReleaseData()
	{
		qDeleteAll(m_hashXYAxisData.begin(), m_hashXYAxisData.end());
		m_hashXYAxisData.clear();
	}
protected:
	QString m_strPointName;
	QHash<qint64, XYData*> m_hashXYAxisData;
	QString m_strPointType;
};

class HorizontalData
{
public:
	HorizontalData(){}
	~HorizontalData(){}

	bool SetData(QString strTime, QString strName)
	{
		QDateTime dt = QDateTime::fromString(strTime.trimmed(), GRAPH_DATETIME_FORMAT);
		if ( !dt.isValid() )
		{
			qDebug() << "ERROR: from string to time: " << strTime << "\t" << __FILE__ << "\t" << __LINE__;
			return false;
		}

		m_strDateTime = strTime.trimmed();
		m_strPointName = strName.trimmed();
		m_unDateTime = dt.toMSecsSinceEpoch();

		return true;
	}

	QString GetStringDateTime(){ return m_strDateTime; }
	QString GetPointName(){ return m_strPointName; }
	qint64 GetMilliSeconds(){ return m_unDateTime; }
protected:
	QString m_strDateTime;
	QString m_strPointName;
	qint64 m_unDateTime;
};

class DialogData
{
public:
	DialogData(){}
	~DialogData(){}

	bool SetData(QString strTime, QString strName, QString strVal)
	{
		QDateTime dt = QDateTime::fromString(strTime.trimmed(), GRAPH_DATETIME_FORMAT);
		if ( !dt.isValid() )
		{
			qDebug() << "ERROR: from string to time: " << strTime << "\t" << __FILE__ << "\t" << __LINE__;
			return false;
		}

		m_strDateTime = strTime.trimmed();
		m_strPointName = strName.trimmed();
		m_strVal = strVal.trimmed();
		m_unDateTime = dt.toMSecsSinceEpoch();

		return true;
	}

	QString GetStringDateTime(){ return m_strDateTime; }
	QString GetPointName(){ return m_strPointName; }
	QString GetVal(){ return m_strVal; }
	qint64 GetMilliSeconds(){ return m_unDateTime; }
protected:
	QString m_strDateTime;
	QString m_strPointName;
	QString m_strVal;
	qint64 m_unDateTime;
};

#endif