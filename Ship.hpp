#pragma once

#include "Drawable.hpp"
#include "Primitives.hpp"
#include "Engine.h"
#include "Bullet.hpp"

#include <memory>


class Ship: public Drawable, public Updatable, public Movable {
public:
    Ship(position_t pos): Movable(pos) {
        _graphics = std::make_unique<EquilateralTriangle>(ship_size, pos, colors::green);
        //TODO: pass constructor parameters directly to emplace_back
        _engine_graphics.emplace_back(std::make_unique<Square>(ship_size/3.0f, position_t{pos.x, pos.y + ship_size/3.0f}, 0xff9f00));
        //_graphics = std::make_unique<Square>(ship_size, pos, colors::green);
        //_graphics = std::make_unique<Circle>(ship_size, pos);
    }

    void draw(Renderer& renderer) override {
        _graphics->draw(renderer);
        if (_engine_on) {
            for (auto& g : _engine_graphics) {
                g->draw(renderer);
            }
        }
    }

    void update(const updateinfo& info) override {
        update_speed(info);
        update_position(info);
        update_rotation(info);

        shoot(info);
        
        _engine_on = is_key_pressed(VK_UP);
    }

private:
    float acceleration = 0.9f;
    float speed_decay = 0.08f;
    float min_speed = 0.000001f;
    float rotation_speed = 1.5f;
    float angle = 0.0f;
    float ship_size = 4.0f;
    float bullet_speed = 5.0f;
    position_t speed = {0.0f, 0.0f};
    float shooting_rate = 0.3f;
    float since_last_shot = 0.0f;
    bool _engine_on = false;

    std::vector<std::unique_ptr<InteractiveDrawable>> _engine_graphics;
    std::unique_ptr<InteractiveDrawable> _graphics;

    position_t get_engine_graphics_pos() {
        return rotate_point({_pos.x, _pos.y + ship_size/3.0f}, _pos, angle);
    }

    void shoot(const updateinfo& info) {
        since_last_shot += info.dt;
        if (!is_key_pressed(VK_SPACE) || since_last_shot < shooting_rate) {
            return;
        }

        since_last_shot = 0.0f;

        auto direction = get_direction(angle);
        position_t bullet_pos = {_pos.x + direction.x * ship_size, _pos.y - direction.y * ship_size};

        auto bullet = std::make_shared<Bullet>(bullet_pos, position_t{direction.x * bullet_speed, direction.y * bullet_speed});
        info.drawable.push_back(bullet);
        info.updatable.push_back(bullet);
    }

    void update_rotation(const updateinfo& info) {
        if (is_key_pressed(VK_LEFT)) {
            angle -= rotation_speed*info.dt;
        }

        if (is_key_pressed(VK_RIGHT)) {
            angle += rotation_speed*info.dt;
        }
        _graphics->set_rotation(angle);
        _engine_graphics[0]->set_rotation(angle);
    }

    void update_position(const updateinfo& info) {
        _pos.x += speed.x;
        _pos.y -= speed.y;
        _graphics->set_position(_pos);
        _engine_graphics[0]->set_position(get_engine_graphics_pos());
    }

    void update_speed(const updateinfo& info) {
        if (is_key_pressed(VK_UP)) {
            auto dir = get_direction(angle);
            //std::cout << "angle: " << angle << "\ndirection: " << dir.x << ' ' << dir.y << '\n';
            float additional_speed = acceleration * info.dt;
            speed.x += dir.x * additional_speed;
            speed.y += dir.y * additional_speed;
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
