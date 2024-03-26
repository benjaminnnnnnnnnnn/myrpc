#include "test.pb.h"
#include <iostream>

int main(){

    fixbug::LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("zhangsan123");

    std::string send_str;
    req.SerializeToString(&send_str);

}