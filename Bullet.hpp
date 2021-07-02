#pragma once

#include "Drawable.hpp"
#include "Primitives.hpp"
#include "Asteroid.hpp"

#include <memory>
#include <array>


class Bullet : public Drawable, public Updatable, public Movable {
public:
    Bullet(position_t pos, position_t speed, float livetime = 0.3f): Movable(pos), _speed(speed), _time_left(livetime) {
        _graphics = std::make_unique<Square>(_bullet_size, pos, colors::yellow);
        _last_pos = _pos;
    }

    void draw(Renderer& renderer) override {
        _graphics->draw(renderer);
    }

    bool upd_useless() override {
        return _useless;
    }

    bool dr_useless() override {
        return _useless;
    }

    void update(const updateinfo& info) override {
        float offset = _bullet_size / 2.0f;


        std::array<position_t, 5> points {
            position_t{_pos.x - offset, _pos.y - offset},
            position_t{_pos.x - offset, _pos.y + offset},
            position_t{_pos.x + offset, _pos.y + offset},
            position_t{_pos.x + offset, _pos.y - offset},
            
            position_t{(_pos.x + _last_pos.x) / 2.0f, (_pos.y + _last_pos.y) / 2.0f}
        };
        for (auto& ast : info.asteroids) {
            auto collision_check = [&](const position_t& pos) -> bool { return ast->check_collision(pos); };
            bool collision = std::ranges::any_of(points, collision_check);
            if (collision) {
                ast->destroy();
                _useless = true;
                info.score += 1;
                break;
            }
        }

        update_position(info);

        _time_left -= info.dt;
        if (_time_left < 0.0f) {
            _useless = true;
        }
    }

private:
    std::unique_ptr<InteractiveDrawable> _graphics;
    position_t _speed;
    bool _useless = false;
    float _time_left; 
    float _bullet_size = 0.7f;
    position_t _last_pos;

    void update_position(const updateinfo& info) {
        _last_pos = _pos;
        _pos.x += _speed.x * info.dt;
        _pos.y -= _speed.y * info.dt;

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
    }


};
