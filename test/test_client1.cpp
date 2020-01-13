/**
 * Created by rrzhang on 2019/12/11.
 */

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

void PrintVector(std::vector<remote::StructHandle::Entry> entry){
    for(auto iter : entry){
        cout << "path : " << iter.m_path << ", depth : " << iter.m_depth << endl;
    }
}

int main(int argc, char *argv[]) {
    std::cout << "client main" << std::endl;

    string dir = "/home/zhangrongrong/test_dir";
    string current_path = "./ibdata1";

    remote::RemoteClient *remote_client;
    if (argc > 1) {
        assert(argc > 1);
        std::cout << "arg count : " << argc << std::endl;
        std::cout << "argv : " << argv[1] << " " << argv[2] << std::endl;
        remote_client = new remote::RemoteClient(argv[1], argv[2], "10000");
    } else {
        remote_client = new remote::RemoteClient("10.11.6.120", "50051", "10001");
    }


    {
        int fd;
        int ret;
        ssize_t size;
        unsigned char buf[12];

        {
            //! open2,打开或创建文件
            cout << "--- TEST open2." << endl;
            fd = remote_client->remote_open((char *) current_path.data(), O_RDWR | O_CREAT, 0644);
            cout << "open2 file : " << current_path << ", fd : " << fd << endl << endl;
        }
        {
            cout << "--- TEST stat." << endl;
            struct stat stat1;
            ret = remote_client->remote_stat(current_path.data(), &stat1);
            remote::StructHandle::PrintStructReturnStat(&stat1);
            cout << "stat ret : " << ret << endl << endl;
            assert(0 == ret);

//            struct stat stat2;
//            ret = stat(current_path.data(), &stat2);
//            cout << "stat ret : " << ret << endl << endl;
//            assert(0 > ret);
        }
        {
            //! 写入 123456789
            cout << "--- TEST pwrite." << endl;
            size = remote_client->remote_pwrite(fd, "123456789", 9, 0);
            cout << "pwrite size : " << size << endl << endl;
            assert(9 == size);
            string str(1048576, '1');
            size = remote_client->remote_pwrite(fd, str.data(), str.length(), 9);
        }
//        {
//            //! 从头读 9 个字节，应该为 123456789
//            cout << "--- TEST pread." << endl;
//            memset(buf, 0, 12);
//            size = remote_client->remote_pread(fd, buf, 9, 0);
//            cout << "pread size : " << size ;
//            cout << ", read content : " << buf << endl << endl;
//            assert(0 == strncmp("123456789", (const char *) buf, 9));
//        }
//        {
//            //! fsync
//            cout << "--- TEST fsync." << endl;
//            ret = remote_client->remote_fsync(fd);
//            cout << "fsync ret : " << ret << endl << endl;
//            assert(0 == ret);
//        }
//        {
//            //! lseek 到 offset=3，写入 a, 再次读取出来应为 123a56789
//            cout << "--- TEST lseek." << endl;
//            ret = remote_client->remote_lseek(fd, 3, SEEK_SET);
//            cout << "lseek ret : " << ret << endl;
//            size = remote_client->remote_write(fd, "a", 1);
//            cout << "write size : " << size << endl;
//            memset(buf, 0, 12);
//            size = remote_client->remote_pread(fd, buf, 9, 0);
//            cout << "pread size : " << size << ", read content : " << buf << endl << endl;
//            assert(0 == strncmp("123a56789", (const char *) buf, 9));
//        }
//        {
//            //!  close
//            cout << "--- TEST close." << endl;
//            ret = remote_client->remote_close(fd);
//            cout << "close ret : " << ret << endl;
//            cout << "after close, fd : " << fd << endl;
////            memset(buf, 0, 12);
////            size = remote_client->remote_pread(fd, buf, 9, 0);
////            cout << "pread size : " << size << ", read content : " << buf << endl << endl;
////            assert(0 != strncmp("123a56789", (const char *) buf, 9));
//        }
//        {
//            //! open1, 重新打开文件并读取
//            cout << "--- TEST open1." << endl;
//            fd = remote_client->remote_open((char *) current_path.data(), O_RDWR);
//            cout << "open1 fd : " << fd << endl;
//            memset(buf, 0, 12);
//            size = remote_client->remote_pread(fd, buf, 9, 0);
//            cout << "pread size : " << size << ", read content : " << buf << endl << endl;
//            assert(0 == strncmp("123a56789", (const char *) buf, 9));
//        }
////        {
////            //! fcntl1, 从 fd 复制一个 fd, 复制后可以用新的 fd 读
////            int fd2 = remote_client->remote_fcntl(fd, F_DUPFD);
////            cout << "fcntl1 new fd : " << fd2 << ", old fd : " << fd << endl;
//        memset(buf, 0, 12);
////            size = remote_client->remote_pread(fd, buf, 9, 0);
////            cout << "pread size : " << size << ", read content : " << buf << endl << endl;
////            assert(0 == strncmp("123a56789", (const char *) buf, 9));
////        }
////        {
////            //! fcntl2, 对文件上锁
////            ret = remote_client->remote_fcntl(fd, F_SETFL, O_DIRECT);
////            cout << "fcntl2 setlk : " << ret << endl;
////            assert(!(0 > ret));
////        }
//        {
//            //! fcntl3, 对文件上读写锁，然后释放
//            cout << "--- TEST fcntl3." << endl;
//            flock lk;
//            lk.l_type = F_WRLCK;
//            lk.l_whence = SEEK_SET;
//            lk.l_start = lk.l_len = 0;
//            ret = remote_client->remote_fcntl(fd, F_SETLK, &lk);
//            cout << "fcntl2 setlk : " << ret << endl << endl;
//            assert(0 == ret);
//        }
//        {
//            //! rename 命名 test_remote -> test_remote2,再反向改回来
//            cout << "--- TEST rename." << endl;
//            string path2 = current_path + "2";
//            ret = remote_client->remote_rename(current_path.data(), path2.data());
//            cout << "rename ret : " << ret << endl;
//            assert(0 == ret);
//            fd = remote_client->remote_open((char *) path2.data(), O_RDWR);
//            cout << "open fd : " << fd << endl;
//            memset(buf, 0, 12);
//            size = remote_client->remote_pread(fd, buf, 9, 0);
//            cout << "pread size : " << size << ", read content : " << buf << endl;
//            assert(0 == strncmp("123a56789", (const char *) buf, 9));
//            ret = remote_client->remote_rename(path2.data(), current_path.data());
//            cout << "rename ret : " << ret << endl;
//            assert(0 == ret);
//        }
//        {
//            //! ftruncate 先从 9 bytes 扩大到 12 bytes，在减小到 6 bytes
//            cout << "--- TEST ftruncate." << endl;
//            ret = remote_client->remote_ftruncate(fd, 12);
//            cout << "ftruncate ret : " << ret << endl;
//            assert(0 == ret);
//            memset(buf, 0, 12);
//            size = remote_client->remote_pread(fd, buf, 12, 0);
//            cout << "pread size : " << size << ", read content : " << buf << endl;
//            assert(0 == strncmp("123a56789\0\0\0", (const char *) buf, 12));
//            ret = remote_client->remote_ftruncate(fd, 6);
//            cout << "ftruncate ret : " << ret << endl;
//            assert(0 == ret);
//            memset(buf, 0, 12);
//            size = remote_client->remote_pread(fd, buf, 12, 0);
//            cout << "pread size : " << size << ", read content : " << buf << endl << endl;
//            assert(0 == strncmp("123a56\0\0\0\0\0\0", (const char *) buf, 12));
//
//        }
//        {
//            //! fallocate && stat 文件打洞和获取文件信息
//            cout << "--- TEST fallocate." << endl;
//            const int mode = FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE;
//            ret = remote_client->remote_fallocate(fd, mode, 0, 1024 * 5);
//            cout << "fallocate ret : " << ret << endl;
//            assert(0 == ret);
//            memset(buf, 0, 12);
//            size = remote_client->remote_pread(fd, buf, 9, 0);
//            cout << "pread size : " << size << ", read content : " << buf << endl << endl;
//            assert(0 == strncmp("\0", (const char *) buf, 1));
//        }
//        {
//            //! opendir
//            cout << "--- TEST opendir." << endl;
//            std::vector<remote::StructHandle::Entry> entry = remote_client->remote_opendir("/home/zhangrongrong/test_dir", true);
//            PrintVector(entry);
//        }
//        {
//            //! mkdir && rmdir
//            cout << "--- TEST mkdir && rmdir." << endl;
//            ret = remote_client->remote_mkdir(dir.data(), 0770);
//            cout << "mkdir ret : " << ret << endl << endl;
//            assert(0 == ret);
//            ret = rmdir(dir.data());
//            cout << "unlink ret : " << ret << endl << endl;
//            assert(0 == ret);
//        }
//        {
//            ! unlink, 删除文件 test_remote
//            cout << "--- TEST unlink." << endl;
//            ret = remote_client->remote_unlink(current_path.data());
//            cout << "unlink ret : " << ret << endl;
//            assert(0 == ret);
//        }
    }

    return 0;
}