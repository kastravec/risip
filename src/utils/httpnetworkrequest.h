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
#ifndef HTTPNETWORKREQUEST_H
#define HTTPNETWORKREQUEST_H

#include <QUrlQuery>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>

class QHttpMultiPart;
class QNetworkAccessManager;

class HttpNetworkRequest : public QObject
{
    Q_OBJECT
public:
    enum RequestType {
        Json = 11,
        XML,
        PlainText,
        Html,
        Unknown = -1
    };

    enum Status {
        RequestReady = 101,
        RequestRunning,
        ResponseReady,
        ResponseError,
        RequestInvalid,
        Null = -1
    };

    Q_ENUM(RequestType)
    Q_ENUM(Status)
    Q_PROPERTY(int requestType READ requestType WRITE setRequestType NOTIFY requestTypeChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(QUrl baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)
    Q_PROPERTY(QJsonObject urlParameters READ urlParameters WRITE setUrlParameters NOTIFY urlParametersChanged)
    Q_PROPERTY(QJsonObject httpHeaders READ httpHeaders WRITE setHttpHeaders NOTIFY httpHeadersChanged)

    explicit HttpNetworkRequest(QObject *parent = 0);
    ~HttpNetworkRequest();

    int requestType() const;
    void setRequestType(int type);

    int status() const;

    QUrl baseUrl() const;
    void setBaseUrl(const QUrl &url);

    QUrl url() const;
    void setUrl(const QUrl &url);

    QJsonObject urlParameters() const;
    void setUrlParameters(const QJsonObject &param);

    QJsonObject httpHeaders() const;
    void setHttpHeaders(QJsonObject &headers);

    Q_INVOKABLE void setHttpAttribute(QNetworkRequest::Attribute attribute, const QVariant &value = QVariant());
    Q_INVOKABLE void validateHttpRequest();
    Q_INVOKABLE void get();
    Q_INVOKABLE void post(const QByteArray &data);
    Q_INVOKABLE void postMultipart(QHttpMultiPart *parts);
    Q_INVOKABLE void put(const QByteArray &data);
    Q_INVOKABLE void putMultipart(QHttpMultiPart *parts);
    Q_INVOKABLE void patch(const QByteArray &data);
    Q_INVOKABLE void patchMultipart(QHttpMultiPart *parts);
    Q_INVOKABLE void deleteData();

Q_SIGNALS:
    void requestTypeChanged(int type);
    void statusChanged(int status);
    void baseUrlChanged(const QUrl &url);
    void urlPrefixChanged(const QString &prefix);
    void urlChanged(const QUrl &url);
    void urlParametersChanged(QJsonObject &parameters);
    void httpHeadersChanged(QJsonObject &headers);
    void replyReady(const QByteArray &data);

private Q_SLOTS:
    void networkResponseHandler();
    void networkResponseMetadataHandler();
    void networkResponseErrorHandler(int code);
    void networkResponseDownloadHandler(qint64 bytesReceived, qint64 bytesTotal);
    void networkResponseUploadHandler(qint64 bytesSent, qint64 bytesTotal);
    void networkResponseSslErrorsHandler(const QList<QSslError> &errors);

private:
    void setStatus(int status);
    void sendHttpRequest(const QString &httpVerb, const QByteArray &data = QByteArray());
    void sendMultipartRequest(const QString &httpVerb, QHttpMultiPart *httpParts);

    int m_type;
    int m_status;
    QUrl m_baseUrl;
    QJsonObject m_urlParameters;
    QJsonObject m_httpHeaders;
    QHash<int, QVariant> m_httpAttributes;
    QUrlQuery m_urlQuery;
    QNetworkRequest m_httpRequest;
    QNetworkReply *m_httpResponse;
    static QNetworkAccessManager *m_networkAccessManager;
};

#endif // HTTPNETWORKREQUEST_H
