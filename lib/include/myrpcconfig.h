#pragma once
#include <unordered_map>
#include <string>

//key value and just for once

//config file class
class MyrpcConfig{
public:
    //load config file to map
    void LoadConfigFile(const char *config_file);
    //key->value get value
    std::string Load(const std::string &key);

private:
    std::unordered_map<std::string,std::string> m_configmap;
    //erase extra space
    void Trim(std::string &src_buf);
};