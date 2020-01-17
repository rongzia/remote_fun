//
// Created by rrzhang on 2019/12/24.
//
#include "handle_grpc.h"
#include "easylogger.h"

namespace remote {
    ClientGrpcHandle::ClientGrpcHandle(const std::string &connect_to_addr, const std::string &connect_to_port)
            : stub_(JsonCall::NewStub(grpc::CreateChannel(
            std::string("") + connect_to_addr + ":" + connect_to_port, grpc::InsecureChannelCredentials()))) {
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "connect to: " << connect_to_addr << ":" << connect_to_port;
    }

    std::string ClientGrpcHandle::Send(const std::string &json) {
        JsonRequest request;
        JsonReply reply;
        grpc::ClientContext context;

        request.set_json(json);
        grpc::Status status = stub_->DoCall(&context, request, &reply);

#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "client send size:" << json.length();
#endif // MULTI_MASTER_ZHANG_LOG_FUN
#ifdef MULTI_MASTER_ZHANG_LOG_JSON
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "client send content:" << json;
#endif // MULTI_MASTER_ZHANG_LOG_JSON

        if (status.ok()) {
            return reply.ret_string();
        } else {
            return "-100";
        }
    }

}
