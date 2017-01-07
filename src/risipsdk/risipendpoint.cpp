/***********************************************************************************
**    Copyright (C) 2016  Petref Saraci
**    http://risip.io
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You have received a copy of the GNU General Public License
**    along with this program. See LICENSE.GPLv3
**    A copy of the license can be found also here <http://www.gnu.org/licenses/>.
**
************************************************************************************/

#include "risipendpoint.h"
#include "risipaccountconfiguration.h"
#include "risip.h"

#include "pjsipwrapper/pjsipendpoint.h"

#include <QDebug>

class RisipEndpoint::Private
{
public:
    PjsipEndpoint *pjsipEndpoint;
    TransportId activeTransportId;
    EpConfig endpointConfig;
    Error error;
};

RisipEndpoint::RisipEndpoint(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->activeTransportId = -1;
}

RisipEndpoint::~RisipEndpoint()
{
    delete PjsipEndpoint::instance();
    m_data->pjsipEndpoint = NULL;

    delete m_data;
    m_data = NULL;
}

/**
 * @brief RisipEndpoint::status
 * @return status of the SIP endpoint / engine
 *
 * Use this property to see the status of the SIP library whether it has started, stoped or
 * it has an error.
 */
int RisipEndpoint::status() const
{
    if(!m_data->pjsipEndpoint)
        return RisipEndpoint::NotStarted;

    switch (m_data->pjsipEndpoint->libGetState()) {
    case PJSUA_STATE_NULL:
    case PJSUA_STATE_CREATED:
    case PJSUA_STATE_INIT:
    case PJSUA_STATE_STARTING:
    case PJSUA_STATE_CLOSING:
        return RisipEndpoint::NotStarted;
    case PJSUA_STATE_RUNNING:
        return RisipEndpoint::Started;
    default:
        return RisipEndpoint::EngineError;
    }
}

/**
 * @brief RisipEndpoint::errorCode
 * @return the last error code if the status of SIP endpoint is EngineError
 *
 * If the status of this endpoint is EngineError @see RisipEndpoint::status then use this property
 * to errorCode get the last error code.
 */
int RisipEndpoint::errorCode() const
{
    return m_data->error.status;
}

/**
 * @brief RisipEndpoint::errorMessage
 * @return returns the last error message
 *
 * Use this property to retrieve the last error message.
 */
QString RisipEndpoint::errorMessage() const
{
    return QString::fromStdString(m_data->error.reason);
}

/**
 * @brief RisipEndpoint::errorInfo
 * @return returns the last error complete info message
 *
 * Use this property to see the complete error information
 */
QString RisipEndpoint::errorInfo() const
{
    return QString::fromStdString(m_data->error.info(true));
}

int RisipEndpoint::activeTransportId() const
{
    return m_data->activeTransportId;
}

bool RisipEndpoint::createTransportNetwork(RisipAccountConfiguration *accountConf)
{
    if(status() == NotStarted
            || status() == EngineError)
        return false;

    pjsip_transport_type_e netType;
    switch (accountConf->networkProtocol()) {
    case RisipAccountConfiguration::UDP:
        netType = PJSIP_TRANSPORT_UDP;
        break;
    case RisipAccountConfiguration::TCP:
        netType = PJSIP_TRANSPORT_TCP;
        break;
    case RisipAccountConfiguration::TLS:
        netType = PJSIP_TRANSPORT_TLS;
        break;
//    case RisipAccountConfiguration::UDP6:
//        netType = PJSIP_TRANSPORT_UDP6;
//        break;
//    case RisipAccountConfiguration::TCP6:
//        netType = PJSIP_TRANSPORT_TCP6;
//        break;
//    case RisipAccountConfiguration::TLS6:
//        netType = PJSIP_TRANSPORT_TLS6;
//        break;
    default:
        netType = PJSIP_TRANSPORT_UDP; //defaults to UDP always
        break;
    }

    try {
        m_data->activeTransportId = m_data->pjsipEndpoint->transportCreate(netType, accountConf->pjsipTransportConfig());
    } catch (Error& err) {
        setError(err);
        return false;
    }

    return true;
}

