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

#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QObject>
#include <QJsonObject>

class QNetworkReply;

class HttpResponse : public QObject
{
    Q_OBJECT

public:

    enum Status {
        Ready = 1,
        InProgress,
        Error,
        Blank
    };

    Q_ENUM(Status)

    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(int httpResponseCode READ httpResponseCode NOTIFY httpResponseCodeChanged)
    Q_PROPERTY(QByteArray dataRaw READ dataRaw NOTIFY dataRawChanged)
    Q_PROPERTY(QJsonObject json READ json NOTIFY jsonChanged)
    Q_PROPERTY(int networkErrorCode READ networkErrorCode NOTIFY networkErrorCodeChanged)

    HttpResponse(QNetworkReply *reply = 0);
    ~HttpResponse();

    int status() const;
    int httpResponseCode();
    QByteArray dataRaw() const;
    QJsonObject json() const;
    int networkErrorCode() const;

    void setNetworkReply(QNetworkReply *reply);
    QNetworkReply *networkReplyObject() const;

Q_SIGNALS:
    void statusChanged(int);
    void httpResponseCodeChanged(int);
    void dataRawChanged(QString);
    void jsonChanged(QJsonObject);
    void networkErrorCodeChanged(int);

private Q_SLOTS:
    void httpResponseHandler();
    void httpResponseCodeHandler();
    void httpResponseErrorHandler();

private:
    void setStatus(int s);
    void setJsonRaw(const QByteArray &json);

    int m_status;
    QByteArray m_dataRaw;
    QNetworkReply *m_httpReply;
};

#endif // HTTPRESPONSE_H
