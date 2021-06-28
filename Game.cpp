#include "Engine.h"
#include <stdlib.h>
#include <memory.h>

#include <stdio.h>

#include "Render.hpp"
#include "Primitives.hpp"
#include "Ship.hpp"
#include "Commons.hpp"
#include "Space.hpp"
#include "Asteroid.hpp"

#include <iostream>
#include <memory>
#include <numbers>

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, VK_RETURN)
//
//  schedule_quit_game() - quit game after act()


static obj_container<Drawable> drawable_objects;
static obj_container<Updatable> updatable_objects;
static obj_container<Asteroid> asteroids;

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
  ++counter;
  static float since_last_output;
  since_last_output += dt;
  if (counter % 100 == 0) {
      auto fps = 1.0f / (since_last_output / 100.0f);
      std::cout << "fps: " << fps << '\n';
      std::cout << "updatable size: " << updatable_objects.size() << '\n';
      since_last_output = 0.0f;
  }

  if (is_key_pressed(VK_ESCAPE)) {
    schedule_quit_game();
  }

  const auto rem_dr = std::ranges::remove_if(drawable_objects, &Drawable::dr_useless);
  drawable_objects.erase(rem_dr.begin(), rem_dr.end());
  const auto rem_upd = std::ranges::remove_if(updatable_objects, &Updatable::upd_useless);
  updatable_objects.erase(rem_upd.begin(), rem_upd.end());
  const auto rem_ast = std::ranges::remove_if(asteroids, &Updatable::upd_useless);
  asteroids.erase(rem_ast.begin(), rem_ast.end());

  obj_container<Drawable> drawable_objects_addition;
  obj_container<Updatable> updatable_objects_addition;
  updateinfo info{dt, drawable_objects_addition, updatable_objects_addition};
  for (auto& obj : updatable_objects) {
    obj->update(info);
  }

  std::ranges::move(drawable_objects_addition, std::back_inserter(drawable_objects));
  std::ranges::move(updatable_objects_addition, std::back_inserter(updatable_objects));

  static float since_last_asteroid = 0.0f;
  since_last_asteroid += dt;

  float asteroid_creation_rate = 0.7;
  if (since_last_asteroid >= asteroid_creation_rate && asteroids.size() < 20) {
    since_last_asteroid = 0.0f;
    auto r = get_renderer();
    float x = std::min(r.get_width(), static_cast<float>(counter % 150));
    float y = std::min(r.get_height(), static_cast<float>(counter % 100));
    position_t position = {x, y};
    float angle = static_cast<float>(counter % 360) / (2.0f * std::numbers::pi_v<float>);
    position_t speed = {static_cast<float>(counter % 10) * 0.1f + 0.2f, static_cast<float>((counter % 23) % 10) * 0.1f + 0.2f};
    auto direction = get_direction(angle);
    speed = {speed.x * direction.x, speed.y * direction.y};
    auto rot_speed = static_cast<float>(counter % 20);
    uint32_t num_parts = counter % 5;

    auto ast = std::make_shared<Asteroid>(position, speed, rot_speed, num_parts);

    std::cout << "created asteroid at position " << position.x << ' ' << position.y << '\n';

    drawable_objects.push_back(ast);
    updatable_objects.push_back(ast);
    asteroids.push_back(ast);
  }

}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw() {
  Renderer& renderer = get_renderer();
  renderer.clear();

  for (auto& obj : drawable_objects) {
    obj->draw(renderer);
  }
}

// free game data in this function
void finalize() {
}

