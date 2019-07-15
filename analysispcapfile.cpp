#include "analysispcapfile.h"
#include "ui_analysispcapfile.h"
#include "time.h"
#include <QFileInfo>
#include <QDebug>




AnalysisPcapFile::AnalysisPcapFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalysisPcapFile)
{
    ui->setupUi(this);
    //设置界面
    ui->splitter->setHandleWidth(3);
    ui->splitter_3->setHandleWidth(3);
    ui->splitter->setHandleWidth(3);
    ui->splitter->setStretchFactor(0, 2);
    ui->splitter->setStretchFactor(1, 8);
    ui->splitter_3->setStretchFactor(0, 4);
    ui->splitter_3->setStretchFactor(1, 6);
    ui->splitter_2->setStretchFactor(0, 6);
    ui->splitter_2->setStretchFactor(1, 4);
    ui->tabWidget->setTabText(0,QString::fromLocal8Bit("报文解析"));


    m_PcapFileName = ui->PcapFileName;
    m_PcapFileName->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_PcapFileName->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_PcapFileName->verticalHeader()->hide();
    m_PcapFileName->horizontalHeader()->setStretchLastSection(true);
    connect(m_PcapFileName,SIGNAL(cellClicked(int, int)), this,SLOT(PcapFileName (int, int )));


    m_PcapHreadInfo = ui->PcapHreadInfo;
    m_PcapHreadInfo->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_PcapHreadInfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_PcapHreadInfo = ui->PcapHreadInfo;
    m_PcapHreadInfo->setColumnCount(7);
    m_PcapHreadInfo->setSelectionBehavior ( QAbstractItemView::SelectRows);
   // m_PcapHreadInfo->horizontalHeader()->setClickable(false); //设置表头不可点击（默认点击后进行排序）
    m_PcapHreadInfo->verticalHeader()->setDefaultSectionSize(20); //设置行高
    m_PcapHreadInfo->setFrameShape(QFrame::NoFrame); //设置无边框
   // m_PcapHreadInfo->setShowGrid(false); //设置不显示格子线
    m_PcapHreadInfo->horizontalHeader()->setStretchLastSection(true);
    m_PcapHreadInfo->horizontalHeader()->setVisible(true); //设置垂直头不可见
    m_PcapHreadInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_PcapHreadInfo->horizontalHeader()->setStretchLastSection(true);
    connect(m_PcapHreadInfo, SIGNAL(cellClicked(int, int)),this, SLOT(checkColumnSlot(int, int )));
    QStringList header;  //QString类型的List容器
    header<<"时间"<<"源地址"<<"目标地址"<<"协议"<<"长度"<<"端口"<<"描述";
    m_PcapHreadInfo->setHorizontalHeaderLabels(header);


    m_PcapTreeinfo =  ui->PcapTreeinfo;
    m_PcapTreeinfo->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_PcapTreeinfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    InitWireshark();

    m_FileOrInfo = true;

/*****************测试代码************************/




    QList <QString> test;
    SetPacpFileToTree(test);

    treeItemLevel = -1 ;





    //m_pkgInfo.clear();

/*************************************************/

   m_PcapBinaryInfo = ui->PcapBinaryInfo;

   m_PcapBinaryInfo->setReadOnly(true);
}

AnalysisPcapFile::~AnalysisPcapFile()
{
    delete ui;
}

void AnalysisPcapFile::InitWireshark()
{
    init_process_policies();
    epan_init(register_all_protocols, register_all_protocol_handoffs,NULL, NULL, NULL, NULL, NULL,NULL);
    init_dissection();
}

void AnalysisPcapFile::ReleaseWireshark()
{
    cleanup_dissection();
    epan_cleanup();
    ClearDataInfo();
}

void AnalysisPcapFile::SetPacpFileToTree( QList <QString> strFilePath)
{
    //----------------------------------ce shi dai ma ----------------------------------
    strFilePath.append("/home/Progect/DataPackageAnalysis/mmstest.pcap");
    strFilePath.append("/home/Progect/DataPackageAnalysis/mmstest1.pcap");
    strFilePath.append("/home/Progect/DataPackageAnalysis/mmstest2.pcap");
    strFilePath.append("/home/Progect/DataPackageAnalysis/OnePackage.pcap");
    strFilePath.append("/home/Progect/DataPackageAnalysis/OnePackage1.pcap");
    strFilePath.append("/home/Progect/DataPackageAnalysis/mms20181119105830.pcap");
    //----------------------------------ce shi dai ma ----------------------------------
    SetFileToTree(strFilePath);
}

