#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QRegularExpression>
#include <QChar>

class Utils
{
public:
    Utils();
    ~Utils();
    static Utils *getInstance();

    bool isAlphabet(const QString& str);
    bool isNumber(const QString& str);
    bool isPIDValid(const QString& str);
    bool isValidCanByteInfo(const QString& str,int limit);
private:
    static Utils s_instance;
};

#endif // UTILS_H
