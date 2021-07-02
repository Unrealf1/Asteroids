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
#include "GUI.hpp"
#include "PowerUps.hpp"

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
static obj_container<GUI> gui;
static obj_container<PowerUp> powerups;

Renderer& get_renderer() {
  static Renderer r(&buffer[0][0], SCREEN_HEIGHT, SCREEN_WIDTH, colors::black);
  return r;
}

static uint64_t score = 0;
static uint64_t lives = 3;

// initialize game data in this function
void initialize() {
  //std::cout << "initializing...\n";

  auto& r = get_renderer();

  drawable_objects.push_back(std::make_shared<Space>(150));

  auto ship = std::make_shared<Ship>(position_t{50.0f, 50.0f});
  drawable_objects.push_back(ship);
  updatable_objects.push_back(ship);

  auto score = std::make_shared<Score>();
  gui.push_back(score);
  updatable_objects.push_back(score);
  auto lives = std::make_shared<Lives>();
  gui.push_back(lives);
  updatable_objects.push_back(lives);
  auto fps = std::make_shared<FPS>();
  gui.push_back(fps);
  updatable_objects.push_back(fps);
}

std::shared_ptr<Asteroid> createAsteroid(uint64_t counter) {
    counter % 2 ? counter : (counter/10);
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

    float speed_value = (static_cast<float>(counter % 9 + 4) / 10.0f) * 20.0f;
    position_t speed = get_direction(direction_angle, speed_value);
    speed.y *= -1.0f;
    auto rot_speed = static_cast<float>(counter % 20);
    uint32_t num_parts = counter % 4 + 1;
    return std::make_shared<Asteroid>(spawn_position, speed, rot_speed, num_parts);
}

