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

#include "appstream.h"

using namespace Appstream;

Component::Component(AsApp *item, QObject *parent = nullptr)
    : QObject(parent), m_item(item)
{
    m_icon = as_app_get_icon_default(item);
}

void Component::convertIconNameToPath(QString *path)
{
    // TODO: Do something with this
    GError *error = nullptr;

    if (m_icon != nullptr && as_icon_get_kind(ic) == AS_ICON_KIND_LOCAL) {
		QString iconFilename = as_utils_find_icon_filename_full(path,
				as_icon_get_name (ic), AS_UTILS_FIND_ICON_HI_DPI, &error);

        if (iconFilename)
	        as_icon_set_filename(m_icon, iconFilename);
	}
}

Store::Store(QObject *parent = nullptr)
    : QObject(parent)
{
    m_store = as_store_new();
}

Store::~Store() {
    delete m_store;
}

bool Store::load(QString path)
{
    // TODO: Do something with this
    GError *error = nullptr;

    return as_store_load_path(m_store, path, NULL, &error));
}

Component Store::componentById(QString *id) const
{
    AsApp *item = as_store_get_app_by_id(store, desktop_id);
    return Component(item);
}
