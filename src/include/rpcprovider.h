#pragma once
#include "google/protobuf/service.h"
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <functional>
#include <google/protobuf/descriptor.h>
#include <unordered_map>
#include <string>

//publish rpc net service class
class RpcProvider{
public:
    //the func must could receive any kind of service ,not one kind of service
    void NotifyService(google::protobuf::Service *service);
    //start the rpc service node, could provide rpc service
    void Run();

private:
    //combine Tcpserver
    //std::unique_ptr<muduo::net::TcpServer> m_tcpserverPtr;
    //combine EventLoop
    muduo::net::EventLoop m_eventLoop;

    //service info
    struct ServiceInfo{
        //service object
        google::protobuf::Service *m_service;
        //service methods map
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap;
    };

    //registered service map
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;

    //new socket link callback
    void OnConnection(const muduo::net::TcpConnectionPtr&);

    //message read&write linked callback
    void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);
};