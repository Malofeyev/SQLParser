#pragma once

#include <string>
#include <vector>

class JsonNode {
public:
    JsonNode();
    virtual std::string toString(int shift) const = 0;
    virtual ~JsonNode();
    virtual bool isOneLineStructure() const = 0; 
};

class JsonMap : public JsonNode {
public:
    JsonMap();
    void push(std::pair<std::string, JsonNode*> keyValue);
    std::string toString(int shift) const override;
    bool isOneLineStructure() const override; 
    ~JsonMap();
private:
    std::vector<std::pair<std::string, JsonNode*>> dict_;
};

class JsonList : public JsonNode {
public:
    JsonList();
    void push(JsonNode* element);
    std::string toString(int shift) const override;
    bool isOneLineStructure() const override; 
    ~JsonList();
private:
    std::vector<JsonNode*> elements_;
};

class JsonString : public JsonNode {
public:
    JsonString(std::string value);
    std::string toString(int shift) const override;
    bool isOneLineStructure() const override; 
    ~JsonString();
private:
    std::string value_;
};


