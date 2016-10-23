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

#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QNetworkReply>
#include <QJsonObject>
#include <QQmlListProperty>

class HttpResponse;
class QNetworkAccessManager;
class QQmlPropertyMap;

class HttpManager : public QObject
{
    Q_OBJECT
public:
    enum Error {
        NoError = 1,
        FileNotExisting,
        FileNotOpened,
        UnkownError,
        InvalidNetworkRequest
    };

    Q_ENUM(Error)
    Q_PROPERTY(int errorCode READ errorCode NOTIFY errorCodeChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    Q_PROPERTY(QJsonObject urlParameters READ urlParameters WRITE setUrlParameters NOTIFY urlParametersChanged)
    Q_PROPERTY(QJsonObject httpHeaders READ httpHeaders WRITE setHttpHeaders NOTIFY httpHeadersChanged)

    HttpManager(QObject *parent = 0);
    ~HttpManager();

    int errorCode() const;

    QString host() const;
    void setHost(const QString &h);

    QUrl url() const;
    void setUrl(const QUrl &url);

    QString baseUrl() const;
    void setBaseUrl(const QString &url);

    QJsonObject urlParameters() const;
    void setUrlParameters(const QJsonObject &param);

    QJsonObject httpHeaders() const;
    void setHttpHeaders(QJsonObject &headers);

    Q_INVOKABLE void addHttpHeader(const QString &key, const QString &val);
    Q_INVOKABLE void setHttpAttribute(QNetworkRequest::Attribute attribute, const QVariant &value = QVariant());
    Q_INVOKABLE HttpResponse *sendHttpRequest(const QString &httpVerb, const QJsonObject &json = QJsonObject());
    Q_INVOKABLE HttpResponse *sendHttpRequest(const QString &httpVerb, const QByteArray &postData);
    HttpResponse *sendHttpMultiPartRequest(const QString &httpVerb, QHttpMultiPart *httpParts);

Q_SIGNALS:
    void hostChanged(QString);
    void baseUrlChanged(QString);
    void urlChanged(QUrl arg);
    void errorCodeChanged(int);
    void urlParametersChanged(QJsonObject);
    void httpHeadersChanged(QJsonObject);

protected:
    void setErrorCode(int err);

    int m_errorCode;
    QString m_host;
    QString m_baseUrl;
    QHash<int, QVariant> m_httpAttributes;
    QJsonObject m_httpHeaders;
    QJsonObject m_parameters;
    static QNetworkAccessManager *m_networkAccessManager;
    QNetworkRequest m_httpRequest;
};

#endif // HTTPMANAGER_H
