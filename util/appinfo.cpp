#include "appinfo.h"

AppInfo AppInfo::invalidAppInfo;
std::map<const int, AppInfo> AppInfo::appInfos;

AppInfo & AppInfo::getAppInfo(const int id)
{
    auto app = appInfos.find(id);
    if(app != appInfos.end())
        return (*app).second;
    return AppInfo::invalidAppInfo;
}

AppInfo & AppInfo::getAppInfo(const QString & name)
{
    for( auto app = appInfos.begin(); app != appInfos.end(); app++)
    {
        if((*app).second.name == name)
            return (*app).second;
    }
    return AppInfo::invalidAppInfo;
}

void AppInfo::addAppInfo(const int id, AppInfo & appInfo)
{
    appInfos.insert(std::map<const int, AppInfo>::value_type(id, appInfo));
}

AppInfo::AppInfo(QObject *parent) : QObject(parent), id(-1), size(-1), installSize(-1)
{

}

AppInfo::AppInfo(const AppInfo & appInfo)
{
    *this = appInfo;
}

bool AppInfo::isValid()
{
    return id != -1 && installPath != "" && launcher != "";
}

bool AppInfo::versionCompare(const QString & version)
{
    return version > this->version;
}
