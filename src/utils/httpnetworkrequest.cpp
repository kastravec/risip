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
#include "httpnetworkrequest.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>

QNetworkAccessManager *HttpNetworkRequest::m_networkAccessManager = new QNetworkAccessManager;

HttpNetworkRequest::HttpNetworkRequest(QObject *parent)
    :QObject(parent)
    ,m_type(Unknown)
    ,m_status(Null)
    ,m_baseUrl()
    ,m_urlParameters()
    ,m_httpHeaders()
    ,m_httpAttributes()
    ,m_urlQuery()
    ,m_httpResponse(NULL)
{
}

HttpNetworkRequest::~HttpNetworkRequest()
{
}

int HttpNetworkRequest::requestType() const
{
    return m_type;
}

void HttpNetworkRequest::setRequestType(int type)
{
    if(m_type != type) {
        m_type = type;

        switch (m_type) {
        case Json:
            m_httpHeaders.insert(QString("Content-type"), QJsonValue(QString("application/json")));
            m_httpHeaders.insert(QString("Accept"), QJsonValue(QString("application/json")));
            break;
        case XML:
            m_httpHeaders.insert(QString("Content-type"), QJsonValue(QString("application/xml")));
            m_httpHeaders.insert(QString("Accept"), QJsonValue(QString("application/json")));
        default:
            break;
        }
        emit requestTypeChanged(m_type);
    }
}

int HttpNetworkRequest::status() const
{
    return m_status;
}

void HttpNetworkRequest::setStatus(int status)
{
    if(m_status != status) {
        m_status = status;
        emit statusChanged(m_status);
    }
}

QUrl HttpNetworkRequest::baseUrl() const
{
    return m_baseUrl;
}

void HttpNetworkRequest::setBaseUrl(const QUrl &url)
{
    if(m_baseUrl != url) {
        m_baseUrl = url;
        emit baseUrlChanged(m_baseUrl);
    }
}

QUrl HttpNetworkRequest::url() const
{
    return m_httpRequest.url();
}

void HttpNetworkRequest::setUrl(const QUrl &url)
{
    if(m_httpRequest.url() != url ) {
        m_httpRequest.setUrl(url);
        emit urlChanged(m_httpRequest.url());
    }
}

QJsonObject HttpNetworkRequest::urlParameters() const
{
    return m_urlParameters;
}

void HttpNetworkRequest::setUrlParameters(const QJsonObject &param)
{
    m_urlParameters = param;
    QString key;
    for(int i=0; i<m_urlParameters.count(); ++i) {
        key = m_urlParameters.keys()[i];
        m_urlQuery.addQueryItem(key, m_urlParameters.value(key).toString());
    }

    emit urlParametersChanged(m_urlParameters);
}

QJsonObject HttpNetworkRequest::httpHeaders() const
{
    return m_httpHeaders;
}

void HttpNetworkRequest::setHttpHeaders(QJsonObject &headers)
{
    m_httpHeaders = headers;

    QString key;
    for(int i=0; i<m_httpHeaders.count(); ++i) {
        key = m_httpHeaders.keys()[i];
        m_httpRequest.setRawHeader(key.toLatin1(), m_httpHeaders.value(key).toString().toLatin1());
    }

    emit httpHeadersChanged(m_httpHeaders);
}

void HttpNetworkRequest::setHttpAttribute(QNetworkRequest::Attribute attribute, const QVariant &value)
{
    //TODO check validity of attribute values
    m_httpRequest.setAttribute(attribute, value);

    if(value.isNull())
        m_httpAttributes.remove(attribute);
    else
        m_httpAttributes[attribute] = value;

}

/**
 * @brief HttpNetworkRequest::validateHttpRequest
 *
 * Call this function each to check and validate your http request.
 * This function is called by Risip on each network request.
 */
void HttpNetworkRequest::validateHttpRequest()
{
    //check if base url is not set
    if(m_baseUrl.isEmpty()) {
        setStatus(RequestInvalid);
        return;
    }

    //setting the url query first
    //check if url is valid and then setting the url if valid
    m_baseUrl.setQuery(m_urlQuery);
    if(!m_baseUrl.isValid()) {
        setStatus(RequestInvalid);
        return;
    }

    setUrl(m_baseUrl);
    setStatus(RequestReady); //MUST set this in order for a request to succeed
}

void HttpNetworkRequest::get()
{
    sendHttpRequest(QString("GET"));
}

void HttpNetworkRequest::post(const QByteArray &data)
{
    sendHttpRequest(QString("POST"), data);
}

void HttpNetworkRequest::postMultipart(QHttpMultiPart *parts)
{
    sendMultipartRequest(QString("POST"), parts);
}

void HttpNetworkRequest::put(const QByteArray &data)
{
    sendHttpRequest(QString("PUT"), data);
}

