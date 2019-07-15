#ifndef ANALYSISPCAPFILE_H
#define ANALYSISPCAPFILE_H

#include <QWidget>
#include <QList>
#include <StructClass.h>
#include <QTreeWidget>
#include <QTableWidget>
#include "QTextEdit"
#include <QFile>
#include <pcap.h>



namespace Ui {
class AnalysisPcapFile;
}

class AnalysisPcapFile : public QWidget
{
    Q_OBJECT
    
public:
    explicit AnalysisPcapFile(QWidget *parent = 0);
    ~AnalysisPcapFile();
private:
    QMap<QString ,FileInfo> m_filePaht;
    QMap<QString ,QTreeWidgetItem *> m_PcapNameItem;
  //  QMap<int,TableShowInfo*> m_pkgInfo;
    //显示报文名称Tree控件
    QTableWidget* m_PcapFileName;
    //显示一个文件中通信对表格数据
    QTableWidget* m_PcapHreadInfo;
    //显示条通讯对中的信息Tree
    QTreeWidget* m_PcapTreeinfo;
    //十六进制显示报文信息
    QTextEdit *m_PcapBinaryInfo;

    bool m_FileOrInfo;

    QMap<int ,PacpFileInfo> m_MapData;

    QString strCurFilePath;

    int  treeItemLevel;

    QMap<int,void*> m_TreeWidget;

    QTreeWidgetItem * PrentTreeItem;

public:
    //初始化wireshark，在new完这个里后面使用。
    void InitWireshark();
    //释放wireshark，在使用析构此类之前使用。
    void ReleaseWireshark();
    //设置文件到Tree空间中
    void SetPacpFileToTree( QList <QString> strFilePath);

    //报文件解析
    void TryDissect(const u_char * body,int iPackegLen,struct pcap_pkthdr *pkthdr);

    void PrintBuffer(void* pBuff, unsigned int nLen);

    void print_tree(proto_tree* tree, int level);

    void ClearDataInfo();
public slots:
    //单机文件名的响应事件
    void PcapFileName(int irow, int icolumn);

    //切换文件名称时候调用。释放本文件申请的控件。
    void checkColumnSlot(int irow, int icolumn);
public:
    void SetFileToTree(QList<QString> filePath);


private:
    bool GetFileName(FileInfo & fileinfo,QString filePath);
    void AddFileNameToTree();
    void ReadPcapFile(QString strFileFullName);
private:
    Ui::AnalysisPcapFile *ui;
};

#endif // ANALYSISPCAPFILE_H
