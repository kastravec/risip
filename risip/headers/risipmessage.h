/***********************************************************************************
**    Copyright (C) 2016  Petref Saraci
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
**    A copy of the license is also here <http://www.gnu.org/licenses/>.
**
************************************************************************************/

#ifndef RISIPMESSAGE_H
#define RISIPMESSAGE_H

#include <QObject>

class RisipMessage : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString messageBody READ messageBody NOTIFY messageBodyChanged)
    Q_PROPERTY(QString contactUri READ contactUri NOTIFY contactUriChanged)
    Q_PROPERTY(QString senderUri READ senderUri NOTIFY senderUriChanged)
    Q_PROPERTY(QString contentType READ contentType NOTIFY contentTypeChanged)

    RisipMessage(QObject *parent = 0);
    ~RisipMessage();

    QString messageBody() const;
    QString contactUri() const;
    QString senderUri() const;
    QString contentType() const;

    void createMessage(const QString &messBody, const QString &contactUri,
                       const QString senderUri, const QString &contentType);

Q_SIGNALS:
    void messageBodyChanged(QString &message);
    void contactUriChanged(QString &contact);
    void senderUriChanged(QString &sender);
    void contentTypeChanged(QString &contentType);

private:
    void setMessageBody(const QString &messageBody);
    void setContactUri(const QString &contact);
    void setSenderUri(const QString &sender);
    void setContentType(const QString &type);

    QString m_messageBody;
    QString m_contactUri;
    QString m_senderUri;
    QString m_contentType;
};

#endif // RISIPMESSAGE_H
