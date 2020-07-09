/*
 * HiRedis.h
 *
 *  Created on: Sep 12, 2017
 *      Author: root
 */

#ifndef HIREDIS_H_
#define HIREDIS_H_
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include "hiredis.h"
#include <pthread.h>
#include <list>
#include <alloca.h>
#include <assert.h>

using namespace std;


#define REDIS_DATATRANSMISSION "DataTransmission"
#define REDIS_TCPSERVER        "TcpServer"
#define REDIS_COLLECTDATA      "SyslogCollector"
#define REDIS_PROCESSMANAGE    "Processmanage"
#define DEVICEASSET            "DeviceAsset"
#define SURICATA               "suricata"

#define SingleRedisHelp RedisHelper::GetInstance()

class RedisHelper
{
public:
    static RedisHelper *GetInstance();

public:
    bool setConnParas(const char* host,unsigned short u_port,string strPasswd="");

    bool subscribe(string channel, ...);

    bool set(string key, string value);

    bool publish(string channel, string message, string key = "");

    bool getMessage(string& message);

    bool getMessage(string& message, string& channel);

    bool disConnect();
private:
    static RedisHelper * redisHelper_;

    RedisHelper();
    virtual ~RedisHelper();
private:
	redisContext * m_pContextSub;
	redisContext * m_pContextPub;

	pthread_mutex_t  m_dataMutex; //线程锁

	bool m_bRunFlag;
};

#endif /* HIREDIS_H_ */
