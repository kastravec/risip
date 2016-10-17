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

#include "httpmanager.h"
#include "httpresponse.h"

#include <QQmlPropertyMap>
#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QDebug>
#include <QByteArray>
#include <QUrlQuery>

QNetworkAccessManager *HttpManager::m_networkAccessManager = new QNetworkAccessManager;

HttpManager::HttpManager(QObject *parent)
    :QObject(parent)
    ,m_baseUrl()
{
}

HttpManager::~HttpManager()
{
}

QString HttpManager::baseUrl() const
{
    return m_baseUrl;
}

void HttpManager::setBaseUrl(const QString &url)
{
    if( !url.isEmpty() && !url.isNull() && url != m_baseUrl) {
        m_baseUrl = url;
        //checking if the '/' character is missing from the url
        QChar lastChar = url.at(url.length() -1);
        if(lastChar != QChar('/'))
            m_baseUrl.append("/");

        emit baseUrlChanged(m_baseUrl);
    }
}

QUrl HttpManager::url() const
{
    return m_httpRequest.url();
}

void HttpManager::setUrl(const QUrl &url)
{
    if(!url.isEmpty()) {
        m_httpRequest.setUrl(url);
        emit urlChanged(url);
    }
}

int HttpManager::errorCode() const
{
    return m_errorCode;
}

QString HttpManager::host() const
{
    return m_host;
}

void HttpManager::setHost(const QString &h)
{
    if(m_host != h) {
        m_host = h;
        emit hostChanged(m_host);
    }
}

QJsonObject HttpManager::httpHeaders() const
{
    return m_httpHeaders;
}

void HttpManager::setHttpHeaders(QJsonObject &headers)
{
    m_httpHeaders = headers;
    emit httpHeadersChanged(m_httpHeaders);
}

QJsonObject HttpManager::urlParameters() const
{
    return m_parameters;
}

void HttpManager::setUrlParameters(const QJsonObject &param)
{
    if(m_parameters != param) {
        m_parameters = param;
        emit urlParametersChanged(m_parameters);
    }
}

void HttpManager::addHttpHeader(const QString &key, const QString &val)
{
    if(!key.isEmpty() && !val.isEmpty()) {
        m_httpHeaders.insert(key, QJsonValue::fromVariant(val));
        m_httpRequest.setRawHeader(key.toUtf8(), val.toUtf8());
    }
}

void HttpManager::setHttpAttribute(QNetworkRequest::Attribute attribute, const QVariant &value)
{
    //TODO check validity of attribute values
    m_httpRequest.setAttribute(attribute, value);

    if(value.isNull())
        m_httpAttributes.remove(attribute);
    else
        m_httpAttributes[attribute] = value;
}

void HttpManager::setErrorCode(int err)
{
    if(m_errorCode != err) {
        m_errorCode = err;
        emit errorCodeChanged(m_errorCode);
    }
}

HttpResponse *HttpManager::sendHttpRequest(const QString &httpVerb, const QJsonObject &json)
{
    if(json.isEmpty())
        return sendHttpRequest(httpVerb, QByteArray());

    QJsonDocument jsDoc(json);
    return sendHttpRequest(httpVerb, jsDoc.toJson());
}

HttpResponse *HttpManager::sendHttpRequest(const QString &httpVerb, const QByteArray &postData)
{
    HttpResponse *response = new HttpResponse;

    if(m_networkAccessManager->networkAccessible() == QNetworkAccessManager::NotAccessible)
        return response;

    if(m_httpRequest.url().isEmpty())
        m_httpRequest.setUrl(baseUrl());

    qDebug()<<"Request URL: " << m_httpRequest.url();

    if(httpVerb == QStringLiteral("GET")) {
        if(!postData.isEmpty()) {
            setErrorCode(HttpManager::InvalidNetworkRequest);
            return response;
        }
        response->setNetworkReply(m_networkAccessManager->get(m_httpRequest));
    } else if(httpVerb == QStringLiteral("POST")) {
        response->setNetworkReply(m_networkAccessManager->post(m_httpRequest, postData));

    } else if(httpVerb == QStringLiteral("PUT")) {
        response->setNetworkReply(m_networkAccessManager->put(m_httpRequest, postData));

    } else if(httpVerb == QStringLiteral("PATCH")) {
        return response;

    } else if(httpVerb == QStringLiteral("DELETE")) {
        return response;
    } else {
        return response;
    }

    return response;
}

HttpResponse *HttpManager::sendHttpMultiPartRequest(const QString &httpVerb, QHttpMultiPart *httpParts)
{
    HttpResponse *response = new HttpResponse;

    // do not continue if network is not accessible
    if(m_networkAccessManager->networkAccessible() == QNetworkAccessManager::NotAccessible
            || httpParts == NULL)
        return response;

    if(httpVerb == QStringLiteral("POST")) {
        response->setNetworkReply(m_networkAccessManager->post(m_httpRequest, httpParts));
        httpParts->setParent(response);
    } else if(httpVerb == QStringLiteral("PUT")) {
        response->setNetworkReply(m_networkAccessManager->put(m_httpRequest, httpParts));

    } else if(httpVerb == QStringLiteral("PATCH")) {
        response = new HttpResponse;
    }

    return response;
}
