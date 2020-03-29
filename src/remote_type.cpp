/**
 * Created by rrzhang on 2019/12/11.
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

#include "remote_type.h"
#include "easylogger.h"
#include "remote_string.h"

namespace remote {
        std::string RemoteType::SizeToString(size_t val) {
            return std::to_string(val);
        }

        std::string RemoteType::SSizeToString(ssize_t val) {
            return std::to_string(val);
        }

        std::string RemoteType::Off64ToString(off64_t val) {
            return std::to_string(val);
        }

        std::string RemoteType::ModeToString(mode_t val) {
            return std::to_string(val);
        }

        std::string RemoteType::IntToString(int val){
            return std::to_string(val);
        }
        //! or return std::strtoull(val.data(), nullptr, 10);
        size_t RemoteType::StringToSize(const std::string &val) {
            return std::stoul(val, nullptr, 10);
        }

        //! or return std::strtoll(val.data(), nullptr, 10);
        ssize_t RemoteType::StringToSSize(const std::string &val) {
            int x = -1;
            try {
                x = std::stoll(val, nullptr, 10);
            } catch(std::exception &e) {
                EasyLoggerWithTrace(multi_master::path_log_client, EasyLogger::info).force_flush() << "exception error:" << e.what();
            }
            return x;
        }

        //! or return std::strtoll(val.data(), nullptr, 10);
        off64_t RemoteType::StringToOff64(const std::string &val) {
            int x = -1;
            try {
                x = std::stoll(val, nullptr, 10);
            } catch(std::exception &e) {
                EasyLoggerWithTrace(multi_master::path_log_client, EasyLogger::info).force_flush() << "exception error:" << e.what();
            }
            return x;
        }

        size_t RemoteType::StringToMode(const std::string &val) {
            int x = -1;
            try {
                x = std::stoul(val, nullptr, 10);
            } catch(std::exception &e) {
                EasyLoggerWithTrace(multi_master::path_log_client, EasyLogger::info).force_flush() << "exception error:" << e.what();
            }
            return x;
        }

        int RemoteType::StringToInt(const std::string &val) {
            int x = -1;
            try {
                x = std::stoi(val, nullptr, 10);
            } catch(std::exception &e) {
                EasyLoggerWithTrace(multi_master::path_log_client, EasyLogger::info).force_flush() << "exception error:" << e.what();
            }
            return x;
        }

}
