/**
 * Created by rrzhang on 2019/12/9.
 */

#include <iostream>
#include "multi_macro.h"
#include "remote_server.h"
#include "handle_net.h"


using namespace std;

int main() {

    remote::RemoteServer *remote_server = new remote::RemoteServer(std::stoi(multi_master::server_port));
    remote_server->GetServerNetHandle()->RunServer();

    return 0;
}