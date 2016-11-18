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
#include <QQmlListProperty>
class RisipPhoneContact;
class RisipPhoneNumbersModel;

class RisipPhoneNumber : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(RisipPhoneContact *phoneContact READ phoneContact WRITE setPhoneContact NOTIFY phoneContactChanged)
    Q_PROPERTY(QString rawNumber READ rawNumber WRITE setRawNumber NOTIFY rawNumberChanged)
    Q_PROPERTY(QString countryPrefix READ countryPrefix NOTIFY countryPrefixChanged)
    Q_PROPERTY(QString nationalPrefix READ nationalPrefix NOTIFY nationalPrefixChanged)
    Q_PROPERTY(QString number READ number NOTIFY numberChanged)
    Q_PROPERTY(QString fullNumber READ fullNumber NOTIFY fullNumberChanged)
    Q_PROPERTY(QString countryCode READ countryCode NOTIFY countryCodeChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)

    explicit RisipPhoneNumber(const QString &rawNumber = QString(), QObject *parent = 0);
    ~RisipPhoneNumber();

    RisipPhoneContact *phoneContact() const;
    void setPhoneContact(RisipPhoneContact *contact);

    QString rawNumber() const;
    void setRawNumber(const QString &number);

    QString label() const;
    void setLabel(const QString &label);

    QString countryPrefix() const;
    QString nationalPrefix() const;
    QString number() const;
    QString fullNumber() const;
    QString countryCode() const;
    bool valid() const;

Q_SIGNALS:
    void phoneContactChanged(RisipPhoneContact *contact);
    void rawNumberChanged(const QString &number);
    void countryPrefixChanged(const QString &prefix);
    void nationalPrefixChanged(const QString &prefix);
    void numberChanged(const QString &number);
    void fullNumberChanged(const QString &number);
    void countryCodeChanged(const QString &code);
    void validChanged(bool valid);
    void labelChanged(const QString &label);

private:
    void validate();
    void reset();

    RisipPhoneContact *m_phoneContact;
    QString m_rawNumber;
    QString m_countryPrefix;
    QString m_nationalPrefix;
    QString m_number;
    QString m_countryCode;
    QString m_label;
};

class RisipPhoneContact : public QObject
{
    Q_OBJECT
public:

    Q_PROPERTY(int contactId READ contactId NOTIFY contactIdChanged)
    Q_PROPERTY(QString fullName READ fullName NOTIFY fullNameChanged)
    Q_PROPERTY(QString email READ email NOTIFY emailChanged)
    Q_PROPERTY(RisipPhoneNumbersModel * phoneNumbersModel READ phoneNumbersModel NOTIFY phoneNumbersModelChanged)
    Q_PROPERTY(QQmlListProperty<RisipPhoneNumber> phoneNumbers READ phoneNumbers CONSTANT)

    explicit RisipPhoneContact(QObject *parent = 0);
    ~RisipPhoneContact();

    int contactId() const;
    void setContactId(int id);

    QString fullName() const;
    void setFullName(const QString &name);

    QString email() const;
    void setEmail(const QString &email);

    RisipPhoneNumbersModel *phoneNumbersModel() const;

    QQmlListProperty<RisipPhoneNumber> phoneNumbers();
    QList<RisipPhoneNumber *> phoneNumberList() const;
    void addPhoneNumber(const QString &number, const QString &label = QString());
    void addPhoneNumber(RisipPhoneNumber *number);
    void removePhoneNumber(RisipPhoneNumber *number);

Q_SIGNALS:
    void contactIdChanged(int id);
    void fullNameChanged(const QString &name);
    void emailChanged(const QString &email);
    void phoneNumbersModelChanged(RisipPhoneNumbersModel *phoneNumbersModel);
    void phoneNumbersChanged(const QStringList &numbers);

private:
    int m_id;
    QString m_fullName;
    QString m_email;
    QHash<QString, RisipPhoneNumber *> m_phoneNumbers;
    RisipPhoneNumbersModel *m_phoneNumbersModel;
};

#endif // RISIPPHONECONTACT_H
