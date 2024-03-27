#pragma once

#include <myrpcconfig.h>

//myrpc base class
class MyrpcApplication{
public:
    //initialization
    static void Init(int argc, char **argv);
    static MyrpcApplication& GetInstance();

private:
//config file
    static MyrpcConfig m_config;

    MyrpcApplication(){}
    MyrpcApplication(const MyrpcApplication&) = delete;
    MyrpcApplication(MyrpcApplication&&) = delete;
};