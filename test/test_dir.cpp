/**
 * Created by rrzhang on 2019/12/11.
 */
#define _FILE_OFFSET_BITS 64

#include <iostream>
#include <dirent.h>
//#include <fcntl.h>
//#include <cassert>
#include <string.h>
#include <sys/stat.h>

#include "remote_client.h"

using namespace std;
using namespace remote;

bool IsDirectory(const std::string &path) {
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

//! 测试用
void PrintVector(std::vector<remote::StructHandle::Entry> entry) {
    for (auto iter : entry) {
        std::cout << "path : " << iter.m_path << ", depth : " << iter.m_depth << std::endl;
    }
}

int main(int argc, char *argv[]) {
    std::cout << "client main" << std::endl;

    RemoteClient *remote_client = new RemoteClient("10.11.6.119", "50051", "10002");
    std::vector<StructHandle::Entry> vp = remote_client->remote_opendir("/home/zhangrongrong/mysql/data", false);

    PrintVector(vp);

    return 0;
}