void HttpNetworkRequest::putMultipart(QHttpMultiPart *parts)
{
    sendMultipartRequest(QString("PUT"), parts);
}

void HttpNetworkRequest::patch(const QByteArray &data)
{
    sendHttpRequest(QString("PATCH"), data);
}

void HttpNetworkRequest::patchMultipart(QHttpMultiPart *parts)
{
    sendMultipartRequest(QString("PATCH"), parts);
}

void HttpNetworkRequest::deleteData()
{
    sendHttpRequest(QString("DELETE"));
}

void HttpNetworkRequest::setRisipData(void *dt)
{
    m_customData = dt;
}

void *HttpNetworkRequest::risipData()
{
    return m_customData;
}

/**
 * @brief HttpNetworkRequest::sendHttpRequest
 * @param httpVerb the HTTP verb for the request, i.e. GET, POST, etc
 * @param data any data to be sent, i.e. for a POST type of requests
 *
 * Internal API.
 */
void HttpNetworkRequest::sendHttpRequest(const QString &httpVerb, const QByteArray &data)
{
    if(httpVerb.isEmpty())
        return;

    //always validating request
    validateHttpRequest();

    if(m_status == RequestReady) {
        if(httpVerb == QString("GET"))
            m_httpResponse = m_networkAccessManager->get(m_httpRequest);
        else if(httpVerb == QString("POST"))
            m_httpResponse = m_networkAccessManager->post(m_httpRequest, data);
        else if(httpVerb == QString("PUT"))
            m_networkAccessManager->put(m_httpRequest, data);
        else if(httpVerb == QString("PATCH"))
            m_networkAccessManager->sendCustomRequest(m_httpRequest, QByteArray("PATCH"));
        else if(httpVerb == QString("DELETE"))
            m_networkAccessManager->deleteResource(m_httpRequest);
        else {
            //esle there is no proper HTTP verb set so simply update status and return;
            setStatus(RequestInvalid);
            return;
        }

        connect(m_httpResponse, &QNetworkReply::finished,
                this, &HttpNetworkRequest::networkResponseHandler, Qt::DirectConnection);
        connect(m_httpResponse, &QNetworkReply::metaDataChanged,
                this, &HttpNetworkRequest::networkResponseMetadataHandler, Qt::DirectConnection);
        connect(m_httpResponse, &QNetworkReply::sslErrors,
                this, &HttpNetworkRequest::networkResponseSslErrorsHandler, Qt::DirectConnection);
        connect(m_httpResponse, &QNetworkReply::downloadProgress,
                this, &HttpNetworkRequest::networkResponseDownloadHandler, Qt::DirectConnection);
        connect(m_httpResponse, &QNetworkReply::uploadProgress,
                this, &HttpNetworkRequest::networkResponseUploadHandler, Qt::DirectConnection);

        //updating status
        setStatus(RequestRunning);
    }
}

/**
 * @brief HttpNetworkRequest::sendMultipartRequest
 * @param httpVerb
 * @param httpParts
 *
 * Internal API.
 *
 * Same as sendHttpRequest but for Multi HTTP parts.
 *
 */
void HttpNetworkRequest::sendMultipartRequest(const QString &httpVerb, QHttpMultiPart *httpParts)
{
    if(httpVerb.isEmpty())
        return;

    validateHttpRequest();
}

/**
 * @brief HttpNetworkRequest::networkResponseHandler
 *
 * Internal API.
 *
 * Handling the response from a request. This is connected to the QNetworkReply::finished signal.
 */
void HttpNetworkRequest::networkResponseHandler()
{
    if(m_httpResponse->error() == QNetworkReply::NoError) {
        QByteArray data = m_httpResponse->readAll();
        setStatus(ResponseReady);
        emit replyReady(data);
    } else {
        qDebug()<<"ERROR RESPONSE: "<< m_httpResponse->readAll() <<m_httpResponse->errorString() <<m_httpResponse->error();
        setStatus(ResponseError);
    }

    //closing and deleting the finished network resposne.
    m_httpResponse->close();
    m_httpResponse->deleteLater();
}

/**
 * @brief HttpNetworkRequest::networkResponseMetadataHandler
 *
 * Internal API.
 */
void HttpNetworkRequest::networkResponseMetadataHandler()
{

}

void HttpNetworkRequest::networkResponseErrorHandler(int code)
{
    emit error(code);
}

void HttpNetworkRequest::networkResponseDownloadHandler(qint64 bytesReceived, qint64 bytesTotal)
{

}

void HttpNetworkRequest::networkResponseUploadHandler(qint64 bytesSent, qint64 bytesTotal)
{

}

void HttpNetworkRequest::networkResponseSslErrorsHandler(const QList<QSslError> &errors)
{
    emit sslErrors(errors);
}
