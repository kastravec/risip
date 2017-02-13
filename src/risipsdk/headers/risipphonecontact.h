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
#ifndef RISIPPHONECONTACT_H
#define RISIPPHONECONTACT_H

#include "risipsdkglobal.h"
#include "risipphonenumber.h"
#include <QQmlListProperty>

namespace risip {

class RisipPhoneNumbersModel;

class RISIP_VOIPSDK_EXPORT RisipPhoneContact : public QObject
{
    Q_OBJECT
public:

    Q_PROPERTY(int contactId READ contactId NOTIFY contactIdChanged)
    Q_PROPERTY(QString fullName READ fullName NOTIFY fullNameChanged)
    Q_PROPERTY(QString initials READ initials NOTIFY initialsChanged)
    Q_PROPERTY(QString email READ email NOTIFY emailChanged)
    Q_PROPERTY(RisipPhoneNumbersModel * phoneNumbersModel READ phoneNumbersModel NOTIFY phoneNumbersModelChanged)
    Q_PROPERTY(QQmlListProperty<RisipPhoneNumber> phoneNumbers READ phoneNumbers CONSTANT)
    Q_PROPERTY(QByteArray contactImageData READ contactImageData NOTIFY contactImageDataChanged)

    explicit RisipPhoneContact(QObject *parent = 0);
    ~RisipPhoneContact();

    int contactId() const;
    void setContactId(int id);

    QString fullName() const;
    void setFullName(QString name);

    QString email() const;
    void setEmail(const QString &email);

    QString initials() const;

    RisipPhoneNumbersModel *phoneNumbersModel() const;
    QByteArray contactImageData() const;
    void setContactImageData(QByteArray imageData);

    QQmlListProperty<RisipPhoneNumber> phoneNumbers();
    QList<RisipPhoneNumber *> phoneNumberList() const;
    Q_INVOKABLE RisipPhoneNumber *phoneNumberForNumber(const QString &number);

    void addPhoneNumber(const QString &number, const QString &label = QString());
    void addPhoneNumber(RisipPhoneNumber *number);
    void removePhoneNumber(RisipPhoneNumber *number);

Q_SIGNALS:
    void contactIdChanged(int id);
    void fullNameChanged(const QString &name);
    void initialsChanged(const QString &initials);
    void emailChanged(const QString &email);
    void phoneNumbersModelChanged(RisipPhoneNumbersModel *phoneNumbersModel);
    void phoneNumbersChanged(const QStringList &numbers);
    void contactImageDataChanged(const QByteArray &imageData);

private:
    class Private;
    Private *m_data;
};

} //end of risip namespace

#endif // RISIPPHONECONTACT_H
