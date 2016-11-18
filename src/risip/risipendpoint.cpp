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

#include <QDebug>

PjsipEndpoint *PjsipEndpoint::pjsipEndpoinInstance = NULL;
PjsipEndpoint *PjsipEndpoint::instance()
{
    if(pjsipEndpoinInstance == NULL)
        pjsipEndpoinInstance = new PjsipEndpoint;

    return pjsipEndpoinInstance;
}

PjsipEndpoint::PjsipEndpoint()
    :Endpoint()
    ,m_risipEndpoint(NULL)
{
    delete pjsipEndpoinInstance;
    pjsipEndpoinInstance = NULL;
}

PjsipEndpoint::~PjsipEndpoint()
{
    libDestroy();
}

void PjsipEndpoint::onNatDetectionComplete(const OnNatDetectionCompleteParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipEndpoint::onNatCheckStunServersComplete(const OnNatCheckStunServersCompleteParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipEndpoint::onTransportState(const OnTransportStateParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipEndpoint::onTimer(const OnTimerParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipEndpoint::onSelectAccount(OnSelectAccountParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipEndpoint::setRisipEndpointInterface(RisipEndpoint *endpoint)
{
    m_risipEndpoint = endpoint;
}

RisipEndpoint::RisipEndpoint(QObject *parent)
    :QObject(parent)
    ,m_activeTransportId(-1)
    ,m_error()
{
}

RisipEndpoint::~RisipEndpoint()
{
    delete PjsipEndpoint::instance();
    m_pjsipEndpoint = NULL;

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
    if(!m_pjsipEndpoint)
        return RisipEndpoint::NotStarted;

    switch (m_pjsipEndpoint->libGetState()) {
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
    return m_error.status;
}

/**
 * @brief RisipEndpoint::errorMessage
 * @return returns the last error message
 *
 * Use this property to retrieve the last error message.
 */
QString RisipEndpoint::errorMessage() const
{
    return QString::fromStdString(m_error.reason);
}

/**
 * @brief RisipEndpoint::errorInfo
 * @return returns the last error complete info message
 *
 * Use this property to see the complete error information
 */
QString RisipEndpoint::errorInfo() const
{
    return QString::fromStdString(m_error.info(true));
}

int RisipEndpoint::activeTransportId() const
{
    return m_activeTransportId;
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
        m_activeTransportId = m_pjsipEndpoint->transportCreate(netType, accountConf->pjsipTransportConfig());
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
    if(m_activeTransportId == -1)
        return true;

    //closing it current active network transport.
    try {
        m_pjsipEndpoint->transportClose(m_activeTransportId);
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
    m_pjsipEndpoint = PjsipEndpoint::instance();
    m_pjsipEndpoint->setRisipEndpointInterface(this);

//    m_endpointConfig.uaConfig.userAgent
//    m_endpointConfig.uaConfig.maxCalls = 4;

    try {
        m_pjsipEndpoint->libCreate();
    } catch (Error &err) {
        emit statusChanged(status());
        setError(err);
        return;
    }

    try {
        m_pjsipEndpoint->libInit(m_endpointConfig);
    } catch (Error &err) {
        emit statusChanged(status());
        setError(err);
        return;
    }

    try {
        m_pjsipEndpoint->libStart();
    } catch (Error &err) {
        emit statusChanged(status());
        setError(err);
        return;
    }

    //FIXME do not call this here - better handling
    Risip::instance()->readSettings();
    emit statusChanged(status());

    //FIXME Codec priorities
    //TODO Codecs settings page
    Endpoint::instance().codecSetPriority("PCMA/8000", 255);
    Endpoint::instance().codecSetPriority("PCMU/8000", 254);
    Endpoint::instance().codecSetPriority("g722/16000", 200);
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
}

/**
 * @brief RisipEndpoint::stop
 *
 * Call this function to stop the SIP endpoint / library
 */
void RisipEndpoint::stop()
{
    if(m_pjsipEndpoint && m_pjsipEndpoint->libGetState() != PJSUA_STATE_NULL)
        m_pjsipEndpoint->libDestroy();

    emit statusChanged(status());
}

void RisipEndpoint::setError(const Error &error)
{
    qDebug()<<"ERROR: " <<"code: "<<error.status <<" info: " << QString::fromStdString(error.info(true));

    if(m_error.status != error.status) {

        m_error.status = error.status;
        m_error.reason = error.reason;
        m_error.srcFile = error.srcFile;
        m_error.srcLine = error.srcLine;
        m_error.title = error.title;

        emit errorCodeChanged(m_error.status);
        emit errorMessageChanged(QString::fromStdString(m_error.reason));
        emit errorInfoChanged(QString::fromStdString(m_error.info(true)));
    }
}
