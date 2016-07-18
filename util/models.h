#ifndef MODELS_H
#define MODELS_H

#include "modelbase.h"

DECLARE_MODEL_BEGIN(UserInfo, JObject)

MetaPropertyPublicSet(QString, nickname)
MetaPropertyPublicSet(QString, photo)
MetaPropertyPublicSet(QString, introduction)
MetaPropertyPublicSet(int, age)
MetaPropertyPublicSet(int, gender)

Q_OBJECT

DECLARE_MODEL_END(UserInfo)


DECLARE_MODEL_BEGIN(AppInfo, JObject)

MetaPropertyPublicSet(QString, installPath)
MetaPropertyPublicSet(QString, launcher)
MetaPropertyPublicSet(QString, uninstallLauncher)
MetaPropertyPublicSet(QString, version)
MetaPropertyPublicSet(int, size)
MetaPropertyPublicSet(int, installSize)

Q_OBJECT

DECLARE_MODEL_END(AppInfo)

#endif // MODELS_H
