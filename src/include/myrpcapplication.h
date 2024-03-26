#pragma once

//myrpc base class
class MyrpcApplication{
public:
    //initialization
    static void Init(int argc, char **argv);
    static MyrpcApplication& GetInstance();

private:
    MyrpcApplication(){}
    MyrpcApplication(const MyrpcApplication&) = delete;
    MyrpcApplication(MyrpcApplication&&) = delete;
};