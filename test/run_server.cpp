/**
 * Created by rrzhang on 2019/12/9.
 */

#include <iostream>
#include "remote_server.h"
#include "handle_net.h"


using namespace std;

int main() {

    int listen_port = 50051;
    remote::RemoteServer *remote_server = new remote::RemoteServer(listen_port);
    remote_server->GetServerNetHandle()->RunServer();

    return 0;
}