std::shared_ptr<PowerUp> createPowerup(uint64_t counter) {
    // counter % 10 = 0
    counter /= 10;
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

    float speed_value = (static_cast<float>(counter % 9 + 4) / 10.0f) * 7.0f;
    position_t speed = get_direction(direction_angle, speed_value);
    speed.y *= -1.0f;
    powerup_type type;

    switch (counter % 3) {
    case 0:
      type = powerup_type::health;
      break;
    case 1:
      type = powerup_type::invincibility;
      break;
    case 2:
      type = powerup_type::double_cannon;
      break;
    
    default:
      type = powerup_type::health;
      break;
    }

    return std::make_shared<PowerUp>(spawn_position, speed, type);
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt) {
  //quit on escape
  if (is_key_pressed(VK_ESCAPE)) {
    schedule_quit_game();
  }

  // some settings
  constexpr uint32_t asteroids_per_level = 10;
  constexpr uint32_t points_per_level = 10;
  constexpr uint32_t fps_rate = 150u; // once per how many frames fps is calculated
  constexpr float pause_between_levels = 3.0f;

  auto& renderer = get_renderer();

  // setup static variables
  static bool pause = false; // is game paused
  static uint32_t level = 1; // current level
  static uint32_t asteroids_left_on_level = asteroids_per_level * level;
  static uint64_t counter = 0; // how many times this function was called. used for pseudorandom
  static float since_last_output; // time elapsed since last fps calculation
  static float fps = 0.0f; // current fps
  static float left_for_pause = 0.0f; // how much time left for game to be paused
  static bool finished = false;
  static float since_last_asteroid = 0.0f; // time since last asteroid was created
  

  if (finished) {
    return;
  }

  // update static variables
  ++counter;
  since_last_asteroid += dt;
  since_last_output += dt;

  if (left_for_pause > 0.0f) {
    pause = true;
    left_for_pause -= dt;
  } else {
    pause = false;
  }

  // calculate fps
  if (counter % fps_rate == 0) {
      fps = 1.0f / (since_last_output / static_cast<float>(fps_rate));
      since_last_output = 0.0f;
  }

  // finish the game
  if (lives == 0) {
    finished = true;

    float text_height = renderer.get_height() / 2.0f - 10.0f;
    float text_size = 9.0f;

    // ALL YOUR BASE ARE BELONG TO US
    auto game_over = std::make_shared<Text>("GAME OVER", position_t{10.0f, text_height}, text_size, colors::white);
    drawable_objects.push_back(game_over);
    text_height += text_size * 1.5f;

    text_size = 4.0f;
    auto score_text = std::make_shared<Text>("YOUR SCORE - " + std::to_string(score), position_t{10.0f, text_height}, text_size, colors::white);
    drawable_objects.push_back(score_text);
    text_height += text_size * 1.5f;

    auto level_text = std::make_shared<Text>("YOUR LEVEL - " + std::to_string(level), position_t{10.0f, text_height}, text_size, colors::white);
    drawable_objects.push_back(level_text);
    text_height += text_size * 1.5f;

    text_size = 2.0f;
    auto quit_text = std::make_shared<Text>("PRESS ESC TO QUIT", position_t{30.0f, text_height}, text_size, 0xeeeeee);
    drawable_objects.push_back(quit_text);
  }

  // remove all useless objects
  const auto rem_dr = std::ranges::remove_if(drawable_objects, &Drawable::dr_useless);
  drawable_objects.erase(rem_dr.begin(), rem_dr.end());
  const auto rem_upd = std::ranges::remove_if(updatable_objects, &Updatable::upd_useless);
  updatable_objects.erase(rem_upd.begin(), rem_upd.end());
  const auto rem_ast = std::ranges::remove_if(asteroids, &Updatable::upd_useless);
  asteroids.erase(rem_ast.begin(), rem_ast.end());
  const auto rem_pw = std::ranges::remove_if(powerups, &Updatable::upd_useless);
  powerups.erase(rem_pw.begin(), rem_pw.end());

  // actual update
  if (!pause) {
    // temporary containers for new objects
    obj_container<Drawable> drawable_objects_addition;
    obj_container<Updatable> updatable_objects_addition;
    updateinfo info{
      pause ? 0.0f : dt,
      counter,
      drawable_objects_addition, 
      updatable_objects_addition, 
      asteroids,
      powerups,
      score,
      lives,
      renderer.get_width(),
      renderer.get_height(),
      fps,
      level
    };

    for (auto& obj : updatable_objects) {
      obj->update(info);
    }

    std::ranges::move(drawable_objects_addition, std::back_inserter(drawable_objects));
    std::ranges::move(updatable_objects_addition, std::back_inserter(updatable_objects));
  }
  

  float asteroid_creation_rate = std::max(0.85f / static_cast<float>(level), 0.1f);
  if (since_last_asteroid >= asteroid_creation_rate && asteroids_left_on_level > 0 && !pause) {
    asteroids_left_on_level--;
    since_last_asteroid = 0.0f;

    if (counter % 10 == 0) {
      auto pw = createPowerup(counter);
      drawable_objects.push_back(pw);
      updatable_objects.push_back(pw);
      powerups.push_back(pw);
    } else {
      auto ast = createAsteroid(counter);
      drawable_objects.push_back(ast);
      updatable_objects.push_back(ast);
      asteroids.push_back(ast);
    }

  }

  // level is cleared
  if (asteroids_left_on_level == 0 && asteroids.size() == 0) {
    left_for_pause = pause_between_levels;
    auto additional_points = points_per_level * level;
    score += additional_points;
    level++;
    asteroids_left_on_level = asteroids_per_level * level;
    auto level_text = std::make_shared<SelfDestructableDrawable>(
      std::move(std::make_unique<Text>("LEVEL " + std::to_string(level), position_t{10.0f, renderer.get_height() / 2.0f}, 9.0f, colors::white)),
      0.0f
    );

    auto points_text = std::make_shared<SelfDestructableDrawable>(
      std::move(std::make_unique<Text>("+" + std::to_string(additional_points) + " POINTS", position_t{10.0f, renderer.get_height() / 2.0f + 9.0f*1.5f}, 5.0f, colors::white)),
      0.0f
    );

    drawable_objects.push_back(level_text);
    updatable_objects.push_back(level_text);

    drawable_objects.push_back(points_text);
    updatable_objects.push_back(points_text);
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

  // draw gui separately, so it is always on top
  for (auto& obj : gui) {
    obj->draw(renderer);
  }
}

// free game data in this function
void finalize() {
}

