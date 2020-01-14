/**
 * Created by rrzhang on 2019/12/11.
 */


#include "remote_client.h"
#include <dirent.h>
#include <c++/7.4.0/vector>
#include "remote_type.h"
#include "remote_string.h"
#include "handle_struct.h"
#include "handle_json.h"
#include "easylogger.h"

#include "handle_libevent.h"
#include "handle_grpc.h"

namespace remote {


    ssize_t RemoteClient::remote_pwrite(int fd, const void *buf, size_t nbytes, off64_t offset) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_pwrite()";
#endif
        auto *struct_ptr = new StructHandle::StructPwrite();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->fd = fd;
            struct_ptr->buf = (char *) buf;
//            struct_ptr->buf = new char[1];
            struct_ptr->nbytes = nbytes;
            struct_ptr->offset = offset;
            struct_map.insert(std::make_pair((int64_t) &struct_ptr->buf - (int64_t) struct_ptr,
                                             std::string(struct_ptr->buf, nbytes)));
        }
        std::string json = JsonHandle::ToJson(kPwrite, struct_map, struct_ptr, sizeof(StructHandle::StructPwrite));
        mtx.lock();
        int size = RemoteType::StringToSSize(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_pwrite fd : " << fd << ", size : " << size;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return size;
    }

    ssize_t RemoteClient::remote_write(int fd, const void *buf, size_t nbytes) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_write()";
#endif
        auto *struct_ptr = new StructHandle::StructWrite();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->fd = fd;
            struct_ptr->buf = (char *) buf;
            struct_ptr->nbytes = nbytes;
            struct_map.insert(std::make_pair((int64_t) &struct_ptr->buf - (int64_t) struct_ptr,
                                             std::string(struct_ptr->buf, nbytes)));
        }
        std::string json = JsonHandle::ToJson(kWrite, struct_map, struct_ptr, sizeof(StructHandle::StructWrite));
        mtx.lock();
        int size = RemoteType::StringToSSize(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_write fd : " << fd << ", size : " << size;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return size;
    }


    int RemoteClient::remote_fsync(int fd) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_fsync()";
#endif
        auto *struct_ptr = new StructHandle::StructFsync();
        std::map<int, std::string> struct_map;
        { struct_ptr->fd = fd; }

        std::string json = JsonHandle::ToJson(kFsync, struct_map, struct_ptr, sizeof(StructHandle::StructFsync));
        mtx.lock();
        int ret = RemoteType::StringToSSize(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_fsync fd : " << fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return ret;
    }

    off64_t RemoteClient::remote_lseek(int fd, off64_t offset, int whence) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_lseek()";
