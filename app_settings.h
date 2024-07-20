#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

#include <QString>
#include <QSettings>

class AppSettings
{
private:
    static AppSettings s_instance;
    QSettings settings;
public:
    AppSettings();
    static AppSettings *getInstance();

    void updateSettings(const QString &key,const QString &value);
    void updateSettings(const QString &key,bool value);
    void updateSettings(const QString &key,int value);

    QString fetchSettingsAsQString(const QString &key) const;
    bool fetchSettingsAsBool(const QString &key) const;
    int fetchSettingsAsInt(const QString &key) const;

};

#endif // APP_SETTINGS_H
