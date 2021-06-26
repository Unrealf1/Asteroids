#include "Engine.h"
#include <stdlib.h>
#include <memory.h>

#include <stdio.h>

#include "Render.hpp"
#include "Primitives.hpp"
#include "Ship.hpp"

#include <iostream>
#include <memory>

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, VK_RETURN)
//
//  schedule_quit_game() - quit game after act()

static std::vector<std::shared_ptr<Drawable>> drawable_objects;
static std::vector<std::shared_ptr<Updatable>> updatable_objects;

Renderer& get_renderer() {
  static Renderer r(&buffer[0][0], SCREEN_HEIGHT, SCREEN_WIDTH, colors::white);
  return r;
}

// initialize game data in this function
void initialize() {
  std::cout << "initializing...\n";

  auto& r = get_renderer();

  //drawable_objects.push_back(std::make_shared<Square>(2.0f, position_t{50.0f, 50.0f}));
  
  auto ship = std::make_shared<Ship>(position_t{50.0f, 50.0f});
  drawable_objects.push_back(ship);
  updatable_objects.push_back(ship);

}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt) {
  if (is_key_pressed(VK_ESCAPE)) {
    schedule_quit_game();
  }
  updateinfo info{dt};
  for (auto& obj : updatable_objects) {
    obj->update(info);
  }
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  // memset(buffer, uint32_t(0xFF0000u), SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
  // buffer[0][0] = 0xFFF0000;
  // for (int i = 0; i < 20; ++i) {
  //   for (int j = 0; j < SCREEN_WIDTH; ++j) {
  //     buffer[i][j] = 0xFF0000u;
  //   }
  // }
  // std::cout << uint32_t(buffer[0][0]) << std::endl;
  Renderer& renderer = get_renderer();
  renderer.clear();

  for (auto& obj : drawable_objects) {
    obj->draw(renderer);
  }

  //memset(buffer, 0b11111111101010101010000000000000, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
}

// free game data in this function
void finalize() {
}

