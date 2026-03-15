#include "Button.hpp"

Button::Button(int gpioPin)
    : pin(gpioPin)
{}


bool Button::init() {

    chip = gpiod_chip_open(chip_path);
    if (!chip) {
        perror("gpiod_chip_open");
        return false;
    }

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

    request =
        gpiod_chip_request_lines(
            chip,
            req_cfg,
            line_cfg);

    gpiod_line_settings_free(settings);
    gpiod_line_config_free(line_cfg);
    gpiod_request_config_free(req_cfg);

    if (!request)
        return false;

    buffer = gpiod_edge_event_buffer_new(4);
    fd = gpiod_line_request_get_fd(request);

    return fd >= 0;
}


bool Button::poll()
{
    if (!request) {
        perror("Button: request is null");
        return false;
    }

    struct pollfd pfd{};
    pfd.fd = fd;
    pfd.events = POLLIN;

    // ZERO timeout → never blocks UI
    int ret = ::poll(&pfd, 1, 0);

    if (ret < 0) {
        perror("Button poll failed");
        return false;
    }

    if (ret == 0) {
        // nothing happened — normal
        return false;
    }

    int events =
        gpiod_line_request_read_edge_events(
            request,
            buffer,
            4);

    for (int i = 0; i < events; ++i)
    {
        auto* ev =
            gpiod_edge_event_buffer_get_event(
                buffer,
                i);

        if (gpiod_edge_event_get_event_type(ev)
            == GPIOD_EDGE_EVENT_FALLING_EDGE)
        {
            return true;   // ONE press detected
        }
    }

    return false;
}


// Release the GPIO line and close the chip.
void Button::release() {
    gpiod_edge_event_buffer_free(buffer);
    gpiod_line_request_release(request);
    gpiod_chip_close(chip);
}