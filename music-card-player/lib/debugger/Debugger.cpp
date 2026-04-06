#include "Debugger.hpp"

bool Debugger::debugMode = false;

void Debugger::debug_msg(const std::string& msg) {
    if (debugMode) {
        std::cout << msg << std::endl;
    }
}