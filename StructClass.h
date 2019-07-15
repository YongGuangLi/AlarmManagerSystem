#ifndef STRUCTCLASS_H
#define STRUCTCLASS_H
#include <QString>

// see \wireshark-1.8.4\CMakeLists.txt,#481
#define WS_VAR_IMPORT extern
// see \wireshark-1.8.4\CMakeLists.txt,#482
#define WS_MSVC_NORETURN


#include "epan/epan.h"
#include "epan/epan_dissect.h"
#include "epan/proto.h"
#include "epan/packet_info.h"
#include "epan/frame_data.h"
#include "epan/packet.h"
#include "wsutil/privileges.h"


// 为了保证在windows和linux下都能正常编译，放弃使用INT64或者_int_64
typedef short _Int16;
typedef	long  _Int32;
typedef char  Byte;
// Pcap文件头
#pragma pack( push, 1)

typedef struct _PacpFileInfo
{
    int iDataLen;
    u_char * data;
}PacpFileInfo;


typedef struct _FileInfo{
    QString strFilePath;
    QString strFileName;
}FileInfo;

typedef struct _PackageTreeItemToData
{
    int iStart;
    int iLen;
    char Data[2048];

}PackageTreeItemToData;



//typedef struct _FileHeader
//{
//    _Int32	iMagic;
//    _Int16	iMaVersion;
//    _Int16	iMiVersion;
//    _Int32	iTimezone;
//    _Int32	iSigFlags;
//    _Int32	iSnapLen;
//    _Int32	iLinkType;
//}FileHeader;
//// 数据包头
//typedef struct _DataPackageHeader
//{
//    _Int32		iTimeSecond;
//    _Int32		iTimeSS;
//    _Int32		iCapLength;
//    _Int32		iLength;
//}DataPackageHeader;

//typedef struct _TableShowInfo
//{
//    QString strTime;
//    QString strTimeLag;
//    QString strSrcAddr;
//    QString strDscAddr;
//    QString strPoro;
//    int iLen;
//    QString strInfo;
//    QString strPort;
//    //struct _proto_node *first_child;
//    epan_dissect_t  *edt;
//    frame_data  *fdata;
//    guchar * data;

//}TableShowInfo;

#pragma pack( pop)

#endif // STRUCTCLASS_H
