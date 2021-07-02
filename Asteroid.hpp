#pragma once

#include "Drawable.hpp"
#include "Primitives.hpp"
#include <memory>
#include <algorithm>


class Asteroid : public Drawable, public Updatable, public Movable, public Collidable {
public:
    Asteroid(position_t pos, position_t speed, float rotation_speed, uint32_t num_parts): Movable(pos), _speed(speed), _rotation_speed(rotation_speed) {
        for (uint32_t i = 0; i < num_parts; ++i) {
            float angle = static_cast<float>(i) / static_cast<float>(num_parts) * 2.0f * std::numbers::pi_v<float>;
            position_t offset = get_direction(angle, static_cast<float>(num_parts));
            _graphics.push_back(std::make_unique<Circle>(
                static_cast<float>(i % 3 + 1) * 2.0f, position_t{pos.x + offset.x, pos.y + offset.y}, 0xaaaaaa, 20));
            _graphic_positions.push_back(offset);
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
            //std::cout << "deleting asteroid as it is too far. x/y: " << _pos.x << " / " << _pos.y << '\n';
            _useless = true;
        }
    }

    void destroy() {
        if (_graphics.size() == 1) {
            _useless = true;
        } else {
            _split = true;
        }
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
        if (_split) {
            _graphics.pop_back();
            _graphic_positions.pop_back();
            _split = false;
        }

        update_position(info);
    }

private:
    std::vector<std::unique_ptr<InteractiveDrawable>> _graphics;
    std::vector<position_t> _graphic_positions;

    position_t _speed;
    float _rotation_speed;
    bool _useless = false;
    bool _split = false;

    void update_position(const updateinfo& info) {
        _pos.x += _speed.x;
        _pos.y -= _speed.y;

        for (uint32_t i = 0; i < _graphics.size(); ++i) {
            position_t pos = _graphic_positions[i];
            _graphics[i]->set_position({pos.x + _pos.x, pos.y + _pos.y});
        }
    }


};
