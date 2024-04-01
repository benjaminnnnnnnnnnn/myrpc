#include <myrpcchannel.h>
#include <rpcheader.pb.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <myrpcapplication.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <myrpccontroller.h>
#include "zookeeperutil.h"

//all method invoked by stub goes here
void MyrpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                          google::protobuf::Message* response, google::protobuf::Closure* done){
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();


    //get serialized args string size
    int args_size = 0;
    std::string args_str;
    if(request->SerializeToString(&args_str)){
        args_size = args_str.size();
    }else{
        controller->SetFailed("serialize request failed!");
        return;
    }

    //define request header
    myrpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if(rpcHeader.SerializeToString(&rpc_header_str)){
        header_size = rpc_header_str.size();
    }else{
        controller->SetFailed("serialize rpc header error!");
        return;
    }

    //organize str to send
    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size,4));//header_size
    send_rpc_str += rpc_header_str;//rpc_header_str
    send_rpc_str += args_str;//request

    //std::cout << "Request:" << rpc_header_str << std::endl;
    //std::cout << "service_name:" << service_name << std::endl;
    //std::cout << "method_name:" << method_name << std::endl;
    //std::cout << "args_str:" << args_str << std::endl;

    //use tcp coding, complete rpc method invoke
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == clientfd){
        char errtxt[512] = {0};
        sprintf(errtxt, "create socket errno! errno:%d", errno);
        controller->SetFailed(errtxt);
        return;
    }

    //std::string ip = MyrpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    //uint16_t port = atoi(MyrpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    //get ip and port by zookeeper
    ZkClient zkCli;
    zkCli.Start();
    // /UserServiceRpc/Login
    std::string method_path = "/" + service_name + "/" + method_name;
    std::string host_data = zkCli.GetData(method_path.c_str());
    if(host_data == ""){
        controller->SetFailed(method_path + "does not exist!");
        return;
    }
    int idx = host_data.find(":");
    if(idx == -1){
        controller->SetFailed(method_path + "is invalid!");
        return;
    }
    std::string ip = host_data.substr(0, idx);
    uint16_t port = atoi(host_data.substr(idx+1, host_data.size()-idx).c_str());

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    //connect rpc server node
    if(-1 == connect(clientfd,(struct sockaddr*)&server_addr,sizeof(server_addr))){
        char errtxt[512] = {0};
        sprintf(errtxt, "connect errno! errno:%d", errno);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }

    //send rpc request
    if(-1 == send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0)){
        char errtxt[512] = {0};
        sprintf(errtxt, "send errno! errno:%d", errno);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }

    //receive response value
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if(-1 == (recv_size = recv(clientfd,recv_buf,1024,0))){
        char errtxt[512] = {0};
        sprintf(errtxt, "receive errno! errno:%d", errno);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }

    //write response into response
    //std::string response_str(recv_buf,0,recv_size);//construct error
    if(!response->ParseFromArray(recv_buf, recv_size)){
        char errtxt[2046] = {0};
        sprintf(errtxt, "parse errno! received_str:%s", recv_buf);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }
    
    close(clientfd);
}