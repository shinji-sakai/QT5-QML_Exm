#ifndef MISC_H
#define MISC_H

#define _GLIBCXX_USE_CXX11_ABI 0

#include <map>
#include <list>
#include <yaml.h>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

enum class Language {
    en_US,
    fr_FR,
    zh_CN
};

typedef enum {
    eInt,
    eBool,
    eString,
    eEnum,
    eListInt,
    eListEnum
} KeyType;


Language getEnumFromLangName(std::string langName);
std::string getLangNameFromEnum(Language lang);
KeyType getEnumFromKeyType(std::string typeString);
std::string getKeyTypeFromEnum(KeyType keyType);
bool isDigits(const std::string &str);

enum DataObjectType {
    UNDEFINED,
    INT,
    STRING,
    BOOL
};

enum DataNodeType {
    HASHNODE,
    ARRAYNODE,
    VALUENODE
};

enum YAMLParsingEvent {
    SEQUENCE_START,
    SEQUENCE_END,
    MAP_START,
    MAP_END,
    ALIAS,
    SCALAR
};

#endif // MISC_H
