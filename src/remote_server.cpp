/**
 * Created by rrzhang on 2019/12/11.
 */


#include "remote_server.h"
#include <fcntl.h>      //! for struct flock
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>

#include "remote_string.h"
#include "handle_struct.h"
#include "handle_json.h"
#include "easylogger.h"

#include "handle_libevent.h"
#include "handle_grpc.h"

namespace remote {
    std::string RemoteServer::DoPwrite(std::string json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoPwrite().";
#endif
        auto *struct_ptr = new StructHandle::StructPwrite();
        {
            std::map<int, std::string> struct_map = JsonHandle::FromJson(json, (void *) struct_ptr
                                                                         , sizeof(StructHandle::StructPwrite));
            auto iter = struct_map.begin();
            struct_ptr->buf = new char[iter->second.length()];
            memcpy(struct_ptr->buf, iter->second.data(), iter->second.length());
        }
        std::cout << "before pwrite" << std::endl;
        ssize_t size = pwrite(struct_ptr->fd, struct_ptr->buf, struct_ptr->nbytes, struct_ptr->offset);
        std::cout << "after pwrite" << std::endl;
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        char path_buf[1024];
        GetPathByFd(struct_ptr->fd, path_buf);
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoPwrite file : " << path_buf << ", fd : " << struct_ptr->fd << ", size : "
                << struct_ptr->nbytes;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(size);
    }

    std::string RemoteServer::DoWrite(std::string json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoWrite().";
#endif
        auto *struct_ptr = new StructHandle::StructWrite();
        {
            std::map<int, std::string> struct_map = JsonHandle::FromJson(json, (void *) struct_ptr
                                                                         , sizeof(StructHandle::StructWrite));
            auto iter = struct_map.begin();
            struct_ptr->buf = new char[iter->second.length()];
            memcpy(struct_ptr->buf, iter->second.data(), iter->second.length());
        }
        ssize_t size = write(struct_ptr->fd, struct_ptr->buf, struct_ptr->nbytes);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        char path_buf[1024];
        GetPathByFd(struct_ptr->fd, path_buf);
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoWrite file : " << path_buf << ", fd : " << struct_ptr->fd << ", size : "
                << struct_ptr->nbytes;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(size);
    }

    std::string RemoteServer::DoFsync(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoFsync().";
#endif
        auto *struct_ptr = new StructHandle::StructFsync();
        {
            JsonHandle::FromJson(json, (void *) struct_ptr, sizeof(StructHandle::StructFsync));
        }
        int ret = fsync(struct_ptr->fd);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        char path_buf[1024];
        GetPathByFd(struct_ptr->fd, path_buf);
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoFsync file : " << path_buf << ", fd : " << struct_ptr->fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(ret);
    }

    std::string RemoteServer::DoLseek(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoLseek().";
#endif
        auto *struct_ptr = new StructHandle::StructLseek();
        {
            JsonHandle::FromJson(json, (void *) struct_ptr, sizeof(StructHandle::StructLseek));
        }
        off64_t offset = lseek(struct_ptr->fd, struct_ptr->offset, struct_ptr->whence);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        char path_buf[1024];
        GetPathByFd(struct_ptr->fd, path_buf);
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoLseek file : " << path_buf << ", fd : " << struct_ptr->fd << ", offset : " << offset;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(offset);
    }

    std::string RemoteServer::DoOpen1(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoOpen1().";
#endif
        auto *struct_ptr = new StructHandle::StructOpen();
        {
            std::map<int, std::string> struct_map = JsonHandle::FromJson(json, (void *) struct_ptr
                                                                         , sizeof(StructHandle::StructOpen));
            auto iter = struct_map.begin();
            struct_ptr->pathname = new char[iter->second.length() + 1];
            memcpy(struct_ptr->pathname, iter->second.data(), iter->second.length());
            struct_ptr->pathname[iter->second.length()] = '\0';
        }
        int fd = open(struct_ptr->pathname, struct_ptr->flag);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoOpen1 file : " << struct_ptr->pathname << ", fd : " << fd;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(fd);
    }


