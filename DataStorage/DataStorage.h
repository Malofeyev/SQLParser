#pragma once 

#include <string>
#include <unordered_set>
#include <unordered_map>

class DataStorage {
public:
    DataStorage();
    ~DataStorage();
    typedef std::unordered_set<std::string> Fields;
    typedef std::unordered_map<std::string, Fields> Tables;

    Tables tables;
};
