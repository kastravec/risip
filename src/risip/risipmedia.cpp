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

#include "risipmedia.h"
#include "risipcall.h"
#include "risipendpoint.h"
#include "risipaccount.h"

#include "pjsipwrapper/pjsipcall.h"
#include "pjsipwrapper/pjsipendpoint.h"

#include <QDebug>

class RisipMedia::Private
{
public:
    VidDevManager *pjsipVideoManager;
    AudDevManager *pjsipAudioManager;
    AudioMedia *callAudio;
    AudioMedia *localAudioMedia;
    AudioMedia *audioDevice;
    RisipCall *activeCall;
    RisipEndpoint *sipEndpoint;
    bool keepMediaSettings;
    Error error;
    bool loudSpeaker;
};

RisipMedia::RisipMedia(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->pjsipAudioManager = NULL;
    m_data->callAudio = NULL;
    m_data->localAudioMedia = NULL;
    m_data->audioDevice = NULL;
    m_data->activeCall = NULL;
    m_data->sipEndpoint = NULL;
    m_data->keepMediaSettings = true;
    m_data->loudSpeaker = false;
}

RisipMedia::~RisipMedia()
{
    delete m_data;
    m_data = NULL;
}

RisipCall *RisipMedia::activeCall() const
{
    return m_data->activeCall;
}

RisipEndpoint *RisipMedia::sipEndpoint() const
{
    return m_data->sipEndpoint;
}

/**
 * @brief RisipMedia::setSipEndpoint
 * @param endpoint instance of a RisipEndpoint
 *
 * Must set this always in order to initialize the internal pjsip media objects correctly.
 * Not having this property set then no media capabilities will be available when handling a call or
 * simply playing audio.
 *
 * This property is automatically set for each incoming and outgoing call you make.
 */
void RisipMedia::setSipEndpoint(RisipEndpoint *endpoint)
{
    if(m_data->sipEndpoint != endpoint) {
        m_data->sipEndpoint = endpoint;
        emit sipEndpointChanged(m_data->sipEndpoint);

        if(m_data->sipEndpoint) {
            m_data->pjsipAudioManager = &(m_data->sipEndpoint->endpointInstance()->audDevManager());

            try {
                m_data->localAudioMedia = &m_data->pjsipAudioManager->getCaptureDevMedia();
            } catch (Error &err) {
                setError(err);
            }
            m_data->audioDevice = &m_data->pjsipAudioManager->getPlaybackDevMedia();
        }
    }
}

/**
 * @brief RisipMedia::setActiveCall
 * @param call
 *
 * Set the parameter call as the active call for this RisipMedia object. The media can start transmiting audio
 * for this call. It sets the active media for that call and therefore call can start answering calls.
 *
 * So when the active_call is answered (@see RisipCall::answer) then media is automatically transimited.
 *
 * internal method - @see RisipCall::initializeMediaHandler and @see RisispCall::setMedia
 */
void RisipMedia::setActiveCall(RisipCall *call)
{
    if(m_data->activeCall != call) {
        m_data->activeCall = call;
        emit activeCallChanged(m_data->activeCall);

        //setting an instance of the risip endpoint object in order to use the singleton instance of pjsip library.
        // see setSipEndpoint for more details
        if(m_data->activeCall)
            setSipEndpoint(m_data->activeCall->account()->sipEndPoint());
    }
}

int RisipMedia::speakerVolume() const
{
    if(m_data->pjsipAudioManager)
        return m_data->pjsipAudioManager->getOutputVolume();

    return -1;
}

/**
 * @brief RisipMedia::setSpeakerVolume
 * @param volume in percentage 0-100 %
 */
void RisipMedia::setSpeakerVolume(int volume)
{
    if(m_data->pjsipAudioManager) {
        if(m_data->pjsipAudioManager->getOutputVolume() != volume) {
            m_data->pjsipAudioManager->setOutputVolume(volume, m_data->keepMediaSettings);
            emit speakerVolumeChanged(volume);
        }
    }
}

qlonglong RisipMedia::micVolume() const
{
    if(!m_data->activeCall || !m_data->sipEndpoint)
        return -1.0;

    return m_data->callAudio->getTxLevel();
}

/**
 * @brief RisipMedia::setMicVolume
 * @param volume in betweeb 0 and 1 floating numbers
 */
void RisipMedia::setMicVolume(qlonglong volume)
{
    if((!m_data->activeCall || !m_data->sipEndpoint)
            && (0.0 <= volume >= 1.0) )
        return;

    if(m_data->activeCall->pjsipCall()->isActive()) {
        m_data->localAudioMedia->adjustTxLevel(volume);
        emit micVolumeChanged(volume);
    }
}

bool RisipMedia::loudSpeaker() const
{
    return m_data->loudSpeaker;
}

void RisipMedia::setLoudSpeaker(bool loudspeaker)
{
    if(m_data->loudSpeaker != loudspeaker) {
        m_data->loudSpeaker = loudspeaker;

        if(m_data->sipEndpoint && m_data->activeCall->pjsipCall()->isActive()) {
            try {
                if(loudspeaker)
                    m_data->pjsipAudioManager->setOutputRoute(PJMEDIA_AUD_DEV_ROUTE_LOUDSPEAKER);
                else
                    m_data->pjsipAudioManager->setOutputRoute(PJMEDIA_AUD_DEV_ROUTE_DEFAULT);
            } catch (Error &err) {
                setError(err);
            }
        }

        emit loudSpeakerChanged(m_data->loudSpeaker);
    }
}

bool RisipMedia::keepMediaSettings() const
{
    return m_data->keepMediaSettings;
}

void RisipMedia::setKeepMediaSettings(const bool keep)
{
    if(m_data->keepMediaSettings != keep) {
        m_data->keepMediaSettings = keep;
        emit keepMediaSettingsChanged(m_data->keepMediaSettings);
    }
}

int RisipMedia::errorCode() const
{
    return m_data->error.status;
}

QString RisipMedia::errorMessage() const
{
    return QString::fromStdString(m_data->error.reason);
}

QString RisipMedia::errorInfo() const
{
    return QString::fromStdString(m_data->error.info(true));
}

/**
 * @brief RisipMedia::startCallMedia
 *
 * This slot initializes the media engine of Pjsip and starts to transimit multimedia data (this is only audio for now)
 * for an active call. Hence, an active call must have been set prior calling this slot, otherwise the function simply
 * returns.
 *
 * You do not need to ever use or call this function, it is automatically called when you answer a call.
 */
void RisipMedia::startCallMedia()
{
    if(!m_data->activeCall || !m_data->sipEndpoint)
        return;

    CallInfo callInfo = m_data->activeCall->pjsipCall()->getInfo();
    // Iterate all the call medias
    for (int i = 0; i < callInfo.media.size(); ++i) {
        if (callInfo.media[i].type == PJMEDIA_TYPE_AUDIO && m_data->activeCall->pjsipCall()->getMedia(i)) {
            m_data->callAudio = (AudioMedia *)m_data->activeCall->pjsipCall()->getMedia(i);
            break;
        }
    }
    // Connect the call audio media to sound device
    try {
        m_data->localAudioMedia->startTransmit(*m_data->callAudio);
    } catch (Error &err) {
        setError(err);
    }

    try {
        m_data->callAudio->startTransmit(*m_data->audioDevice);
    } catch (Error &err) {
        setError(err);
    }
}

void RisipMedia::setError(Error &error)
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
