#include "configreader.h"

ConfigReader::ConfigReader()
{
}

void ConfigReader::loadConfig(std::string fileName)
{
    m_YAMLTree.loadYAML(fileName);

    YAMLTreeNode* docRoot = m_YAMLTree.getRoot();
    YAMLTreeNode* categories = (*docRoot)["categories"];
    YAMLTreeNode* keys = (*docRoot)["keys"];

    mMapInt.clear();
    mMapBool.clear();
    mMapStr.clear();
    mMapListInt.clear();
    mMapListEnum.clear();
    mMapEnumEntries.clear();
    mMapInfo.clear();

    mCategoryList.clear();
    mKeyList.clear();

    if (categories) {
        for (auto &category : categories->m_List) {
            std::string categoryName = (*category)["name"]->m_Value.m_String;
            mCategoryList.push_back(categoryName);

            if ((*category)["description"])
                mMapInfo[categoryName]["description"] = (*category)["description"]->m_Value.m_String;

            for (int i = (int)Language::en_US; i <= (int)Language::zh_CN; i ++) {
                char nameStr[20], descStr[20];
                snprintf(nameStr, sizeof(nameStr), "name[%s]", getLangNameFromEnum((Language)i).c_str());
                snprintf(descStr, sizeof(nameStr), "description[%s]", getLangNameFromEnum((Language)i).c_str());

                if ((*category)[nameStr])
                    mMapInfo[categoryName][nameStr] = (*category)[nameStr]->m_Value.m_String;
                if ((*category)[descStr])
                    mMapInfo[categoryName][descStr] = (*category)[descStr]->m_Value.m_String;
            }
        }
    }
    if (keys) {
        for (auto &key : keys->m_List) {
            std::string keyName = (*key)["key"]->m_Value.m_String;
            mKeyList.push_back(keyName);

            if ((*key)["description"])
                mMapInfo[keyName]["description"] = (*key)["description"]->m_Value.m_String;

            for (int i = (int)Language::en_US; i <= (int)Language::zh_CN; i ++) {
                char nameStr[20], descStr[20];
                snprintf(nameStr, sizeof(nameStr), "name[%s]", getLangNameFromEnum((Language)i).c_str());
                snprintf(descStr, sizeof(nameStr), "description[%s]", getLangNameFromEnum((Language)i).c_str());

                if ((*key)[nameStr])
                    mMapInfo[keyName][nameStr] = (*key)[nameStr]->m_Value.m_String;
                if ((*key)[descStr])
                    mMapInfo[keyName][descStr] = (*key)[descStr]->m_Value.m_String;
                if ((*key)["type"])
                    mMapInfo[keyName]["type"] = (*key)["type"]->m_Value.m_String;
            }

            if (mMapInfo[keyName]["type"] == "int") {
                mMapMinMax[keyName] = std::make_pair(atoi((*key)["min"]->m_Value.m_String.c_str()), atoi((*key)["max"]->m_Value.m_String.c_str()));
                mMapInt[keyName] = atoi((*key)["default"]->m_Value.m_String.c_str());
            }
            else if (mMapInfo[keyName]["type"] == "bool") {
                std::string valueStr = (*key)["default"]->m_Value.m_String;
                bool value;
                if (valueStr == "true" || valueStr == "yes")
                    value = true;
                else
                    value = false;
                mMapBool[keyName] = value;
            }
            else if (mMapInfo[keyName]["type"] == "string") {
                mMapStr[keyName] = (*key)["default"]->m_Value.m_String;
            }
            else if (mMapInfo[keyName]["type"] == "enum") {
                int defValue = 0;

                for (auto &iter : (*key)["entries"]->m_List) {
                    std::string entryName = (*iter)["entry"]->m_Hash["name"]->m_Value.m_String;
                    std::map<int, std::string> i18N;

                    for (int i = (int)Language::en_US; i <= (int)Language::zh_CN; i ++) {
                        char nameStr[20], descStr[20];
                        snprintf(nameStr, sizeof(nameStr), "name[%s]", getLangNameFromEnum((Language)i).c_str());

                        if ((*iter)["entry"]->m_Hash[nameStr])
                            i18N[i] = (*iter)["entry"]->m_Hash[nameStr]->m_Value.m_String;
                    }

                    mMapEnumEntries[keyName][entryName] = i18N;
                }

                for (auto &iter : mMapEnumEntries[keyName]) {
                    if ((*key)["default"]->m_Value.m_String == iter.first)
                        break;
                    defValue ++;
                }
                if (defValue >= mMapEnumEntries[keyName].size())
                    defValue = 0;

                mMapEnum[keyName] = defValue;
            }
            else if (mMapInfo[keyName]["type"] == "list<int>") {
                std::list<int> defList;
                mMapMinMax[keyName] = std::make_pair(atoi((*key)["min"]->m_Value.m_String.c_str()), atoi((*key)["max"]->m_Value.m_String.c_str()));
                for (auto &def : (*key)["default"]->m_List)
                {
                    defList.push_back(atoi(def->m_Value.m_String.c_str()));
                }
                mMapListInt[keyName] = defList;
            }
            else if (mMapInfo[keyName]["type"] == "list<enum>") {
                std::list<int> defList;

                for (auto &iter : (*key)["entries"]->m_List) {
                    std::string entryName = (*iter)["entry"]->m_Hash["name"]->m_Value.m_String;
                    std::map<int, std::string> i18N;

                    for (int i = (int)Language::en_US; i <= (int)Language::zh_CN; i ++) {
                        char nameStr[20], descStr[20];
                        snprintf(nameStr, sizeof(nameStr), "name[%s]", getLangNameFromEnum((Language)i).c_str());

                        if ((*iter)["entry"]->m_Hash[nameStr])
                            i18N[i] = (*iter)["entry"]->m_Hash[nameStr]->m_Value.m_String;
                    }

                    mMapEnumEntries[keyName][entryName] = i18N;
                }

                for (auto &def : (*key)["default"]->m_List)
                {
                    int defValue = 0;

                    for (auto &iter : mMapEnumEntries[keyName]) {
                        if (def->m_Value.m_String == iter.first)
                            break;
                        defValue ++;
                    }

                    if (defValue >= mMapEnumEntries[keyName].size())
                        defValue = 0;

                    defList.push_back(defValue);
                }

                mMapListEnum[keyName] = defList;
            }
        }
    }
}

