#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <algorithm>
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
    line, dots, fill, cycle_line
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
                render_dots(points);
                break;

            case RenderMode::fill:
                render_fill(points);
                break;

            case RenderMode::cycle_line:
                render_cycle_line(points);
                break;
        }
    }

    void clear() {
        // memset can set only a value of byte size
        if (_clear_color == colors::white || _clear_color == colors::black) {
            std::memset(_render_buffer, _clear_color, _buffer_height * _buffer_width * sizeof(color_t));
        } else {
            std::fill(_render_buffer, _render_buffer + _buffer_height * _buffer_width, _clear_color);
        }
    }   

private:
    uint32_t* _render_buffer;
    const uint32_t _buffer_height;
    const uint32_t _buffer_width;
    color_t _clear_color = 0;
    const float virtual_height = 100.0f;
    const float virtual_width = virtual_height * static_cast<float>(_buffer_width) / static_cast<float>(_buffer_height);
    //RenderOptions _options;

    const float virtual_to_pixel_x = static_cast<float>(_buffer_width) / virtual_width;
    const float virtual_to_pixel_y = static_cast<float>(_buffer_height) / virtual_height;

    Pixel to_pixel(const Point& p) const {
        auto x = static_cast<int32_t>(std::lround(p.x * virtual_to_pixel_x));
        auto y = static_cast<int32_t>(std::lround(p.y * virtual_to_pixel_y));
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

    void render_cycle_line(const points_t& points);
    void render_line(const points_t& points);
    void render_dots(const points_t& points);
    void render_fill(const points_t& points);

};



// counter clockwise, angle in radians
inline position_t rotate_point(const position_t& position, const position_t& center, float angle) {
    float c = std::cos(angle);
    float s = std::sin(angle);

    position_t center_respect_pos = {position.x - center.x, position.y - center.y};
    position_t center_respect_rot = {
        center_respect_pos.x * c - center_respect_pos.y * s, 
        center_respect_pos.x * s + center_respect_pos.y * c
    };

    return {center_respect_rot.x + center.x, center_respect_rot.y + center.y};
}

inline points_t rotate_points(const points_t& points, const position_t& center, float angle) {
    float c = std::cos(angle);
    float s = std::sin(angle);

    //TODO: make faster by calculating cos and sin only one time
    points_t res;
    res.reserve(points.size());
    std::ranges::transform(points, std::back_inserter(res), [&](const Point& p) -> Point { 
        auto new_pos = rotate_point({p.x, p.y}, center, angle);
        return {new_pos.x, new_pos.y, p.c};
    });
    return res;
}

inline points_t move_points(const points_t& points, const position_t& vec) {
    points_t res;
    res.reserve(points.size());
    std::ranges::transform(points, std::back_inserter(res), [&](const Point& p) -> Point { 
        return {p.x + vec.x, p.y + vec.y, p.c};
    });
    return res;
}


inline position_t get_direction(float angle) {
    return {std::sin(angle), std::cos(angle)};
}
