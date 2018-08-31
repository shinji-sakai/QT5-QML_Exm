#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include "yamltree.h"

class ConfigReader
{
public:
    ConfigReader();
    void loadConfig(std::string fileName);
    void saveConfig(std::string fileName);

    YAMLTreeNode* saveCategories();
    YAMLTreeNode* saveKeys();
    YAMLTreeNode* saveBool(const char* fieldName);
    YAMLTreeNode* saveInt(const char* fieldName);
    YAMLTreeNode* saveEnum(const char* fieldName);
    YAMLTreeNode* saveString(const char* fieldName);
    YAMLTreeNode* saveListInt(const char* fieldName);
    YAMLTreeNode* saveListEnum(const char* fieldName);

    setLanguage(Language lang) { m_eLanguage = lang; }

    bool getBool(char const* key);
    int getInt(char const* key);
    int getEnum(char const* key);
    std::string getString(char const* key);
    std::list<int> getListInt(char const* key);
    std::list<int> getListEnum(char const* key);

    void setBool(char const* key, bool value);
    void setInt(char const* key, int value);
    void setEnum(char const* key, int value);
    void setString(char const* key, std::string value);
    void setListInt(char const* key, std::list<int> value);
    void setListEnum(char const* key, std::list<int> value);

    std::map<std::string, std::map<int, std::string>> getEntries(char const* key);
    std::pair<int, int> getMinMax(char const* key);

    // Get list of configuration keys in a category
    std::list<std::string> getKeysForCategory(char const * category);

    // Get name of key (translated if language is set)
    std::string getKeyName(char const * key);

    // Get description of key
    std::string getKeyDescription(char const * key);

    KeyType getKeyType(char const * key);

protected:
    YAMLTree m_YAMLTree;
    Language m_eLanguage;

    std::list<std::string> mCategoryList;
    std::list<std::string> mKeyList;

protected:
    std::map<std::string, int> mMapInt;
    std::map<std::string, bool> mMapBool;
    std::map<std::string, int> mMapEnum;
    std::map<std::string, std::string> mMapStr;
    std::map<std::string, std::list<int>> mMapListInt;
    std::map<std::string, std::list<int>> mMapListEnum;

    std::map<std::string, std::map<std::string, std::map<int, std::string>>> mMapEnumEntries;
    std::map<std::string, std::pair<int, int>> mMapMinMax;

    std::map<std::string, std::map<std::string, std::string>> mMapInfo;
};

#endif // CONFIGREADER_H