void AnalysisPcapFile::PcapFileName(int irow, int icolumn)
{
        m_FileOrInfo = false;
        ClearDataInfo();
        m_PcapBinaryInfo->clear();
        m_PcapTreeinfo->clear();
        m_PcapHreadInfo->clearContents();
        m_PcapHreadInfo->setRowCount(0);
        strCurFilePath =  m_filePaht[m_PcapFileName->item(irow,icolumn)->text()].strFilePath;
        ReadPcapFile(strCurFilePath);
}


void AnalysisPcapFile::ReadPcapFile(QString strFileFullName)
{
    struct pcap_pkthdr *pkthdr = NULL;
    const u_char *packet = NULL;
    char errbuf[256] = {0};
    for (int i =0; i < 1; i ++)
    {
        pcap_t *handle  = pcap_open_offline(strFileFullName.toStdString().c_str(), errbuf);
        int result;
        while((result =  pcap_next_ex(handle, &pkthdr, &packet)) >= 0)
        {
            TryDissect(packet,pkthdr->len,pkthdr);
        }
        pcap_close(handle);
    }
}


void AnalysisPcapFile::TryDissect(const u_char * body,int iPackegLen, struct pcap_pkthdr *pkthdr)
 {

    frame_data  *fdata;
    epan_dissect_t  *edt;
    union wtap_pseudo_header  pseudo_header;
    pseudo_header.eth.fcs_len = -1;
    fdata = (frame_data*)g_new(frame_data, 1);
    memset(fdata, 0, sizeof(frame_data));
    fdata->pfd  = NULL;
    fdata->num = 1;
    fdata->interface_id = 0;
    fdata->pkt_len  = iPackegLen;
    fdata->cap_len  = iPackegLen;
    fdata->cum_bytes = 0;
    fdata->file_off = 0;
    fdata->subnum = 0;
    fdata->lnk_t = WTAP_ENCAP_ETHERNET;
    fdata->flags.encoding = PACKET_CHAR_ENC_CHAR_ASCII;
    fdata->flags.visited = 0;
    fdata->flags.marked = 0;
    fdata->flags.ref_time = 0;
    fdata->color_filter = NULL;
    fdata->abs_ts.secs = 0;
    fdata->abs_ts.nsecs = 0;
    fdata->opt_comment = NULL;


    edt = epan_dissect_new(TRUE, TRUE);
    epan_dissect_run( edt, &pseudo_header, body, fdata, NULL);
    if(m_FileOrInfo == false)
    {
        //处理报文头
        int RowCont;
        RowCont=m_PcapHreadInfo->rowCount();
        m_PcapHreadInfo->insertRow(RowCont);//增加一行
        char ip[50] = {0};
        char ipTemp[50] = {0};
        memset(ip,0,50);
        time_t itime= pkthdr->ts.tv_sec;
        sprintf(ip,"%s",ctime(&itime));
        m_PcapHreadInfo->setItem(RowCont,0,new QTableWidgetItem(QString("%1.%2").arg(ip).arg(pkthdr->ts.tv_usec)));

        memset(ip,0,50);
        memset(ipTemp,0,50);
        memcpy(ip , edt->pi.net_src.data,edt->pi.net_src.len);
        sprintf(ipTemp,"%d.%d.%d.%d\n", ip[0]& 0xff,ip[1]& 0xff,ip[2]& 0xff,ip[3]& 0xff);
        m_PcapHreadInfo->setItem(RowCont,1,new QTableWidgetItem(ipTemp));

        memset(ip,0,50);
        memset(ipTemp,0,50);
        memcpy(ip , edt->pi.net_dst.data,edt->pi.net_dst.len);
        sprintf(ipTemp,"%d.a%d.%d.%d", ip[0]& 0xff,ip[1]& 0xff,ip[2]& 0xff,ip[3]& 0xff);
        m_PcapHreadInfo->setItem(RowCont,2,new QTableWidgetItem(ipTemp));


        m_PcapHreadInfo->setItem(RowCont,4,new QTableWidgetItem(QString("%1").arg(iPackegLen)));
        m_PcapHreadInfo->setItem(RowCont,5,new QTableWidgetItem(QString(QString("source port:%1 - destination port:%2").arg(edt->pi.srcport).arg(edt->pi.destport))));

        u_char * data = (u_char*)malloc(sizeof(u_char) * (iPackegLen + 1));
        memset(data,0,sizeof(u_char) * (iPackegLen + 1));
        memcpy(data,body,sizeof(u_char) * iPackegLen);
        PacpFileInfo info;
        info.data = data;
        info.iDataLen = iPackegLen;
        m_MapData[RowCont] = info;
    }
    else
    {
        print_tree(edt->tree->first_child, 0);
        PrintBuffer((void*)body,iPackegLen);
    }
    epan_dissect_free( edt );
    g_free(fdata);
    return;
}