#endif
        auto *struct_ptr = new StructHandle::StructLseek();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->fd = fd;
            struct_ptr->offset = offset;
            struct_ptr->whence = whence;
        }
        std::string json = JsonHandle::ToJson(kLseek, struct_map, struct_ptr, sizeof(StructHandle::StructLseek));
        mtx.lock();
        int return_offset = RemoteType::StringToSSize(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_lseek fd : " << fd << ", off : " << return_offset;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return return_offset;
    }

    bool remote_open_except(const char *path) {
        std::string path1("/home/zhangrongrong/mysql/local/mysql80/share/english/errmsg.sys");
        std::string path2("/home/zhangrongrong/mysql/local/mysql80/share/charsets/Index.xml");
        if (0 == strncmp(std::string(path).data(), path1.data(), path1.length())
            || 0 == strncmp(std::string(path).data(), path2.data(), path2.length())
                ) {
            return true;
        }
        return false;
    }

    int RemoteClient::remote_open(const char *pathname, int flag) {
//      if (true == remote_open_except(pathname)) {
//#ifdef MULTI_MASTER_ZHANG_LOG_PATH
//        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
//                << "open1 file : " << pathname << ", open fd: " << "-2";
//#endif // MULTI_MASTER_ZHANG_LOG_PATH
//        return -2;
//      }
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_open1()";
#endif
        auto *struct_ptr = new StructHandle::StructOpen();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->pathname = (char *) pathname;
            struct_ptr->flag = flag;
            struct_map.insert(std::make_pair((int64_t) &struct_ptr->pathname - (int64_t) struct_ptr,
                                             std::string(struct_ptr->pathname, strlen(struct_ptr->pathname))));
        }
        std::string json = JsonHandle::ToJson(kOpen1, struct_map, struct_ptr, sizeof(StructHandle::StructOpen));
        mtx.lock();
        int fd = RemoteType::StringToInt(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_open1 file : " << pathname << ", fd : " << fd;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return fd;
    }

    int RemoteClient::remote_open(const char *pathname, int flag, mode_t mode) {
//      if (true == remote_open_except(pathname)) {
//#ifdef MULTI_MASTER_ZHANG_LOG_PATH
//        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
//                << "open1 file : " << pathname << ", open fd : " << "-2";
//#endif  // MULTI_MASTER_ZHANG_LOG_PATH
//        return -2;
//      }
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_open2()";
#endif
        auto *struct_ptr = new StructHandle::StructOpen();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->pathname = (char *) pathname;
            struct_ptr->flag = flag;
            struct_ptr->mode = mode;
            struct_map.insert(std::make_pair((int64_t) &struct_ptr->pathname - (int64_t) struct_ptr,
                                             std::string(struct_ptr->pathname, strlen(struct_ptr->pathname))));
        }
        std::string json = JsonHandle::ToJson(kOpen2, struct_map, struct_ptr, sizeof(StructHandle::StructOpen));
        mtx.lock();
        int fd = RemoteType::StringToInt(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_open2 file : " << pathname << ", fd : " << fd;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return fd;
    }

    int RemoteClient::remote_stat(const char *pathname, struct stat *buf, int *remote_errno) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_stat()";
#endif
        auto *struct_ptr = new StructHandle::StructStat();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->pathname = (char *) pathname;
            struct_map.insert(std::make_pair((int64_t) &struct_ptr->pathname - (int64_t) struct_ptr,
                                             std::string(struct_ptr->pathname, strlen(struct_ptr->pathname))));
        }

        std::string json = JsonHandle::ToJson(kStat, struct_map, struct_ptr, sizeof(StructHandle::StructStat));
        mtx.lock();
        std::string return_json = client_net_handle_->Send(json);
        mtx.unlock();
        {
            auto *return_struct_ptr = new StructHandle::StructReturnStat();
            JsonHandle::FromJson(return_json, return_struct_ptr, sizeof(StructHandle::StructReturnStat));
            *buf = return_struct_ptr->stat1;
            *remote_errno = return_struct_ptr->return_remote_errno;
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
            EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                    << "[path] remote_stat file : " << pathname << ", ret : " << return_struct_ptr->ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
            return return_struct_ptr->ret;
        }
    }

    int RemoteClient::remote_close(int fd) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_close()";
#endif
        auto *struct_ptr = new StructHandle::StructClose();
        std::map<int, std::string> struct_map;
        struct_ptr->fd = fd;

        std::string json = JsonHandle::ToJson(kClose, struct_map, struct_ptr, sizeof(StructHandle::StructClose));
        mtx.lock();
        int ret = RemoteType::StringToInt(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_close fd : " << fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return ret;
    }

    int RemoteClient::remote_fcntl(int fd, int cmd)  {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_fcntl1()";
#endif
        auto *struct_ptr = new StructHandle::StructFcntl();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->fd = fd;
            struct_ptr->cmd = cmd;
        }
        std::string json = JsonHandle::ToJson(kFcntl1, struct_map, struct_ptr, sizeof(StructHandle::StructFcntl));
        mtx.lock();
        int ret = RemoteType::StringToInt(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_fcntl1 fd : " << fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return ret;
    }

    int RemoteClient::remote_fcntl(int fd, int cmd, long arg) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_fcntl2()";
