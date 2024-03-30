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
        return true;
    }
    
    bool Register(uint32_t id, std::string name, std::string pwd){
        std::cout << "local service: Register" << std::endl;
        std::cout << "id" << id << " name:" << name << " pwd:" << pwd << std::endl;
        return true;
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

    void Register(::google::protobuf::RpcController* controller,
                       const ::fixbug::RegisterRequest* request,
                       ::fixbug::RegisterResponse* response,
                       ::google::protobuf::Closure* done){
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Register(id,name, pwd);

        response->set_success(ret);
        fixbug::ResultCode *rc = response->mutable_result();
        rc->set_errcode(0);
        rc->set_errmsg("");

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