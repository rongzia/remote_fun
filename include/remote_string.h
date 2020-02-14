/**
 * Created by rrzhang on 2019/12/11.
 */

#ifndef MULTI_MASTER_REMOTE_STRING_H
#define MULTI_MASTER_REMOTE_STRING_H

#include <cstring>
#include <unistd.h>

namespace remote {
    //! kPwrite2、stop 测试用
    static const std::string kPwrite2 = "pwrite2";
    static const std::string kStop = "stop";

    static const std::string kPwrite = "pwrite";
    static const std::string kWrite = "write";
    static const std::string kFsync = "fsync";
    static const std::string kLseek = "lseek";
    static const std::string kOpen1 = "open1";
    static const std::string kOpen2 = "open2";
    static const std::string kStat = "stat";
    static const std::string kReturnStat = "returnstat";
    static const std::string kClose = "close";
    static const std::string kFcntl1 = "fcntl";
    static const std::string kFcntl2 = "fcntl2";
    static const std::string kFcntl3 = "fcntl3";
    static const std::string kFallocate = "fallocate";
    static const std::string kPread = "pread";
    static const std::string kReturnPread = "pread";
    static const std::string kUnlink = "unlink";
    static const std::string kFtruncate = "ftruncate";
    static const std::string kRename = "rename";
    static const std::string kMkdir = "mkdir";
    static const std::string kRmdir = "rmdir";
    static const std::string kOpendir = "opendir";

    static const std::string path_log_server = "/home/zhangrongrong/LOG_REMOTE_SERVER";
    static const std::string path_log_client = "/home/zhangrongrong/LOG_REMOTE_CLIENT";


    class JsonKeyName {
    public:
        static std::string NamePtrKeys(std::string k_name) {
            return k_name + "_keys";
        }

        static std::string NameOffsets(std::string k_name, int64_t offset) {
            return k_name + "_" + std::to_string(offset);
        }

        static std::string NameStruct(std::string k_name) {
            return std::string("") + "struct_" + k_name;
        }

        static std::string NamePtrs(std::string k_name) {
            return k_name + "_ptrs";
        }
    };

    static int GetPathByFd(int fd, char *buf) {
        char path[1024];
        memset(path, 0, 1024);
        snprintf(path, 1024, "/proc/%ld/fd/%d", (long) getpid(), fd);
        int ret = readlink(path, buf, 1024);
        return ret;
    }

    static std::string GetPathByFd(int fd) {
        char path[1024];
        memset(path, 0, 1024);
        snprintf(path, 1024, "/proc/%ld/fd/%d", (long) getpid(), fd);
        char buf[1024];
        memset(buf, 0, 1024);
        int ret = readlink(path, buf, 1024);
        return std::string(buf);
    }
}
#endif //MULTI_MASTER_REMOTE_STRING_H
