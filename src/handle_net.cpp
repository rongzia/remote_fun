/**
 * Created by rrzhang on 2019/12/11.
 */

#include "handle_net.h"

namespace remote {


    ServerNetHandle::ServerNetHandle(remote::RemoteServer *remote_server) : remote_server_handle_(remote_server) {}

    RemoteServer *ServerNetHandle::RemoteServerHandle() const {
        return remote_server_handle_;
    }
}