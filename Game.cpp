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

  // auto ship2 = std::make_shared<Ship>(position_t{57.0f, 52.0f});
  // drawable_objects.push_back(ship2);
  // updatable_objects.push_back(ship2);

  // auto ship3 = std::make_shared<Ship>(position_t{43.0f, 52.0f});
  // drawable_objects.push_back(ship3);
  // updatable_objects.push_back(ship3);


}

std::shared_ptr<Asteroid> createAsteroid(uint64_t counter) {
    counter;
    auto r = get_renderer();
    float spawn_angle = static_cast<float>(counter % 360) / 360.0f * (2.0f * std::numbers::pi_v<float>);
    float direction_angle = spawn_angle + std::numbers::pi_v<float>;
    float direction_flaw = static_cast<float>(counter*counter*counter % 45) / 180.0f * std::numbers::pi_v<float>;
    if (counter % 2) {
      direction_angle += direction_flaw;
    } else {
      direction_angle -= direction_flaw;
    }

    static float spawn_offset = std::min(r.get_width(), r.get_height()) / 2.0f + 10.0f;
    auto spawn_dir = get_direction(spawn_angle, spawn_offset);
    position_t center = {r.get_width()/2.0f, r.get_height()/2.0f};

    position_t spawn_position = {center.x + spawn_dir.x, center.y + spawn_dir.y};

    float speed_value = (static_cast<float>(counter % 9 + 1.0f) / 10.0f) * 0.7f;
    position_t speed = get_direction(direction_angle, speed_value);
    speed.y *= -1.0f;
    auto rot_speed = static_cast<float>(counter % 20);
    uint32_t num_parts = counter % 5;
    std::cout << "Creating asteroid at position " << spawn_position.x << ' ' << spawn_position.y << '\n';
    std::cout << "Speed: " << speed.x << ' ' << speed.y << '\n';
    return std::make_shared<Asteroid>(spawn_position, speed, rot_speed, num_parts);
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
      //std::cout << "updatable size: " << updatable_objects.size() << '\n';
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
  updateinfo info{
    dt, 
    drawable_objects_addition, 
    updatable_objects_addition, 
    asteroids
  };

  for (auto& obj : updatable_objects) {
    obj->update(info);
  }

  std::ranges::move(drawable_objects_addition, std::back_inserter(drawable_objects));
  std::ranges::move(updatable_objects_addition, std::back_inserter(updatable_objects));

  static float since_last_asteroid = 0.0f;
  since_last_asteroid += dt;

  float asteroid_creation_rate = 0.7;
  if (since_last_asteroid >= asteroid_creation_rate && asteroids.size() < 15) {
    since_last_asteroid = 0.0f;
    auto ast = createAsteroid(counter);

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

