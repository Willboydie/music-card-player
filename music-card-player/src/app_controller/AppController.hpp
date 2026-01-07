#pragma once

class AppController {
public:
    AppController();
    ~AppController();
    int run();
private:
    int argc;
    char** argv;
};