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
        std::cout << "serialize request failed!" << std::endl;
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
        std::cout <<"serialize rpc header error!" << std::endl;
        return;
    }

    //organize str to send
    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size),4);//header_size
    send_rpc_str += rpc_header_str;//rpc_header_str
    send_rpc_str += args_str;//request

    std::cout << "=========================================" << std::endl;
    std::cout << "header_size:" << header_size << std::endl;
    std::cout << "rpc_header_str:" << rpc_header_str << std::endl;
    std::cout << "service_name:" << service_name << std::endl;
    std::cout << "method_name:" << method_name << std::endl;
    std::cout << "args_str:" << args_str << std::endl;
    std::cout << "=========================================" << std::endl;

    //use tcp coding, complete rpc method invoke
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == clientfd){
        std::cout << "create socket errno:" << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string ip = MyrpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MyrpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    //connect rpc server node
    if(-1 == connect(clientfd,(struct sockaddr*)&server_addr,sizeof(server_addr))){
        std::cout << "connect error! errno:" << errno << std::endl;
        close(clientfd);
        exit(EXIT_FAILURE);
    }

    //send rpc request
    if(-1 == send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0)){
        std::cout << "send error! errno:" << errno << std::endl;
        close(clientfd);
        return;
    }

    //receive response value
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if(-1 == recv(clientfd,recv_buf,1024,0)){
        std::cout<< "recv error! errno:" << errno << std::endl;
        close(clientfd);
        return;
    }

    //write response into response
    std::string response_str(recv_buf,0,recv_size);
    if(response->ParseFromString(response_str)){
        std::cout << "parse error! response_str:" << response_str << std::endl;
        close(clientfd);
        return;
    }

    close(clientfd);
}