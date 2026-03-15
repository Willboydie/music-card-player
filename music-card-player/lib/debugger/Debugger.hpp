#pragma once

#include <string>
#include <iostream>

class Debugger {
public:
    static bool debugMode;
    static void debug_msg(const std::string& msg);
};
