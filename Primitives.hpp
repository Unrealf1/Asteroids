#pragma once

#include "Drawable.hpp"
#include <iostream>
#include <memory>
#include <cmath>
#include <algorithm>
#include <numbers>


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
        
        auto left = -offset;
        auto right = offset;

        auto top = -offset;
        auto bot = offset;

        points_t mesh = {
            {left, top, _col},
            {left, bot, _col},
            {right, bot, _col},
            {right, top, _col}
        };

        renderer.render(
            move_points(rotate_points(mesh, {0.0f, 0.0f}, _rot), _pos), 
            RenderMode::cycle_line
        );
    }

    bool check_collision(const position_t& pos) override {
        bool x_cond = (pos.x < (_pos.x + _scale / 2.0f)) && (pos.x > (_pos.x - _scale/2.0f));
        bool y_cond = (pos.y < (_pos.y + _scale / 2.0f)) && (pos.y > (_pos.y - _scale/2.0f));
        return x_cond && y_cond;
    }

private:
    color_t _col;
};

class Circle : public InteractiveDrawable {
public:
    Circle(float radius, position_t center_position, color_t color = colors::black, uint32_t num_points = 50): 
    InteractiveDrawable(center_position, 0.0f, radius), _num_points(num_points), _col(color), _radius(radius)  { 
        _points.reserve(num_points);
        const auto pi = std::numbers::pi_v<float>;

        float current_angle = 0.0f;
        float angle_step = pi * 2.0f / static_cast<float>(num_points);

        for (uint32_t i = 0; i < num_points; ++i) {
            _points.push_back({std::cos(current_angle) * radius, std::sin(current_angle) * radius, color});
            current_angle += angle_step;
        }
    }


    void draw(Renderer& renderer) override {
        points_t transposed_points;
        transposed_points.reserve(transposed_points.size());
        std::ranges::transform(_points, std::back_inserter(transposed_points), [this](const Point& p) -> Point { 
            return {p.x + _pos.x, p.y + _pos.y, p.c};
        });


        renderer.render(transposed_points, RenderMode::cycle_line);
    }

    bool check_collision(const position_t& pos) override {
        return positions_close(pos, _pos, _radius*1.1f);
    }


private:
    color_t _col;
    uint32_t _num_points;
    points_t _points;
    float _radius;
};

class EquilateralTriangle : public InteractiveDrawable {
public:
    EquilateralTriangle(float side_len, position_t center_position, color_t color = colors::black): 
    InteractiveDrawable(center_position, 0.0f, side_len), _col(color), _circumradius(side_len / std::sqrt(3.0f))  {
        float a = side_len / 2.0f;
        float b = _circumradius / 2.0f;

        _mesh = {
            {0.0f, -_circumradius, colors::red},
            {-a, b, _col},
            {a, b, _col},
        };
     }

    bool check_collision(const position_t& pos) override {
        return false;
    }

    void draw(Renderer& renderer) override {        
        renderer.render(
            move_points(rotate_points(_mesh, {0.0f, 0.0f}, _rot), _pos), 
            RenderMode::cycle_line
        );
    }

private:
    color_t _col;
    points_t _points;
    float _circumradius;
    points_t _mesh;

};
