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
#include <QDir>
#include <QDebug>

using namespace Appstream;

QStringList stringsByTagName(QDomElement element, QString tagName) {
    QStringList strings;

    QDomNodeList nodes = element.elementsByTagName(tagName);
    for (int i = 0; i < nodes.count(); i++) {
        QDomElement subElement = nodes.at(i).toElement();
        if (!subElement.isNull())
            strings << subElement.text();
    }

    return strings;
}

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
    qDebug() << "Loading from appdata file!";
    QDomDocument doc("appdata");

    if (!loadDocumentFromFile(&doc, filename))
        return false;

    QDomElement appNode = doc.firstChildElement("application");
    if (appNode.isNull())
        appNode = doc.firstChildElement("component");
    if (appNode.isNull())
        return false;

    for (QDomNode node = appNode.firstChild(); !node.isNull(); node = node.nextSibling()) {
        QDomElement element = node.toElement();

        if (element.isNull())
            continue;

        QString tagName = element.tagName();
        QString text = element.text();

        qDebug() << tagName << text;

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
        } else if (tagName == "developer_name") {
            QString language = element.attribute("xml:lang");
            m_developerNames[language] = text;
        } else if (tagName == "description") {
            qDebug() << "WARNING: description not parsed";
        } else if (tagName == "icon") {
            qDebug() << "WARNING: icon not parsed";
        } else if (tagName == "categories") {
            m_categories << stringsByTagName(element, "category");
        } else if (tagName == "architectures") {
            m_architectures << stringsByTagName(element, "architecture");
        } else if (tagName == "keywords") {
            QDomNodeList keywords = doc.elementsByTagName("keyword");
            for (int i = 0; i < keywords.count(); i++) {
                QDomElement keyword = keywords.at(i).toElement();
                if (!keyword.isNull()) {
                    QString language = element.attribute("xml:lang");
                    addKeyword(keyword.text(), language);
                }
            }
        } else if (tagName == "kudos") {
            m_kudos << stringsByTagName(element, "kudo");
        } else if (tagName == "permissions") {
            m_permissions << stringsByTagName(element, "permission");
        } else if (tagName == "vetos") {
            m_vetos << stringsByTagName(element, "veto");
        } else if (tagName == "mimetypes") {
            m_mimetypes << stringsByTagName(element, "mimetype");
        } else if (tagName == "project_license") {
            m_projectLicense = text;
        } else if (tagName == "metadata_license") {
            m_metadataLicense = text;
        } else if (tagName == "source_pkgname") {
            m_sourcePackageName = text;
        } else if (tagName == "updatecontact" || tagName == "update_contact") {
            m_updateContact = text;
        } else if (tagName == "url") {
            qDebug() << "WARNING: url not parsed";
        } else if (tagName == "project_group") {
            m_projectGroup = text;
        } else if (tagName == "compulsory_for_desktop") {
            m_compulsoryForDesktops << text;
        } else if (tagName == "extends") {
            m_extends << text;
        } else if (tagName == "screenshots") {
            qDebug() << "WARNING: screenshots not parsed";
        } else if (tagName == "releases") {
            qDebug() << "WARNING: releases not parsed";
        } else if (tagName == "provides") {
            qDebug() << "WARNING: provides not parsed";
        } else if (tagName == "languages") {
            qDebug() << "WARNING: languages not parsed";
        } else if (tagName == "metadata") {
            qDebug() << "WARNING: metadata not parsed";
        } else {
            qFatal("Tag not supported: %s", qPrintable(tagName));
        }
    }

    return true;
}

void Component::addKeyword(QString keyword, QString locale) {
    QStringList keywords = m_keywords.contains(locale) ? m_keywords[locale] : QStringList();

    keywords << keyword;

    m_keywords[locale] = keywords;
}

QString Component::name(QString locale) const {
    return lookupLocale(m_names, locale);
}

QString Component::comment(QString locale) const {
    return lookupLocale(m_comments, locale);
}

QString Component::developerName(QString locale) const {
    return lookupLocale(m_developerNames, locale);
}

QStringList Component::keywords(QString locale) const {
    return lookupLocale(m_keywords, locale);
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

template<typename T>
T Component::lookupLocale(QHash<QString, T> hash, QString locale) {
    if (locale.isEmpty())
        locale = defaultLocale();
    return hash[locale];
}

QString Component::defaultLocale() {
    return QLocale::system().name();
}