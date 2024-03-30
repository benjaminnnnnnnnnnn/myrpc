#include <iostream>
#include <myrpcapplication.h>
#include <user.pb.h>


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

    //define a controller
    MyrpcController controller;

    //rpc method invoke ----->callmethod
    stub.Login(&controller,&request,&response,nullptr);

    //send failed, print error reason
    if(controller.Failed()){
        std::cout << controller.ErrorText() << std::endl;
        return 0;
    }else{
        //rpc invoke done, load response
        if(!response.result().errcode()){
            std::cout << "rpc login response success:" << response.success() << std::endl;
        }else{
            std::cout << "rpc login response error:" << response.result().errmsg() << std::endl;
        }
    }

    MyrpcController controllerForRegister;
    fixbug::RegisterRequest req;
    req.set_id(1111);
    req.set_name("lisi");
    req.set_pwd("123456");
    fixbug::RegisterResponse rsp;
    stub.Register(&controllerForRegister, &req, &rsp, nullptr);

    if(controllerForRegister.Failed()){
        std::cout << controllerForRegister.ErrorText() << std::endl;
        return 0;
    }else{
        if(!rsp.result().errcode()){
            std::cout << "rpc register response success:" << rsp.success() << std::endl;
        }else{
            std::cout << "rpc register response error:" << rsp.result().errmsg() << std::endl;
        }
    }

    return 0;
}