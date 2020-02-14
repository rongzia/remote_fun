//
// Created by rrzhang on 2019/12/24.
//
#include "handle_grpc.h"
#include "remote_server.h"
#include "remote_string.h"
#include "easylogger.h"

namespace remote {
    ServerGprcHandle::ServerGprcHandle(RemoteServer *remote_server, int listen_port)
            : ServerNetHandle(remote_server)
              , listen_port_(listen_port) {}

    void ServerGprcHandle::RunServer() {
        EasyLoggerWithTrace(path_log_server, EasyLogger::info).force_flush() << "ServerGprcHandle::RunServer()";
        std::string server_address(std::string("0.0.0.0:") + std::to_string(listen_port_));

        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(this);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

        EasyLoggerWithTrace(path_log_server, EasyLogger::info).force_flush() << "Server listening on:" << server_address;
        std::cout << "Server listening on:" << server_address << std::endl;

        server->Wait();
    }

    grpc::Status ServerGprcHandle::DoCall(grpc::ServerContext *context, const JsonRequest *request, JsonReply *reply) {
//#ifdef MULTI_MASTER_ZHANG_LOG
//        EasyLoggerWithTrace(path_log_server, EasyLogger::info).force_flush() << "server recive size:" << request->json().length();
//#endif
#ifdef MULTI_MASTER_ZHANG_LOG_JSON
        EasyLoggerWithTrace(path_log_server, EasyLogger::info).force_flush()
                << "server recive content:" << request->json();
#endif
        std::string json = request->json();
        std::string ret = ServerNetHandle::RemoteServerHandle()->SelectDoCall(json);
        reply->set_ret_string(ret);

        return grpc::Status::OK;
    }
}