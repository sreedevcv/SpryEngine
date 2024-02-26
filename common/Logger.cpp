#include "Logger.hpp"

#include <iostream>

void spry::Logger::msg_log(std::string_view msg)
{
    std::cout << msg << "\n";
}

std::ostream& spry::Logger::log(int level)
{
    auto lvl_msg = "";
    if (level == 0) {
        lvl_msg = "DEBUG";
    }
    else if (level == 1) {
        lvl_msg = "WARN";
    }
    else if (level == 1) {
        lvl_msg = "FATAL";
    }
    return std::cout << lvl_msg << "::" ;
}


