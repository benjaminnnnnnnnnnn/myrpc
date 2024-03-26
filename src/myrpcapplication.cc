#include <myrpcapplication.h>
#include <iostream>

void MyrpcApplication::Init(int argc, char **argv){
    if(argc < 2){
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }
}
    
    
    
MyrpcApplication& MyrpcApplication::GetInstance(){
    static MyrpcApplication app;
    return app;
}