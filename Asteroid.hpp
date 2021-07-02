#pragma once

#include "Drawable.hpp"
#include "Primitives.hpp"
#include <memory>
#include <algorithm>


class Asteroid : public Drawable, public Updatable, public Movable, public Collidable {
    using graphics_t = std::vector<std::unique_ptr<InteractiveDrawable>>;
public:
    Asteroid(position_t pos, position_t speed, float rotation, uint32_t num_parts): Movable(pos), _speed(speed), _rotation(rotation) {
        for (uint32_t i = 0; i < num_parts; ++i) {
            float angle = static_cast<float>(i) / static_cast<float>(num_parts) * 2.0f * std::numbers::pi_v<float>;
            position_t offset = get_direction(angle + rotation, static_cast<float>(num_parts));
            _graphics.push_back(std::make_unique<Circle>(
                static_cast<float>(i % 3 + 1) * 2.0f, position_t{pos.x + offset.x, pos.y + offset.y}, 0xaaaaaa, (i%3 + 3) * 5));
            _graphic_positions.push_back(offset);
        }
    }

    // constructor for internal use
    Asteroid(position_t pos, position_t speed, float rotation, graphics_t& parts, std::vector<position_t>& positions):
     Movable(pos), _speed(speed), _rotation(rotation), _graphics(std::move(parts)), _graphic_positions(std::move(positions)) {

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
            do_split(info);
        }

        update_position(info);
    }

private:
    graphics_t _graphics;
    std::vector<position_t> _graphic_positions;

    position_t _speed;
    float _rotation;
    bool _useless = false;
    bool _split = false;

    void update_position(const updateinfo& info) {
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

        for (uint32_t i = 0; i < _graphics.size(); ++i) {
            position_t pos = _graphic_positions[i];
            _graphics[i]->set_position({pos.x + _pos.x, pos.y + _pos.y});
        }
    }

    void do_split(const updateinfo& info) {
        uint64_t split_idx = info.counter % _graphics.size();
        split_idx = std::max(static_cast<uint64_t>(1), split_idx);
        graphics_t first_pack;
        std::vector<position_t> first_pack_pos;
        for (uint64_t i = 0; i < split_idx; ++i) {
            first_pack.push_back(std::move(_graphics[i]));
            first_pack_pos.push_back(_graphic_positions[i]);
        }

        graphics_t second_pack;
        std::vector<position_t> second_pack_pos;
        for (uint64_t i = split_idx; i < _graphics.size(); ++i) {
            second_pack.push_back(std::move(_graphics[i]));
            second_pack_pos.push_back(_graphic_positions[i]);
        }

        float angle_diff = static_cast<float>(info.counter % 10 + 1) / 10.0f * std::numbers::pi_v<float> / 2.0f;

        float old_speed_val = std::sqrt(_speed.x * _speed.x + _speed.y * _speed.y);

        auto dir1 = get_direction(angle_diff, old_speed_val / 2.0f);
        auto dir2 = get_direction(-angle_diff, old_speed_val / 2.0f);

        _speed = {_speed.x * 0.5f, _speed.y * 0.5f};

        position_t speed1 = {_speed.x + dir1.x, _speed.y + dir1.y};
        position_t speed2 = {_speed.x + dir2.x, _speed.y + dir2.y};

        auto ast1 = std::make_shared<Asteroid>(_pos, speed1, _rotation, first_pack, first_pack_pos);
        auto ast2 = std::make_shared<Asteroid>(_pos, speed2, _rotation, second_pack, second_pack_pos);

        info.asteroids.push_back(ast1);
        info.asteroids.push_back(ast2);
        info.drawable.push_back(ast1);
        info.drawable.push_back(ast2);
        info.updatable.push_back(ast1);
        info.updatable.push_back(ast2);

        _graphics.clear();
        _graphic_positions.clear();
        _split = false;
        _useless = true;
    }

};
