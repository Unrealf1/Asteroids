#pragma once

#include "Render.hpp"
#include "Commons.hpp"


class Updatable;
class Drawable;

struct updateinfo {
    float dt;
    obj_container<Drawable>& drawable;
    obj_container<Updatable>& updatable;
};

class Updatable {
public:
    virtual ~Updatable() = default;
    virtual void update(const updateinfo&) = 0;
};

class Drawable {
public:
    virtual void draw(Renderer&) = 0;

    virtual ~Drawable() = default;
};

using rotation_t = float; //angle in radians
using scale_t = float;


class Movable {
public:
    Movable(position_t position): _pos(position) {}

    virtual ~Movable() = default;

    virtual void move(const position_t& diff) {
        _pos.x += diff.x;
        _pos.y += diff.y;
    }

    virtual void set_position(const position_t& destination) {
        _pos = destination;
    }

    virtual position_t get_position() {
        return _pos;
    }

protected:
    position_t _pos;
};

class Rotatable {
public:

    Rotatable(rotation_t rotation): _rot(rotation){}
    virtual ~Rotatable() = default;

    virtual void rotate(rotation_t rotation) {
        _rot += rotation;
    }

    virtual void set_rotation(rotation_t rotation) {
        _rot = rotation;
    }

    virtual rotation_t get_rotation() {
        return _rot;
    }

protected:
    rotation_t _rot;

};

class Scalable {
public:

    Scalable(scale_t scale): _scale(scale){}
    virtual ~Scalable() = default;

    virtual void set_scale(scale_t scale) {
        _scale = scale;
    }

    virtual scale_t get_scale() {
        return _scale;
    }

protected:
     scale_t _scale;

};

class InteractiveDrawable : public Drawable, public Movable, public Rotatable, public Scalable {
public:
    virtual ~InteractiveDrawable() = default;
    InteractiveDrawable(position_t center_position, float angle, float scale): Movable(center_position), Rotatable(0.0f), Scalable(scale) {}
};
