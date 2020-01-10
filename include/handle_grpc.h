/**
 * Created by rrzhang on 2019/12/11.
 */

#ifndef MULTI_MASTER_HANDLE_GRPC_H
#define MULTI_MASTER_HANDLE_GRPC_H
#ifdef REMOTE_FUN_WITH_GRPC

#include "handle_net.h"

#include <grpcpp/grpcpp.h>
#include "json.grpc.pb.h"

namespace remote {

    class ServerGprcHandle : public JsonCall::Service, public ServerNetHandle {
    public:
        grpc::Status
        DoCall(grpc::ServerContext *context, const remote::JsonRequest *request, remote::JsonReply *reply) override;

        void RunServer() override;

        explicit ServerGprcHandle(RemoteServer *);

        ServerGprcHandle() = delete;

        ServerGprcHandle(const ServerGprcHandle &) = delete;

        ServerGprcHandle &operator=(const ServerGprcHandle &) = delete;
    };

    class ClientGrpcHandle : public ClientNetHandle {
    public:
        std::string Send(const std::string &) const override;

        explicit ClientGrpcHandle(const std::string &connect_to_addr, const std::string &connect_to_port);

        ClientGrpcHandle(const ClientGrpcHandle &) = delete;

        ClientGrpcHandle &operator=(const ClientGrpcHandle &) = delete;


    private:
        std::unique_ptr<remote::JsonCall::Stub> stub_;
    };
}
#endif // REMOTE_FUN_WITH_GRPC
#endif //MULTI_MASTER_HANDLE_GRPC_H
