#include "test.pb.h"
#include <iostream>

int main(){
    //define login data
    fixbug::LoginRequest req;
    req.set_name("zhangsan");
    req.set_pwd("zhangsan123");

    //serialization
    std::string send_str;
    if(req.SerializeToString(&send_str))
    {
        std::cout << send_str << std::endl;
    }
    
    //de-serialization
    fixbug::LoginRequest reqB;
    if(reqB.ParseFromString(send_str))
    {
        std::cout << reqB.name() << std::endl;
        std::cout << reqB.pwd() << std::endl;
    }
    //set_  mutable_  add_  for different kind of objects
    //mutable_ and add_ return a pointer then use set_ to change the value
    
    fixbug::GetFriendListsResponse rsp;
    fixbug::ResultCode * rc = rsp.mutable_result();
    rc->set_errcode(0);
    rc->set_errmsg("");

    fixbug::User *user1 = rsp.add_friend_list();
    user1->set_age(18);
    user1->set_name("alan");
    user1->set_sex(fixbug::User::MAN);

    std::cout  <<  user1->age()  << std::endl;
    std::cout  <<  user1->name()  << std::endl;
    return 0;
}