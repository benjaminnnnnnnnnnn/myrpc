#pragma once
#include "google/protobuf/service.h"

//publish rpc net service class
class RpcProvider{
public:
    //the func must could receive any kind of service ,not one kind of service
    void NotifyService(google::protobuf::Service *service);
    //start the rpc service node, could provide rpc service
    void Run();
};