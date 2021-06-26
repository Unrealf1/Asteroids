#pragma once

#include "Drawable.hpp"
#include "Primitives.hpp"
#include "Engine.h"

#include <memory>


class Ship: public Drawable, public Updatable, public Movable {
public:
    Ship(position_t pos): Movable(pos) {
        _graphics = std::make_unique<Square>(4.0f, pos);
    }

    void draw(Renderer& renderer) override {
        _graphics->draw(renderer);
    }

    void update(const updateinfo& info) override {
        update_speed(info);
        update_position(info);
    }

private:
    float acceleration = 0.9f;
    float speed_decay = 0.2f;
    float min_speed = 0.000001f;
    position_t speed = {0.0f, 0.0f};
    std::unique_ptr<InteractiveDrawable> _graphics;

    void update_position(const updateinfo& info) {
        _pos.x += speed.x;
        _pos.y += speed.y;
        _graphics->set_position(_pos);
    }

    void update_speed(const updateinfo& info) {
        std::cout << "speed is: " << speed.x << ' ' << speed.y << '\n';
        if (is_key_pressed(VK_LEFT)) {
            speed.x -= acceleration * info.dt;
        }
        if (is_key_pressed(VK_RIGHT)) {
            speed.x += acceleration * info.dt;
        } 
        if (is_key_pressed(VK_UP)) {
            speed.y -= acceleration * info.dt;
        } 
        if (is_key_pressed(VK_DOWN)) {
            speed.y += acceleration * info.dt;
        }

        position_t friction = {speed.x * speed_decay, speed.y * speed_decay};
        position_t speed_diff = {speed.x - friction.x, speed.y - friction.y}; 

        speed.x -= speed_diff.x * info.dt;
        speed.y -= speed_diff.y * info.dt;

        if (speed.x*speed.x + speed.y*speed.y < min_speed) {
            speed.x = 0.0f;
            speed.y = 0.0f;
        }
    }
};
