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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QObject>

#include <QDomDocument>
#include <QString>
#include <QHash>

namespace Appstream
{

class Component
{

public:
    enum SourceKind {
        Appstream,
        Appdata,
        Metadata,
        Unknown
    };

    bool loadFromFile(QString filename);

    bool loadFromAppdataFile(QString filename);

    bool isNull() const { return m_id.isEmpty(); }
    QString name(QString locale = "") const;
    QString comment(QString locale = "") const;
    QString developerName(QString locale = "") const;
    QStringList keywords(QString locale = "") const;

    QString m_id;
    QString m_kind;
    int m_priority;
    QStringList m_packageNames;
    QStringList m_categories;
    QStringList m_architectures;

private:
    void addKeyword(QString keyword, QString locale);

    static SourceKind kindFromFilename(QString filename);

    static bool loadDocumentFromFile(QDomDocument *document, QString filename);

    template<typename T>
    static T lookupLocale(QHash<QString, T> hash, QString locale = "");
    static QString defaultLocale();

    QHash<QString,QString> m_names;
    QHash<QString,QString> m_comments;
    QHash<QString,QString> m_developerNames;
    QHash<QString,QStringList> m_keywords;
};

}

#endif // COMPONENT_H
