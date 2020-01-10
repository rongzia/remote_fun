/**
 * Created by rrzhang on 2019/12/11.
 */

#ifndef MULTI_MASTER_HANDLE_NET_H
#define MULTI_MASTER_HANDLE_NET_H

#include <string>
#include <cstring>

namespace remote {
    class RemoteServer;

    class ServerNetHandle {
    public:
        ServerNetHandle() = delete;

        ServerNetHandle(const ServerNetHandle &) = delete;

        ServerNetHandle &operator=(const ServerNetHandle &) = delete;

        virtual void RunServer() = 0;

    protected:
        explicit ServerNetHandle(remote::RemoteServer *);

        RemoteServer *RemoteServerHandle() const;

    private:
        RemoteServer *remote_server_handle_;
    };

    class RemoteClient;

    class ClientNetHandle {
    public:
        ClientNetHandle(const ClientNetHandle &) = delete;

        ClientNetHandle &operator=(const ClientNetHandle &) = delete;

        virtual std::string Send(const std::string &) const = 0;

    protected:
        ClientNetHandle() = default;
    };
}
#endif //MULTI_MASTER_HANDLE_NET_H
