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
#ifndef RISIPPHONENUMBER_H
#define RISIPPHONENUMBER_H

#include "risipsdkglobal.h"
#include <QObject>

namespace risip {

class RisipPhoneContact;

class RISIP_VOIPSDK_EXPORT RisipPhoneNumber : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(RisipPhoneContact *phoneContact READ phoneContact WRITE setPhoneContact NOTIFY phoneContactChanged)
    Q_PROPERTY(QString rawNumber READ rawNumber WRITE setRawNumber NOTIFY rawNumberChanged)
    Q_PROPERTY(QString countryPrefix READ countryPrefix NOTIFY countryPrefixChanged)
    Q_PROPERTY(QString secondPrefix READ secondPrefix NOTIFY secondPrefixChanged)
    Q_PROPERTY(QString number READ number NOTIFY numberChanged)
    Q_PROPERTY(QString fullNumber READ fullNumber NOTIFY fullNumberChanged)
    Q_PROPERTY(QString countryCode READ countryCode NOTIFY countryCodeChanged)
    Q_PROPERTY(QString countryName READ countryName NOTIFY countryNameChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(QStringList fullNumberParts READ fullNumberParts NOTIFY fullNumberPartsChanged)

    explicit RisipPhoneNumber(const QString &rawNumber = QString(), QObject *parent = 0);
    ~RisipPhoneNumber();

    RisipPhoneContact *phoneContact() const;
    void setPhoneContact(RisipPhoneContact *contact);

    QString rawNumber() const;
    void setRawNumber(const QString &number);

    QString label() const;
    void setLabel(const QString &label);

    QStringList fullNumberParts() const;
    void setNumberParts(const QStringList &parts);

    QString countryPrefix() const;
    void setCountryPrefix(const QString &prefix);

    QString countryCode() const;
    void setCountryCode(const QString &code);

    QString countryName() const;
    void setCountryName(const QString &name);

    QString secondPrefix() const;
    void setSecondPrefix(const QString &prefix);

    QString number() const;
    void setNumber(const QString &number);

    QString fullNumber() const;
    void setFullNumber(const QString &name);

    bool valid() const;

Q_SIGNALS:
    void phoneContactChanged(RisipPhoneContact *contact);
    void rawNumberChanged(const QString &number);
    void countryPrefixChanged(const QString &prefix);
    void secondPrefixChanged(const QString &prefix);
    void numberChanged(const QString &number);
    void fullNumberChanged(const QString &number);
    void countryCodeChanged(const QString &code);
    void countryNameChanged(const QString &name);
    void validChanged(bool valid);
    void labelChanged(const QString &label);
    void fullNumberPartsChanged(const QStringList &parts);

private:
    void reset();

    class Private;
    Private *m_data;
};

} //end of risip namespace

#endif // RISIPPHONENUMBER_H
