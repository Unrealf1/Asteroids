#include "Engine.h"
#include <stdlib.h>
#include <memory.h>

#include <stdio.h>

#include "Render.hpp"
#include "Primitives.hpp"
#include "Ship.hpp"
#include "Commons.hpp"
#include "Space.hpp"

#include <iostream>
#include <memory>

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, VK_RETURN)
//
//  schedule_quit_game() - quit game after act()


static obj_container<Drawable> drawable_objects;
static obj_container<Updatable> updatable_objects;

Renderer& get_renderer() {
  static Renderer r(&buffer[0][0], SCREEN_HEIGHT, SCREEN_WIDTH, colors::black);
  return r;
}

// initialize game data in this function
void initialize() {
  std::cout << "initializing...\n";

  auto& r = get_renderer();

  //drawable_objects.push_back(std::make_shared<Square>(2.0f, position_t{50.0f, 50.0f}));
  drawable_objects.push_back(std::make_shared<Space>(150));

  auto ship = std::make_shared<Ship>(position_t{50.0f, 50.0f});
  drawable_objects.push_back(ship);
  updatable_objects.push_back(ship);

  auto ship2 = std::make_shared<Ship>(position_t{57.0f, 52.0f});
  drawable_objects.push_back(ship2);
  updatable_objects.push_back(ship2);

  auto ship3 = std::make_shared<Ship>(position_t{43.0f, 52.0f});
  drawable_objects.push_back(ship3);
  updatable_objects.push_back(ship3);


}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt) {
  static uint64_t counter = 0;
  static float since_last;
  since_last += dt;
  if (counter % 100 == 0) {
      auto fps = 1.0f / (since_last / 100.0f);
      std::cout << "fps: " << fps << '\n';
      std::cout << "updatable size: " << updatable_objects.size() << '\n';
      since_last = 0.0f;
  }

  if (is_key_pressed(VK_ESCAPE)) {
    schedule_quit_game();
  }

  const auto rem_dr = std::ranges::remove_if(drawable_objects, &Drawable::dr_useless);
  drawable_objects.erase(rem_dr.begin(), rem_dr.end());
  const auto rem_upd = std::ranges::remove_if(updatable_objects, &Updatable::upd_useless);
  updatable_objects.erase(rem_upd.begin(), rem_upd.end());

  obj_container<Drawable> drawable_objects_addition;
  obj_container<Updatable> updatable_objects_addition;
  updateinfo info{dt, drawable_objects_addition, updatable_objects_addition};
  for (auto& obj : updatable_objects) {
    obj->update(info);
  }

  std::ranges::move(drawable_objects_addition, std::back_inserter(drawable_objects));
  std::ranges::move(updatable_objects_addition, std::back_inserter(updatable_objects));
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  //memset(buffer, colors::red, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

  // for (int i = 0; i < SCREEN_HEIGHT; ++i) {
  //   for (int j = 0; j < SCREEN_WIDTH; ++j) {
      
  //     buffer[i][j] = colors::blue; //+ j*512;
  //     //std::cout << std::hex << buffer[i][j] << '\n';
  //   }
  // }

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

