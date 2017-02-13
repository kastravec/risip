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
#include "pjsipendpoint.h"

#include "risipendpoint.h"

namespace risip {

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

} //end of risip namespace
