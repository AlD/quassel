/***************************************************************************
 *   Copyright (C) 2005-08 by the Quassel Project                          *
 *   devel@quassel-irc.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) version 3.                                           *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef UISETTINGS_H
#define UISETTINGS_H

#include "clientsettings.h"
#include "uistyle.h"

class UiSettings : public ClientSettings {
public:
  UiSettings(const QString &group = "Ui");

  inline void setValue(const QString &key, const QVariant &data) { setLocalValue(key, data); }
  inline QVariant value(const QString &key, const QVariant &def = QVariant()) { return localValue(key, def); }
  inline void remove(const QString &key) { removeLocalKey(key); }
};


class UiStyleSettings : public ClientSettings {
public:
  UiStyleSettings(const QString &group = "UiStyle");

  inline void setValue(const QString &key, const QVariant &data) { setLocalValue(key, data); }
  inline QVariant value(const QString &key, const QVariant &def = QVariant()) { return localValue(key, def); }
  inline void remove(const QString &key) { removeLocalKey(key); }

  void setCustomFormat(UiStyle::FormatType, QTextCharFormat);
  QTextCharFormat customFormat(UiStyle::FormatType);

  void removeCustomFormat(UiStyle::FormatType);
  QList<UiStyle::FormatType> availableFormats();
};

#endif
