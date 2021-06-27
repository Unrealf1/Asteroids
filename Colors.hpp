#pragma once

#include <cstdint>
#include <cmath>


using color_t = uint32_t;

enum colors: color_t {
    red = 0xff0000,
    green = 0x00ff00,
    blue = 0x0000ff,
    black = 0,
    white = 0xffffff,
    yellow = 0xffff00
};


inline color_t create_color(uint8_t red, uint8_t green, uint8_t blue) {
    return red * 256 * 256 + green * 256 + blue * 256;
};

// arguments are from 0.0f to 0.1f
inline color_t create_color(float red, float green, float blue) {
    auto int_red = static_cast<uint8_t>(std::lroundf(red * 255.0f));
    auto int_green = static_cast<uint8_t>(std::lroundf(green * 255.0f));
    auto int_blue = static_cast<uint8_t>(std::lroundf(blue * 255.0f));

    return create_color(int_red, int_green, int_blue);
};

