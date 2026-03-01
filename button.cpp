#include <gpiod.h>
#include <iostream>
#include <poll.h>
#include <cstdlib>

int main(int argc, char *argv[]) {

    // init
    const char *chip_path = "../../../dev/gpiochip0";
    unsigned int pin = atoi(argv[1]);
    if (pin == 0) {
        std::cerr << "Usage: " << argv[0] << " <pin>\n";
        return 1;
    }

    gpiod_chip *chip = gpiod_chip_open(chip_path);

    auto *settings = gpiod_line_settings_new();

    gpiod_line_settings_set_direction(
        settings,
        GPIOD_LINE_DIRECTION_INPUT);

    // Detect button press
    gpiod_line_settings_set_edge_detection(
        settings,
        GPIOD_LINE_EDGE_FALLING);

    // Hardware debounce
    gpiod_line_settings_set_debounce_period_us(
        settings,
        10000);   // 10 ms

    auto *line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(
        line_cfg,
        &pin,
        1,
        settings);

    auto *req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(
        req_cfg,
        "button");

    auto *request =
        gpiod_chip_request_lines(
            chip,
            req_cfg,
            line_cfg);

    gpiod_edge_event_buffer *event_buffer = gpiod_edge_event_buffer_new(1);

    int fd = gpiod_line_request_get_fd(request);
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    
    while (true) {
        // Poll
        int ret = poll(&pfd, 1, 0); // non-blocking

        if (ret > 0 && (pfd.revents & POLLIN)) {
            int num_events =
                gpiod_line_request_read_edge_events(
                    request,
                    event_buffer,
                    1);

            for (int i = 0; i < num_events; i++) {
                gpiod_edge_event *event =
                    gpiod_edge_event_buffer_get_event(
                        event_buffer,
                        i);

                if (gpiod_edge_event_get_event_type(event)
                    == GPIOD_EDGE_EVENT_FALLING_EDGE)
                {
                    std::cout << "Button pressed\n";
                }
            }
        }
    }

    // release
    gpiod_edge_event_buffer_free(event_buffer);
    gpiod_line_request_release(request);
    gpiod_chip_close(chip);
    return 0;
}