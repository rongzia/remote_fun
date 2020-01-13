//
// Created by rrzhang on 2019/12/24.
//
#include "handle_libevent.h"
#include "libevent_handle.h"     //! for eventHandle_


namespace remote {

    ClientLibEventHandle::ClientLibEventHandle(const std::string &connect_to_addr, const std::string &connect_to_port, const std::string &listen_port)
            : connect_to_addr_(connect_to_addr)
              , connect_to_port_(std::stoi(connect_to_port))
              , listen_port_(std::stoi(listen_port)){
        Init();
    }

    int ClientLibEventHandle::Init() {
        eventHandle_ = new LibeventHandle();

        if (!eventHandle_->init_handle(listen_port_)) {
            std::cout << "[remote fun] init error" << std::endl;
        }
        conn_id_ = eventHandle_->get_connection_id(connect_to_addr_.data(), connect_to_port_, true);
        std::cout << "[remote fun] connect to : " << connect_to_addr_ << ":" << connect_to_port_
        << ", conn_id : " << conn_id_ << ", listen port : " << listen_port_ << std::endl;
        //! return 0, represent success.
        return 0;
    }

    std::string ClientLibEventHandle::Send(const std::string &json) {
        bool ret = eventHandle_->send(conn_id_, json.data(), json.length());
        // TODO
//        std::cout << "[remote fun] libevent send " << (ret ? "success." : "fail.") << " send size " << json.length() << std::endl;

        // recive
        memset(buffer_, 0, sizeof(char) * 10496000);
//        std::cout << "[remote fun] recived buffer size : " << eventHandle_->get_recive_buffer_length(conn_id_) << std::endl;
        int buffer_len = eventHandle_->wait_recive(conn_id_, buffer_);
//        std::cout << "[remote fun] libevent recive success. recive size " << buffer_len << ", recive content : " << buffer_ << std::endl;
        return std::string(buffer_, buffer_len);
    }
}