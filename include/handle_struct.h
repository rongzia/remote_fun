/**
 * Created by rrzhang on 2019/12/9.
 */

#ifndef MULTI_MASTER_HANDLE_STRUCT_H
#define MULTI_MASTER_HANDLE_STRUCT_H

#include "remote_type.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

struct flock;
struct stat;
struct timespec;

namespace remote {

    class StructHandle {
    public:
        struct StructPwrite {
            int fd;
            char *buf;
            size_t nbytes;
            off64_t offset;
        };
        struct StructWrite {
            int fd;
            char *buf;
            size_t nbytes;
        };
        struct StructFsync {
            int fd;
        };
        struct StructLseek {
            int fd;
            off64_t offset;
            int whence;
        };
        //! TODO : 还共有两个重载函数
        struct StructOpen {
            char *pathname;
            int flag;
            mode_t mode;
        };
        struct StructStat {
            char *pathname;
            struct stat stat1;
        };
        struct StructReturnStat {
            int ret;
            struct stat stat1;
        };
        struct StructClose {
            int fd;
        };
        //! TODO : 共有三个重载函数
        struct StructFcntl {
            int fd;
            int cmd;
            long arg;
            ::flock lock;
        };
        struct StructFallocate {
            int fd;
            int mode;
            off64_t offset;
            off64_t len;
        };
        struct StructPread {
            int fd;
            char *buf;
            size_t nbytes;
            off64_t offset;
        };
        struct StructReturnPread {
            int size;
            char *buf;
        };
        struct StructUnlink {
            char *name;
        };
        struct StructFtruncate {
            int fd;
            off64_t length;
        };
        struct StructRename {
            char *__old;
            char *__new;
        };
        struct StructMkdir {
            char *path;
            mode_t mode;
        };
        struct StructRmdir {
            char *path;
        };
        struct StructOpendir {
            char *basedir;
            bool recursive;
        };

        struct Entry {
            Entry(const std::string &path, size_t depth) : m_path(path), m_depth(depth) {}

            std::string m_path;
            size_t m_depth;
        };


        static void PrintStructPwrite(StructPwrite *struct_ptr);

        static void PrintStructWrite(StructWrite *struct_ptr);

        static void PrintStructFsync(StructFsync *struct_ptr);

        static void PrintStructLseek(StructLseek *struct_ptr);

        static void PrintStructOpen(StructOpen *struct_ptr);

        static void PrintStructStat(StructStat *struct_ptr);
        static void PrintStructReturnStat(struct stat *struct_ptr);



        static void PrintStructClose(StructClose *struct_ptr);

        static void PrintStructFcntl3(StructFcntl *struct_ptr);

        static void PrintStructFallocate(StructFallocate *struct_ptr);

        static void PrintStructPread(StructPread *struct_ptr);

        static void PrintStructUnlink(StructUnlink *struct_ptr);

        static void PrintStructFtruncate(StructFtruncate *struct_ptr);

        static void PrintStructRename(StructRename *struct_ptr);

        static void PrintStructMkdir(StructMkdir *struct_ptr);

        static void PrintStructRmdir(StructRmdir *struct_ptr);

        static void PrintStructOpendir(StructOpendir *struct_ptr);

    };
}

#endif //MULTI_MASTER_HANDLE_STRUCT_H
