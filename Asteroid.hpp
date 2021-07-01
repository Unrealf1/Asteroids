#pragma once

#include "Drawable.hpp"
#include "Primitives.hpp"
#include <memory>
#include <algorithm>


class Asteroid : public Drawable, public Updatable, public Movable, public Collidable {
public:
    Asteroid(position_t pos, position_t speed, float rotation_speed, uint32_t num_parts): Movable(pos), _speed(speed), _rotation_speed(rotation_speed) {
        
        //_graphics.push_back(std::make_unique<Circle>(5.0f, position_t{0.0f, 0.0f}, colors::white, 20));
        for (uint32_t i = 0; i < num_parts; ++i) {
            _graphics.push_back(std::make_unique<Circle>(
                static_cast<float>(i % 3 + 1) * 2.0f, position_t{pos.x + i, pos.y + i}, colors::white, 20));
        }
    }

    void draw(Renderer& renderer) override {
        
        for (auto& g : _graphics) {
            g->draw(renderer);
        }

        float useless_offset = 20.0f;
        if (
            _pos.x < -useless_offset || 
            _pos.x > renderer.get_width() + useless_offset ||
            _pos.y < -useless_offset ||
            _pos.y > renderer.get_height() + useless_offset
        ) {
            std::cout << "deleting asteroid as it is too far. x/y: " << _pos.x << " / " << _pos.y << '\n';
            _useless = true;
        }
    }

    void destroy() {
        _useless = true;
    }

    bool check_collision(const position_t& pos) override {
        return std::ranges::any_of(_graphics, [&](std::unique_ptr<InteractiveDrawable>& dr) -> bool{
            return dr->check_collision(pos);
        });
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
    std::vector<std::unique_ptr<InteractiveDrawable>> _graphics;
    position_t _speed;
    float _rotation_speed;
    bool _useless = false;

    void update_position(const updateinfo& info) {
        _pos.x += _speed.x;
        _pos.y -= _speed.y;

        for (auto& g : _graphics) {
            //g->move(_speed);
            g->set_position(_pos);
        }
    }


};
