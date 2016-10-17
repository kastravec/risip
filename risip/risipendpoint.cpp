/***********************************************************************************
**    Copyright (C) 2016  Petref Saraci
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

#include <QDebug>
#include <QTimer>

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
{
}

RisipEndpoint::~RisipEndpoint()
{
    delete PjsipEndpoint::instance();
    m_pjsipEndpoint = NULL;

}

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

int RisipEndpoint::error() const
{
    //FIXME
    return 0;
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
    case RisipAccountConfiguration::UDP6:
        netType = PJSIP_TRANSPORT_UDP6;
        break;
    case RisipAccountConfiguration::TCP6:
        netType = PJSIP_TRANSPORT_TCP6;
        break;
    case RisipAccountConfiguration::TLS:
        netType = PJSIP_TRANSPORT_TLS;
        break;
    case RisipAccountConfiguration::TLS6:
        netType = PJSIP_TRANSPORT_TLS6;
        break;
    default:
        netType = PJSIP_TRANSPORT_UDP; //defaults to UDP always
        break;
    }

    try {
        m_activeTransportId = m_pjsipEndpoint->transportCreate(netType, accountConf->pjsipTransportConfig());
    } catch (Error& err) {
        qDebug()<<"Error in creating network transport protocol. " <<QString::fromStdString( err.info());
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
        qDebug()<<"Error closing the active transport for account: " <<QString::fromStdString( err.info() );
        return false;
    }

    return true;
}

PjsipEndpoint *RisipEndpoint::endpointInstance()
{
    return PjsipEndpoint::instance();
}

void RisipEndpoint::start()
{
    m_pjsipEndpoint = PjsipEndpoint::instance();
    m_pjsipEndpoint->setRisipEndpointInterface(this);

    try {
        m_pjsipEndpoint->libCreate();
    } catch (Error &err) {
        qDebug()<<"Error creating the sip endpoint: " <<QString::fromStdString(err.info(true));
    }

    try {
        m_pjsipEndpoint->libInit(m_endpointConfig);
    } catch (Error &err) {
        qDebug()<<"Error initializing the sip endpoint: " <<QString::fromStdString(err.info(true));
    }

    try {
        m_pjsipEndpoint->libStart();
    } catch (Error &err) {
        qDebug()<<"Error starting the sip endpoint: " <<QString::fromStdString(err.info(true));
    }

    emit statusChanged(status());

    //FIXME Codec priorities
    //TODO Codecs settings page
    Endpoint::instance().codecSetPriority("iLBC/8000", 255);
    Endpoint::instance().codecSetPriority("speex/16000", 0);
    Endpoint::instance().codecSetPriority("speex/8000", 0);
    Endpoint::instance().codecSetPriority("speex/32000", 0);
}

void RisipEndpoint::stop()
{
    if(m_pjsipEndpoint && m_pjsipEndpoint->libGetState() != PJSUA_STATE_NULL)
        m_pjsipEndpoint->libDestroy();

    emit statusChanged(status());
}

void RisipEndpoint::transportClosed()
{
    m_activeTransportId = -1;
}

