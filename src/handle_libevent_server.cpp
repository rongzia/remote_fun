//
// Created by rrzhang on 2019/12/24.
//
#include "handle_libevent.h"
#include "remote_server.h"  //! for RemoteServer::SelectDoCall()
#include "libevent_handle.h"     //! for eventHandle_
#include "easylogger.h"

namespace remote {
    ServerLibeventHandle::ServerLibeventHandle(RemoteServer *remote_server) : ServerNetHandle(remote_server) {
        Init();
    }

    int *ServerLibeventHandle::ListenArray() {
        return listen_array_;
    }

    int ServerLibeventHandle::Init() {
        libeventHandle_ = new LibeventHandle();

        server_port_ = 50051;
        if (!libeventHandle_->init_handle(server_port_)) {
            EasyLoggerWithTrace(multi_master::path_log_server, EasyLogger::info).force_flush()
                    << "init error";
        }
        EasyLoggerWithTrace(multi_master::path_log_server, EasyLogger::info).force_flush()
                << "server listen on port: " << server_port_;
        std::cout << "Server listening on:" << server_port_ << std::endl;

        //! return 0, represent success.
        return 0;
    }

    void ServerLibeventHandle::RunServer() {
        EasyLoggerWithTrace(multi_master::path_log_server, EasyLogger::info).force_flush()
                << "ServerLibeventHandle::RunServer().";

        while (1) {
            memset(buffer_, 0, sizeof(char) * 10496000);
            libeventHandle_->get_listen_connection_array(ListenArray());

            int readsize_1 = libeventHandle_->wait_recive(ListenArray()[0], buffer_);
            if (readsize_1 > 0) {

#ifdef MULTI_MASTER_ZHANG_LOG_JSON
                EasyLoggerWithTrace(multi_master::path_log_server, EasyLogger::info).force_flush()
                        << "server recive size:" << readsize_1;
                EasyLoggerWithTrace(multi_master::path_log_server, EasyLogger::info).force_flush()
                        << "server recive content:" << buffer_;
#endif // MULTI_MASTER_ZHANG_LOG_JSON

                std::string json = std::string(buffer_, readsize_1);
                std::string ret = ServerNetHandle::RemoteServerHandle()->SelectDoCall(json);
                libeventHandle_->send(ListenArray()[0], ret.data(), ret.length());

#ifdef MULTI_MASTER_ZHANG_LOG_JSON
                EasyLoggerWithTrace(multi_master::path_log_server, EasyLogger::info).force_flush()
                        << "server send size:" << ret.length();
                EasyLoggerWithTrace(multi_master::path_log_server, EasyLogger::info).force_flush()
                        << "server send content:" << ret;
#endif // MULTI_MASTER_ZHANG_LOG_JSON
            }
        }
    }
}