    std::string RemoteServer::DoOpen2(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoOpen2().";
#endif
        auto *struct_ptr = new StructHandle::StructOpen();
        {
            std::map<int, std::string> struct_map = JsonHandle::FromJson(json, (void *) struct_ptr
                                                                         , sizeof(StructHandle::StructOpen));
            auto iter = struct_map.begin();
            struct_ptr->pathname = new char[iter->second.length() + 1];
            memcpy(struct_ptr->pathname, iter->second.data(), iter->second.length());
            struct_ptr->pathname[iter->second.length()] = '\0';
        }

        int fd = open(struct_ptr->pathname, struct_ptr->flag, struct_ptr->mode);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoOpen2 file : " << struct_ptr->pathname << ", fd : " << fd;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return RemoteType::IntToString(fd);
    }

    std::string RemoteServer::DoStat(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoStat().";
#endif
        auto *struct_ptr = new StructHandle::StructStat();
        {
            std::map<int, std::string> struct_map = JsonHandle::FromJson(json, (void *) struct_ptr
                                                                         , sizeof(StructHandle::StructStat));
            auto iter = struct_map.begin();
            struct_ptr->pathname = new char[iter->second.length() + 1];
            memcpy(struct_ptr->pathname, iter->second.data(), iter->second.length());
            struct_ptr->pathname[iter->second.length()] = '\0';
        }
        // TODO 库函数
        int ret = 0;
        ret = stat(struct_ptr->pathname, &(struct_ptr->stat1));
        if(ret < 0){
            ret = errno;
        }
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoStat file : " << struct_ptr->pathname << ", ret : " << ret
                << ", errno : " << errno << ", ENOENT : " << ENOENT;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        {
            auto *return_struct_ptr = new StructHandle::StructHandle::StructReturnStat();
            return_struct_ptr->ret = ret;
            return_struct_ptr->stat1 = struct_ptr->stat1;
            std::map<int, std::string> struct_map;
            std::string return_json = JsonHandle::ToJson(kReturnStat, struct_map, return_struct_ptr
                                                         , sizeof(StructHandle::StructReturnStat));
            return return_json;
        }
    }

    std::string RemoteServer::DoClose(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoClose().";
#endif
        auto *struct_ptr = new StructHandle::StructClose();
        {
            JsonHandle::FromJson(json, (void *) struct_ptr, sizeof(StructHandle::StructClose));
        }
        int ret = close(struct_ptr->fd);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        char path_buf[1024];
        GetPathByFd(struct_ptr->fd, path_buf);
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoClose file : " << struct_ptr->fd << ", fd : " << struct_ptr->fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(ret);
    }

    std::string RemoteServer::DoFcntl1(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoFcntl1().";
#endif
        auto *struct_ptr = new StructHandle::StructFcntl();
        {
            JsonHandle::FromJson(json, (void *) struct_ptr, sizeof(StructHandle::StructFcntl));
        }
        int ret = fcntl(struct_ptr->fd, struct_ptr->cmd);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        char path_buf[1024];
        GetPathByFd(struct_ptr->fd, path_buf);
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoFcntl1 file : " << path_buf << ", fd : " << struct_ptr->fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(ret);
    }

    std::string RemoteServer::DoFcntl2(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoFcntl2().";
#endif
        auto *struct_ptr = new StructHandle::StructFcntl();
        {
            JsonHandle::FromJson(json, (void *) struct_ptr, sizeof(StructHandle::StructFcntl));
        }
        int ret = fcntl(struct_ptr->fd, struct_ptr->cmd, struct_ptr->arg);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        char path_buf[1024];
        GetPathByFd(struct_ptr->fd, path_buf);
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoFcntl2 file : " << path_buf << ", fd : " << struct_ptr->fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(ret);
    }

