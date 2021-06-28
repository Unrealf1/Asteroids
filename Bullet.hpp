#pragma once

#include "Drawable.hpp"
#include "Primitives.hpp"
#include <memory>


class Bullet : public Drawable, public Updatable, public Movable {
public:
    Bullet(position_t pos, position_t speed): Movable(pos), _speed(speed) {
        _graphics = std::make_unique<Square>(0.5f, pos, colors::yellow);
    }

    void draw(Renderer& renderer) override {
        _graphics->draw(renderer);
        float useless_offset = 20.0f;
        if (
            _pos.x < -useless_offset || 
            _pos.x > renderer.get_width() + useless_offset ||
            _pos.y < -useless_offset ||
            _pos.y > renderer.get_height() + useless_offset
        ) {
            _useless = true;
        }
    }

    bool upd_useless() override {
        return _useless;
    }

    bool dr_useless() override {
        return _useless;
    }

    void update(const updateinfo& info) override {
        update_position(info);
    }

private:
    std::unique_ptr<InteractiveDrawable> _graphics;
    position_t _speed;
    bool _useless = false;

    void update_position(const updateinfo& info) {
        _pos.x += _speed.x;
        _pos.y -= _speed.y;
        _graphics->set_position(_pos);
    }


};
