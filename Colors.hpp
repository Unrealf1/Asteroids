#pragma once

#include <cstdint>

using color_t = uint32_t;

enum colors: color_t {
    red = 0xff0000,
    green = 0x00ff00,
    blue = 0x0000ff,
    black = 0,
    white = 0xffffff
};