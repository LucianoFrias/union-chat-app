
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Union::Logging {


class Logger
{
private:
    static std::string currentTime()
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;

        ss << std::put_time(
            std::localtime(&time),
            "%Y-%m-%d %H:%M:%S"
        );

        return ss.str();
    }

public:
    static void info(const std::string& message)
    {
        std::cout
            << '['
            << currentTime()
            << "] [INFO] "
            << message
            << '\n';
    }

    static void error(const std::string& message)
    {
        std::cerr
            << '['
            << currentTime()
            << "] [ERROR] "
            << message
            << '\n';
    }
};

}