    // TODO : flock
    std::string RemoteServer::DoFcntl3(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoFcntl3().";
#endif
        auto *struct_ptr = new StructHandle::StructFcntl();
        {
            JsonHandle::FromJson(json, (void *) struct_ptr, sizeof(StructHandle::StructFcntl));
        }

        int ret = ::fcntl(struct_ptr->fd, struct_ptr->cmd, &(struct_ptr->lock));
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        char path_buf[1024];
        GetPathByFd(struct_ptr->fd, path_buf);
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoFcntl3 file : " << path_buf << ", fd : " << struct_ptr->fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(ret);
    }

    std::string RemoteServer::DoFallocate(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoFallocate().";
#endif
        auto *struct_ptr = new StructHandle::StructFallocate();
        {
            JsonHandle::FromJson(json, (void *) struct_ptr, sizeof(StructHandle::StructFallocate));
        }
        int ret = fallocate(struct_ptr->fd, struct_ptr->mode, struct_ptr->offset, struct_ptr->len);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        char path_buf[1024];
        GetPathByFd(struct_ptr->fd, path_buf);
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoFallocate file : " << path_buf << ", fd : " << struct_ptr->fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(ret);
    }

    // TODO : read buffer
    std::string RemoteServer::DoPread(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoPread().";
#endif
        auto *struct_ptr = new StructHandle::StructPread();
        {
            std::map<int, std::string> struct_map = JsonHandle::FromJson(
                    json, (void *) struct_ptr, sizeof(StructHandle::StructPread));
            auto iter = struct_map.begin();
            struct_ptr->buf = new char[iter->second.length()];
        }
        ssize_t size = pread(struct_ptr->fd, struct_ptr->buf, struct_ptr->nbytes, struct_ptr->offset);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        char path_buf[1024];
        GetPathByFd(struct_ptr->fd, path_buf);
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoPread file : " << path_buf << ", fd : " << struct_ptr->fd << ", size : " << size;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        {
            auto *return_struct_ptr = new StructHandle::StructReturnPread();
            std::map<int, std::string> return_struct_map;
            return_struct_ptr->size = size;
            return_struct_ptr->buf = struct_ptr->buf;
            return_struct_map.insert(std::make_pair((int64_t) &return_struct_ptr->buf - (int64_t) return_struct_ptr
                                                    , std::string(return_struct_ptr->buf, size)));
            std::string return_json = JsonHandle::ToJson(kReturnPread, return_struct_map, return_struct_ptr
                                                         , sizeof(StructHandle::StructReturnPread));
            return return_json;
        }
    }

    std::string RemoteServer::DoUnlink(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoUnlink().";
#endif
        auto *struct_ptr = new StructHandle::StructUnlink();
        {
            std::map<int, std::string> struct_map = JsonHandle::FromJson(json, (void *) struct_ptr
                                                                         , sizeof(StructHandle::StructUnlink));
            auto iter = struct_map.begin();
            struct_ptr->name = new char[iter->second.length() + 1];
            memcpy(struct_ptr->name, iter->second.data(), iter->second.length());
            struct_ptr->name[iter->second.length()] = '\0';
        }
        int ret = unlink(struct_ptr->name);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoUnlink file : " << struct_ptr->name << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(ret);
    }

    std::string RemoteServer::DoFtruncate(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoFtruncate().";
#endif
        auto *struct_ptr = new StructHandle::StructFtruncate();
        {
            JsonHandle::FromJson(json, (void *) struct_ptr, sizeof(StructHandle::StructFtruncate));
        }
        int ret = ftruncate(struct_ptr->fd, struct_ptr->length);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        char path_buf[1024];
        GetPathByFd(struct_ptr->fd, path_buf);
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoFtruncate file : " << path_buf << ", fd : " << struct_ptr->fd << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(ret);
    }

    std::string RemoteServer::DoRename(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoRename().";
#endif
        auto *struct_ptr = new StructHandle::StructRename();
        {
            std::map<int, std::string> struct_map = JsonHandle::FromJson(json, (void *) struct_ptr
                                                                         , sizeof(StructHandle::StructRename));
            auto iter = struct_map.begin();
            struct_ptr->__old = new char[iter->second.length() + 1];
            memcpy(struct_ptr->__old, iter->second.data(), iter->second.length());
            struct_ptr->__old[iter->second.length()] = '\0';
            iter++;
            struct_ptr->__new = new char[iter->second.length() + 1];
            memcpy(struct_ptr->__new, iter->second.data(), iter->second.length());
            struct_ptr->__new[iter->second.length()] = '\0';
        }
        int ret = rename(struct_ptr->__old, struct_ptr->__new);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoRename : " << struct_ptr->__old << " to " << struct_ptr->__new << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(ret);
    }

    std::string RemoteServer::DoMkdir(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoMkdir().";
#endif
        auto *struct_ptr = new StructHandle::StructMkdir();
        {
            std::map<int, std::string> struct_map = JsonHandle::FromJson(json, (void *) struct_ptr
                                                                         , sizeof(StructHandle::StructMkdir));
            auto iter = struct_map.begin();
            struct_ptr->path = new char[iter->second.length() + 1];
            memcpy(struct_ptr->path, iter->second.data(), iter->second.length());
            struct_ptr->path[iter->second.length()] = '\0';
        }
        int ret = mkdir(struct_ptr->path, struct_ptr->mode);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoMkdir : " << struct_ptr->path << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(ret);
    }

    std::string RemoteServer::DoRmdir(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoRmdir().";
#endif
        auto *struct_ptr = new StructHandle::StructRmdir();
        {
            std::map<int, std::string> struct_map = JsonHandle::FromJson(json, (void *) struct_ptr
                                                                         , sizeof(StructHandle::StructRmdir));
            auto iter = struct_map.begin();
            struct_ptr->path = new char[iter->second.length() + 1];
            memcpy(struct_ptr->path, iter->second.data(), iter->second.length());
            struct_ptr->path[iter->second.length()] = '\0';
        }
        int ret = rmdir(struct_ptr->path);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoRmdir : " << struct_ptr->path << ", ret : " << ret;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return std::to_string(ret);
    }


    bool IsDirectory(std::string path) {
        struct stat statinfo;
        stat(path.data(), &statinfo);
        if (S_ISDIR(statinfo.st_mode)) {
            return true;
        } else {
            return false;
        }
    };

    void DirWalk(StructHandle::Entry current, bool recursive, std::vector<StructHandle::Entry> *vp) {
        DIR *parent = opendir(current.m_path.c_str());
        dirent *dirent = nullptr;
        for (;;) {
            dirent = readdir(parent);
            if (dirent == nullptr) {
                closedir(parent);
                return;
            }
            if (strcmp(dirent->d_name, ".") == 0 ||
                strcmp(dirent->d_name, "..") == 0) {
                continue;
            }
            std::string path(current.m_path);
            if (path.back() != '/' && path.back() != '\\') {
                path += '/';
            }
            path.append(dirent->d_name);

            if (IsDirectory(path) && recursive) {
                DirWalk(StructHandle::Entry(path, current.m_depth + 1), recursive, vp);
            } else if (!IsDirectory(path)) {
                vp->push_back(StructHandle::Entry(path, current.m_depth + 1));
            }
        }
    };

    void PrintVector(std::vector<remote::StructHandle::Entry> entry) {
        for (auto iter : entry) {
            std::cout << "path : " << iter.m_path << ", depth : " << iter.m_depth << std::endl;
        }
    }

    std::string RemoteServer::DoOpendir(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::DoOpendir().";
#endif
        auto *struct_ptr = new StructHandle::StructOpendir();
        {
            std::map<int, std::string> struct_map = JsonHandle::FromJson(json, (void *) struct_ptr
                                                                         , sizeof(StructHandle::StructOpendir));
            auto iter = struct_map.begin();
            struct_ptr->basedir = new char[iter->second.length() + 1];
            memcpy(struct_ptr->basedir, iter->second.data(), iter->second.length());
            struct_ptr->basedir[iter->second.length()] = '\0';
        }
        StructHandle::PrintStructOpendir(struct_ptr);
        std::vector<StructHandle::Entry> vp;
        DirWalk(StructHandle::Entry(struct_ptr->basedir, 0), struct_ptr->recursive, &vp);
        //! TODO remove PrintVector
        PrintVector(vp);
#ifdef MULTI_MASTER_ZHANG_LOG_PATH
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[path] DoOpendir dir : " << struct_ptr->basedir;
#endif // MULTI_MASTER_ZHANG_LOG_PATH
        return JsonHandle::DirVectorToJson(vp);
    }

    std::string RemoteServer::SelectDoCall(const std::string &json) const {
#ifdef MULTI_MASTER_ZHANG_LOG_FUN
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "[fun ] RemoteServer::SelectDoCall().";
#endif
        //! TODO 整理json
#ifdef MULTI_MASTER_ZHANG_DEBUG_JSON
        std::cout << "recive json string : " << json << std::endl;
#endif // MULTI_MASTER_ZHANG_DEBUG_JSON
        std::string fun_name = JsonHandle::FunctionNameFromJson(json);
        std::string ret;
        if (fun_name == kPwrite) {
            ret = DoPwrite(json);
        } else if (fun_name == kWrite) {
            ret = DoWrite(json);
        } else if (fun_name == kFsync) {
            ret = DoFsync(json);
        } else if (fun_name == kLseek) {
            ret = DoLseek(json);
        } else if (fun_name == kOpen1) {
            ret = DoOpen1(json);
        } else if (fun_name == kOpen2) {
            ret = DoOpen2(json);
        } else if (fun_name == kStat) {
            ret = DoStat(json);
        } else if (fun_name == kClose) {
            ret = DoClose(json);
        } else if (fun_name == kFcntl1) {
            ret = DoFcntl1(json);
        } else if (fun_name == kFcntl2) {
            ret = DoFcntl2(json);
        } else if (fun_name == kFcntl3) {
            ret = DoFcntl3(json);
        } else if (fun_name == kFallocate) {
            ret = DoFallocate(json);
        } else if (fun_name == kPread) {
            ret = DoPread(json);
        } else if (fun_name == kUnlink) {
            ret = DoUnlink(json);
        } else if (fun_name == kFtruncate) {
            ret = DoFtruncate(json);
        } else if (fun_name == kRename) {
            ret = DoRename(json);
        } else if (fun_name == kMkdir) {
            ret = DoMkdir(json);
        } else if (fun_name == kRmdir) {
            ret = DoRmdir(json);
        } else if (fun_name == kOpendir) {
            ret = DoOpendir(json);
        }
        return ret;
    }

    RemoteServer::RemoteServer() {
        //! TODO 整理格式
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "RemoteServer::RemoteServer().";
#ifdef REMOTE_FUN_WITH_LIBEENT
        server_net_handle_ = new ServerLibeventHandle(this);
#endif // REMOTE_FUN_WITH_LIBEENT
#ifdef REMOTE_FUN_WITH_GRPC
        server_net_handle_ = new ServerGprcHandle(this);
#endif // REMOTE_FUN_WITH_GRPC
        char path[512];

        char *p = getcwd(path, 512);
        std::cout << "getcwd : " << path << std::endl;

//        getcurdir( 0, char *directory);

        int ret = chdir("/home/zhangrongrong/mysql/data");
        std::cout << "current path ret : " << ret << std::endl;

        p = getcwd(path, 512);
        std::cout << "getcwd : " << path << std::endl;
    }

    RemoteServer::~RemoteServer() {
        EasyLoggerWithTrace("/home/zhangrongrong/LOG_REMOTE_SERVER", EasyLogger::info).force_flush()
                << "RemoteServer::~RemoteServer().";
    }

    ServerNetHandle *RemoteServer::GetServerNetHandle() const {
        return server_net_handle_;
    }
}