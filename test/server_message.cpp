#include "event_mess_handle.h"

using namespace std;


void server_recive_cb(EventMessageHandle *mess_handle, EventMessage *mess, void *arg) {
    EventMessage return_message;
    cout << "[remote fun] revice : " << std::string(mess->message, mess->message_size) << endl;
    return_message.prepare_send("remote_fun", "return", mess->send_host_name
                                , "yes", 3);
    int return_code = mess_handle->sendMessage(&return_message);

    if (return_code < 0) {
            cout << "send error" << std::endl;
            cout << MessageError::getEventErrorStr(return_message.error_no) << endl;
        } else {
//            cout << return_message.message << std::endl;
        }

    cout << "return : yes" << endl;
}

int main(void) {
    EventMessageHandle handler;

    string host_config_path = "/home/zhangrongrong/CLionProjects/Percona-Share-Storage/percona-server/extra/multi_master/remote_fun/test/host_config2.json";
    string mess_config_path = "/home/zhangrongrong/CLionProjects/Percona-Share-Storage/percona-server/extra/multi_master/remote_fun/test/mess_config.json";
    handler.init_handle(host_config_path.c_str(), mess_config_path.c_str());

//    handler.register_recive_handler("remote_fun", "call", server_recive_cb, NULL);

    cout << "Message Server Test" << endl;
    //cout << "---------- Message Callback Recive Test ------------" << endl;

    int count = 0;
    int return_code = 0;
    while (1) {
//        cout << "---------- Message Constantly Recive Test ------------" << endl;
//        EventMessage revice_message;
//        revice_message.prepare_recive("remote_fun", "call");
//        handler.readMessage(&revice_message);



//        if (return_code < 0) {
//            cout << "send error" << std::endl;
//            cout << MessageError::getEventErrorStr(return_message.error_no) << endl;
//        } else {
//            cout << return_message.message << std::endl;
//        }

        cout << "Message Count : " << handler.get_unprocessed_message_count("remote_fun", "call") << endl;
        sleep(5);
    }
}