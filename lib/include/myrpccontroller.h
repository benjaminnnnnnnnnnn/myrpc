#pragma once
#include <google/protobuf/service.h>
#include <string>

class MyrpcController : public google::protobuf::RpcController{
public:
    MyrpcController();
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void SetFailed(const std::string& reason);

    //unnecessary func
    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure* callback);

private:
    bool m_failed;//status
    std::string m_errText;//if failed, error message
};