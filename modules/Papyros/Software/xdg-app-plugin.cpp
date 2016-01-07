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
#include "xdg-app-plugin.h"

#include <QProcess>
#include <QStringList>
#include <QDebug>

static void xdgAppChanged(GFileMonitor *monitor, GFile *child, GFile *other_file,
        GFileMonitorEvent event_type, XdgApp *xdgapp) {
    emit xdgapp->installationChanged();
}

XdgApp::XdgApp(QObject *parent)
    : QObject(parent)
{
    // Nothing needed here yet.
}

bool XdgApp::initialize()
{
    // Don't reinitialize
    if (m_installation != nullptr)
        return true;

    // TODO: Do something with these
    GCancellable *cancellable = nullptr;
    GError *error = nullptr;

    m_installation = xdg_app_installation_new_user(cancellable, &error);

    if (m_installation == nullptr) return false;

    m_monitor = xdg_app_installation_create_monitor(m_installation, cancellable, &error);

    if (m_monitor == nullptr) return false;

	g_signal_connect(m_monitor, "changed", G_CALLBACK(xdgAppChanged), this);

    return true;
}

QList<Remote *> XdgApp::listRemotes()
{
    QList<Remote *> remotes;

    if (!initialize())
        return remotes;

    // TODO: Do something with these
    GCancellable *cancellable = nullptr;
    GError *error = nullptr;

    g_autoptr(GPtrArray) xremotes = xdg_app_installation_list_remotes(m_installation,
            cancellable, &error);

    if (xremotes == nullptr)
		return remotes;

	for (uint i = 0; i < xremotes->len; i++) {
		XdgAppRemote *xremote = (XdgAppRemote *) g_ptr_array_index(xremotes, i);

        QString name = xdg_app_remote_get_name(xremote);
        QString title = xdg_app_remote_get_title(xremote);
        QString url = xdg_app_remote_get_url(xremote);

        remotes << new Remote(name, title, url, this);
	}

	return remotes;
}