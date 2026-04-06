#include "../lib/app_controller/AppController.hpp"

int main(int argc, char *argv[]) {
    bool debug = false;
    if (argc > 1) {
        debug = std::string(argv[1]) == "debug";
    }
    AppController appController;
    appController.run(debug);
    return 0;
}
