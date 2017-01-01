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

NetworkAccessManager *NetworkAccessManager::m_instance = NULL;
NetworkAccessManager *NetworkAccessManager::instance()
{
    if(m_instance == NULL)
        m_instance = new NetworkAccessManager;

    return m_instance;
}

NetworkAccessManager::~NetworkAccessManager()
{
}

NetworkAccessManager::NetworkAccessManager(QObject *parent)
    :QNetworkAccessManager(parent)
{

}

class HttpNetworkRequest::Private
{
public:
    int status;
    QUrl baseUrl;
    QJsonObject urlParameters;
    QJsonObject httpHeaders;
    QHash<int, QVariant> httpAttributes;
    QUrlQuery urlQuery;
    QNetworkRequest httpRequest;
    QNetworkReply *httpResponse;
    void *customData;
    NetworkAccessManager *networkAccessManager;
    QByteArray lastResponseData;
};

HttpNetworkRequest::HttpNetworkRequest(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->status = Null;
    m_data->httpResponse = NULL;
    m_data->networkAccessManager = NetworkAccessManager::instance();
}

HttpNetworkRequest::~HttpNetworkRequest()
{
    delete m_data;
    m_data = NULL;
}

int HttpNetworkRequest::status() const
{
    return m_data->status;
}

void HttpNetworkRequest::setStatus(int status)
{
    if(m_data->status != status) {
        m_data->status = status;
        emit statusChanged(m_data->status);
    }
}

QUrl HttpNetworkRequest::baseUrl() const
{
    return m_data->baseUrl;
}

void HttpNetworkRequest::setBaseUrl(const QUrl &url)
{
    if(m_data->baseUrl != url) {
        m_data->baseUrl = url;
        emit baseUrlChanged(m_data->baseUrl);
    }
}

QUrl HttpNetworkRequest::url() const
{
    return m_data->httpRequest.url();
}

void HttpNetworkRequest::setUrl(const QUrl &url)
{
    if(m_data->httpRequest.url() != url ) {
        m_data->httpRequest.setUrl(url);
        emit urlChanged(m_data->httpRequest.url());
    }
}

QJsonObject HttpNetworkRequest::urlParameters() const
{
    return m_data->urlParameters;
}

void HttpNetworkRequest::setUrlParameters(const QJsonObject &param)
{
    m_data->urlParameters = param;
    for(int i=0; i<m_data->urlParameters.count(); ++i) {
        m_data->urlQuery.addQueryItem(m_data->urlParameters.keys()[i],
                                      m_data->urlParameters.value(m_data->urlParameters.keys()[i]).toString());
    }

    emit urlParametersChanged(m_data->urlParameters);
}

QJsonObject HttpNetworkRequest::httpHeaders() const
{
    return m_data->httpHeaders;
}

void HttpNetworkRequest::setHttpHeaders(QJsonObject &headers)
{
    m_data->httpHeaders = headers;

    QString key;
    for(int i=0; i<m_data->httpHeaders.count(); ++i) {
        key = m_data->httpHeaders.keys()[i];
        m_data->httpRequest.setRawHeader(key.toLatin1(), m_data->httpHeaders.value(key).toString().toLatin1());
    }

    emit httpHeadersChanged(m_data->httpHeaders);
}

QByteArray HttpNetworkRequest::lastResponseData() const
{
    return m_data->lastResponseData;
}

void HttpNetworkRequest::setHttpAttribute(QNetworkRequest::Attribute attribute, const QVariant &value)
{
    //TODO check validity of attribute values
    m_data->httpRequest.setAttribute(attribute, value);

    if(value.isNull())
        m_data->httpAttributes.remove(attribute);
    else
        m_data->httpAttributes[attribute] = value;

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
    if(m_data->baseUrl.isEmpty()) {
        setStatus(RequestInvalid);
        return;
    }

    //setting the url query first
    //check if url is valid and then setting the url if valid
    if(!m_data->urlQuery.isEmpty())
        m_data->baseUrl.setQuery(m_data->urlQuery);

    if(!m_data->baseUrl.isValid()) {
        setStatus(RequestInvalid);
        return;
    }

    setUrl(m_data->baseUrl);
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
    m_data->customData = dt;
}

void *HttpNetworkRequest::risipData()
{
    return m_data->customData;
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

    if(m_data->status == RequestReady) {
        if(httpVerb == QString("GET"))
            m_data->httpResponse = m_data->networkAccessManager->get(m_data->httpRequest);
        else if(httpVerb == QString("POST"))
            m_data->httpResponse = m_data->networkAccessManager->post(m_data->httpRequest, data);
        else if(httpVerb == QString("PUT"))
            m_data->networkAccessManager->put(m_data->httpRequest, data);
        else if(httpVerb == QString("PATCH"))
            m_data->networkAccessManager->sendCustomRequest(m_data->httpRequest, QByteArray("PATCH"));
        else if(httpVerb == QString("DELETE"))
            m_data->networkAccessManager->deleteResource(m_data->httpRequest);
        else {
            //esle there is no proper HTTP verb set so simply update status and return;
            setStatus(RequestInvalid);
            return;
        }

        connect(m_data->httpResponse, &QNetworkReply::finished,
                this, &HttpNetworkRequest::networkResponseHandler, Qt::DirectConnection);
        connect(m_data->httpResponse, &QNetworkReply::metaDataChanged,
                this, &HttpNetworkRequest::networkResponseMetadataHandler, Qt::DirectConnection);
        connect(m_data->httpResponse, &QNetworkReply::sslErrors,
                this, &HttpNetworkRequest::networkResponseSslErrorsHandler, Qt::DirectConnection);
        connect(m_data->httpResponse, &QNetworkReply::downloadProgress,
                this, &HttpNetworkRequest::networkResponseDownloadHandler, Qt::DirectConnection);
        connect(m_data->httpResponse, &QNetworkReply::uploadProgress,
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
//    qDebug()<<"RAW HTTP REPLY " << m_data->httpResponse->readAll();

    m_data->lastResponseData = m_data->httpResponse->readAll();
    if(m_data->httpResponse->error() == QNetworkReply::NoError) {
        emit replyReady(m_data->lastResponseData);
        setStatus(ResponseReady);
    } else {
        emit replyError(m_data->httpResponse->error(), m_data->lastResponseData);
        setStatus(ResponseError);
    }

    emit lastResponseDataChanged(m_data->lastResponseData);
    //closing and deleting the finished network resposne.
    m_data->httpResponse->close();
    m_data->httpResponse->deleteLater();
}

/**
 * @brief HttpNetworkRequest::networkResponseMetadataHandler
 *
 * Internal API.
 */
void HttpNetworkRequest::networkResponseMetadataHandler()
{
//    qDebug()<<"HTTP REPLY METADATA: " <<m_data->httpResponse->rawHeaderList();
}

void HttpNetworkRequest::networkResponseErrorHandler(int code)
{
//    qDebug()<<"HTTP RESPONSE ERROR: " << code;
//    emit replyError(code);
}

void HttpNetworkRequest::networkResponseDownloadHandler(qint64 bytesReceived, qint64 bytesTotal)
{
//    qDebug()<<"HTTP DOWNLOAD PROGRESS: " << bytesReceived << bytesTotal;
}

void HttpNetworkRequest::networkResponseUploadHandler(qint64 bytesSent, qint64 bytesTotal)
{
//    qDebug()<<"HTTP UPLOAD PROGRESS: " <<bytesSent <<bytesTotal;
}

void HttpNetworkRequest::networkResponseSslErrorsHandler(const QList<QSslError> &errors)
{
    emit sslErrors(errors);
}
