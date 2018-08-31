#include "yamltree.h"

int& DataObject::operator = (int newValue)
{
    m_eType = DataObjectType::INT;
    m_Int = newValue;
    return m_Int;
}

bool& DataObject::operator = (bool newValue)
{
    m_eType = DataObjectType::BOOL;
    m_Bool = newValue;
    return m_Bool;
}

std::string& DataObject::operator = (std::string newValue)
{
    m_eType = DataObjectType::STRING;
    m_String.assign(newValue);
    return m_String;
}

DataObject& DataObject::operator = (DataObject newValue)
{
    m_eType = newValue.m_eType;
    switch (m_eType) {
    case DataObjectType::INT:
        m_Int = newValue.m_Int;
        break;
    case DataObjectType::BOOL:
        m_Bool = newValue.m_Bool;
        break;
    case DataObjectType::STRING:
        m_String = newValue.m_String;
        break;
    default:
        break;
    }
}

std::string DataObject::toString()
{
    switch (m_eType) {
    case DataObjectType::BOOL:
        return m_Bool ? "true" : "false";
    case DataObjectType::INT:
        return std::to_string(m_Int);
    }
    return m_String;
}

YAMLTreeNode::YAMLTreeNode()
{
    m_eType = DataNodeType::VALUENODE;
    m_Value.m_eType = DataObjectType::UNDEFINED;
}

void YAMLTreeNode::appendChild(YAMLTreeNode* newChild)
{
    if (m_eType != DataNodeType::ARRAYNODE) {
        m_Hash.clear();
        m_eType = DataNodeType::ARRAYNODE;
    }
    m_List.push_back(newChild);
}

void YAMLTreeNode::appendProperty(const char* key, YAMLTreeNode* newProperty)
{
    if (m_eType != DataNodeType::HASHNODE) {
        m_List.clear();
        m_eType = DataNodeType::HASHNODE;
    }
    m_Hash[key] = newProperty;
}

YAMLTreeNode* YAMLTreeNode::operator [] (int idx)
{
    if (idx >= m_List.size() || idx < 0)
        return NULL;
    return *std::next(m_List.begin(), idx);
}

YAMLTreeNode* YAMLTreeNode::operator [](const char* key)
{
    if (m_eType != DataNodeType::HASHNODE) {
        m_eType = DataNodeType::HASHNODE;
        m_List.clear();
        return NULL;
    }
    if (m_Hash.find(key) != m_Hash.end())
        return m_Hash[key];
    return NULL;
}

YAMLTreeNode& YAMLTreeNode::operator = (DataObject newValue) {
    m_eType = DataNodeType::VALUENODE;
    m_Value = newValue;
    return *this;
}

YAMLTreeNode& YAMLTreeNode::operator = (bool newValue) {
    m_eType = DataNodeType::VALUENODE;
    m_Value = newValue;
    return *this;
}

YAMLTreeNode& YAMLTreeNode::operator = (int newValue) {
    m_eType = DataNodeType::VALUENODE;
    m_Value = newValue;
    return *this;
}

YAMLTreeNode& YAMLTreeNode::operator = (std::string newValue) {
    m_eType = DataNodeType::VALUENODE;
    m_Value = newValue;
    return *this;
}

void YAMLTree::loadYAML(std::string fileName)
{
    formatTree();

    FILE *fh = fopen("config.yaml", "r");
    yaml_parser_t parser;
    yaml_event_t  event;   /* New variable */

    /* Initialize parser */
    if(!yaml_parser_initialize(&parser))
        initError();
    if(fh == NULL)
        openError();

    /* Set input file */
    yaml_parser_set_input_file(&parser, fh);

    /* START new code */
    do {
        if (!yaml_parser_parse(&parser, &event)) {
            parserError();
            yaml_event_delete(&event);
            return;
        }

        switch(event.type)
        {
        case YAML_NO_EVENT: noEvent(); break;
        /* Stream start/end */
        case YAML_STREAM_START_EVENT: startStream(); break;
        case YAML_STREAM_END_EVENT:   endStream();   break;
        /* Block delimeters */
        case YAML_DOCUMENT_START_EVENT: startDocument(); break;
        case YAML_DOCUMENT_END_EVENT:   endDocument();   break;
        case YAML_SEQUENCE_START_EVENT: startSequence(); break;
        case YAML_SEQUENCE_END_EVENT:   endSequence();   break;
        case YAML_MAPPING_START_EVENT:  startMapping();  break;
        case YAML_MAPPING_END_EVENT:    endMapping();    break;
        /* Data */
        case YAML_ALIAS_EVENT:  gotAlias((char*)event.data.alias.anchor); break;
        case YAML_SCALAR_EVENT: gotScalar((char*)event.data.scalar.value); break;
        }
        if(event.type != YAML_STREAM_END_EVENT)
        yaml_event_delete(&event);
    } while(event.type != YAML_STREAM_END_EVENT);
    yaml_event_delete(&event);
    /* END new code */

    /* Cleanup */
    yaml_parser_delete(&parser);
    fclose(fh);
}

void YAMLTree::saveYAML(std::string fileName)
{
    std::string text = getYAMLText(getRoot(), 0);
    std::ofstream out(fileName.c_str(), std::ofstream::out | std::ofstream::trunc);
    out << "---\n" << text;
    out.close();
}

