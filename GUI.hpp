#pragma once

#include "Drawable.hpp"
#include "Symbols.hpp"
#include <sstream>
#include <iomanip>


void drawText(const std::string& text, const position_t& pos, Renderer& renderer, color_t color = colors::white, float size = 1.0f) {
    position_t current_pos = pos;
    for (char c : text) {
        drawSymbol(c, current_pos, renderer, color, size, size);
        current_pos.x += 1.5f * size;
    }
}



class GUI : public Drawable, public Updatable {
public:
    virtual ~GUI() = default;
private:
};

class Score : public GUI {
public:
    void draw(Renderer& renderer) override {
        float left = 0.2f;
        float right = 20.0f;
        float top = 0.2f;
        float bot = 5.0f;

        auto frame_color = colors::white;

        points_t frame = {
            {left, top, frame_color},
            {left, bot, frame_color},
            {right, bot, frame_color},
            {right, top, frame_color}  
        };

        renderer.render(frame, RenderMode::cycle_line);

        auto score_text = std::to_string(_score);
        drawText(score_text, {left + 1.5f, (bot + top) / 2.0f}, renderer, colors::white,  1.0f);
    }

    virtual void update(const updateinfo& info) override {
        _score = info.score;
    }

private:
    uint64_t _score = 0;
};

class Lives : public GUI {
public:
    Lives() {
        _graphics = std::make_unique<EquilateralTriangle>(_ship_size, position_t{0.0f, 0.0f}, colors::green);
    }

    void draw(Renderer& renderer) override {
        float left = 0.2f;
        float right = 20.0f;
        float top = 5.2f;
        float bot = 10.0f;

        float center_y = (bot + top) / 2.0f;

        auto frame_color = colors::white;

        points_t frame = {
            {left, top, frame_color},
            {left, bot, frame_color},
            {right, bot, frame_color},
            {right, top, frame_color}  
        };

        float current_x = left + 1.5f;
        for (uint64_t i = 0; i < _lives; ++i) {
            _graphics->set_position({current_x, center_y});
            current_x += _ship_size + 1.5f;
            _graphics->draw(renderer);
        }
    }

    virtual void update(const updateinfo& info) override {
        _lives = info.lives;
    }

private:
    uint64_t _lives;
    std::unique_ptr<EquilateralTriangle> _graphics;
    float _ship_size = 2.0f;
};

class FPS : public GUI {
public:
    void draw(Renderer& renderer) override {
        if (!is_key_pressed(VK_RETURN)) {
            return;
        }

        position_t pos = {renderer.get_width() - 11.0f, 2.0f};

        drawText(_string_repr, pos, renderer, colors::white, 0.85f);
    }

    virtual void update(const updateinfo& info) override {
        // exact equality here, as fps updates realy rarely
        // and this is a check to see if it fps counter
        // was changesd at all
        if (_fps == info.fps) {
            return;
        }
        _fps = info.fps;

        std::stringstream ss;
        ss << "FPS " << std::setprecision(1) << std::fixed << _fps;
        _string_repr = ss.str();
    }

private:
    float _fps;
    std::string _string_repr = "FPS 0.0";
};

class Text: public Drawable {
public:
    Text(std::string text, position_t pos, float size, color_t color): _text(text), _pos(pos), _size(size), _col(color){}

    void draw(Renderer& renderer) override {
        drawText(_text, _pos, renderer, _col, _size);
    }
private:
    std::string _text;
    position_t _pos;
    float _size;
    color_t _col;
};
