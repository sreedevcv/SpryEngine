#pragma once

#include <string_view>

namespace spry
{

class Logger
{
private:
    
public:
    Logger() = default;
    ~Logger() = default;

    static void msg_log(std::string_view msg);
    static void debug_log(std::string_view msg);
    static std::ostream& log(int level);
};

} 
