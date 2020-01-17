/**
 * Created by rrzhang on 2019/12/11.
 */
#define _FILE_OFFSET_BITS 64

#include <iostream>
#include <fcntl.h>
#include <cassert>
#include <string.h>
#include <sys/stat.h>

#include "remote_client.h"
#include "handle_net.h"
#include "handle_struct.h"

using namespace std;
struct stat;

int main(int argc, char *argv[]) {
    std::cout << "client main" << std::endl;

    string current_path = "./ibdata1";

    remote::RemoteClient *remote_client_1 = new remote::RemoteClient("10.11.6.120", "50051", "11111");


    {
        int fd;
        int ret;
        ssize_t size;
        int buf_size = 1048576;
        unsigned char buf[buf_size];

        {
            //! open2,打开或创建文件
            cout << "--- TEST open2." << endl;
            fd = remote_client_1->remote_open((char *) current_path.data(), O_RDWR | O_CREAT, 0644);
            cout << "open2 file : " << current_path << ", fd : " << fd << endl << endl;
        }
        {
            //! 写入
            ssize_t offset = 0;
            cout << "--- TEST pwrite." << endl;
            for (size_t i = 0; i < 3000; i++) {
                memset(buf, '1', buf_size);
                size = remote_client_1->remote_pwrite(fd, buf, buf_size, offset);
                offset += size;
                if (i % 100 == 0) {
                    cout << "pwrite size : " << offset << endl << endl;
                }
            }
            cout << "pwrite size : " << offset << endl << endl;
            assert(buf_size * 3000 == offset);
        }
        {
//            //! 从头读 9 个字节，应该为 123456789
//            cout << "--- TEST pread." << endl;
//            memset(buf, 0, 12);
//            size = remote_client_2->remote_pread(fd, buf, 9, 0);
//            cout << "pread size : " << size << ", read content : " << buf << endl << endl;
//            assert(0 == strncmp("123456789", (const char *) buf, 9));
        }
    }

    return 0;
}