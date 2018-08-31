#include "configmanager.h"

ConfigManager::ConfigManager() : ConfigReader()
{
}

void ConfigManager::loadConfig(QString fileName)
{
    ConfigReader::loadConfig(fileName.toStdString());
}

void ConfigManager::saveConfig(QString fileName)
{
    ConfigReader::saveConfig(fileName.toStdString());
}

QString ConfigManager::language()
{
    return QString::fromStdString(getLangNameFromEnum(m_eLanguage));
}

void ConfigManager::setLanguage(QString lang)
{
    m_eLanguage = getEnumFromLangName(lang.toStdString());
    emit languageChanged();
}

QStringList ConfigManager::keys()
{
    QStringList res;
    for (auto &iter : mKeyList)
        res.append(QString::fromStdString(iter));
    return res;
}

QStringList ConfigManager::categories()
{
    QStringList res;
    for (auto &iter : mCategoryList)
        res.append(QString::fromStdString(iter));
    return res;
}

QString ConfigManager::getName(QString fieldName)
{
    return QString::fromStdString(getKeyName(fieldName.toStdString().c_str()));
}

QString ConfigManager::getDescription(QString fieldName)
{
    return QString::fromStdString(getKeyDescription(fieldName.toStdString().c_str()));
}

QJsonObject ConfigManager::getKeyValue(QString fieldName)
{
    QVariantHash res;
    QStringList entries;
    QVariantList defaultList;
    std::string name = fieldName.toStdString();
    std::string type = fieldName.contains(".") ? "key" : "category";

    res["objType"] = QString::fromStdString(type);

    if (res["objType"].toString() != "key")
       return QJsonObject::fromVariantHash(res);

    KeyType keyType = getKeyType(name.c_str());

    res["type"] = QString::fromStdString(getKeyTypeFromEnum(keyType));

    switch (keyType) {
    case eBool:
       res["default"] = mMapBool[name];
       break;
    case eInt:
       res["min"] = mMapMinMax[name].first;
       res["max"] = mMapMinMax[name].second;
       res["default"] = mMapInt[name];
       break;
    case eString:
       res["default"] = QString::fromStdString(mMapStr[name]);
       break;
    case eEnum:
       for (auto &iter : mMapEnumEntries[name])
       {
           QString entry = QString::fromStdString(iter.first);
           if (!iter.second[(int)m_eLanguage].empty())
               entry = QString::fromStdString(iter.second[(int)m_eLanguage]);
           entries.append(entry);
       }
       res["entries"] = entries;
       res["default"] = mMapEnum[name];
       break;
    case eListInt:
       res["min"] = mMapMinMax[name].first;
       res["max"] = mMapMinMax[name].second;
       for (auto &iter : mMapListInt[name])
           defaultList.append(iter);
       res["default"] = defaultList;
       break;
    case eListEnum:
       for (auto &iter : mMapEnumEntries[name])
       {
           QString entry = QString::fromStdString(iter.first);
           if (!iter.second[(int)m_eLanguage].empty())
               entry = QString::fromStdString(iter.second[(int)m_eLanguage]);
           entries.append(entry);
       }
       res["entries"] = entries;
       for (auto &iter : mMapListEnum[name])
           defaultList.append(iter);
       res["default"] = defaultList;
       break;
    default:
       break;
    }
    return QJsonObject::fromVariantHash(res);
}

void ConfigManager::setKeyValue(QString fieldName, QVariant value)
{
    std::list<int> intList;
    std::list<int> enumList;
    std::string name = fieldName.toStdString();
    KeyType keyType = getKeyType(name.c_str());

    switch (keyType) {
    case eBool:
        setBool(name.c_str(), value.toBool());
        break;
    case eInt:
        setInt(name.c_str(), value.toInt());
        break;
    case eString:
        setString(name.c_str(), value.toString().toStdString());
        break;
    case eEnum:
        setEnum(name.c_str(), value.toInt());
        break;
    case eListInt:
        foreach (QVariant defValue, value.toList()) {
            intList.push_back(defValue.toInt());
        }
        setListInt(name.c_str(), intList);
        break;
    case eListEnum:
        foreach (QVariant defValue, value.toList()) {
            enumList.push_back(defValue.toInt());
        }
        setListEnum(name.c_str(), enumList);
        break;
    }
}
