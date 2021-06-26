#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include "Colors.hpp"
#include "Engine.h"
#include "Commons.hpp"

// X to the right 0.0 -> 100.0
// Y to the bottom 0.0 -> 100.0

struct Point {
    float x;
    float y;
    color_t c = colors::red;
};

struct Pixel {
    int32_t x;
    int32_t y;
    color_t c;
};

enum class RenderMode {
    line, dots, fill
};

// struct RenderOptions {
//     uint32_t width = 1;
// };

using points_t = std::vector<Point>;

class Renderer {
public:
    Renderer(uint32_t* buffer, uint32_t height, uint32_t width, color_t clear_color=colors::black): 
    _render_buffer(buffer), _buffer_height(height), _buffer_width(width), _clear_color(clear_color) { }

    // void set_render_options(RenderOptions options) {
    //     _options = options;
    // }

    position_t get_center_coords() {
        return {virtual_width / 2.0f, virtual_height / 2.0f};
    }

    float get_width() {
        return virtual_width;
    }

    float get_height() {
        return virtual_height;
    }

    void render(const points_t& points, RenderMode mode) {
        switch (mode) {
            case RenderMode::line:
                render_line(points);
                break;

            case RenderMode::dots:
                render_line(points);
                break;

            case RenderMode::fill:
                render_line(points);
                break;
        }
    }

    void clear() {
        std::memset(_render_buffer, _clear_color, _buffer_height * _buffer_width * sizeof(color_t));
    }   

private:
    uint32_t* _render_buffer;
    const uint32_t _buffer_height;
    const uint32_t _buffer_width;
    color_t _clear_color = 0;
    const float virtual_height = 100.0f;
    const float virtual_width = virtual_height * static_cast<float>(_buffer_width) / static_cast<float>(_buffer_height);
    //RenderOptions _options;

    Pixel to_pixel(const Point& p) const {
        auto x = static_cast<int32_t>(std::lround(p.x / virtual_width * static_cast<float>(_buffer_width)));
        auto y = static_cast<int32_t>(std::lround(p.y / virtual_height * static_cast<float>(_buffer_height)));
        return {x, y, p.c};
    }

    void render_pixels(const std::vector<Pixel>& pxs) {
        for (const auto& px : pxs) {
            if (px.x < 0 || px.x >= _buffer_width || px.y < 0 || px.y >= _buffer_height) {
                continue;
            }
            _render_buffer[px.y * _buffer_width + px.x] = px.c;
        }
    }

    void render_line(const points_t& points);
    void render_dots(const points_t& points);
    void render_fill(const points_t& points);

};
