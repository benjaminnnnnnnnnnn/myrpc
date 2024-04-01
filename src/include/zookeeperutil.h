#pragma once

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>


class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    //zkclient start, link zkserver
    void Start();
    //create zk node
    void Create(const char* path, const char* data, int datalen, int state = 0);
    //get data by path
    std::string GetData(const char *path);
private:
    zhandle_t* m_zhandle;
};