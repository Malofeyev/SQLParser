#include "Json.h"


std::string getWhitespaceString(int numWhitespace) {
    std::string result;
    for (int i = 0; i < numWhitespace; i++) {
        result += " ";
    }
    return result;
}

JsonNode::JsonNode() {}

JsonNode::~JsonNode() = default;


JsonMap::JsonMap() = default;

void JsonMap::push(std::pair<std::string, JsonNode*> keyValue) {
    dict_.push_back(keyValue);
}

std::string JsonMap::toString(int shift) const {
    std::string result = "{";
    for (int i = 0; i < dict_.size(); i++) {
        const auto& e = dict_.at(i);
        result = result + "\n" + 
            getWhitespaceString(shift+1) + e.first + " : " + 
            (e.second->toString(shift+1));
        result += (i == dict_.size() - 1?"":",");
    }
    result += "\n" + getWhitespaceString(shift) + "}"; 
    return result;
}

bool JsonMap::isOneLineStructure() const {
    return false;
}

JsonMap::~JsonMap() {
    for (auto& e : dict_) {
        delete e.second;
    }
}


JsonList::JsonList() = default;

void JsonList::push(JsonNode* element) {
    elements_.push_back(element);
}

std::string JsonList::toString(int shift) const {
    bool isOneLine = isOneLineStructure();
    std::string separator = (isOneLine?" ":"\n" + getWhitespaceString(shift+1));
    std::string result = "[" + separator;
    for (int i = 0; i < elements_.size(); i++) {
        const auto& e = elements_.at(i);
        result += e->toString(shift+1);
        result += (i == elements_.size() - 1?"":"," + separator);
    }
    result += (isOneLine?" ":"\n" + getWhitespaceString(shift)) + "]";
    return result;
}

bool JsonList::isOneLineStructure() const {
    bool isOneLine = true;
    for (const auto& e : elements_) {
        isOneLine = isOneLine && e->isOneLineStructure();
    }
    return isOneLine;
}

JsonList::~JsonList() {
    for (auto& e : elements_) {
        delete e;
    }
}


JsonString::JsonString(std::string value) : value_(value) {} 

std::string JsonString::toString(int shift) const {
    return value_;    
}

bool JsonString::isOneLineStructure() const {
    return true;
}

JsonString::~JsonString() = default;

