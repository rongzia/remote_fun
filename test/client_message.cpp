#include "event_mess_handle.h"

using namespace std;

 void client_send_cb(EventMessageHandle * mess_handle,EventMessage * mess,void * arg)
 {
//        EventMessage recive_meassage;
//        recive_meassage.prepare_recive("remote_fun", "return");
//        mess_handle->readMessage(&recive_meassage);
        cout << "[remote fun] recive : " << std::string(mess->message, mess->message_size) << endl;
 }


int main(void) {
    EventMessageHandle handler;

    string host_config_path = "/home/zhangrongrong/CLionProjects/Percona-Share-Storage/percona-server/extra/multi_master/remote_fun/test/host_config1.json";
    string mess_config_path = "/home/zhangrongrong/CLionProjects/Percona-Share-Storage/percona-server/extra/multi_master/remote_fun/test/mess_config.json";

    handler.init_handle(host_config_path.c_str(), mess_config_path.c_str());
    handler.register_recive_handler("remote_fun", "return", client_send_cb, NULL);

    cout << "[remote fun] Message Client Test" << endl;
    for(int i = 0; i< 10; i++){
        cout << "[remote fun] ---------- Message Constantly Send Test ------------" << endl;
        EventMessage send_message;
        send_message.prepare_send("remote_fun", "call", "host2", std::to_string(i).data(), 1);
        int return_code = handler.sendMessage(&send_message);
        cout << "[remote fun] send : " << std::to_string(i) << endl;


        if (return_code < 0) {
            cout << "[remote fun] send error" << std::endl;
            cout << MessageError::getEventErrorStr(send_message.error_no) << endl;
        } else {
//            cout << send_message.message << std::endl;
        }

//        EventMessage recive_meassage;
//        recive_meassage.prepare_recive("remote_fun", "return");

//        sleep(5);
    }
}