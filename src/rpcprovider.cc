#include <rpcprovider.h>
#include <myrpcapplication.h>

//the func must could receive any kind of service ,not one kind of service
void RpcProvider::NotifyService(google::protobuf::Service *service){
    ServiceInfo service_info;

    //get service description
    const google::protobuf::ServiceDescriptor * pserviceDesc = service->GetDescriptor();
    //get service name
    std::string service_name = pserviceDesc->name();
    //get method count
    int methodCnt = pserviceDesc->method_count();

    std::cout << "service_name:" << service_name << std::endl;

    for(int i = 0; i<methodCnt; ++i){
        //get methods descriptor under service by index (abstract)
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name,pmethodDesc});

        std::cout << "method_name:" << method_name << std::endl;
    }
    

    service_info.m_service = service;
    m_serviceMap.insert({service_name, service_info});
    
}

//start the rpc service node, could provide rpc service
void RpcProvider::Run(){
    std::string ip = MyrpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MyrpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip, port);

    //create tcpserver object
    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");
 
    //bond link reinvoke and message read&write reinvike method
    //seperate net code and service code
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    //set muduo thread number
    server.setThreadNum(4);

    std::cout << "RpcProvider start service at ip:" << ip << "port:" << port << std::endl;

    //start net service
    server.start();
    m_eventLoop.loop();
}

//new socket link callback
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn){

}

//message read&write linked callback
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp){

}