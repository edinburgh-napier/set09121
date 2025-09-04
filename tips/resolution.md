---
title: "Resolution Scaling"
keywords: Resolution
tags: [Resolution]
permalink:  resolution.html
sidebar: home_sidebar
---


```cpp
#include "engine.h"
#include <SFML/Graphics/Sprite.hpp>
#include <system_renderer.h>
#include <system_resources.h>

using namespace std;
using namespace sf;

//Try this
//#define GAMEX 1280
//#define GAMEY 720

#define GAMEX 1000
#define GAMEY 1000

class MainScene : public Scene {

public:
  MainScene() = default;
  ~MainScene() = default;
  sf::Sprite sprite;
  sf::Texture tex;
  Vector2f target;
  sf::RectangleShape shapes[4];
  sf::Vertex background[4] = {sf::Vertex({0, 0}, sf::Color::Red),
                              sf::Vertex({0, GAMEY}, sf::Color::Magenta),
                              sf::Vertex({GAMEX, GAMEY}, sf::Color::Cyan),
                              sf::Vertex({GAMEX, 0}, sf::Color::Yellow)};

  void Load() override {
    tex = *Resources::load<Texture>("dat_boi.png");
    target = {640, 360};
    sprite.setPosition(500, 500);
    sprite.setOrigin(115, 64);
    sprite.setTexture(tex);
    shapes[0].setSize({GAMEX, 100});
    shapes[1].setSize({GAMEX, 100});
    shapes[2].setSize({100, GAMEY});
    shapes[3].setSize({100, GAMEY});
    shapes[0].setOrigin({GAMEX / 2, 50});
    shapes[1].setOrigin({GAMEX / 2, 50});
    shapes[2].setOrigin({50, GAMEY / 2 });
    shapes[3].setOrigin({50, GAMEY / 2});
    shapes[0].setPosition({GAMEX / 2, 50});
    shapes[1].setPosition({GAMEX / 2, GAMEY - 50});
    shapes[2].setPosition({50, GAMEY / 2});
    shapes[3].setPosition({GAMEX - 50, GAMEY / 2});
    for (auto& s : shapes) {
      s.setOutlineThickness(-1);
      s.setOutlineColor(sf::Color::Black);
    }
  }

  void Update(const double& dt) override {

    Scene::Update(dt);
    auto d = target - sprite.getPosition();
    if (length(d) < 64) {
      target = {100.f + (rand() % 800), 100.f + (rand() % 800)};
    }
    sprite.setScale(d.x > 0.f ? -1.f : 1.f, 1.f);
    sprite.move((float)dt * 100.0f * normalize(d));

    if (Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
      //  ------------------------------------------------- 
      Engine::GetWindow().setSize({1000, 1000});
      //  ------------------------------------------------- 
    } else if (Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
      //  ------------------------------------------------- 
      Engine::GetWindow().setSize({1280, 720});
      //  ------------------------------------------------- 
    } else if (Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
      //  ------------------------------------------------- 
      // Change viewport to match window size
      // Doesn't scale anything! But now we get clipping!
      Engine::GetWindow().setSize({1280, 720});
      sf::FloatRect visibleArea(0.f, 0.f, 1280, 720);
      Engine::GetWindow().setView(sf::View(visibleArea));
      //  ------------------------------------------------- 
    } else if (Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
      //  ------------------------------------------------- 
      // Change viewport to match window size
      Engine::GetWindow().setSize({1280, 720});
      sf::FloatRect visibleArea(0.f, 0.f, 1000, 1000);
      auto v = sf::View(visibleArea);

      // constrain 1000x1000 to 1280x720 & Maintain Aspect. so 720x720;
      const float widthScale(720.0 / 1280.0);
      v.setViewport(sf::FloatRect(0, 0, widthScale, 1.0f));
      Engine::GetWindow().setView(v);
      //  ------------------------------------------------- 
    } else if (Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
      //  ------------------------------------------------- 
      // All together now in a reusable solution.
      const sf::Vector2u screensize(1280, 720);
      const sf::Vector2u gamesize(GAMEX, GAMEY);
      //set View as normal
      Engine::GetWindow().setSize(screensize);
      sf::FloatRect visibleArea(0.f, 0.f, gamesize.x, gamesize.y);
      auto v = sf::View(visibleArea);
      // figure out how to scale and maintain aspect;
      auto viewport = CalculateViewport(screensize, gamesize);
      //Optionally Center it
      bool centered = true;
      if (centered) {
        viewport.left = (1.0 - viewport.width) * 0.5;
        viewport.top = (1.0 - viewport.height) * 0.5;
      }
      //set!
      v.setViewport(viewport);
      Engine::GetWindow().setView(v);
      //  ------------------------------------------------- 
    } else if (Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
      //  ------------------------------------------------- 
      // Have to Hold 6 Key for this to work magic
      static float t = 0;
      t += dt;
      const sf::Vector2u screensize((sin(t) + 1.3) *0.5 * 1000,
                                    (cos(t) + 1.3)* 0.5 * 800);

      const sf::Vector2u gamesize(GAMEX, GAMEY);
      Engine::GetWindow().setSize(screensize);
      sf::FloatRect visibleArea(0.f, 0.f, gamesize.x, gamesize.y);
      auto v = sf::View(visibleArea);
      auto viewport = CalculateViewport(screensize, gamesize);
      viewport.left = (1.0 - viewport.width) * 0.5;
      viewport.top = (1.0 - viewport.height) * 0.5;
      v.setViewport(viewport);
      Engine::GetWindow().setView(v);
    }
  }

  // Create FloatRect to fits Game into Screen while preserving aspect
  sf::FloatRect CalculateViewport(const sf::Vector2u& screensize,
                                  const sf::Vector2u& gamesize) {

    const Vector2f screensf(screensize.x, screensize.y );
    const Vector2f gamesf(gamesize.x, gamesize.y);
    const float gameAspect = gamesf.x / gamesf.y;
    const float screenAspect = screensf.x / screensf.y;
    float scaledWidth;  // final size.x of game viewport in piels
    float scaledHeight; //final size.y of game viewport in piels
    bool scaleSide = false; // false = scale to screen.x, true = screen.y

    //Work out which way to scale
    if (gamesize.x > gamesize.y) { // game is wider than tall
      // Can we use full width?
      if (screensf.y < (screensf.x / gameAspect)) {
        //no, not high enough to fit game height
        scaleSide = true;
      } else {
        //Yes, use all width available
        scaleSide = false;
      }
    } else { // Game is Square or Taller than Wide
      // Can we use full height?
      if (screensf.x < (screensf.y * gameAspect)) {
        // No, screensize not wide enough to fit game width
        scaleSide = false;
      } else {
        // Yes, use all height available
        scaleSide = true;
      }
    }

    if (scaleSide) { // use max screen height
      scaledHeight = screensf.y;
      scaledWidth = floor(scaledHeight * gameAspect);
    } else { //use max screen width
      scaledWidth = screensf.x;
      scaledHeight = floor(scaledWidth / gameAspect);
    }

    //calculate as percent of screen
    const float widthPercent = (scaledWidth / screensf.x);
    const float heightPercent = (scaledHeight / screensf.y);

    return sf::FloatRect(0, 0, widthPercent, heightPercent);
  }

  void Render() override {
    Engine::GetWindow().draw(background, 4, sf::Quads);
    for (const auto& s : shapes) {
      Renderer::queue(&s);
    }
    Renderer::queue(&sprite);
  }
};

MainScene scene;

int main() {
  Engine::Start(1000, 1000, "Deployed", &scene);
  return 0;
}
```