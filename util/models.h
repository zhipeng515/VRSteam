#ifndef MODELS_H
#define MODELS_H

#include "modelbase.h"

DECLARE_MODEL_BEGIN(UserInfo, JObject)

Q_OBJECT

MetaPropertyPublicSet(int, id)
MetaPropertyPublicSet(QString, name)
MetaPropertyPublicSet(QString, nickname)
MetaPropertyPublicSet(QString, photo)
MetaPropertyPublicSet(QString, introduction)
MetaPropertyPublicSet(int, age)
MetaPropertyPublicSet(int, gender)

DECLARE_MODEL_END(UserInfo)


DECLARE_MODEL_BEGIN(AppInfo, JObject)

Q_OBJECT

MetaPropertyPublicSet(int, id)
MetaPropertyPublicSet(QString, name)
MetaPropertyPublicSet(QString, iconUrl)
MetaPropertyPublicSet(QString, downloadUrl)
MetaPropertyPublicSet(QString, localInstallFile)
MetaPropertyPublicSet(QString, installPath)
MetaPropertyPublicSet(QString, launcher)
MetaPropertyPublicSet(QString, uninstallLauncher)
MetaPropertyPublicSet(QString, version)
MetaPropertyPublicSet(int, size)
MetaPropertyPublicSet(int, installSize)

DECLARE_MODEL_END(AppInfo)

#endif // MODELS_H
