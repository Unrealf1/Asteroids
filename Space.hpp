#pragma once

#include "Drawable.hpp"
#include "Primitives.hpp"
#include <memory>


// struct Star {
//     position_t pos;
//     color_t col;
//     float size;
// };

class Space : public Drawable, public Updatable {
public:
    Space(uint32_t num_stars): _num_stars(num_stars) {
        for (uint32_t i = 0; i < num_stars; ++i) {
            auto color = i % 7 == 0 ? colors::red : (i % 5 == 0 ? colors::yellow : colors::white);
            float x = static_cast<float>(i % 100);
            float y = static_cast<float>(i * i % 100);

            if (i % 7 == 0) {
                x += 7.0f;
            }

            if (i % 13 == 0) {
                x -= 13.f;
            }

            if (i*i % 27 == 0) {
                y -= 5.0f;
                x += 7.0f;
            }

            stars.push_back({x, y, color});
        }
    }

    void draw(Renderer& renderer) override {
        renderer.render(stars, RenderMode::dots);
    }

    void update(const updateinfo& info) override {
        //update_position(info);
    }
private:
    uint32_t _num_stars;
    points_t stars;
};
