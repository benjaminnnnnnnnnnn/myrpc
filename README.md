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
when invoke main func, it needs two args, for the config file
you should create a *.conf file under executable file directory
run the executable file
```
./provider/./consumer -i *.conf
```
and you should write something in *.conf file
provider has to fill the rpcserverip and port by its ip and port
and zookeeper ip&port with the right ip&port
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

### For provider/server

### For comsumer/client




一、标题写法：
第一种方法：
1、在文本下面加上 等于号 = ，那么上方的文本就变成了大标题。等于号的个数无限制，但一定要大于0个哦。。
2、在文本下面加上 下划线 - ，那么上方的文本就变成了中标题，同样的 下划线个数无限制。
3、要想输入=号，上面有文本而不让其转化为大标题，则需要在两者之间加一个空行。
另一种方法：（推荐这种方法；注意⚠️中间需要有一个空格）
关于标题还有等级表示法，分为六个等级，显示的文本大小依次减小。不同等级之间是以井号  #  的个数来标识的。一级标题有一个 #，二级标题有两个# ，以此类推。
例如：
# 一级标题  
## 二级标题  
### 三级标题  
#### 四级标题  
##### 五级标题  
###### 六级标题 
二、编辑基本语法  
1、字体格式强调
 我们可以使用下面的方式给我们的文本添加强调的效果
*强调*  (示例：斜体)  
 _强调_  (示例：斜体)  
**加重强调**  (示例：粗体)  
 __加重强调__ (示例：粗体)  
***特别强调*** (示例：粗斜体)  
___特别强调___  (示例：粗斜体)  
2、代码  
`<hello world>`  
3、代码块高亮  
```  
@Override
protected void onDestroy() {
    EventBus.getDefault().unregister(this);
    super.onDestroy();
}
```  
4、表格 （建议在表格前空一行，否则可能影响表格无法显示）
 
 表头  | 表头  | 表头
 ---- | ----- | ------  
 单元格内容  | 单元格内容 | 单元格内容 
 单元格内容  | 单元格内容 | 单元格内容  
 
5、其他引用
图片  
![图片名称](https://www.baidu.com/img/bd_logo1.png)  
链接  
[链接名称](https://www.baidu.com/)    
6、列表 
1. 项目1  
2. 项目2  
3. 项目3  
   * 项目1 （一个*号会显示为一个黑点，注意⚠️有空格，否则直接显示为*项目1） 
   * 项目2   
 
7、换行（建议直接在前一行后面补两个空格）
直接回车不能换行，  
可以在上一行文本后面补两个空格，  
这样下一行的文本就换行了。
或者就是在两行文本直接加一个空行。
也能实现换行效果，不过这个行间距有点大。  
 
8、引用
> 第一行引用文字  
> 第二行引用文字   