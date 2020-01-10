//
// Created by rrzhang on 2019/12/24.
//
#ifndef MULTI_MASTER_HANDLE_LIBEVENT_H
#define MULTI_MASTER_HANDLE_LIBEVENT_H
#ifdef REMOTE_FUN_WITH_LIBEENT

#include "handle_net.h"

class LibeventHandle;

namespace remote {
    class RemoteServer;

    class ServerLibeventHandle : public ServerNetHandle {
    public :
        int *ListenArray();

        int Init();

        void RunServer() override;


        explicit ServerLibeventHandle(RemoteServer *);

        ServerLibeventHandle() = delete;

        ServerLibeventHandle(const ServerNetHandle &) = delete;

        ServerLibeventHandle &operator=(const ServerLibeventHandle &) = delete;

    private:
        LibeventHandle *libeventHandle_;
        int server_port_;
        int listen_array_[10];
        char buffer_[1049600];
//        int buffer_size_ = 1049600;
    };

    class ClientLibEventHandle : public ClientNetHandle {
    public:

        std::string Send(const std::string &json) const override;

        int Init();

        explicit ClientLibEventHandle(const std::string &connect_to_addr, const std::string &connect_to_port, const std::string &listen_port);

        ClientLibEventHandle(const ClientLibEventHandle &) = delete;

        ClientLibEventHandle &operator=(const ClientLibEventHandle &) = delete;

    private:
        LibeventHandle *eventHandle_;
        int conn_id_;

        const std::string connect_to_addr_;
        const int connect_to_port_;
        int listen_port_;
    };
}

#endif // REMOTE_FUN_WITH_LIBEENT
#endif // MULTI_MASTER_HANDLE_LIBEVENT_H