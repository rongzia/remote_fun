//
// Created by rrzhang on 2020/1/3.
//

#include <iostream>
#include <fcntl.h>
#include <zconf.h>

using namespace std;

int multi_master_zhang_get_full_path(int fd, char *buf) {
    char path[1024];
    snprintf(path, 1024, "/proc/%ld/fd/%d", (long) getpid(), fd);

    int ret = readlink(path, buf, 1024);
    return ret;
}

int main() {
    int fd = open("/home/zhangrongrong/LOG_REMOTE_SERVER", O_RDWR | O_CREAT, 0644);
    char buf[1024];
    multi_master_zhang_get_full_path(fd, buf);
    cout << buf;
    return 0;
}