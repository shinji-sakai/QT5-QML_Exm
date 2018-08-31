#ifndef DATATREE_H
#define DATATREE_H

#include "misc.h"

struct DataObject
{
    DataObjectType m_eType;

    int m_Int;
    bool m_Bool;
    std::string m_String;

    DataObject() { m_eType = DataObjectType::UNDEFINED; }

    int& operator = (int newValue);
    bool& operator = (bool newValue);
    std::string& operator = (std::string newValue);
    DataObject& operator = (DataObject newValue);
    std::string toString();
};

struct YAMLTreeNode {
    DataNodeType m_eType;
    YAMLTreeNode* m_pParent;

    DataObject m_Value;
    std::map<std::string, YAMLTreeNode*> m_Hash;
    std::list<YAMLTreeNode*> m_List;

    YAMLTreeNode();

    void appendChild(YAMLTreeNode*);
    void appendProperty(const char*, YAMLTreeNode*);
    YAMLTreeNode* operator [] (int);
    YAMLTreeNode* operator [] (const char*);
    YAMLTreeNode& operator = (DataObject newValue);
    YAMLTreeNode& operator = (bool newValue);
    YAMLTreeNode& operator = (int newValue);
    YAMLTreeNode& operator = (std::string newValue);
};

class YAMLTree
{
public:
    void loadYAML(std::string fileName);
    void saveYAML(std::string fileName);
    std::string getYAMLText(YAMLTreeNode* node, int level);
    void formatTree(YAMLTreeNode* node = NULL);
    YAMLTreeNode* getRoot() { return &doc; }

private:
    YAMLTreeNode doc;

    void initError();
    void openError();

    void startStream();
    void endStream();
    void parserError();

    void noEvent();
    void startDocument();
    void endDocument();
    void startSequence();
    void endSequence();
    void startMapping();
    void endMapping();

    void gotAlias(std::string alias);
    void gotScalar(std::string scalar);

    void parseLinearDocument();
    void parseYAML(YAMLTreeNode*, int*);

    std::string flag;
    std::list<std::pair<YAMLParsingEvent, std::string>> m_ParsingHistory;
};

#endif // DATATREE_H
