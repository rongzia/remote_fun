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
#ifdef MULTI_MASTER_ZHANG_DEBUG_FUN
      std::cout << "ClientLibEventHandle::ClientLibEventHandle()." << std::endl;
#endif // MULTI_MASTER_ZHANG_DEBUG_FUN
        Init();
    }

    int ClientLibEventHandle::Init() {
        eventHandle_ = new LibeventHandle();

        if (!eventHandle_->init_handle(listen_port_)) {
            std::cout << "init error" << std::endl;
        }
        conn_id_ = eventHandle_->get_connection_id(connect_to_addr_.data(), connect_to_port_, true);
        std::cout << "connect to : " << connect_to_addr_ << ":" << connect_to_port_
        << ", conn_id : " << conn_id_ << ", listen port : " << listen_port_ << std::endl;
        //! return 0, represent success.
        return 0;
    }

    std::string ClientLibEventHandle::Send(const std::string &json) const {
        bool ret = eventHandle_->send(conn_id_, json.data(), json.length());
        // TODO
        std::cout << "libevent send " << (ret ? "success." : "fail.") << " send size " << json.length() << std::endl;

        // recive
        char buffer[1049600];
        memset(buffer, 0, 1049600);
//        std::cout << "recived buffer size : " << eventHandle_->get_recive_buffer_length(conn_id_) << std::endl;
        int buffer_len = eventHandle_->wait_recive(conn_id_, buffer);
//        std::cout << "libevent recive success. recive size " << buffer_len << ", recive content : " << buffer << std::endl;
        return std::string(buffer, buffer_len);
    }
}