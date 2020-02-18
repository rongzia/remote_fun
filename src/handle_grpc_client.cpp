//
// Created by rrzhang on 2019/12/24.
//
#include "handle_grpc.h"

#include "remote_string.h"
#include "easylogger.h"

namespace remote {
    ClientGrpcHandle::ClientGrpcHandle(const std::string &connect_to_addr, const std::string &connect_to_port)
            : stub_(JsonCall::NewStub(grpc::CreateChannel(
            std::string("") + connect_to_addr + ":" + connect_to_port, grpc::InsecureChannelCredentials()))) {
        EasyLoggerWithTrace(path_log_client, EasyLogger::info).force_flush() << "connect to: " << connect_to_addr << ":" << connect_to_port;
    }

    std::string ClientGrpcHandle::Send(const std::string &json) {
        JsonRequest request;
        JsonReply reply;
        grpc::ClientContext context;

        request.set_json(json);
        grpc::Status status = stub_->DoCall(&context, request, &reply);

//#ifdef MULTI_MASTER_ZHANG_LOG
//        EasyLoggerWithTrace(path_log_client, EasyLogger::info).force_flush() << "client send size:" << json.length();
//#endif
#ifdef MULTI_MASTER_ZHANG_LOG_JSON
        EasyLoggerWithTrace(path_log_client, EasyLogger::info).force_flush() << "client send content:" << json;
#endif

        if (status.ok()) {
            return reply.ret_string();
        } else {
#ifdef MULTI_MASTER_ZHANG_LOG
            EasyLoggerWithTrace(path_log_client, EasyLogger::info).force_flush() << "grpc call error_details:" << status.error_details() << ", json:" << json.substr(0, 30);
#endif
            return "grpc call error";
        }
    }
}
