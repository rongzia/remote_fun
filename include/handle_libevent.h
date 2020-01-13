//
// Created by rrzhang on 2019/12/24.
//
#ifndef MULTI_MASTER_HANDLE_LIBEVENT_H
#define MULTI_MASTER_HANDLE_LIBEVENT_H
#ifdef REMOTE_FUN_WITH_LIBEVENT

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
        char buffer_[10496000];
    };

    class ClientLibEventHandle : public ClientNetHandle {
    public:

        std::string Send(const std::string &json) override;

        int Init();

        explicit ClientLibEventHandle(const std::string &connect_to_addr, const std::string &connect_to_port, const std::string &listen_port);

        ClientLibEventHandle(const ClientLibEventHandle &) = delete;

        ClientLibEventHandle &operator=(const ClientLibEventHandle &) = delete;

    private:
        const std::string connect_to_addr_;
        const int connect_to_port_;
        int listen_port_;

        LibeventHandle *eventHandle_;
        int conn_id_;
        char buffer_[10496000];
    };
}

#endif // REMOTE_FUN_WITH_LIBEVENT
#endif // MULTI_MASTER_HANDLE_LIBEVENT_H