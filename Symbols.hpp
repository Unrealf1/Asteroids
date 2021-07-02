#pragma once

#include "Render.hpp"
#include "Primitives.hpp"


static void drawSymbol(char c, position_t pos, Renderer& renderer, color_t color = colors::white, float height = 1.0f, float width = 1.0f) {
    float left = pos.x - width / 2.0f;
    float right = pos.x + width / 2.0f;
    float top = pos.y - height / 2.0f;
    float bot = pos.y + height / 2.0f;

    Point left_top = {left, top, color};
    Point left_mid = {left, pos.y, color};
    Point left_bot = {left, bot, color};

    Point right_top = {right, top, color};
    Point right_mid = {right, pos.y, color};
    Point right_bot = {right, bot, color};

    switch (c) {
    case '0': {
        renderer.render(
            {
                left_top,
                left_bot,
                right_bot,
                right_top,
                left_top
            },
            RenderMode::line
        );
        Circle c(0.05f * std::min(width, height), pos, color, 5u);
        c.draw(renderer);

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
    case '-': {
        renderer.render(
            {
                left_mid,
                right_mid
            },
            RenderMode::line
        );
        break;
    }
    case 'A': {
        renderer.render(
            {
                left_bot,
                left_top,
                right_top,
                right_bot,
            },
            RenderMode::line
        );
        renderer.render(
            {
                left_mid,
                right_mid,
            },
            RenderMode::line
        );
        break;
    }
    case 'B': {
        renderer.render(
            {
                left_top,
                left_bot,
                right_bot,
                {right, pos.y + 0.1f * height, color},
                left_mid,
                {left, pos.y - 0.1f * height, color},
                right_top,
                left_top

            },
            RenderMode::line
        );
        break;
    }
    case 'C': {
        renderer.render(
            {
                right_top,
                left_top,
                left_bot,
                right_bot
            },
            RenderMode::line
        );
        break;
    }
    case 'D': {
        renderer.render(
            {
                left_top,
                left_bot,
                {right, bot - 0.1f * height, color},
                {right, top + 0.1f * height, color},
                left_top

            },
            RenderMode::line
        );
        break;
    }
    case 'E': {
        renderer.render(
            {
                right_top,
                left_top,
                left_bot,
                right_bot,
            },
            RenderMode::line
        );
        renderer.render(
            {
                left_mid, 
                right_mid
            },
            RenderMode::line
        );
        break;
    }
    case 'F': {
        renderer.render(
            {
                right_top,
                left_top,
                left_bot,
            },
            RenderMode::line
        );
        renderer.render(
            {
                left_mid, 
                right_mid
            },
            RenderMode::line
        );
        break;
    }
    case 'G': {
        renderer.render(
            {
                right_top,
                left_top,
                left_bot,
                right_bot,
                right_mid,
                {pos.x, pos.y, color}
            },
            RenderMode::line
        );
        break;
    }
    case 'H': {
        renderer.render(
            {
                left_top,
                left_bot,
            },
            RenderMode::line
        );
        renderer.render(
            {
                right_top,
                right_bot,
            },
            RenderMode::line
        );
        renderer.render(
            {
                left_mid,
                right_mid,
            },
            RenderMode::line
        );
        break;
    }
    case 'I': {
        float offset = 0.2f * width;
        renderer.render(
            {
                {pos.x - offset, top, color},
                {pos.x + offset, top, color},
            },
            RenderMode::line
        );
        renderer.render(
            {
                {pos.x - offset, bot, color},
                {pos.x + offset, bot, color},
            },
            RenderMode::line
        );
        renderer.render(
            {
                {pos.x, top, color},
                {pos.x, bot, color},
            },
            RenderMode::line
        );
        break;
    }
    case 'J': {
        renderer.render(
            {
                right_top,
                right_bot,
                left_bot
            },
            RenderMode::line
        );
        break;
    }
    case 'K': {
        renderer.render(
            {
                left_top,
                left_bot,
            },
            RenderMode::line
        );
        renderer.render(
            {
                left_mid,
                right_top,
            },
            RenderMode::line
        );
        renderer.render(
            {
                left_mid,
                right_bot,
            },
            RenderMode::line
        );
        break;
    }
    case 'L': {
        renderer.render(
            {
                left_top,
                left_bot,
                right_bot
            },
            RenderMode::line
        );
        break;
    }
    case 'M': {
        renderer.render(
            {
                left_bot,
                left_top,
                {pos.x, pos.y, color},
                right_top,
                right_bot
            },
            RenderMode::line
        );
        break;
    }
    case 'N': {
        renderer.render(
            {
                left_bot,
                left_top,
                right_bot,
                right_top
            },
            RenderMode::line
        );
        break;
    }
    case 'O': {
        renderer.render(
            {
                left_top,
                left_bot,
                right_bot,
                right_top,
                left_top
            },
            RenderMode::line
        );
        break;
    }
    case 'P': {
        renderer.render(
            {
                left_bot,
                left_top,
                right_top,
                right_mid,
                left_mid
            },
            RenderMode::line
        );
        break;
    }
    case 'Q': {
        auto almost_bot = bot - 0.15f * height;
        auto almost_right = right - 0.15f * width;
        renderer.render(
            {
                left_top,
                {left, almost_bot, color},
                {almost_right, almost_bot, color},
                {almost_right, top, color},
                left_top
            },
            RenderMode::line
        );
        renderer.render(
            {
                right_bot,
                {right - 0.3f * width, bot - 0.3f * height, color}
            },
            RenderMode::line
        );
        break;
    }
    case 'R': {
        renderer.render(
            {
                left_bot,
                left_top,
                right_top,
                right_mid,
                left_mid,
                right_bot
            },
            RenderMode::line
        );
        break;
    }
    case 'S': {
        renderer.render(
            {
                right_top,
                left_top,
                left_mid,
                right_mid,
                right_bot,
                left_bot
            },
            RenderMode::line
        );
        break;
    }
    case 'T': {
        renderer.render(
            {
                left_top,
                right_top,
            },
            RenderMode::line
        );
        renderer.render(
            {
                {pos.x, top, color},
                {pos.x, bot, color},
            },
            RenderMode::line
        );
        break;
    }
    case 'U': {
        renderer.render(
            {
                left_top,
                left_bot,
                right_bot,
                right_top
            },
            RenderMode::line
        );
        break;
    }
    case 'V': {
        renderer.render(
            {
                left_top,
                {pos.x, bot, color},
                right_top,
            },
            RenderMode::line
        );
        break;
    }
    case 'W': {
        renderer.render(
            {
                left_top,
                left_bot,
                {pos.x, top, color},
                right_bot,
                right_top,
            },
            RenderMode::line
        );
        break;
    }
    case 'X': {
        renderer.render(
            {
                left_top,
                right_bot,
            },
            RenderMode::line
        );
        renderer.render(
            {
                right_top,
                left_bot,
            },
            RenderMode::line
        );
        break;
    }
    case 'Y': {
        renderer.render(
            {
                left_top,
                {pos.x, pos.y, color},
                right_top,
            },
            RenderMode::line
        );
        renderer.render(
            {
                {pos.x, pos.y, color},
                {pos.x, bot, color}
            },
            RenderMode::line
        );
        break;
    }
    case 'Z': {
        renderer.render(
            {
                left_top,
                right_top,
                left_bot,
                right_bot
            },
            RenderMode::line
        );
        break;
    }
    case '.': {
        float rad = 0.05f;
        Circle c(rad * std::min(width, height), {left + rad*width, bot + rad*height}, color, 5u);
        c.draw(renderer);
        break;
    }
    case '+': {
        renderer.render(
            {
                {pos.x, top, color},
                {pos.x, bot, color},
            },
            RenderMode::line
        );
        renderer.render(
            {
                left_mid,
                right_mid
            },
            RenderMode::line
        );
        break;
    }
    default:            
        break;
    }
}