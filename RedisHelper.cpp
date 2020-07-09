/*
 * HiRedis.cpp
 *
 *  Created on: Sep 12, 2017
 *      Author: root
 */

#include "RedisHelper.h"

RedisHelper *RedisHelper::redisHelper_ = NULL;

RedisHelper::RedisHelper() {
	// TODO Auto-generated constructor stub
	pthread_mutex_init(&m_dataMutex,NULL);
	m_pContextPub = NULL;
	m_pContextSub = NULL;
	m_bRunFlag = true;
}

RedisHelper::~RedisHelper() {
	redisFree(m_pContextPub);
	redisFree(m_pContextSub);
	m_bRunFlag = false;
	m_pContextPub = NULL;
	m_pContextSub = NULL;
	pthread_mutex_destroy(&m_dataMutex);
}

RedisHelper *RedisHelper::GetInstance()
{
    if (redisHelper_ == NULL)
    {
        redisHelper_ = new RedisHelper();
    }

    return redisHelper_;
}

bool RedisHelper::setConnParas(const char* host,unsigned short u_port,string strPasswd  )
{
    bool result = true;
    struct timeval timeout = { 1, 500000 };

	m_pContextSub = redisConnectWithTimeout(host,u_port,timeout);
    if ( NULL == m_pContextSub || m_pContextSub->err != 0)
	{
		printf( "%s\n", m_pContextSub->errstr );
		return false;
	}
    if(!strPasswd.empty())
    {
        redisReply *reply = (redisReply *)redisCommand(m_pContextSub,"%s %s", "AUTH", strPasswd.c_str());
        if (reply)
        {
            switch (reply->type)
            {
                case REDIS_REPLY_ERROR:
                    result = false;
                    break;
                default:
                    break;
            }
            freeReplyObject(reply);
        }
        else
            result = false;
	}

	m_pContextPub = redisConnectWithTimeout(host,u_port,timeout);
    if ( NULL == m_pContextPub || m_pContextPub->err != 0)
	{
		printf( "%s\n", m_pContextPub->errstr );
		return false;
	}
    if(!strPasswd.empty())
    {
        redisReply *reply = (redisReply *)redisCommand(m_pContextPub,"%s %s", "AUTH", strPasswd.c_str());
        if (reply)
        {
            switch (reply->type)
            {
                case REDIS_REPLY_ERROR:
                    result = false;
                    break;
                default:
                    break;
            }
            freeReplyObject(reply);
        }
        else
            result = false;
    }

    return result;
}

bool RedisHelper::subscribe(string channel, ...)
{
    bool result = true;
    va_list pArgs;
    va_start(pArgs, channel);
    const char* tmpChannel = channel.c_str();
    while (tmpChannel != NULL)
    {
        redisReply * reply = (redisReply *)redisCommand( this->m_pContextSub, "SUBSCRIBE %s", tmpChannel);
        if (reply)
        {
            switch (reply->type)
            {
                case REDIS_REPLY_ERROR:
                    result = false;
                    break;
                default:
                    break;
            }
            freeReplyObject(reply);
        }
        else
            result = false;

        tmpChannel = va_arg(pArgs, char*);
    }
    va_end(pArgs);
    return result;
}

bool RedisHelper::set(string key, string value)
{
    bool result = true;

    pthread_mutex_lock(&m_dataMutex);
    redisReply *reply = (redisReply *)redisCommand( this->m_pContextPub, "set %s %b" , key.c_str(),value.c_str(),(size_t) value.length());
    pthread_mutex_unlock(&m_dataMutex);

    if (reply)
    {
        switch (reply->type)
        {
            case REDIS_REPLY_ERROR:
                result = false;
                break;
            default:
                break;
        }
        freeReplyObject(reply);
    }
    else
        result = false;

    return result;
}

bool RedisHelper::publish(string channel, string message, string key)
{
    bool result = true;

    if(key.length() > 0)
        set(key, message);

    pthread_mutex_lock(&m_dataMutex);
    redisReply *reply = (redisReply *)redisCommand( this->m_pContextPub, "PUBLISH %s %b" , channel.c_str(), message.c_str(),  (size_t) message.length());
    pthread_mutex_unlock(&m_dataMutex);

    if (reply)
    {
        switch (reply->type)
        {
            case REDIS_REPLY_ERROR:
                result = false;
                break;
            case REDIS_REPLY_INTEGER:
//                printf("Redis integer %lld\n", reply->integer);
                break;
            default:
                break;
        }
        freeReplyObject(reply);
    }
    else
        result = false;

    return result;
}


bool RedisHelper::getMessage(string &message)
{
    bool result = false;

    redisReply * pReply = NULL;

    if ( redisGetReply( m_pContextSub, (void **)&pReply ) == REDIS_OK )
    {
        if ( pReply != NULL && pReply->elements == 3 )
        {
            redisReply * messageReply = pReply->element[2];
            message = string(messageReply->str,messageReply->len);

            freeReplyObject( pReply );
            pReply = NULL;
            result = true;
        }
    }
    return result;
}

bool RedisHelper::getMessage(string &message, string &channel)
{
    bool result = false;

    redisReply * pReply = NULL;

    if(redisGetReply( m_pContextSub, (void **)&pReply ) == REDIS_OK )
    {
        if(pReply != NULL && pReply->elements == 3 )
        {
            redisReply * messageReply = pReply->element[2];
            message = string(messageReply->str,messageReply->len);

            redisReply * channelReply = pReply->element[1];
            channel = string(channelReply->str,channelReply->len);

            freeReplyObject( pReply );
            pReply = NULL;
            result = true;
        }
    }
    return result;
}


bool RedisHelper::disConnect()
{
    redisFree(m_pContextPub);
    redisFree(m_pContextSub);
    m_pContextPub = NULL;
    m_pContextSub = NULL;
    return true;
}

