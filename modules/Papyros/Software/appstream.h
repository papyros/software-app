/*
 * Papyros Software - The app store for Papyros
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 * Copyright (C) 2015 Richard Hughes <richard@hughsie.com>
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

#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

#include <appstream-glib.h>

namespace Appstream
{

class Component: public QObject
{
    Q_OBJECT

public:
    Component(AsApp *item, QObject *parent = nullptr);

    void convertIconNameToPath(QString *path);

    QString name() const { return as_app_get_name(m_item, NULL); }
    QString comment() const { return as_app_get_comment(m_item, NULL); }
    // TODO: Add urls
    QString license() const { return as_app_get_project_license(m_item);  }
    QStringList keywords() const { return as_app_get_keywords(m_item, NULL); }
    // TODO: Add description
    QStringList categories() const { return as_app_get_categories(m_item); }
    QString projectGroup() const { return as_app_get_project_group(m_item); }
    QString bundleDefault() const { return as_app_get_bundle_default(m_item); }
    // TODO: ID kind
    bool isRequiredForDesktop(const QString &desktop);


private:
    AsApp *m_item;
    AsIcon *m_icon;
}

class Store: public QObject
{
    Q_OBJECT

public:
    Store(QObject *parent = nullptr);
    ~Store();

    bool load(QString path);

    Component *componentById(QString *id) const;

private:
    AsStore *m_store;
};

}

#endif // BACKEND_H