#endif
        auto *struct_ptr = new StructHandle::StructFcntl();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->fd = fd;
            struct_ptr->cmd = cmd;
            struct_ptr->arg = arg;
        }

        std::string json = JsonHandle::ToJson(kFcntl2, struct_map, struct_ptr, sizeof(StructHandle::StructFcntl));
        mtx.lock();
        int ret = RemoteType::StringToInt(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_fcntl2 fd : " << fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return ret;
    }


    int RemoteClient::remote_fcntl(int fd, int cmd, ::flock *lock) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_fcntl3()";
#endif
        auto *struct_ptr = new StructHandle::StructFcntl();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->fd = fd;
            struct_ptr->cmd = cmd;
            struct_ptr->lock.l_type = lock->l_type;
            struct_ptr->lock.l_whence = lock->l_whence;
            struct_ptr->lock.l_start = lock->l_start;
            struct_ptr->lock.l_len = lock->l_len;
        }

        std::string json = JsonHandle::ToJson(kFcntl3, struct_map, struct_ptr, sizeof(StructHandle::StructFcntl));
        mtx.lock();
        int ret = RemoteType::StringToInt(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_fcntl3 fd : " << fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return ret;
    }

    int RemoteClient::remote_fallocate(int fd, int mode, off64_t offset, off64_t len) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_fallocate()";
#endif
        auto *struct_ptr = new StructHandle::StructFallocate();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->fd = fd;
            struct_ptr->mode = mode;
            struct_ptr->offset = offset;
            struct_ptr->len = len;
        }

        std::string json = JsonHandle::ToJson(kFallocate, struct_map, struct_ptr,
                                              sizeof(StructHandle::StructFallocate));
        mtx.lock();
        int ret = RemoteType::StringToInt(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_fallocate fd : " << fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return ret;
    }

    ssize_t RemoteClient::remote_pread(int fd, void *buf, size_t nbytes, off64_t offset) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_pread()";
#endif
        auto *struct_ptr = new StructHandle::StructPread();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->fd = fd;
//            struct_ptr->buf = (char *) buf;
            struct_ptr->buf = new char[1];
            struct_ptr->nbytes = nbytes;
            struct_ptr->offset = offset;
//            struct_map.insert(std::make_pair((int64_t) &struct_ptr->buf - (int64_t) struct_ptr,
//                                             std::string(struct_ptr->buf, nbytes)));
        }

        std::string json = JsonHandle::ToJson(kPread, struct_map, struct_ptr, sizeof(StructHandle::StructPread));
        mtx.lock();
        std::string return_json = client_net_handle_->Send(json);
        mtx.unlock();
        {
            auto *return_struct_ptr = new StructHandle::StructReturnPread();
            std::map<int, std::string> return_struct_map = JsonHandle::FromJson(
                    return_json, return_struct_ptr, sizeof(StructHandle::StructReturnPread));
            auto iter = return_struct_map.begin();
            return_struct_ptr->buf = new char[iter->second.length()];
            memcpy(return_struct_ptr->buf, iter->second.data(), iter->second.length());
            memcpy(buf, iter->second.data(), iter->second.length());
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
            EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                    << "[path] remote_pread fd : " << fd << ", size : " << return_struct_ptr->size;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
            return return_struct_ptr->size;
        }
    }


    int RemoteClient::remote_unlink(const char *name) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_unlink()";
#endif
        auto *struct_ptr = new StructHandle::StructUnlink();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->name = (char *) name;
            struct_map.insert(std::make_pair((int64_t) &struct_ptr->name - (int64_t) struct_ptr,
                                             std::string(struct_ptr->name, strlen(struct_ptr->name))));
        }
        std::string json = JsonHandle::ToJson(kUnlink, struct_map, struct_ptr, sizeof(StructHandle::StructUnlink));
        mtx.lock();
        int ret = RemoteType::StringToInt(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_unlink file : " << name << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return ret;
    }

    int RemoteClient::remote_ftruncate(int fd, off64_t length) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_ftruncate()";
#endif
        auto *struct_ptr = new StructHandle::StructFtruncate();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->fd = fd;
            struct_ptr->length = length;
        }
        std::string json = JsonHandle::ToJson(kFtruncate, struct_map, struct_ptr,
                                              sizeof(StructHandle::StructFtruncate));
        mtx.lock();
        int ret = RemoteType::StringToInt(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_ftruncate fd : " << fd << ", ret: " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return ret;
    }

    int RemoteClient::remote_rename(const char *__old, const char *__new) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_rename()";
