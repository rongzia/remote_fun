/**
 * Created by rrzhang on 2019/12/11.
 */

#ifndef MULTI_MASTER_REMOTE_SERVER_H
#define MULTI_MASTER_REMOTE_SERVER_H
#define _FILE_OFFSET_BITS 64
#include "remote_type.h"
#include "easylogger.h"
//#include <fcntl.h>      //! for struct flock

/**
 * remote 函数声明里的类型和原型保持一致，转成 string 时通过包装的转换函数进行转化
 */

namespace remote {
    class ServerNetHandle;

    class RemoteServer {
    public:

        std::string DoPwrite(std::string json);

        std::string DoWrite(std::string json);

        std::string DoFsync(const std::string &json);

        std::string DoLseek(const std::string &json);

        std::string DoOpen1(const std::string &json);

        std::string DoOpen2(const std::string &json);

        std::string DoStat(const std::string &json);

        std::string DoClose(const std::string &json);

        std::string DoFcntl1(const std::string &json);

        std::string DoFcntl2(const std::string &json);

        std::string DoFcntl3(const std::string &json);

        std::string DoFallocate(const std::string &json);

        std::string DoPread(const std::string &json);

        std::string DoUnlink(const std::string &json);

        std::string DoFtruncate(const std::string &json);

        std::string DoRename(const std::string &json);

        std::string DoMkdir(const std::string &json);

        std::string DoRmdir(const std::string &json);

        std::string DoOpendir(const std::string &json);

        std::string SelectDoCall(const std::string &json);

        ServerNetHandle *GetServerNetHandle() const;

        explicit RemoteServer(int );

        ~RemoteServer();

        RemoteServer(const RemoteServer &) = delete;

        RemoteServer &operator=(const RemoteServer &) = delete;

    private:
        int listen_port_;
        ServerNetHandle *server_net_handle_;
        ssize_t mysql_ibd_bytes_count_;
        int mysql_ibd_count_;
    };


}

#endif //MULTI_MASTER_REMOTE_SERVER_H
