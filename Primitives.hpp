#pragma once

#include "Drawable.hpp"
#include <iostream>
#include <memory>
#include <cmath>
#include <algorithm>


class group: public Drawable {
public:
    void add(std::unique_ptr<Drawable>&& other) {

    }
private:
};

class Square : public InteractiveDrawable {
public:
    Square(float side_len, position_t center_position, color_t color = colors::black): 
    InteractiveDrawable(center_position, 0.0f, side_len), _col(color) { }

    void draw(Renderer& renderer) override {
        auto offset = _scale / 2.0f;
        
        auto left = _pos.x - offset;
        auto right = _pos.x + offset;

        auto top = _pos.y - offset;
        auto bot = _pos.y + offset;


        renderer.render(
            {
                {left, top, _col},
                {left, bot, _col},
                {right, bot, _col},
                {right, top, _col},
                {left, top, _col}
            }, 
            RenderMode::line);
    }

private:
    color_t _col;
};

class Circle : public InteractiveDrawable {
public:
    Circle(float radius, position_t center_position, color_t color = colors::black, uint32_t num_points = 50): 
    InteractiveDrawable(center_position, 0.0f, radius), _num_points(num_points), _col(color)  { 
        _points.reserve(num_points);
        auto num_steps = num_points / 2;
        float x_step = 2.0f / static_cast<float>(num_steps);
        float initial_x = -1.0f;

        for (uint32_t i = 0; i < num_steps; ++i) {
            float current_x = initial_x + x_step * i;
            _points.push_back({current_x, std::sqrt(radius - current_x*current_x), color});
        }
        // `i < num_steps` is because of unsigned arithmetic
        for (uint32_t i = num_steps - 1u; i < num_steps; --i) {
            std::cout << i << ' ' << num_steps - 1u << '\n';
            float current_x = initial_x + x_step * i;
            _points.push_back({current_x, -std::sqrt(radius - current_x*current_x), color});
        }
    }

    void draw(Renderer& renderer) override {
        points_t transposed_points;
        transposed_points.reserve(transposed_points.size());
        std::ranges::transform(_points, std::back_inserter(transposed_points), [this](const Point& p) -> Point { 
            return {p.x + _pos.x, p.y + _pos.y, p.c};
        });


        renderer.render(transposed_points, RenderMode::line);
    }



private:
    color_t _col;
    uint32_t _num_points;
    points_t _points;
};