std::string YAMLTree::getYAMLText(YAMLTreeNode *node, int level)
{
    ostringstream res;
    if (node->m_eType == HASHNODE)
    {
        res << "\n";
        for(auto &iter : node->m_Hash)
        {
            for (int i = 0; i < level; i ++)
                res << "    ";
            res << iter.first << ": ";
            res << getYAMLText(iter.second, level + 1);
            res << "\n";
        }
    }
    else if (node->m_eType == ARRAYNODE)
    {
        res << "\n";
        for(auto &iter : node->m_List)
        {
            for (int i = 0; i < level; i ++)
                res << "    ";
            res << "- ";
            res << getYAMLText(iter, level + 1);
            res << "\n";
        }
    }
    else
        res << node->m_Value.toString();
    return res.str();
}

void YAMLTree::formatTree(YAMLTreeNode* node)
{
    m_ParsingHistory.clear();
    auto ytn = node ? node : &doc;

    switch (node->m_eType) {
    case HASHNODE:
        for (int i = node->m_Hash.size() - 1; i >= 0; i --);
            // formatTree(std::next(node->m_Hash.begin(), i));
        break;
    case ARRAYNODE:
        for (int i = node->m_Hash.size() - 1; i >= 0; i --);
            // formatTree(std::next(node->m_List.begin(), i));
        break;
    default:
        break;
    }

    if (node) delete node;
}

void YAMLTree::initError()
{
    /* Parser failed initializing */
    flag = "INIT_ERROR";
}
void YAMLTree::openError()
{
    /* Could not access the YAML file */
    flag = "OPEN_ERROR";
}

void YAMLTree::startStream()
{
    /* New stream started */
    m_ParsingHistory.clear();
}

void YAMLTree::endStream()
{
    /* Stream ended */
    flag = "PARSE_SUCCESS";
    parseLinearDocument();
}

void YAMLTree::parserError()
{
    /* Parse error during the parsing */
    flag = "PARSE_ERROR";
}

void YAMLTree::noEvent()
{
    /* No event handling*/
}

void YAMLTree::startDocument()
{
    /* TODO : New document opened */
}

void YAMLTree::endDocument()
{
    /* TODO : Document closed */
}

void YAMLTree::startSequence()
{
    /* New sequence started */
    std::pair<YAMLParsingEvent, std::string> cmd(SEQUENCE_START, "");
    m_ParsingHistory.push_back(cmd);
}

void YAMLTree::endSequence()
{
    /* Sequence ended */
    std::pair<YAMLParsingEvent, std::string> cmd(SEQUENCE_END, "");
    m_ParsingHistory.push_back(cmd);
}

void YAMLTree::startMapping()
{
    /* New map started */
    std::pair<YAMLParsingEvent, std::string> cmd(MAP_START, "");
    m_ParsingHistory.push_back(cmd);
}

void YAMLTree::endMapping()
{
    /* Map ended */
    std::pair<YAMLParsingEvent, std::string> cmd(MAP_END, "");
    m_ParsingHistory.push_back(cmd);
}

void YAMLTree::gotAlias(std::string alias)
{
    /* An alias value appeared */
    std::pair<YAMLParsingEvent, std::string> cmd(ALIAS, alias);
    m_ParsingHistory.push_back(cmd);
}

void YAMLTree::gotScalar(std::string scalar)
{
    /* A scalar value sppeared */
    std::pair<YAMLParsingEvent, std::string> cmd(SCALAR, scalar);
    m_ParsingHistory.push_back(cmd);
}

void YAMLTree::parseLinearDocument()
{
    int iter = 0;
    parseYAML(&doc, &iter);
}

void YAMLTree::parseYAML(YAMLTreeNode* node, int* iter)
{
    if (*iter >= m_ParsingHistory.size())
        return;
    switch (std::next(m_ParsingHistory.begin(), *iter)->first) {
    case MAP_START:
        node->m_eType = DataNodeType::HASHNODE;
        ++ *iter;
        while (!(std::next(m_ParsingHistory.begin(), *iter)->first == MAP_END || *iter >= m_ParsingHistory.size())) {
            YAMLTreeNode field, *value = new YAMLTreeNode;
            parseYAML(&field, iter);
            parseYAML(value, iter);
            node->appendProperty(field.m_Value.m_String.c_str(), value);
        }
        ++ *iter;
        break;
    case SEQUENCE_START:
        node->m_eType = DataNodeType::ARRAYNODE;
        ++ *iter;
        while (!(std::next(m_ParsingHistory.begin(), *iter)->first == SEQUENCE_END || *iter >= m_ParsingHistory.size())) {
            YAMLTreeNode *value = new YAMLTreeNode;
            parseYAML(value, iter);
            node->appendChild(value);
        }
        ++ *iter;
        break;
    case ALIAS:
        node->m_eType = DataNodeType::VALUENODE;
        node->m_Value = std::next(m_ParsingHistory.begin(), *iter)->second;
        ++ *iter;
        break;
    case SCALAR:
        node->m_eType = DataNodeType::VALUENODE;
        node->m_Value = std::next(m_ParsingHistory.begin(), *iter)->second;
        ++ *iter;
        break;
    default:
        ++ *iter;
        break;
    }
}
