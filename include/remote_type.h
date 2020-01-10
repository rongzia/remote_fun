/**
 * 一些内置类型头文件包含，内置类型转 string，及一些说明
 * Created by rrzhang on 2019/11/29.
 */

/**
 * 大多数平台和编译器对于 64 位中的整形，是这么定义的（当然也有例外，头文件会帮我们屏蔽一些差异，且我们项目不用管太多）：
 * 六种整形：(unsigned)int, (unsigned)long, (unsigned)long long. (long, long long 是 long int, long long int 的缩写)
 * 其中 (unsigned)int 32 位，(unsigned)long, (unsigned)long long 64 位。
 *
 * cstdlib 中的 整形/字符串互相转化函数只有 有符号类型，
 * 可能对于 > 2^31-1 的 unsigned int or > 2^63-1 的 unsigned long 转化会有问题，
 * 不过我们可能用不上这么长的正数，后续遇到问题再解决。
 *
 * 库函数主要有一下几种复杂的类型，对应 64 位平台如下：
 * size_t   (unsigned long)
 * ssize_t  (long or long long)
 * off64_t  (long or long long)
 * mode_t   (unsigned int)
 */
#ifndef MULTI_MASTER_REMOTE_TYPE_H
#define MULTI_MASTER_REMOTE_TYPE_H
#include <iostream>

#include <cstddef>      //! for size_t
#include <unistd.h>     //! for ssize_t
#include <cstdlib>      //! for off64_t
#include <sys/stat.h>   //! for mode_t, defined in /usr/include/x86_64-linux-gnu/sys/stat.h
#include <climits>     //! 定义了各类型的最大最小值

namespace remote {
    class RemoteType {
    public:
        static std::string SizeToString(size_t val);

        static std::string SSizeToString(ssize_t val);

        static std::string Off64ToString(off64_t val);

        static std::string ModeToString(mode_t val);

        static std::string IntToString(int val);

        //! or return std::strtoull(val.data(), nullptr, 10); ?
        static size_t StringToSize(const std::string &val);

        //! or return std::strtoll(val.data(), nullptr, 10); ?
        static ssize_t StringToSSize(const std::string &val);

        //! or return std::strtoll(val.data(), nullptr, 10); ?
        static off64_t StringToOff64(const std::string &val);

        static size_t StringToMode(const std::string &val);

        static int StringToInt(const std::string &val);
    };

}


#endif //MULTI_MASTER_REMOTE_TYPE_H
