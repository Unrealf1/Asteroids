#include "Render.hpp"

#include <algorithm>


void Renderer::render_dots(const points_t& points) {
    std::vector<Pixel> pixels;
    pixels.reserve(points.size());

    std::ranges::transform(points, std::back_inserter(pixels), [this](const Point& p) { 
        return to_pixel(p); 
    });

    render_pixels(pixels);
}

void Renderer::render_line(const points_t& points) {
    if (points.empty()) {
        return;
    }

    std::vector<Pixel> pixels;
    pixels.reserve(points.size() * 2);

    auto last_point = points[0];

    for (std::size_t i = 1; i < points.size(); ++i) {
        auto current_point = points[i];
        auto diff_x = current_point.x - last_point.x;
        auto diff_y = current_point.y - last_point.y;

        auto num_pixels_x = std::lround(std::abs(diff_x) * virtual_to_pixel_x);
        auto num_pixels_y = std::lround(std::abs(diff_y) * virtual_to_pixel_y);

        auto num_steps_old = std::max(_buffer_height, _buffer_width);
        auto num_steps = std::max(num_pixels_x, num_pixels_y);
        auto step_x = diff_x / static_cast<float>(num_steps);
        auto step_y = diff_y / static_cast<float>(num_steps);

        auto intermediate_point = last_point;
        for (std::size_t j = 0; j < num_steps; ++j) {
            pixels.push_back(to_pixel(intermediate_point));
            intermediate_point.x += step_x;
            intermediate_point.y += step_y;

            auto ratio = (current_point.x - intermediate_point.x) / diff_x;
            intermediate_point.c = last_point.c * ratio + current_point.c * (1.0f - ratio);
        }
        last_point = current_point;
    }

    std::ranges::transform(points, std::back_inserter(pixels), [this](const Point& p) { 
        return to_pixel(p); 
    });

    render_pixels(pixels);
}

void Renderer::render_cycle_line(const points_t& points) {
    if (points.empty()) {
        return;
    }

    // TODO: this could be optimized
    points_t new_points = points;
    new_points.reserve(points.size() + 1);
    new_points.push_back(points[0]);

    render_line(new_points);
}

void Renderer::render_fill(const points_t& points) {
    std::cerr << "ERROR: 'render_fill' is not yet implemented!\n";
}
