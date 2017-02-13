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

#ifndef RISIPMEDIA_H
#define RISIPMEDIA_H

#include "risipsdkglobal.h"
#include <QObject>
#include <pjsua2.hpp>
using namespace pj;

namespace risip {

class RisipCall;
class RisipEndpoint;

class RISIP_VOIPSDK_EXPORT RisipMedia : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(RisipEndpoint * sipEndpoint READ sipEndpoint WRITE setSipEndpoint NOTIFY sipEndpointChanged)
    Q_PROPERTY(RisipCall * activeCall READ activeCall NOTIFY activeCallChanged)
    Q_PROPERTY(int speakerVolume READ speakerVolume WRITE setSpeakerVolume NOTIFY speakerVolumeChanged)
    Q_PROPERTY(qlonglong micVolume READ micVolume WRITE setMicVolume NOTIFY micVolumeChanged)
    Q_PROPERTY(bool loudSpeaker READ loudSpeaker WRITE setLoudSpeaker NOTIFY loudSpeakerChanged)
    Q_PROPERTY(bool keepMediaSettings READ keepMediaSettings WRITE setKeepMediaSettings NOTIFY keepMediaSettingsChanged)
    Q_PROPERTY(int errorCode READ errorCode NOTIFY errorCodeChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(QString errorInfo READ errorInfo NOTIFY errorInfoChanged)

    RisipMedia(QObject *parent = 0);
    ~RisipMedia();

    RisipCall *activeCall() const;

    RisipEndpoint *sipEndpoint() const;
    void setSipEndpoint(RisipEndpoint *endpoint);

    int speakerVolume() const;
    void setSpeakerVolume(int volume);

    qlonglong micVolume() const;
    void setMicVolume(qlonglong volume);

    bool loudSpeaker() const;
    void setLoudSpeaker(bool loudspeaker);

    bool keepMediaSettings() const;
    void setKeepMediaSettings(const bool keep);

    int errorCode() const;
    QString errorMessage() const;
    QString errorInfo() const;

    void setActiveCall(RisipCall *call);

public Q_SLOTS:
    void startCallMedia();

Q_SIGNALS:
    void sipEndpointChanged(RisipEndpoint *endpoint);
    void activeCallChanged(RisipCall *call);
    void speakerVolumeChanged(int volume);
    void micVolumeChanged(int volume);
    void loudSpeakerChanged(bool loudspeaker);
    void keepMediaSettingsChanged(bool keep);
    void errorCodeChanged(int code);
    void errorMessageChanged(const QString &message);
    void errorInfoChanged(const QString &info);

private:
    void setError(Error &error);
    void loopAudioTest();

    friend class Risip;
    class Private;
    Private *m_data;
};

} //end of risip namespace

#endif // RISIPMEDIA_H
