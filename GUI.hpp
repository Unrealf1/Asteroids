#pragma once

#include "Drawable.hpp"


static void drawSymbol(char c, position_t pos, Renderer& renderer, color_t color = colors::white) {
    float left = pos.x - 0.5f;
    float right = pos.x + 0.5f;
    float top = pos.y - 0.5f;
    float bot = pos.y + 0.5f;

    Point left_top = {left, top, color};
    Point left_mid = {left, pos.y, color};
    Point left_bot = {left, bot, color};

    Point right_top = {right, top, color};
    Point right_mid = {right, pos.y, color};
    Point right_bot = {right, bot, color};

    switch (c)
    {
    case '0': {
        Circle circ(0.5f, pos, color, 20);
        circ.draw(renderer);
        break;
    }
    case '1': {
        points_t points = {
            left_mid,
            right_top,
            right_bot
        };
        renderer.render(points, RenderMode::line);
        break;
    }
    case '2': {
        points_t points = {
            left_top,
            right_top,
            right_mid,
            left_bot,
            right_bot
        };
        renderer.render(points, RenderMode::line);
        break;
    }
    case '3': {
        points_t points = {
            left_top,
            right_top,
            left_mid,
            right_mid,
            left_bot
        };
        renderer.render(points, RenderMode::line);
        break;
    }
    case '4': {
        points_t points = {
            left_top,
            left_mid,
            right_mid,
            right_top,
            right_bot
        };
        renderer.render(points, RenderMode::line);
        break;
    }
    case '5': {
        points_t points = {
            right_top,
            left_top,
            left_mid,
            right_mid,
            right_bot,
            left_bot
        };
        renderer.render(points, RenderMode::line);
        break;
    }
    case '6': {
        points_t points = {
            right_top,
            left_mid,
            left_bot,
            right_bot,
            right_mid,
            left_mid
        };
        renderer.render(points, RenderMode::line);
        break;
    }
    case '7': {
        points_t points = {
            left_top,
            right_top,
            left_mid,
            left_bot,
        };
        renderer.render(points, RenderMode::line);
        break;
    }
    case '8': {
        points_t points = {
            left_mid,
            left_bot,
            right_bot,
            right_mid,
            left_mid,
            left_top,
            right_top,
            right_mid
        };
        renderer.render(points, RenderMode::line);
        break;
    }
    case '9': {
        points_t points = {
            right_mid,
            left_mid,
            left_top,
            right_top,
            right_mid,
            left_bot
        };
        renderer.render(points, RenderMode::line);
        break;
    }
    default:            
        break;
    }
}

void drawText(const std::string& text, const position_t& pos, Renderer& renderer, color_t color = colors::white) {
    position_t current_pos = pos;
    for (char c : text) {
        drawSymbol(c, current_pos, renderer, color);
        current_pos.x += 1.5f;
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
        drawText(score_text, {left + 1.5f, (bot + top) / 2.0f}, renderer);
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
