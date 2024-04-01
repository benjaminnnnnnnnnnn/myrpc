#include "zookeeperutil.h"
#include "myrpcapplication.h"
#include <iostream>
#include <logger.h>

void global_watcher(zhandle_t* zh, int type, int state, const char* path, void *watcherCtx){
    if(type == ZOO_SESSION_EVENT){  //invoke message type session event
        if(state == ZOO_CONNECTED_STATE){  //client and server connect success
            sem_t *sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

ZkClient::ZkClient(): m_zhandle(nullptr){}

ZkClient::~ZkClient(){
    if(m_zhandle){
        zookeeper_close(m_zhandle);
    }
}

//link zkserver
void ZkClient::Start(){
    std::string host = MyrpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = MyrpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;

    //zookeeper mutible thread
    //api invoke thread
    //net I/O thread
    //watcher callback thread
    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);    
    if(nullptr == m_zhandle){
        LOG_ERROR("zookeeper init error!");
        std::cout << "zookeeper init error" << std::endl;
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);
    std::cout << "zookeeper_init success!" << std::endl;
    LOG_INFO("zookeeper init success!");
}

void ZkClient::Create(const char* path, const char* data, int datalen, int state){
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;
    flag = zoo_exists(m_zhandle, path, 0, nullptr);
    if(ZNONODE==flag){//path node not exist
        //create node
        flag = zoo_create(m_zhandle, path, data, datalen, 
                        &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
        if(flag == ZOK){
            std::cout << "znode create success... path:" << path << std::endl;
        }else{
            std::cout << "flag:" << flag << std::endl;
            std::cout << "znode create error... path:" << path << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

std::string ZkClient::GetData(const char* path){
    char buffer[64];
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(m_zhandle, path, 0, buffer, &bufferlen, nullptr);
    if(flag != ZOK){
        std::cout << "get znode error... path:" << path << std::endl;
        return "";
    }else{
        return buffer;
    }

}