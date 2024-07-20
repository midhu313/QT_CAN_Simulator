#include "app_settings.h"
#include "defines.h"

AppSettings AppSettings::s_instance;

AppSettings::AppSettings():settings(ORG_NAME,APP_NAME)
{

}

AppSettings *AppSettings::getInstance()
{
    return &s_instance;
}

void AppSettings::updateSettings(const QString &key, const QString &value)
{
    settings.setValue(key,value);
}

void AppSettings::updateSettings(const QString &key, bool value)
{
    settings.setValue(key,value);
}

void AppSettings::updateSettings(const QString &key, int value)
{
    settings.setValue(key,value);
}

QString AppSettings::fetchSettingsAsQString(const QString &key) const
{
    return settings.value(key).toString();
}

bool AppSettings::fetchSettingsAsBool(const QString &key) const
{
    return settings.value(key).toBool();
}

int AppSettings::fetchSettingsAsInt(const QString &key) const
{
    return settings.value(key).toInt();
}
