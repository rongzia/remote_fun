//
// Created by rrzhang on 2019/12/24.
//
#include "handle_libevent.h"
#include "libevent_handle.h"     //! for eventHandle_
#include "remote_server.h"  //! for RemoteServer::SelectDoCall()
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
            std::cout << "init error" << std::endl;
        }
        std::cout << "server listen on port : " << server_port_ << std::endl;

        //! return 0, represent success.
        return 0;
    }

    void ServerLibeventHandle::RunServer() {
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "RemoteServer::RunServer().";

        while (1) {
            memset(buffer_, 0, sizeof(char) * 1049600);
            libeventHandle_->get_listen_connection_array(ListenArray());

            int readsize_1 = libeventHandle_->wait_recive(ListenArray()[0], buffer_);
            if (readsize_1 > 0) {
                std::cout << "libevent recive success. read size " << readsize_1;
//                std::cout << ", read content : " << buffer_ << std::endl;

                std::string json = std::string(buffer_, readsize_1);
                std::string ret = ServerNetHandle::RemoteServerHandle()->SelectDoCall(json);
                libeventHandle_->send(ListenArray()[0], ret.data(), ret.length());

                std::cout << "libevent send success. send size " << ret.length();
//                std::cout << ", send content : " << ret.data() << std::endl;
            }

//            if(libeventHandle_->get_listen_connection_count() > 0) {
//#ifdef MULTI_MASTER_ZHANG_LOG_REMOTE
//                EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
//                        << "get_listen_connection_count : " << libeventHandle_->get_listen_connection_count();
//#endif // MULTI_MASTER_ZHANG_LOG_REMOTE
//            }
        }
    }
}