/**
 * Created by rrzhang on 2019/11/29.
 */

#ifndef MULTI_MASTER_REMOTE_CLIENT_H
#define MULTI_MASTER_REMOTE_CLIENT_H

#include "remote_type.h"
#include <vector>
#include <thread>
#include <mutex>
#include "handle_struct.h"

struct flock;
//ssize_t remote_pwrite(int fd, const void *buf, size_t nbytes, off64_t offset);
//int remote_fsync(int fd);
//off64_t remote_lseek(int fd, off64_t offset, int whence);
//int remote_open(char *pathname, int flag);
//int remote_open(char *pathname, int flag, mode_t mode);
//!int remote_stat(const char *pathname, struct stat *buf);
//int remote_close(int fd);
//int remote_fcntl(int fd, int cmd);
//int remote_fcntl(int fd, int cmd, long arg);
//int remote_fcntl(int fd, int cmd, struct flock *lock);
//int remote_fallocate(int fd, int mode, off64_t offset, off64_t len);
//ssize_t remote_pread(int fd, void *buf, size_t nbytes, off64_t offset);
//int remote_unlink(const char *name);
//int remote_ftruncate(int fd, off64_t length);
//int remote_rename(const char *__old, const char *__new);
//int remote_mkdir(const char *path, mode_t mode);

namespace remote {

    class ClientNetHandle;

    class RemoteClient {
    public:
        //! TODO : 以下两个个函数测试用，后期删除
        void remote_stop();
        /*friend*/ ssize_t remote_pwrite2(int no, int fd, const void *buf, size_t nbytes, off64_t offset);

        /*friend*/ ssize_t remote_pwrite(int fd, const void *buf, size_t nbytes, off64_t offset);

        ssize_t remote_write(int fd, const void *buf, size_t nbytes);

        /*friend*/ int remote_fsync(int fd);

        /*friend*/ off64_t remote_lseek(int fd, off64_t offset, int whence);

        /*friend*/ int remote_open(const char *pathname, int flag);

        /*friend*/ int remote_open(const char *pathname, int flag, mode_t mode);

        /*friend*/ int remote_stat(const char *pathname, struct stat *buf, int *remote_errno);

        /*friend*/ int remote_close(int fd);

        /*friend*/ int remote_fcntl(int fd, int cmd);

        /*friend*/ int remote_fcntl(int fd, int cmd, long arg);

        /*friend*/ int remote_fcntl(int fd, int cmd, flock *lock);

        //! 文件打洞
        /*friend*/ int remote_fallocate(int fd, int mode, off64_t offset, off64_t len);

        /*friend*/ ssize_t remote_pread(int fd, void *buf, size_t nbytes, off64_t offset) ;

        /*friend*/ int remote_unlink(const char *name);

        //! 改变文件大小
        /*friend*/ int remote_ftruncate(int fd, off64_t length);

        /*friend*/ int remote_rename(const char *__old, const char *__new);

        /*friend*/ int remote_mkdir(const char *path, mode_t mode);

        int remote_rmdir(const char *path);

        std::vector<StructHandle::Entry> remote_opendir(const std::string &basedir, bool recursive);

        explicit RemoteClient(const std::string &connect_to_addr, const std::string &connect_to_port, const std::string &listen_port);

        ~RemoteClient();

        RemoteClient(const RemoteClient &) = delete;

        RemoteClient &operator=(const RemoteClient &) = delete;

    private:
        ClientNetHandle *client_net_handle_;
        std::mutex mtx;
    };


} // namespace remote

#endif // MULTI_MASTER_REMOTE_CLIENT_H
