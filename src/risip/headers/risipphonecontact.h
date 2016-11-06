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

#include <QObject>

class RisipPhoneContact : public QObject
{
    Q_OBJECT
public:

    Q_PROPERTY(int contactId READ contactId NOTIFY contactIdChanged)
    Q_PROPERTY(QString fullName READ fullName NOTIFY fullNameChanged)
    Q_PROPERTY(QStringList phoneNumbers READ phoneNumbers NOTIFY phoneNumbersChanged)

    explicit RisipPhoneContact(QObject *parent = 0);
    ~RisipPhoneContact();

    int contactId() const;
    void setContactId(int id);

    QString fullName() const;
    void setFullName(const QString &name);

    QStringList phoneNumbers() const;
    void setPhoneNumbers(const QStringList &numbers);

Q_SIGNALS:
    void contactIdChanged(int id);
    void fullNameChanged(const QString &name);
    void phoneNumbersChanged(const QStringList &numbers);

private:
    int m_id;
    QString m_fullName;
    QStringList m_phoneNumbers;
};

#endif // RISIPPHONECONTACT_H
