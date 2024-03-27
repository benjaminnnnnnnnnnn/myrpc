#include <myrpcconfig.h>
#include <iostream>
#include <string>
//load config file to map
void MyrpcConfig::LoadConfigFile(const char *config_file){
    FILE *pf = fopen(config_file, "r");
    if(nullptr == pf){
        std::cout << "config file is not exist!" << std::endl;
        exit(EXIT_FAILURE);
    }

    //correct load
    //1.#annotation
    //2.correct config
    //3.extra space
    while(!feof(pf)){
        char buf[512] = {0};
        fgets(buf, 512, pf);

        std::string src_buf(buf);

        //erase extra space
        Trim(src_buf);

        //skip # annotation and empty line
        if(src_buf[0] == '#' || src_buf.empty()){
            continue;
        }

        //load config info
        int idx = src_buf.find('=');
        if(idx == -1){
            //illegal
            continue;
        }
        std::string key;
        std::string value;
        key = src_buf.substr(0,idx);
        int endidx = src_buf.find('\n');
        value = src_buf.substr(idx+1,endidx - idx - 1);
        Trim(key);
        Trim(value);
        m_configmap[key] = value;
    }
}

//key->value look up config info
std::string MyrpcConfig::Load(const std::string &key){
    auto it = m_configmap.find(key);
    if (it == m_configmap.end()){
        return " ";
    }
    return it->second;
}

void MyrpcConfig::Trim(std::string &src_buf){
    int idx = src_buf.find_first_not_of(' ');
    if(idx != -1){
        //space before string
        //substr(start loc, length)
        src_buf = src_buf.substr(idx,src_buf.size()-idx);
    }

    idx = src_buf.find_last_not_of(' ');
    if(idx!=-1){
        //space after string
        src_buf = src_buf.substr(0, idx + 1);
    }
}