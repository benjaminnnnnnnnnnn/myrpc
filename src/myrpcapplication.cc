#include <myrpcapplication.h>
#include <iostream>
#include <unistd.h>
#include <myrpcchannel.h>
#include <myrpccontroller.h>

MyrpcConfig MyrpcApplication::m_config;

void ShowArgsHelp(){//help func, pring the correct args format
    std::cout << "format: command -i <configfile>" << std::endl;
}


void MyrpcApplication::Init(int argc, char **argv){
    if(argc < 2){//no args,exit
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }
    
    int c = 0;
    std::string config_file;

    while((c = getopt(argc,argv,"i:"))!=-1){
        switch(c){
            case 'i'://correct args,get config file
                config_file = optarg;
                break;
            case '?'://invalid args,exit
                std::cout<<"invalid args!" << std::endl;
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            case ':'://no args,exit
                std::cout << "need <configfile>" << std::endl;
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }


    //config file rpcserver_ip rpcserver_port zookeeper_ip zookeeper_port
    m_config.LoadConfigFile(config_file.c_str());
    
    /*
    std::cout << "rpcserverip=" << m_config.Load("rpcserverip") << std::endl;
    std::cout << "rpcserverport=" << m_config.Load("rpcserverport") << std::endl;
    std::cout << "zookeeperip=" << m_config.Load("zookeeperip") << std::endl;
    std::cout << "zookeeperport=" << m_config.Load("zookeeperport") << std::endl;
    */
}
    
    
    
MyrpcApplication& MyrpcApplication::GetInstance(){
    static MyrpcApplication app;
    return app;
}

//return config
MyrpcConfig& MyrpcApplication::GetConfig(){
    return m_config;
}