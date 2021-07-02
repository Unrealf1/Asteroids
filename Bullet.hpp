#pragma once

#include "Drawable.hpp"
#include "Primitives.hpp"
#include <memory>
#include "Asteroid.hpp"


class Bullet : public Drawable, public Updatable, public Movable {
public:
    Bullet(position_t pos, position_t speed, float livetime = 0.3f): Movable(pos), _speed(speed), _time_left(livetime) {
        _graphics = std::make_unique<Square>(0.5f, pos, colors::yellow);
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
        for (auto& ast : info.asteroids) {
            if (ast->check_collision(_pos)) {
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

    void update_position(const updateinfo& info) {
        _pos.x += _speed.x;
        _pos.y -= _speed.y;

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
