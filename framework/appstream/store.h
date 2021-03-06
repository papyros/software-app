/*
 * Papyros Software - The app store for Papyros
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STORE_H
#define STORE_H

#include <QObject>

#include "component.h"

namespace Appstream
{

enum SourceKind
{
    Appstream,
    Appdata,
    Metainfo,
    Desktop,
    Unknown
};

SourceKind kindFromFilename(QString filename);

class Store
{

public:
    bool load(QString path);

    Component componentById(QString id) const;

    QList<Component> allComponents() const { return m_components; }

private:
    void addComponent(Component component);
    bool loadFromAppstreamFile(QString filename, QString iconPath);

    QList<Component> m_components;
};
}

#endif // STORE_H
