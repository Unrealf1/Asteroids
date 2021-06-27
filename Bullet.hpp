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
    }

    void update(const updateinfo& info) override {
        update_position(info);
    }

private:
    std::unique_ptr<InteractiveDrawable> _graphics;
    position_t _speed;

    void update_position(const updateinfo& info) {
        _pos.x += _speed.x;
        _pos.y -= _speed.y;
        _graphics->set_position(_pos);
    }


};
