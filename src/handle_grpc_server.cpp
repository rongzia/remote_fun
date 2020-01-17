//
// Created by rrzhang on 2019/12/24.
//
#include "handle_grpc.h"
#include "remote_server.h"
#include "easylogger.h"

namespace remote {
    ServerGprcHandle::ServerGprcHandle(RemoteServer *remote_server, int listen_port)
            : ServerNetHandle(remote_server)
              , listen_port_(listen_port) {}

    void ServerGprcHandle::RunServer() {
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "ServerGprcHandle::RunServer()";
        std::string server_address(std::string("0.0.0.0:") + std::to_string(listen_port_));

        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(this);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "Server listening on:" << server_address;
        std::cout << "Server listening on:" << server_address << std::endl;

        server->Wait();
    }

    grpc::Status ServerGprcHandle::DoCall(grpc::ServerContext *context, const JsonRequest *request, JsonReply *reply) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "server recive size:" << request->json().length();
#endif // MULTI_MASTER_ZHANG_LOG_FUN
#ifdef MULTI_MASTER_ZHANG_LOG_JSON
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "server recive content:" << request->json();
#endif // MULTI_MASTER_ZHANG_LOG_JSON

        std::string json = request->json();
        std::string ret = ServerNetHandle::RemoteServerHandle()->SelectDoCall(json);
        reply->set_ret_string(ret);
        return grpc::Status::OK;
    }
}