void ConfigReader::saveConfig(std::string fileName)
{
    m_YAMLTree.formatTree();
    m_YAMLTree.getRoot()->appendProperty("categories", saveCategories());
    m_YAMLTree.getRoot()->appendProperty("keys", saveKeys());
    m_YAMLTree.saveYAML(fileName);
}

YAMLTreeNode* ConfigReader::saveCategories()
{
    auto categories = new YAMLTreeNode;
    for (auto &iter : mCategoryList)
    {
        auto category = new YAMLTreeNode;
        auto name = new YAMLTreeNode;
        *name = iter;
        category->appendProperty("name", name);
        for (auto &info : mMapInfo[iter])
        {
            if (info.second == "")
                continue;
            auto property = new YAMLTreeNode;
            *property = info.second;
            category->appendProperty(info.first.c_str(), property);
        }
        categories->appendChild(category);
    }
    return categories;
}

YAMLTreeNode* ConfigReader::saveKeys()
{
    auto keys = new YAMLTreeNode;
    for (auto &category : mCategoryList)
    {
        for (auto &key : getKeysForCategory(category.c_str()))
        {
            KeyType type = getKeyType(key.c_str());
            YAMLTreeNode* keyNode;
            auto keyName = new YAMLTreeNode;
            *keyName = key;
            switch (type) {
            case eBool:
                keyNode = saveBool(key.c_str());
                break;
            case eInt:
                keyNode = saveInt(key.c_str());
                break;
            case eString:
                keyNode = saveString(key.c_str());
                break;
            case eEnum:
                keyNode = saveEnum(key.c_str());
                break;
            case eListInt:
                keyNode = saveListInt(key.c_str());
                break;
            case eListEnum:
                keyNode = saveListEnum(key.c_str());
                break;
            default:
                keyNode = new YAMLTreeNode;
                keyNode->m_eType = HASHNODE;
                break;
            }

            keyNode->appendProperty("key", keyName);

            for (auto &info : mMapInfo[key])
            {
                if (info.second == "")
                    continue;
                auto property  = new YAMLTreeNode;
                *property = info.second;
                keyNode->appendProperty(info.first.c_str(), property);
            }

            keys->appendChild(keyNode);
        }
    }
    return keys;
}

YAMLTreeNode* ConfigReader::saveBool(const char * fieldName)
{
    auto key = new YAMLTreeNode;
    auto value = new YAMLTreeNode;
    *value = mMapBool[fieldName];
    key->appendProperty("default", value);
    return key;
}

YAMLTreeNode* ConfigReader::saveInt(const char * fieldName)
{
    auto key = new YAMLTreeNode;
    auto value = new YAMLTreeNode;
    auto min = new YAMLTreeNode;
    auto max = new YAMLTreeNode;
    *value = mMapInt[fieldName];
    *min = mMapMinMax[fieldName].first;
    *max = mMapMinMax[fieldName].second;
    key->appendProperty("min", min);
    key->appendProperty("max", max);
    key->appendProperty("default", value);
    return key;
}

YAMLTreeNode* ConfigReader::saveString(const char * fieldName)
{
    auto key = new YAMLTreeNode;
    auto value = new YAMLTreeNode;
    *value = mMapStr[fieldName];
    key->appendProperty("default", value);
    return key;
}

YAMLTreeNode* ConfigReader::saveEnum(const char * fieldName)
{
    auto key = new YAMLTreeNode;
    auto value = new YAMLTreeNode;
    auto iter = mMapEnumEntries[fieldName].begin();
    for (int idx = 0; idx < mMapEnum[fieldName]; idx ++)
        ++ iter;
    *value = iter->first;
    key->appendProperty("default", value);
    return key;
}

