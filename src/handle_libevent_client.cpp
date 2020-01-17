//
// Created by rrzhang on 2019/12/24.
//
#include "handle_libevent.h"
#include "libevent_handle.h"     //! for eventHandle_


namespace remote {

    ClientLibEventHandle::ClientLibEventHandle(const std::string &connect_to_addr, const std::string &connect_to_port
                                               , const std::string &listen_port)
            : connect_to_addr_(connect_to_addr)
              , connect_to_port_(std::stoi(connect_to_port))
              , listen_port_(std::stoi(listen_port)) {
        Init();
    }

    int ClientLibEventHandle::Init() {
        eventHandle_ = new LibeventHandle();

        if (!eventHandle_->init_handle(listen_port_)) {
            EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                    << "init error";
        }
        conn_id_ = eventHandle_->get_connection_id(connect_to_addr_.data(), connect_to_port_, true);
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "connect to: " << connect_to_addr << ":" << connect_to_port << ", listen port : " << listen_port_;

        //! return 0, represent success.
        return 0;
    }

    std::string ClientLibEventHandle::Send(const std::string &json) {
        //! 先发送，并等待接收
        bool ret = eventHandle_->send(conn_id_, json.data(), json.length());

#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "client send size:" << json.length();
#endif // MULTI_MASTER_ZHANG_LOG_FUN
#ifdef MULTI_MASTER_ZHANG_LOG_JSON
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "client send content:" << json;
#endif // MULTI_MASTER_ZHANG_LOG_JSON

        //! 接收
        memset(buffer_, 0, sizeof(char) * 10496000);
        //! std::cout << "[remote fun] recived buffer size : " << eventHandle_->get_recive_buffer_length(conn_id_) << std::endl;
        int buffer_len = eventHandle_->wait_recive(conn_id_, buffer_);

#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "client recive size:" << buffer_len;
#endif // MULTI_MASTER_ZHANG_LOG_FUN
#ifdef MULTI_MASTER_ZHANG_LOG_JSON
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "client recive content:" << buffer_;
#endif // MULTI_MASTER_ZHANG_LOG_JSON

        return std::string(buffer_, buffer_len);
    }
}