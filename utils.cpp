#include "utils.h"

Utils Utils::s_instance;

Utils::Utils()
{

}

Utils::~Utils()
{

}

Utils *Utils::getInstance()
{
    return &s_instance;
}

bool Utils::isAlphabet(const QString &str)
{
    return std::all_of(str.begin(), str.end(), [](QChar c) { return (c.isLetter()||c.isSpace()); });
}

bool Utils::isNumber(const QString &str)
{
    return std::all_of(str.begin(), str.end(), [](QChar c) { return c.isNumber(); });
}

bool Utils::isPIDValid(const QString &str)
{
    QRegularExpression hexRegex("^0[xX][0-9a-fA-F]+$");
    if (!hexRegex.match(str).hasMatch()) {
        return false; // Not a valid hex string format
    }
    bool ok;
    int value = str.mid(2).toInt(&ok, 16); // Skip "0x" prefix
    if (!ok) {
        return false; // Conversion failed
    }
    // Check if the value is less than 0x7FF (2047 in decimal)
    return value < 0x7FF;
}

bool Utils::isValidCanByteInfo(const QString &str, int limit)
{
    if (str.length() == 1 && str.at(0).isDigit()) {
        return (str.at(0).digitValue()<=limit);
    }
    return false;
}
