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
#ifndef RISIPCOUNTRYRATESMODEL_H
#define RISIPCOUNTRYRATESMODEL_H

#include "risipsdkglobal.h"
#include "risipglobals.h"
#include <QAbstractListModel>

namespace risip {

class RISIP_VOIPSDK_EXPORT RisipCountryRatesModel: public QAbstractListModel
{
    Q_OBJECT

public:
    enum RisipCountryRateDataRole {
        CountryName = Qt::UserRole +1,
        CountryCode,
        CountryPrefix,
        CountryRate,
        ValidFromDate,
        ValidTillDate
    };

    explicit RisipCountryRatesModel(QObject *parent = 0);
    ~RisipCountryRatesModel();

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public Q_SLOTS:
    void setCountryList(const QList<Country> list);

private:
    QList<Country> m_allCountries;
};

} //end of risip namespace

#endif // RISIPCOUNTRYRATESMODEL_H
