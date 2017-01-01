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
**    A copy of the license can be found also here <http://www.gnu.org/licenses/>.
**
************************************************************************************/

#include "apploader/risipuiloader.h"
#include "apploader/risipapplicationsettings.h"
#include "risipcontactimageprovider.h"
#include "risipcountryflagimageprovider.h"
#include "risipglobals.h"

#include <QQmlApplicationEngine>
#include <QDebug>

class RisipUiLoader::Private
{
public:
    QQmlApplicationEngine *qmlEngine;
    QString qmlFile;
};
RisipUiLoader::RisipUiLoader(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
}

RisipUiLoader::~RisipUiLoader()
{
    delete RisipApplicationSettings::instance();
    delete m_data;
    m_data = NULL;
}

void RisipUiLoader::start()
{
    if(m_data->qmlFile.isEmpty())
        return;

    if(!m_data->qmlEngine)
        m_data->qmlEngine = new QQmlApplicationEngine(this);

    m_data->qmlEngine->addImageProvider("contactIcon", new RisipContactImageProvider);
    m_data->qmlEngine->addImageProvider("countryFlags", new RisipCountryFlagImageProvider);

    m_data->qmlEngine->load(QUrl(m_data->qmlFile));
}

RisipApplicationSettings *RisipUiLoader::applicationSettings()
{
    return RisipApplicationSettings::instance();
}

QString RisipUiLoader::qmlFile() const
{
    return m_data->qmlFile;
}

void RisipUiLoader::setQmlFile(const QString &qmlFile)
{
    if(m_data->qmlFile != qmlFile && !qmlFile.isEmpty()) {
        m_data->qmlFile = qmlFile;
        emit qmlFileChanged(m_data->qmlFile);
    }
}
