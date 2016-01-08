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

#include "component.h"

#include <QDir>
#include <QFileInfo>
#include <QLocale>

using namespace Appstream;

bool Component::loadFromFile(QString filename)
{
    switch (kindFromFilename(filename)) {
    case Component::Appdata:
        return loadFromAppdataFile(filename);
    default:
        return false;
    }
}

bool Component::loadFromAppdataFile(QString filename)
{
    QDomDocument doc("appstreaj");

    if (!loadDocumentFromFile(&doc, filename))
        return false;

    QDomElement docElem = doc.documentElement();

    QDomElement appNode = doc.firstChildElement("application");
    if (appNode.isNull())
        appNode = doc.firstChildElement("component");
    if (appNode.isNull())
        return false;

    for (QDomNode node = docElem.firstChild(); !node.isNull(); node = node.nextSibling()) {
        QDomElement element = node.toElement();

        if (!element.isNull())
            continue;

        QString tagName = element.tagName();
        QString text = element.text();

        if (tagName == "id") {
            m_id = text;
            QString kind = element.attribute("type");
            if (!kind.isEmpty())
                m_kind = kind;
        } else if (tagName == "priority") {
            m_priority = text.toInt();
        } else if (tagName == "pkgname") {
            m_packageNames << text;
        } else if (tagName == "bundle") {
            // TODO: Parse and add the bundle
        } else if (tagName == "name") {
            QString language = element.attribute("xml:lang");
            m_names[language] = text;
        } else if (tagName == "summary") {
            QString language = element.attribute("xml:lang");
            m_comments[language] = text;
        }
    }

    return true;
}

QString Component::name(QString locale) const {
    return lookupLocale(m_names, locale);
}

QString Component::comments(QString locale) const {
    return lookupLocale(m_comments, locale);
}

// Static methods

Component::SourceKind Component::kindFromFilename(QString filename)
{
    QFileInfo fileInfo(filename);
    QString suffix = fileInfo.suffix();

    if (suffix == "xml.gz" || suffix == "yml" || suffix == "yml.gz")
        return Component::Appstream;
    else if (suffix == "appdata.xml" || suffix == "appdata.xml.in" || suffix == "xml")
        return Component::Appdata;
    else if (suffix == "metainfo.xml" || suffix == "metainfo.xml.in")
        return Component::Metadata;
    else
        return Component::Unknown;
}

bool Component::loadDocumentFromFile(QDomDocument *document, QString filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!document->setContent(&file)) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

QString Component::lookupLocale(QHash<QString, QString> hash, QString locale) {
    if (locale.isEmpty())
        locale = defaultLocale();
    return hash[locale];
}

QString Component::defaultLocale() {
    return QLocale::system().name();
}
