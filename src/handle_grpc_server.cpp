//
// Created by rrzhang on 2019/12/24.
//
#include "handle_grpc.h"
#include "remote_server.h"

namespace remote {
    ServerGprcHandle::ServerGprcHandle(RemoteServer *remote_server) : ServerNetHandle(remote_server) {}

    void ServerGprcHandle::RunServer() {
#ifdef MULTI_MASTER_ZHANG_DEBUG_FUN
        std::cout << "ServerGprcHandle::RunServer()." << std::endl;
#endif // MULTI_MASTER_ZHANG_DEBUG_FUN
        std::string server_address("0.0.0.0:50051");

        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(this);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        server->Wait();
    }

    grpc::Status ServerGprcHandle::DoCall(grpc::ServerContext *context, const JsonRequest *request, JsonReply *reply) {
#ifdef MULTI_MASTER_ZHANG_DEBUG_FUN
        std::cout << "ServerGprcHandle::DoCall()." << std::endl;
#endif // MULTI_MASTER_ZHANG_DEBUG_FUN
#ifdef MULTI_MASTER_ZHANG_DEBUG_JSON
        std::cout << "[remote fun] recive json : " << request->json() << std::endl;
#endif // MULTI_MASTER_ZHANG_DEBUG_JSON
        std::string json = request->json();

        std::string ret = ServerNetHandle::RemoteServerHandle()->SelectDoCall(json);

        reply->set_ret_string(ret);
        return grpc::Status::OK;
    }
}