#include <iostream>
#include <string>
#include <user.pb.h>
#include <myrpcapplication.h>
#include <rpcprovider.h>

/*
Local service
provide two methods:
login and getfriendlists
*/
class UserService : public fixbug::UserServiceRpc{
    bool Login(std::string name,std::string pwd){
        std::cout << "local service : login" << std::endl;
        std::cout << "name:" << name << "  pwd:" << pwd << std::endl;
        return false;
    }

    /*
    rewrite the virtual func
    this func is invoked by myrpc
    the example is using myrpc
    caller->login(LoginRequest)->muduo->callee
    callee->Login(LoginRequest)->myrpc rewrite func -> LoginResponse -> muduo
    */
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done){
        //this func gets the data and process the work
        std::string name = request->name();
        std::string pwd = request->pwd();

        //local work
        bool login_result = Login(name,pwd);

        //write the data into response
        response->set_success(login_result);
        fixbug::ResultCode *rc = response->mutable_result();
        rc->set_errcode(0);
        rc->set_errmsg("");

        //re invoke the runfunc
        done->Run();
    }
};


int main(int argc, char **argv){
    //Initialization
    MyrpcApplication::Init(argc,argv);

    //publish service
    //provider is a rpc net service object
    RpcProvider provider;
    provider.NotifyService(new UserService());

    //run the rpc node,wait to be invoked
    provider.Run();
    
    return 0;
}