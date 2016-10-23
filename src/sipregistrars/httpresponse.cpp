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

#define ENABLE_DEBUG = 0

#ifdef ENABLE_DEBUG
#define DEBUG qDebug()
#else
#define DEBUG if (0) qDebug()
#endif

#include "httpresponse.h"

#include <QNetworkReply>
#include <QJsonDocument>

HttpResponse::HttpResponse(QNetworkReply *reply)
    :QObject()
    ,m_status(-1)
{
    m_httpReply = reply;
    if(m_httpReply != NULL) {
        connect(m_httpReply, &QNetworkReply::metaDataChanged,
                this, &HttpResponse::httpResponseCodeHandler);
        connect(m_httpReply, &QNetworkReply::finished,
                this, &HttpResponse::httpResponseHandler);
    }
}

HttpResponse::~HttpResponse()
{
    m_httpReply->close();
}

int HttpResponse::status() const
{
    return m_status;
}

int HttpResponse::httpResponseCode()
{
    if(m_httpReply == NULL) {
        setStatus(HttpResponse::Blank);
        return -1;
    }

    return m_httpReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}

QByteArray HttpResponse::dataRaw() const
{
    return m_dataRaw;
}

QJsonObject HttpResponse::json() const
{
    return QJsonDocument::fromJson(dataRaw()).object();
}

int HttpResponse::networkErrorCode() const
{
    return m_httpReply->error();
}

void HttpResponse::setNetworkReply(QNetworkReply *reply)
{
    m_httpReply = reply;
    connect(m_httpReply, &QNetworkReply::finished,
            this, &HttpResponse::httpResponseHandler);
}

QNetworkReply *HttpResponse::networkReplyObject() const
{
    return m_httpReply;
}

void HttpResponse::httpResponseHandler()
{
    //read reply only if no error has occured
    if(m_httpReply->error() == QNetworkReply::NoError) {
        setJsonRaw(m_httpReply->readAll());
        setStatus(HttpResponse::Ready);
    } else {
        qWarning()<<"Resposne: " <<m_httpReply->readAll()
                 << m_httpReply->error() << m_httpReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        setStatus(HttpResponse::Error);
    }
}

void HttpResponse::httpResponseCodeHandler()
{
    int httpCode = m_httpReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    emit httpResponseCodeChanged(httpCode);

    if(m_httpReply->error() == QNetworkReply::NoError)
        emit networkErrorCodeChanged(m_httpReply->error());

    switch (httpCode) {
    case 200:
        setStatus(HttpResponse::InProgress);
        break;
    case 400:
    case 401:
    case 403:
    case 404:
//        setStatus(HttpResponse::Error);
        break;
    }
}

void HttpResponse::httpResponseErrorHandler()
{

}

void HttpResponse::setStatus(int s)
{
    if(m_status != s) {
        m_status = s;
        emit statusChanged(m_status);
    }
}

void HttpResponse::setJsonRaw(const QByteArray &json)
{
    if(m_dataRaw != json) {
        m_dataRaw = json;
        emit dataRawChanged(m_dataRaw);
    }
}
