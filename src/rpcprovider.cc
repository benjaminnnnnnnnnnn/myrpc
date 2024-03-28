#include <rpcprovider.h>
#include <myrpcapplication.h>
#include <rpcheader.pb.h>

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

    //std::cout << "RpcProvider start service at ip:" << ip << "port:" << port << std::endl;

    //start net service
    server.start();
    m_eventLoop.loop();
}

//new socket link callback
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn){
    if(!conn->connected()){
        //link broked
        conn->shutdown();
    }
}


/*
provider and consumer use the same format
service_name method_name args
use proto to serialization and parse

format:
header_size header_str args_size args_str
*/
//message read&write linked callback
//if there is a remote rpc request, Onmessage will response
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn, 
                            muduo::net::Buffer* buffer, 
                            muduo::Timestamp){
    //received remote request stream(Login args)
    std::string recv_buf = buffer->retrieveAllAsString();

    //read four bytes content->header_size
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0);

    //read header_size bytes as origin stream
    //parsefromstring 
    std::string rpc_header_str = recv_buf.substr(4, header_size);
    myrpc::RpcHeader rpcHeader;

    std::string service_name;
    std::string method_name;
    uint32_t args_size;

    if(rpcHeader.ParseFromString(rpc_header_str)){
        //success
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    }else{
        //failed
        std::cout << "rpc_header_str:" << rpc_header_str << "  parse failed!" << std::endl;
        return;
    }

    std::string args_str = recv_buf.substr(4+header_size, args_size);

    std::cout << "=========================================" << std::endl;
    std::cout << "header_size:" << header_size << std::endl;
    std::cout << "rpc_header_str:" << rpc_header_str << std::endl;
    std::cout << "service_name:" << service_name << std::endl;
    std::cout << "method_name:" << method_name << std::endl;
    std::cout << "args_str:" << args_str << std::endl;
    std::cout << "=========================================" << std::endl;


    //get service and method by name
    //service does not exist
    auto it = m_serviceMap.find(service_name);
    if(it == m_serviceMap.end()){
        std::cout << service_name << "does not exist!" << std::endl;
        return;
    }
    //method does not exist
    auto mit = it->second.m_methodMap.find(method_name);
    if(mit == it->second.m_methodMap.end()){
        std::cout << service_name << ":" << method_name << "does not exist!" << std::endl;
        return;
    }
        
    google::protobuf::Service *service = it->second.m_service;
    const google::protobuf::MethodDescriptor *method = mit->second;

    //generate rpc request and response args
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(args_str)){
        std::cout << "request parse error!" << std::endl;
        return;
    }
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();

    //bind closure callback
    google::protobuf::Closure *done = google::protobuf::NewCallback<RpcProvider,
                                                                    const muduo::net::TcpConnectionPtr&,
                                                                    google::protobuf::Message*>(this, &RpcProvider::SendRpcResponse, conn, response);

    //based on rpc request,invoke method
    //(controller,request,response,done)
    service->CallMethod(method, nullptr, request, response, done);
}

//closure callback, serialize rpc response and send it
void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response){
    std::string response_str;
    if(response->SerializeToString(&response_str)){
        //serilization success
        conn->send(response_str);
        conn->shutdown();//break link
    }else{
        std::cout << "serialize error!" << std::endl;

    }
    conn->shutdown();
}