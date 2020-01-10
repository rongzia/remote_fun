//
// Created by rrzhang on 2019/12/24.
//
#include "handle_grpc.h"

namespace remote {
    ClientGrpcHandle::ClientGrpcHandle(const std::string &connect_to_addr, const std::string &connect_to_port)
            : stub_(JsonCall::NewStub(
            grpc::CreateChannel(std::string("") + connect_to_addr + ":" + connect_to_port, grpc::InsecureChannelCredentials()))) {
#ifdef MULTI_MASTER_ZHANG_DEBUG_FUN
        std::cout << "ClientGrpcHandle::ClientGrpcHandle()." << std::endl;
#endif // MULTI_MASTER_ZHANG_DEBUG_FUN
    }

    std::string ClientGrpcHandle::Send(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_DEBUG_FUN
        std::cout << "ClientGrpcHandle::send()." << std::endl;
#endif // MULTI_MASTER_ZHANG_DEBUG_FUN

        JsonRequest request;
        JsonReply reply;
        grpc::ClientContext context;

        request.set_json(json);
        grpc::Status status = stub_->DoCall(&context, request, &reply);

        if (status.ok()) {
            return reply.ret_string();
        } else {
            return "-100";
        }
    }

}