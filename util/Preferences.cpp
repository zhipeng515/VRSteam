/*
 * This file is part of WhatsQt.
 *
 * WhatsQt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WhatsQt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WhatsQt.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Preferences.h"
#include <QStandardPaths>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>

Preferences * Preferences::getInstance()
{
    static Preferences preferences;
    return &preferences;
}

Preferences::Preferences()
{
    setBlacklist("http://beacon.sina.com.cn/ckctl.html");
    setBlacklist("http://passport.weibo.com/visitor/visitor?from=iframe");

    if(getDownloadPath() == "") {
        QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/VRSteam/Downloads/";
        setDownloadPath(downloadPath);
    }
    if(getSettingPath() == "") {
        QString settingPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/VRSteam/Settings/";
        setSettingPath(settingPath);
    }
}

void Preferences::setNotificationType(NotificationType type)
{
    settings.beginGroup("general");
    settings.setValue("notification_type", type);
    settings.endGroup();
}

Preferences::NotificationType Preferences::getNotificationType()
{
    settings.beginGroup("general");
    auto type = (NotificationType) settings.value("notification_type", 0).toInt();
    settings.endGroup();
    return type;
}

uint Preferences::getNotificationTimeMs()
{
    // hard coded for now
    return 7000;
}

void Preferences::setBlacklist(const QString& url)
{
    blacklist.append(url);
}

bool Preferences::isInBlacklist(const QString &url)
{
    return blacklist.contains(url);
}

void Preferences::setDownloadPath(const QString& path)
{
    mkdir(path);

    settings.beginGroup("path");
    settings.setValue("download", path);
    settings.endGroup();
}

QString Preferences::getDownloadPath()
{
    settings.beginGroup("path");
    auto path = settings.value("download", "").toString();
    settings.endGroup();
    mkdir(path);
    return path;
}

void Preferences::setSettingPath(const QString& path)
{
    mkdir(path);

    settings.beginGroup("path");
    settings.setValue("setting", path);
    settings.endGroup();
}

QString Preferences::getSettingPath()
{
    settings.beginGroup("path");
    auto path = settings.value("setting", "").toString();
    settings.endGroup();
    mkdir(path);
    return path;
}

void Preferences::setPermission(QWebEnginePage::Feature feature, bool granted)
{
    settings.beginGroup("permissions");
    settings.setValue(QString::number(feature), granted);
    settings.endGroup();
}

bool Preferences::isPermissionGranted(QWebEnginePage::Feature feature)
{
    settings.beginGroup("permissions");
    auto granted = settings.value(QString::number(feature), false).toBool();
    settings.endGroup();
    return granted;
}

bool Preferences::isPermissionDenied(QWebEnginePage::Feature feature)
{
    settings.beginGroup("permissions");
    auto denied = !settings.value(QString::number(feature), true).toBool();
    settings.endGroup();
    return denied;
}

void Preferences::mkdir(const QString & path)
{
    if(path != "") {
        QDir dir;
        if(!dir.exists(path))
            dir.mkpath(path);
    }
}
