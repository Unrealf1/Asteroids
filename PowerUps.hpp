#pragma once

#include "Drawable.hpp"
#include "Primitives.hpp"


enum class powerup_type : uint64_t {
    health, invincibility, double_cannon, //shooting_speed
};

class PowerUp : public Drawable, public Updatable, public Movable, public Collidable {
public:
    PowerUp(position_t pos, position_t speed, powerup_type type): Movable(pos), _speed(speed), _type(type) {
        switch (type)
        {
        case powerup_type::health:
            _graphics = std::make_unique<EquilateralTriangle>(_size, pos, colors::red);
            break;
        case powerup_type::invincibility:
            _graphics = std::make_unique<Circle>(_size / 2.0f, pos, colors::blue);
            break;
        case powerup_type::double_cannon:
            _graphics = std::make_unique<Square>(_size, pos, colors::yellow);
            break;
        }
    }

    void draw(Renderer& renderer) override {
        
        _graphics ->draw(renderer);

        float useless_offset = 20.0f;
        if (
            _pos.x < -useless_offset || 
            _pos.x > renderer.get_width() + useless_offset ||
            _pos.y < -useless_offset ||
            _pos.y > renderer.get_height() + useless_offset
        ) {
            //std::cout << "deleting powerup as it is too far. x/y: " << _pos.x << " / " << _pos.y << '\n';
            _useless = true;
        }
    }

    powerup_type get_type() {
        return _type;
    }

    void destroy() {
        _useless = true;
    }

    bool check_collision(const position_t& pos) override {
        return (positions_close(pos, _pos, _size));
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
    bool _useless = false;
    float _size = 3.0f;
    position_t _speed;
    powerup_type _type;

    void update_position(const updateinfo& info) {
        _pos.x += _speed.x;
        _pos.y -= _speed.y;

        _graphics->set_position(_pos);
    }
};