YAMLTreeNode* ConfigReader::saveListInt(const char * fieldName)
{
    auto key = new YAMLTreeNode;
    auto defList = new YAMLTreeNode;
    auto min = new YAMLTreeNode;
    auto max = new YAMLTreeNode;

    *min = mMapMinMax[fieldName].first;
    *max = mMapMinMax[fieldName].second;
    key->appendProperty("min", min);
    key->appendProperty("max", max);

    for (auto &iter : mMapListInt[fieldName])
    {
        auto keyValue = new YAMLTreeNode;
        (*keyValue) = iter;
        defList->appendChild(keyValue);
    }
    key->appendProperty("default", defList);
    return key;
}

YAMLTreeNode* ConfigReader::saveListEnum(const char * fieldName)
{
    auto key = new YAMLTreeNode;
    auto defList = new YAMLTreeNode;
    auto entryList = new YAMLTreeNode;
    for (auto &iter : mMapEnumEntries[fieldName])
    {
        auto entry = new YAMLTreeNode;
        auto entryName = new YAMLTreeNode;
        *entryName = iter.first;

        entry->appendProperty("name", entryName);
        for (auto &i18N : iter.second)
        {
            char nameStr[20];
            snprintf(nameStr, sizeof(nameStr), "name[%s]", getLangNameFromEnum((Language)i18N.first).c_str());

            auto entryI18N = new YAMLTreeNode;
            *entryI18N = i18N.second;
            entry->appendProperty(nameStr, entryI18N);
        }

        entryList->appendChild(entry);
    }
    for (auto &iter : mMapListEnum[fieldName])
    {
        auto keyValue = new YAMLTreeNode;

        auto it = mMapEnumEntries[fieldName].begin();
        for (int idx = 0; idx < mMapEnum[fieldName]; idx ++)
            ++ it;
        *keyValue = it->first;
        defList->appendChild(keyValue);
    }
    key->appendProperty("entries", entryList);
    key->appendProperty("default", defList);
    return key;
}

bool ConfigReader::getBool(char const* key)
{
    return mMapBool[key];
}

int ConfigReader::getInt(char const* key)
{
    return mMapInt[key];
}

int ConfigReader::getEnum(char const* key)
{
    return mMapEnum[key];
}

std::string ConfigReader::getString(char const* key)
{
    return mMapStr[key];
}

std::list<int> ConfigReader::getListInt(char const* key)
{
    return mMapListInt[key];
}

std::list<int> ConfigReader::getListEnum(char const* key)
{
    return mMapListEnum[key];
}

void ConfigReader::setBool(char const* key, bool value)
{
    mMapBool[key] = value;
}

void ConfigReader::setInt(char const* key, int value)
{
    mMapInt[key] = value;
}

void ConfigReader::setEnum(char const* key, int value)
{
    mMapEnum[key] = value;
}

void ConfigReader::setString(char const* key, std::string value)
{
    mMapStr[key] = value;
}

void ConfigReader::setListInt(char const* key, std::list<int> value)
{
    mMapListInt[key] = value;
}

void ConfigReader::setListEnum(char const* key, std::list<int> value)
{
    mMapListEnum[key] = value;
}

std::map<std::string, std::map<int, std::string>> ConfigReader::getEntries(char const* key)
{
    return mMapEnumEntries[key];
}

std::pair<int, int> ConfigReader::getMinMax(char const* key)
{
    return mMapMinMax[key];
}

std::list<std::string> ConfigReader::getKeysForCategory(char const * category)
{
    std::list<std::string> keys;
    for (auto &iter : mKeyList)
    {
        istringstream cateString(iter.c_str());
        std::string s;
        getline(cateString, s, '.');
        if (s == category)
            keys.push_back(iter);
    }
    return keys;
}

std::string ConfigReader::getKeyName(const char *key)
{
    char nameStr[20];
    snprintf(nameStr, sizeof(nameStr), "name[%s]", getLangNameFromEnum(m_eLanguage).c_str());

    istringstream keyLiteral(key);
    std::string keyName;
    getline(keyLiteral, keyName, '.');
    getline(keyLiteral, keyName, '\0');
    if (!mMapInfo[key][nameStr].empty())
        keyName = mMapInfo[key][nameStr];
    return keyName;
}

std::string ConfigReader::getKeyDescription(const char *key)
{
    char descStr[20];
    snprintf(descStr, sizeof(descStr), "description[%s]", getLangNameFromEnum(m_eLanguage).c_str());

    std::string keyName = mMapInfo[key]["description"];
    if (!mMapInfo[key][descStr].empty())
        keyName = mMapInfo[key][descStr];
    return keyName;
}

KeyType ConfigReader::getKeyType(char const * key)
{
    return getEnumFromKeyType(mMapInfo[key]["type"]);
}
