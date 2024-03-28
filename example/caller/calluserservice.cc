#include <iostream>
#include <myrpcapplication.h>
#include <user.pb.h>
#include <myrpcchannel.h>

int main(int argc,char** argv){
    //after start the program, must invike the Init() func to initialize myrpc0, only once
    MyrpcApplication::Init(argc,argv);

    //invoke rpc method login
    fixbug::UserServiceRpc_Stub stub(new MyrpcChannel());

    //rpc method request
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");


    fixbug::LoginResponse response;

    //rpc method invoke ----->callmethod
    stub.Login(nullptr,&request,&response,nullptr);

    //rpc invoke done, load response
    if(!response.result().errcode()){
        std::cout << "rpc login response success:" << response.success() << std::endl;
    }else{
        std::cout << "rpc login response error:" << response.result().errmsg() << std::endl;
    }

    

    return 0;
}