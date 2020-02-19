#include <iostream>
#include <cstring>
#include <cassert>
#include "remote_client.h"

using namespace std;
using namespace remote;

int main() {
//    system("ulimit -s 16384");
    string path[] = {
            "/home/zhangrongrong/mysql/data/ibdata1"
            , "/home/zhangrongrong/mysql/data/ib_logfile0"
            , "/home/zhangrongrong/mysql/data/ib_logfile1"
            , "/home/zhangrongrong/mysql/data/mysql.ibd"
            , "/home/zhangrongrong/mysql/data/undo_001"
            , "/home/zhangrongrong/mysql/data/undo_002"
    };
    RemoteClient *remoteClient = new RemoteClient("10.11.6.120", "50051", "10002");

//    for (int i = 0; i < 6; i++) {
//        struct stat statinfo;
//        int remote_errno;
//        remoteClient->remote_stat(path[i].data(), &statinfo, &remote_errno);
//        int read_fd1 = remoteClient->remote_open(path[i].data(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
//        int read_fd2 = open(path[i].data(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
//        int offset = 0;
//        unsigned char buf1[1048576], buf2[1048576];
//        while(offset < statinfo.st_size) {
//            memset(buf1, 0, 1048576);
//            memset(buf2, 0, 1048576);
//
//            int read_size1 = remoteClient->remote_pread(read_fd1, buf1
//                    , offset + 1048576 > statinfo.st_size ? statinfo.st_size - offset : 1048576, offset);
//            int read_size2 = pread(read_fd2, buf2, 1048576, offset);
//
//            offset += 1048576;
//            cout << "read size1:" << read_size1;
//            cout << ", read size2:" << read_size2 << endl;
//
//            assert(read_size1 == read_size2);
//            int ret = memcmp(buf1, buf2, read_size1);
//            cout << "ret:" << ret << endl;
//            assert(ret == 0);
//        }
//        remoteClient->remote_close(read_fd1);
//        close(read_fd2);
//    }

    int write_fd = remoteClient->remote_open("/home/zhangrongrong/LOG_test", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    unsigned char write_buf[1048576];
    int write_offset = 0;
    for (int i = 0; i < 32; i++) {
        memset(write_buf, 129 + i, 1048576);
        int write_size = remoteClient->remote_pwrite2(write_fd, write_buf, 1048576, write_offset);
        write_offset += write_size;
    }
    remoteClient->remote_close(write_fd);

    int read_fd = remoteClient->remote_open("/home/zhangrongrong/LOG_test", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    unsigned char read_buf[1048576];
    int read_offset = 0;
    for (int i = 0; i < 32; i++) {
        memset(read_buf, 0, 1048576);
        int read_size = remoteClient->remote_pread2(read_fd, read_buf, 1048576, read_offset);
        read_offset +=  read_size;
        for(int j = 0; j< 1048576; j++){
            assert(read_buf[j] == 129 + i);
        }
    }
    remoteClient->remote_close(read_fd);



    return 0;
}