#pragma once

#include "Drawable.hpp"
#include <iostream>
#include <memory>


class group: public Drawable {
public:
    void add(std::unique_ptr<Drawable>&& other) {

    }
private:
};

class Square : public InteractiveDrawable {
public:
    Square(float side_len, position_t center_position, color_t color = colors::black): 
    InteractiveDrawable(center_position, 0.0f, side_len) { }

    void draw(Renderer& renderer) override {
        auto offset = _scale / 2.0f;
        
        auto left = _pos.x - offset;
        auto right = _pos.x + offset;

        auto top = _pos.y - offset;
        auto bot = _pos.y + offset;


        renderer.render(
            {
                {left, top, _col},
                {left, bot, _col},
                {right, bot, _col},
                {right, top, _col},
                {left, top, _col}
            }, 
            RenderMode::line);
    }

private:
    color_t _col;
};