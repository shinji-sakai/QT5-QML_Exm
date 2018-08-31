#include "misc.h"

Language getEnumFromLangName(std::string langName)
{
    if (langName == "en_US")
        return Language::en_US;
    if (langName == "fr_FR")
        return Language::fr_FR;
    if (langName == "zh_CN")
        return Language::zh_CN;
    return Language::en_US;
}

std::string getLangNameFromEnum(Language lang)
{
    switch (lang) {
    case Language::en_US:
        return "en_US";
    case Language::fr_FR:
        return "fr_FR";
    case Language::zh_CN:
        return "zh_CN";
    };
    return "en_US";
}

KeyType getEnumFromKeyType(std::string typeString)
{
    if (typeString == "bool")
        return eBool;
    if (typeString == "int")
        return eInt;
    if (typeString == "string")
        return eString;
    if (typeString == "enum")
        return eEnum;
    if (typeString == "list<int>")
        return eListInt;
    if (typeString == "list<enum>")
        return eListEnum;
    return eString;
}

std::string getKeyTypeFromEnum(KeyType keyType)
{
    switch (keyType) {
    case eBool:
        return "bool";
    case eInt:
        return "int";
    case eString:
        return "string";
    case eEnum:
        return "enum";
    case eListInt:
        return "list<int>";
    case eListEnum:
        return "list<enum>";
    }
    return "string";
}

bool isDigits(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}
