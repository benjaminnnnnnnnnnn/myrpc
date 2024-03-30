#include "myrpccontroller.h"


MyrpcController::MyrpcController(){
    m_failed = false;
    m_errText = "";
}


void MyrpcController::Reset(){
    m_failed = false;
    m_errText = "";
}


bool MyrpcController::Failed() const{
    return m_failed;
}


std::string MyrpcController::ErrorText() const{
    return m_errText;
}


void MyrpcController::SetFailed(const std::string& reason){
    m_failed = true;
    m_errText = reason;
}

void MyrpcController::StartCancel(){}
bool MyrpcController::IsCanceled() const{return false;}
void MyrpcController::NotifyOnCancel(google::protobuf::Closure* callback){}