/**
 * @brief RisipEndpoint::destroyActiveTransport
 * @return true/false if transport is destoryed or not.
 *
 * Internal API.
 */
bool RisipEndpoint::destroyActiveTransport()
{
    //is there an active transport?
    if(m_data->activeTransportId == -1)
        return true;

    //closing it current active network transport.
    try {
        m_data->pjsipEndpoint->transportClose(m_data->activeTransportId);
    } catch(Error& err) {
        setError(err);
        return false;
    }

    return true;
}

PjsipEndpoint *RisipEndpoint::endpointInstance()
{
    return PjsipEndpoint::instance();
}

/**
 * @brief RisipEndpoint::start
 *
 * Use this function to start the SIP endpoint library/engine.
 * MUST call this before any other operation with Risip objects.
 */
void RisipEndpoint::start()
{
    m_data->pjsipEndpoint = PjsipEndpoint::instance();
    m_data->pjsipEndpoint->setRisipEndpointInterface(this);

//    m_endpointConfig.uaConfig.userAgent
//    m_endpointConfig.uaConfig.maxCalls = 4;

    try {
        m_data->pjsipEndpoint->libCreate();
    } catch (Error &err) {
        emit statusChanged(status());
        setError(err);
        return;
    }

    try {
        m_data->pjsipEndpoint->libInit(m_data->endpointConfig);
    } catch (Error &err) {
        emit statusChanged(status());
        setError(err);
        return;
    }

    try {
        m_data->pjsipEndpoint->libStart();
    } catch (Error &err) {
        emit statusChanged(status());
        setError(err);
        return;
    }

    //FIXME Codec priorities
    //TODO Codecs settings page
    Endpoint::instance().codecSetPriority("PCMA/8000", 215);
    Endpoint::instance().codecSetPriority("PCMU/8000", 214);
//    Endpoint::instance().codecSetPriority("g722/16000", 255);
//    Endpoint::instance().codecSetPriority("iLBC/8000", 0);
    Endpoint::instance().codecSetPriority("speex/16000", 0);
    Endpoint::instance().codecSetPriority("speex/8000", 0);
    Endpoint::instance().codecSetPriority("speex/32000", 0);
    CodecInfoVector codecs = Endpoint::instance().codecEnum();

    CodecInfo *codecInfo;
    for(int i=0; i<codecs.size(); ++i) {
        codecInfo = codecs.at(i);
        qDebug()<<"CODEC INFO: " << QString::fromStdString(codecInfo->codecId)
                << QString::fromStdString(codecInfo->desc)
                << codecInfo->priority;
    }

    emit statusChanged(status());
}

/**
 * @brief RisipEndpoint::stop
 *
 * Call this function to stop the SIP endpoint / library
 */
void RisipEndpoint::stop()
{
    if(m_data->pjsipEndpoint && m_data->pjsipEndpoint->libGetState() != PJSUA_STATE_NULL)
        m_data->pjsipEndpoint->libDestroy();

    emit statusChanged(status());
}

void RisipEndpoint::setError(const Error &error)
{
    qDebug()<<"ERROR: " <<"code: "<<error.status <<" info: " << QString::fromStdString(error.info(true));

    if(m_data->error.status != error.status) {

        m_data->error.status = error.status;
        m_data->error.reason = error.reason;
        m_data->error.srcFile = error.srcFile;
        m_data->error.srcLine = error.srcLine;
        m_data->error.title = error.title;

        emit errorCodeChanged(m_data->error.status);
        emit errorMessageChanged(QString::fromStdString(m_data->error.reason));
        emit errorInfoChanged(QString::fromStdString(m_data->error.info(true)));
    }
}
