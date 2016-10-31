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

#ifndef RISIPENDPOINT_H
#define RISIPENDPOINT_H

#include <QObject>
#include <pjsua2.hpp>

using namespace pj;

class RisipEndpoint;
class RisipAccountConfiguration;

class PjsipEndpoint: public Endpoint
{
public :
    ~PjsipEndpoint();
    static PjsipEndpoint *instance();

    void onNatDetectionComplete(const OnNatDetectionCompleteParam &prm);
    void onNatCheckStunServersComplete(const OnNatCheckStunServersCompleteParam &prm);
    void onTransportState(const OnTransportStateParam &prm);
    void onTimer(const OnTimerParam &prm);
    void onSelectAccount(OnSelectAccountParam &prm);

    void setRisipEndpointInterface(RisipEndpoint *endpoint);

private:
    PjsipEndpoint();

    static PjsipEndpoint *pjsipEndpoinInstance;
    RisipEndpoint *m_risipEndpoint;
};

class RisipEndpoint : public QObject
{
    Q_OBJECT
public:
    enum Status {
        Started = 1,
        NotStarted,
        EngineError = -1
    };

    Q_ENUM(Status)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(int activeTransportId READ activeTransportId NOTIFY activeTransportIdChanged)
    Q_PROPERTY(int errorCode READ errorCode NOTIFY errorCodeChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(QString errorInfo READ errorInfo NOTIFY errorInfoChanged)

    RisipEndpoint(QObject *parent = 0);
    ~RisipEndpoint();

    int status() const;
    int activeTransportId() const;
    int errorCode() const;
    QString errorMessage() const;
    QString errorInfo() const;

    bool createTransportNetwork(RisipAccountConfiguration *accountConf);
    bool destroyActiveTransport();

    static PjsipEndpoint *endpointInstance();

public Q_SLOTS:
    void start();
    void stop();

Q_SIGNALS:
    void statusChanged(int status);
    void activeTransportIdChanged(int id);
    void errorCodeChanged(int errorCode);
    void errorMessageChanged(const QString &message);
    void errorInfoChanged(const QString &info);

private:
    void setError(const Error &error);

    PjsipEndpoint *m_pjsipEndpoint;
    TransportId m_activeTransportId;
    EpConfig m_endpointConfig;
    Error m_error;
};

#endif // RISIPENDPOINT_H