#endif
        auto *struct_ptr = new StructHandle::StructRename();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->__old = (char *) __old;
            struct_ptr->__new = (char *) __new;
            struct_map.insert(std::make_pair((int64_t) &struct_ptr->__old - (int64_t) struct_ptr,
                                             std::string((char *) struct_ptr->__old, strlen(struct_ptr->__old))));
            struct_map.insert(std::make_pair((int64_t) &struct_ptr->__new - (int64_t) struct_ptr,
                                             std::string((char *) struct_ptr->__new, strlen(struct_ptr->__new))));
        }
        std::string json = JsonHandle::ToJson(kRename, struct_map, struct_ptr, sizeof(StructHandle::StructRename));
        mtx.lock();
        int ret = RemoteType::StringToInt(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_rename : " << __old << " to " << __new << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return ret;
    }

    int RemoteClient::remote_mkdir(const char *path, mode_t mode) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_mkdir()";
#endif
        auto *struct_ptr = new StructHandle::StructMkdir();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->path = (char *) path;
            struct_ptr->mode = mode;
            struct_map.insert(std::make_pair((int64_t) &struct_ptr->path - (int64_t) struct_ptr,
                                             std::string((char *) struct_ptr->path, strlen(struct_ptr->path))));
        }
        std::string json = JsonHandle::ToJson(kMkdir, struct_map, struct_ptr, sizeof(StructHandle::StructMkdir));
        mtx.lock();
        int ret = RemoteType::StringToInt(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_mkdir : " << path << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return ret;
    }

    int RemoteClient::remote_rmdir(const char *path) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_rmdir()";
#endif
        auto *struct_ptr = new StructHandle::StructRmdir();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->path = (char *) path;
            struct_map.insert(std::make_pair((int64_t) &struct_ptr->path - (int64_t) struct_ptr,
                                             std::string(struct_ptr->path, strlen(struct_ptr->path))));
        }
        std::string json = JsonHandle::ToJson(kRmdir, struct_map, struct_ptr, sizeof(StructHandle::StructRmdir));
        mtx.lock();
        int ret = RemoteType::StringToInt(client_net_handle_->Send(json));
        mtx.unlock();
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_rmdir : " << path << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return ret;
    }


    std::vector<StructHandle::Entry> RemoteClient::remote_opendir(const std::string &basedir, bool recursive) {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::remote_opendir()";
#endif
        auto *struct_ptr = new StructHandle::StructOpendir();
        std::map<int, std::string> struct_map;
        {
            struct_ptr->basedir = (char *) basedir.data();
            struct_ptr->recursive = recursive;
            struct_map.insert(std::make_pair((int64_t) &struct_ptr->basedir - (int64_t) struct_ptr,
                                             std::string(struct_ptr->basedir, strlen(struct_ptr->basedir))));
        }
        std::string json = JsonHandle::ToJson(kOpendir, struct_map, struct_ptr, sizeof(StructHandle::StructOpendir));
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[path] remote_opendir dir : " << basedir;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        mtx.lock();
        std::vector<StructHandle::Entry> v = JsonHandle::DirVectorFromJson(client_net_handle_->Send(json));
        mtx.unlock();
        return v;
    }


    RemoteClient::RemoteClient(const std::string &connect_to_addr, const std::string &connect_to_port,
                               const std::string &listen_port) {
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "RemoteClient::RemoteClient()";

#ifdef REMOTE_FUN_WITH_LIBEVENT
        client_net_handle_ = new ClientLibEventHandle(connect_to_addr, connect_to_port, listen_port);
#endif
#ifdef REMOTE_FUN_WITH_GRPC
        client_net_handle_ = new ClientGrpcHandle(connect_to_addr, connect_to_port);
#endif
    }

    RemoteClient::~RemoteClient() {
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_CLIENT", EasyLogger::info).force_flush()
                << "[fun ] RemoteClient::~RemoteClient().";
    }
}