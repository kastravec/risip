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
#include "risipratemanager.h"
#include "risipglobals.h"
#include "risipcountryratesmodel.h"

namespace risip {

RisipRateManager *RisipRateManager::m_instance = NULL;
RisipRateManager *RisipRateManager::instance()
{
    if(m_instance == NULL)
        m_instance = new RisipRateManager;

    return m_instance;
}

RisipRateManager::RisipRateManager(QObject *parent)
    :QObject(parent)
    ,m_countryRatesModel(new RisipCountryRatesModel(this))
{
}

RisipRateManager::~RisipRateManager()
{
}

RisipCountryRatesModel *RisipRateManager::countryRatesModel() const
{
    if(m_countryRatesModel->rowCount() == 0)
        m_countryRatesModel->setCountryList(RisipGlobals::instance()->countries());

    return m_countryRatesModel;
}

} //end of risip namespace
