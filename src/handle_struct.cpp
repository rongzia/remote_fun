/**
 * Created by rrzhang on 2019/12/11.
 */

#include "handle_struct.h"
#include <map>
#include "remote_string.h"
#include "handle_json.h"

#include "my_rapidjson_size_t.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"

namespace remote {

    void StructHandle::PrintStructPwrite(StructPwrite *struct_ptr) {
        std::cout << "StructPwrite->fd : " << struct_ptr->fd << std::endl;
        std::string buf((char *) struct_ptr->buf, struct_ptr->nbytes);
        std::cout << "StructPwrite->buf : " << buf << std::endl;
        std::cout << "StructPwrite->nbytes : " << struct_ptr->nbytes << std::endl;
        std::cout << "StructPwrite->offset : " << struct_ptr->offset << std::endl;
    }

    void StructHandle::PrintStructWrite(StructWrite *struct_ptr) {
        std::cout << "StructWrite->fd : " << struct_ptr->fd << std::endl;
        std::string buf((char *) struct_ptr->buf, struct_ptr->nbytes);
        std::cout << "StructWrite->buf : " << buf << std::endl;
        std::cout << "StructWrite->nbytes : " << struct_ptr->nbytes << std::endl;
    }


    void StructHandle::PrintStructFsync(StructFsync *struct_ptr) {}

    void StructHandle::PrintStructLseek(StructLseek *struct_ptr) {}

    void StructHandle::PrintStructOpen(StructOpen *struct_ptr) {
        std::cout << "StructOpen->pathname : " << struct_ptr->pathname << std::endl;
        std::cout << "StructOpen->flag : " << struct_ptr->flag << std::endl;
        std::cout << "StructOpen->mode : " << struct_ptr->mode << std::endl;
    }

    void StructHandle::PrintStructStat(StructStat *struct_ptr) {}

    void StructHandle::PrintStructReturnStat(struct stat *struct_ptr) {
        std::cout << "info.st_mode : " << struct_ptr->st_mode << std::endl;
        std::cout << "info.st_size : " << struct_ptr->st_size << std::endl;
        std::cout << "info.st_blksize : " << struct_ptr->st_blksize << std::endl;
        std::cout << "info.st_blocks : " << struct_ptr->st_blocks << std::endl;
        std::cout << "info.st_ctim.tv_sec : " << struct_ptr->st_ctim.tv_sec << std::endl;
        std::cout << "info.st_atim.tv_sec : " << struct_ptr->st_atim.tv_sec << std::endl;
        std::cout << "info.st_mtim.tv_sec : " << struct_ptr->st_mtim.tv_sec << std::endl;
    }

    void StructHandle::PrintStructClose(StructClose *struct_ptr) {}

    void StructHandle::PrintStructFcntl3(StructFcntl *struct_ptr) {
        std::cout << "PrintStructFcntl->fd : " << struct_ptr->fd << std::endl;
        std::cout << "PrintStructFcntl->cmd : " << struct_ptr->cmd << std::endl;
        std::cout << "PrintStructFcntl->lock.l_type : " << struct_ptr->lock.l_type << std::endl;
        std::cout << "PrintStructFcntl->lock.l_whence : " << struct_ptr->lock.l_whence << std::endl;
        std::cout << "PrintStructFcntl->lock.l_start : " << struct_ptr->lock.l_start << std::endl;
        std::cout << "PrintStructFcntl->lock.l_len : " << struct_ptr->lock.l_len << std::endl;
    }

    void StructHandle::PrintStructFallocate(StructFallocate *struct_ptr) {}

    void StructHandle::PrintStructPread(StructPread *struct_ptr) {
        std::cout << "StructPread->fd : " << struct_ptr->fd << std::endl;
        std::string buf((char *) struct_ptr->buf, struct_ptr->nbytes);
        std::cout << "StructPread->buf : " << buf << std::endl;
        std::cout << "StructPread->nbytes : " << struct_ptr->nbytes << std::endl;
        std::cout << "StructPread->offset : " << struct_ptr->offset << std::endl;
    }

    void StructHandle::PrintStructUnlink(StructUnlink *struct_ptr) {
        std::cout << "StructPread->name : " << struct_ptr->name << std::endl;
    }

    void StructHandle::PrintStructFtruncate(StructFtruncate *struct_ptr) {}

    void StructHandle::PrintStructRename(StructRename *struct_ptr) {
        std::string __old(struct_ptr->__old, strlen(struct_ptr->__old));
        std::string __new(struct_ptr->__new, strlen(struct_ptr->__new));
        std::cout << "StructRename->__old : " << __old << std::endl;
        std::cout << "StructRename->__new : " << __new << std::endl;
    }

    void StructHandle::PrintStructMkdir(StructMkdir *struct_ptr) {
        std::string buf((char *) struct_ptr->path, strlen(struct_ptr->path));
        std::cout << "StructMkdir->path : " << buf << std::endl;
        std::cout << "StructMkdir->mode : " << struct_ptr->mode << std::endl;
    }

    void StructHandle::PrintStructRmdir(StructRmdir *struct_ptr) {
        std::string buf((char *) struct_ptr->path, strlen(struct_ptr->path));
        std::cout << "StructRmdir->path : " << buf << std::endl;
    }

    void StructHandle::PrintStructOpendir(StructOpendir *struct_ptr) {
        std::string buf((char *) struct_ptr->basedir, strlen(struct_ptr->basedir));
        std::cout << "StructOpendir->basedir : " << buf << std::endl;
        std::cout << "StructOpendir->recursive" << struct_ptr->recursive << std::endl;
    }
}