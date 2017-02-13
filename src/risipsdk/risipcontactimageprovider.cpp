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
#include "risipcontactimageprovider.h"
#include "risipcontactmanager.h"
#include "risipphonecontact.h"

#include <QPixmap>
#include <QPainter>

#include <QDebug>

namespace risip {

RisipContactImageProvider::RisipContactImageProvider()
    :QQuickImageProvider(QQuickImageProvider::Pixmap)
{

}

QPixmap RisipContactImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    int width = 80;
    int height = 80;
    if (size)
        *size = QSize(width, height);

    QPixmap profileIcon(requestedSize.width() > 0 ? requestedSize.width() : width,
                   requestedSize.height() > 0 ? requestedSize.height() : height);

    RisipPhoneContact *phoneContact = RisipContactManager::instance()->contactForName(id);
    if(phoneContact) {
        if(!profileIcon.loadFromData(phoneContact->contactImageData()))
            profileIcon.load(QString(":/images/icons/128/CircleBlack.png"));
    }

    return profileIcon;
}

} //end of risip namespace
