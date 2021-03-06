#pragma once

#include "Drawable.hpp"
#include "Primitives.hpp"
#include "Engine.h"
#include "Bullet.hpp"
#include "PowerUps.hpp"

#include <memory>
#include <array>


class Ship: public Drawable, public Updatable, public Movable {
public:
    Ship(position_t pos): Movable(pos) {
        _graphics = std::make_unique<EquilateralTriangle>(ship_size, pos, colors::green);
        //TODO: pass constructor parameters directly to emplace_back
        _engine_graphics.emplace_back(std::make_unique<Square>(ship_size/3.0f, position_t{pos.x, pos.y + ship_size/3.0f}, 0xff9f00));
        _invincibility_graphics = std::make_unique<Circle>(ship_size/2.0f, pos, colors::blue);
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
        if (_invincibility_remaining > 0.0f) {
            _invincibility_graphics->draw(renderer);
        }
    }

    void update(const updateinfo& info) override {
        check_asteroid_collision(info);
        check_powerup_collision(info);
        shoot(info);

        update_speed(info);
        update_position(info);
        update_rotation(info);
        
        _engine_on = is_key_pressed(VK_UP);

        if (_invincibility_remaining > 0.0f) {
            _invincibility_remaining -= info.dt;
        }

        if (_double_bullet_remaining > 0.0f) {
            _double_bullet_remaining -= info.dt;
        }
    }

private:
    float acceleration = 50.0f;
    float speed_decay = 0.08f;
    float min_speed = 0.000001f;
    float rotation_speed = 2.3f;
    float angle = 0.0f;
    float ship_size = 4.0f;
    float bullet_speed = 330.0f;
    position_t speed = {0.0f, 0.0f};
    float shooting_rate = 0.3f;
    float since_last_shot = 0.0f;
    bool _engine_on = false;
    const float _invincibility_time = 1.0f;
    float _invincibility_remaining = 0.0f;
    const float _double_bullet_time = 5.0f;
    float _double_bullet_remaining = 0.0f;

    std::vector<std::unique_ptr<InteractiveDrawable>> _engine_graphics;
    std::unique_ptr<InteractiveDrawable> _graphics;
    std::unique_ptr<InteractiveDrawable> _invincibility_graphics;

    position_t get_engine_graphics_pos() {
        return rotate_point({_pos.x, _pos.y + ship_size/3.0f}, _pos, angle);
    }

    std::array<position_t, 4> get_collision_points() {
        auto r = ship_size / std::sqrt(3.0f);
        auto dir1 = get_direction(angle, r);
        auto dir2 = get_direction(angle + std::numbers::pi_v<float> / 3.0f, r);
        auto dir3 = get_direction(angle + std::numbers::pi_v<float> / 3.0f * 2.0f, r);
        return {
            position_t{_pos.x + dir1.x, _pos.y + dir1.y},
            position_t{_pos.x + dir2.x, _pos.y + dir2.y},
            position_t{_pos.x + dir3.x, _pos.y + dir3.y},
            position_t{_pos.x, _pos.y}
        };
    }

    void check_powerup_collision(const updateinfo& info) {
        auto points = get_collision_points();

        for (auto& pw : info.powerups) {
            auto collision_check = [&](const position_t& pos) -> bool { return pw->check_collision(pos); };
            bool collision = std::ranges::any_of(points, collision_check);
            if (collision) {
                pw->destroy();
                switch (pw->get_type())
                {
                    case powerup_type::health:
                        info.lives += 1;
                        break;
                    case powerup_type::invincibility:
                        _invincibility_remaining += 5.0f;
                        break;
                    case powerup_type::double_cannon:
                        _double_bullet_remaining += _double_bullet_time;
                        break;
                }
                break;
            }
        }
    }

    void check_asteroid_collision(const updateinfo& info) {
        if (_invincibility_remaining > 0.0f) {
            return;
        }
        auto points = get_collision_points();
        for (auto& ast : info.asteroids) {
            auto collision_check = [&](const position_t& pos) -> bool { return ast->check_collision(pos); };
            bool collision = std::ranges::any_of(points, collision_check);
            if (collision) {
                ast->destroy();
                info.lives -= 1;
                _invincibility_remaining += _invincibility_time;
                break;
            }
        }
    }

    void shoot(const updateinfo& info) {
        since_last_shot += info.dt;
        if (!is_key_pressed(VK_SPACE) || since_last_shot < shooting_rate || info.dt == 0.0f) {
            return;
        }

        since_last_shot = 0.0f;

        if (_double_bullet_remaining <= 0.0f) {
            auto direction = get_direction(angle);
            position_t bullet_pos = {_pos.x + direction.x * ship_size, _pos.y - direction.y * ship_size};
            auto bullet = std::make_shared<Bullet>(bullet_pos, position_t{direction.x * bullet_speed, direction.y * bullet_speed});
            info.drawable.push_back(bullet);
            info.updatable.push_back(bullet);
        } else {
            auto direction1 = get_direction(angle + std::numbers::pi_v<float> / 9.0f);
            position_t bullet_pos1 = {_pos.x + direction1.x * ship_size, _pos.y - direction1.y * ship_size};
            auto bullet1 = std::make_shared<Bullet>(bullet_pos1, position_t{direction1.x * bullet_speed, direction1.y * bullet_speed});

            auto direction2 = get_direction(angle - std::numbers::pi_v<float> / 9.0f);
            position_t bullet_pos2 = {_pos.x + direction2.x * ship_size, _pos.y - direction2.y * ship_size};
            auto bullet2 = std::make_shared<Bullet>(bullet_pos2, position_t{direction2.x * bullet_speed, direction2.y * bullet_speed});
            
            info.drawable.push_back(bullet1);
            info.updatable.push_back(bullet1);
            info.drawable.push_back(bullet2);
            info.updatable.push_back(bullet2);
        }
        
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
        _pos.x += speed.x * info.dt;
        _pos.y -= speed.y * info.dt;

        if (_pos.x > info.right_border) {
            _pos.x -= info.right_border;
        }
        if (_pos.x < 0.0f) {
            _pos.x += info.right_border;
        }
        if (_pos.y > info.bot_border) {
            _pos.y -= info.bot_border;
        }
        if (_pos.y < 0.0f) {
            _pos.y += info.bot_border;
        }

        _graphics->set_position(_pos);
        _engine_graphics[0]->set_position(get_engine_graphics_pos());
        _invincibility_graphics->set_position(_pos);
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
