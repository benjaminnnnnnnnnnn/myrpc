# MYRPC
MYRPC is a Remote Procedure Call Protocol framework. Assume there's a huge project, functions are deployed on diffrent server, when client need to invoke an func, it has to go to the server provides the func it need. But the client don't know where the server is. That's just why we need an rpc frame.
MYRPC does all the work between service provider and service consumer, all consumer has to do is fill the message about to send, and give it to the MYRPC, if everything works fine, it will get the right response.
And provider needs to make sure itself works fine and wait for the request, and response it.

Before using the project, you should know it can only run on ***LINUX***.

## Intallation and Usage Instructions
### Clone the Repository
You can use the command below to clone the project
```
git clone https://github.com/benjaminnnnnnnnnnn/myrpc.git
```
### Install Dependencies
Before use MYRPC, you should install several dependecies, protobuf, muduo and zookeeper.
#### Protobuf
download protobuf from github and unzip it
```
git clone https://github.com/protocolbuffers/protobuf/releases/tag/v3.17.3
tar zvxf protobuf-python-3.17.3.tar.gz
```
install dependecies
```
cd protobuf-3.17.3/
//./autogen.sh  //some old version need this command, new version could skip
./configure --prefix=/usr/local/protobuf
sudo make
sudo make install
sudo ldconfig   
```
check the protoc version
```
protoc --version
```
add environment variable
```
sudo vim /etc/profile
```
add
```
export PATH=$PATH:/usr/local/protobuf/bin/
export PKG_CONFIG_PATH=/usr/local/protobuf/lib/pkgconfig/
```
refresh
```
source /etc/profile
```
Configure the dynamic link library path
```
sudo vim /etc/ld.so.conf
```
add
```
/usr/local/protobuf/lib
```
#### muduo
muduo is compiled by cmake, so make sure you have installed cmake
```
sudo apt-get install cmake
```
necessary lib
```
sudo apt-get install libboost-dev libboost-test-dev
```
unnecessary lib
```
sudo apt-get install libcurl4-openssl-dev libc-ares-dev
sudo apt-get install protobuf-compiler libprotobuf-dev
```
download muduo from official website
```
unzip muduo-master.zip
cd muduo-master
./build.sh
./build.sh install
```
#### zookeeper
download zookeeper and unzip it
```
wget https://archive.apache.org/dist/zookeeper/zookeeper-3.4.10/zookeeper-3.4.10.tar.gz
tar zvxf zookeeper-3.4.10.tar.gz
```
into /conf and copy the zoo_sample.cfg rename zoo.cfg and change the datadir
```
cd conf
mv zoosample.cfg zoo.cfg
sudo nano zoo.cfg
```
datadir = /your/new/dir/to/save/the/zookeeper_data
and zookeeper default port is 2181

go to src/c, and compile
```
sudo ./configure
sudo make
sudo make install
```
zookeeper depends on java, so you should install jdk

you should start the zookeeper server, so service provider and consumer could link to zkserver
```
cd bin
./zkServer.sh start
```

### compile MYRPC
```
cd myrpc
./autobuild.sh
```
make sure autobuild.sh have authority
you can use cammand chmod
```
chmod 777 autobuild.sh
```
after all above, when you use MYRPC, you just have to include dir lib, there are all the so lib and .h file.

## How to use MYRPC
### configuration for both
#### config file
when invoke main func, it needs two args, for the config file. 
you should create a *.conf file under executable file directory
run the executable file
```
./provider/./consumer -i *.conf
```
and you should write something in *.conf file. 
provider has to fill the rpcserverip and port by its ip and port
and zookeeper ip&port with the right ip&port. 
consumer only need zookeeper ip&port, 
it will get server ip&port by connecting to zookeeper
```
#rpc node ip
rpcserverip=127.0.0.1
#rpc node port
rpcserverport=8000
#zookeeper ip
zookeeperip=127.0.0.1
#zookeeper port
zookeeperport=2181
```
#### protoc file
server and client shou negotiate the common .proto file. 
It stipulates the request message and response message, 
and also the method info.
you can reference to the user.proto under myrpc/example
```
//just for reference
message LoginRequest{
    bytes name = 1;
    bytes pwd = 2;
}
message LoginResponse{
    ResultCode result = 1;
    bool success = 2;
}
service UserServiceRpc{
    rpc Login(LoginRequest) returns(LoginResponse);
    rpc Register(RegisterRequest) returns(RegisterResponse);
}
```
use protoc to generate .pb.cc and .pb.h file
```
protoc user.proto --cpp_out=./
```
### For provider/server
provider should include serveral head file from MYRPC and .pb.h file generated by protoc
```
#include <user.pb.h>
#include <myrpcapplication.h>
#include <rpcprovider.h>
#include "logger.h"
```
provider should define a class inherited from UserServiceRpc(service rpc name defined in .proto file), and rewrite method func defined in .proto file.
```
void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done){
                        //get data from request
                        //do the method and get result
                        //fill response
                        //call back
                        done->Run();
                       }
```
in the main func, init MRPC, instancing rpcprovider and notify service. 
code below is just for reference
```
int main(int argc, char **argv){
    //Initialization
    MyrpcApplication::Init(argc,argv);

    //publish service
    //provider is a rpc net service object
    RpcProvider provider;
    //notify service, it needs a service instance
    provider.NotifyService(new UserService());

    //run the rpc node,wait to be invoked
    provider.Run();
    
    return 0;
}
```
### For comsumer/client
consumer also needs to initiate MYRPC first
```
MyrpcApplication::Init(argc,argv);
```
consumer don't have to inherit class and rewrite func, it just has to instance service_stub by give a new channel
```
fixbug::UserServiceRpc_Stub stub(new MyrpcChannel());
```
controller is unnecessary, nullptr is ok. 
Instance a rpc request and fill info in it. 
Instance a response to receive the response sended back
```
//rpc method request
fixbug::LoginRequest request;
request.set_name("zhang san");
request.set_pwd("123456");

fixbug::LoginResponse response;
```
Invoke the func
```
//rpc method invoke ----->callmethod
stub.Login(&controller,&request,&response,nullptr);
```
and consumer can read response message
```
//response.success()
//response.result().errcode()
```
## Project Structure and File Organization

File/Dir | Description | purpose
 ---- | ----- | ------ 
/.vscode | vscode config dir | vscode save the project environment
/bin | binary dir | saves excutable files
/build | building dir | saves file generated while building the project
/example | example dir | an example how to use MYRPC
/lib | library dir | .so lib and head files
/src | source code dir | saves all source code
autobuild.sh | compile file | run the file to build whole project
CMakeLists.txt | cmake file | cmake file helps build the project
README.md | readme file | help user knowing about MYRPC