void AnalysisPcapFile::checkColumnSlot(int irow, int icolumn)
{
    m_PcapBinaryInfo->clear();
    m_PcapTreeinfo->clear();
    m_FileOrInfo = true;
    TryDissect(m_MapData[irow].data,m_MapData[irow].iDataLen,NULL);
}


void AnalysisPcapFile::PrintBuffer(void* pBuff, unsigned int nLen)
 {
    m_PcapBinaryInfo->clear();
    if (NULL == pBuff || 0 == nLen)
    {
    return;
    }
    const int nBytePerLine = 16;
    unsigned char* p = (unsigned char*)pBuff;
    char szHex[3*nBytePerLine+1] = {0};
    for (unsigned int i=0; i<nLen; ++i)
    {
        int idx = 3 * (i % nBytePerLine);
        if (0 == idx)
        {
            memset(szHex, 0, sizeof(szHex));
        }
        snprintf(&szHex[idx], 4, "%02x ", p[i]); // buff长度要多传入1个字节
        // 以16个字节为一行，进行打印
        if (0 == ((i+1) % nBytePerLine))
        {
            QString strData = QString("%1").arg((char*)szHex);
            m_PcapBinaryInfo->append(strData.toUpper());
        }
    }
    // 打印最后一行未满16个字节的内容
    if (0 != (nLen % nBytePerLine))
    {
     QString strData = QString("%1").arg((char*)szHex);
     m_PcapBinaryInfo->append(strData.toUpper());
    }
 }


void AnalysisPcapFile::print_tree(proto_tree* tree, int level)
 {
      if(tree == NULL)
         return;
     gchar field_str[ITEM_LABEL_LENGTH + 1] = {0};
     if(tree->finfo->rep == NULL)
         proto_item_fill_label(tree->finfo, field_str);
     else
        strcpy(field_str, tree->finfo->rep->representation);
     if(!PROTO_ITEM_IS_HIDDEN(tree))
     {
         PackageTreeItemToData info;
         info.iStart = tree->finfo->start;
         info.iLen = tree->finfo->length;
         if(treeItemLevel != level)
         {
             if(level == 0) //Prent Point
             {
                m_TreeWidget.clear();
                treeItemLevel = 0;
                m_TreeWidget.insert(treeItemLevel,(void*)m_PcapTreeinfo);
                PrentTreeItem = NULL;
                PrentTreeItem = new QTreeWidgetItem((QTreeWidget*)m_PcapTreeinfo,QStringList(QString(field_str)));
             }
             else
             {
                 treeItemLevel = level;
                 if(m_TreeWidget[level -1] != m_TreeWidget.end().value())
                 {
                    m_TreeWidget.insert(treeItemLevel,(void*)PrentTreeItem);
                    PrentTreeItem = new QTreeWidgetItem((QTreeWidgetItem*)PrentTreeItem,QStringList(QString(field_str)));
                 }
             }

         }
         else
         {
            QTreeWidgetItem *Item = new QTreeWidgetItem((QTreeWidgetItem*)PrentTreeItem,QStringList(QString(field_str)));
         }
     }

     print_tree(tree->first_child, level+1);
     print_tree(tree->next, level);
 }

void AnalysisPcapFile::SetFileToTree(QList<QString> filePath)
{
    m_filePaht.clear();
    QList<QString>::iterator it = filePath.begin();
    bool bRet = false;
    for(;it < filePath.end(); it++)
    {
        FileInfo fileinfo;
        bRet = GetFileName(fileinfo,*it);
        if (bRet == true)
        {
            m_filePaht.insert(fileinfo.strFileName,  fileinfo);
        }
    }
    AddFileNameToTree();
}


void AnalysisPcapFile::AddFileNameToTree()
{
    int RowCont;
    RowCont=m_PcapFileName->rowCount();
    QMap<QString ,FileInfo>::iterator it;
    for(it = m_filePaht.begin(); it != m_filePaht.end();it++)
    {
        m_PcapFileName->insertRow(RowCont);//增加一行
        m_PcapFileName->setItem(RowCont,0,new QTableWidgetItem(it.key()));
    }
}


bool AnalysisPcapFile::GetFileName(FileInfo & fileinfo,QString filePath)
{
    QFileInfo info =  QFileInfo(filePath);

    if(info.isFile() == true)
    {
        fileinfo.strFileName = info.baseName();
        fileinfo.strFilePath = filePath;
        return true;
    }
    return false;
}

void AnalysisPcapFile::ClearDataInfo()
{
     QMap<int ,PacpFileInfo>::iterator it = m_MapData.begin();
     for (;it != m_MapData.end(); it ++) {
         free(it.value().data);
         it.value().data = NULL;
     }
     m_MapData.clear();
}
