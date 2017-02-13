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
#include "risipcountryratesmodel.h"
#include "risipglobals.h"

#include <QDebug>

namespace risip {

RisipCountryRatesModel::RisipCountryRatesModel(QObject *parent)
    :QAbstractListModel(parent)
{
    connect(RisipGlobals::instance(), &RisipGlobals::countryListReady,
            this, &RisipCountryRatesModel::setCountryList, Qt::QueuedConnection);
}

RisipCountryRatesModel::~RisipCountryRatesModel()
{
}

QHash<int, QByteArray> RisipCountryRatesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CountryName] = "countryName";
    roles[CountryCode] = "countryCode";
    roles[CountryRate] = "countryRate";
    roles[CountryPrefix] = "countryPrefix";
    roles[ValidFromDate] = "validFromDate";
    roles[ValidTillDate] = "validTillDate";
    return roles;
}

int RisipCountryRatesModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_allCountries.count();
}

QVariant RisipCountryRatesModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()
            || index.row() > m_allCountries.count())
        return QVariant();

    Country country = m_allCountries[index.row()];
    switch (role) {
    case CountryName:
        return country.name;
    case CountryCode:
        return country.code;
    case CountryPrefix:
        return country.prefix;
    case CountryRate:
        return country.rate.actualRate + country.rate.currency + QString("/") + country.rate.timeMeasure;
    case ValidFromDate:
        return country.rate.validFromDate;
    case ValidTillDate:
        return country.rate.validTillDate;
    default:
        return QVariant();
    }

    return QVariant();
}

void RisipCountryRatesModel::setCountryList(const QList<Country> list)
{
    m_allCountries = list;
}

} //end of risip namespace
