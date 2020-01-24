---
title: "Tile Engine 1"
keywords: Tile Engine
tags: [TileEngine]
permalink:  tile_engine_1.html
summary: tiles
sidebar: home_sidebar
---

# Get Started

This lab is design to introduce you to: Writing C++ Helper Libraries, Tile Based logic and 2D coordinate code.
We will be starting this practical from the usual game loop basis. 

Create the main.cpp, and add the new project to CMake.

## The Goal

The Game we are about to make is a maze game. The player moves a purple circle around from a starting point and tries to get to a determined end point in the shortest amount of time.

<video class="middle" width="400" height="300" loop autoplay>
  <source src="assets/videos/maze_game.mp4" type="video/mp4">
</video>


The Maze will be loaded from a text file with a super simple format. The logic for loading, rendering, and managing the maze data will be handled by a standalone \"LevelSystem\" library that we will build. This means we can use this code again in future projects. While we are building libraries, we will also make a small Maths helper library to cover some of the functions that SFML doesn't provide with its vector class.

The maze file, along with all other resources can be found in the [Repo (assets/levels/maze_2.txt)](https://github.com/edinburgh-napier/set09121/tree/master/assets)

```
wwwwwwww
wswe   w
w wwww w
w      w
wwwwwwww
```

## Step One - Player Entity

While we won't have any need for complex inheritance or large software patterns, for this practical we are still going to follow the Entity Model. This will be a slight change form Space invaders in that we are not going to inherit from an SFML class.

### Entity.h and Entity.cpp

For this game we will be working with sf::shapes rather than sf::sprites. They are both sibling classes that derive from sf::drawable, but are incompatible with each other (you can't allocate a shape with a sprite.)

Our base Entity class will be abstract, due to having a pure virtual Render() function.

The default constructor is also deleted, meaning the only way to construct it is through the constructor that takes a sf::shape. Meaning that all classes that derive from Entity must provide an sf::shape when they are constructed.

```cpp 
//entity.h
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Entity {
protected:
  std::unique_ptr<sf::Shape> _shape;
  sf::Vector2f _position;
  Entity(std::unique_ptr<sf::Shape> shp);

public:
  Entity() = delete;
  virtual ~Entity() = default;

  virtual void Update(const double dt);
  virtual void Render(sf::RenderWindow &window) const = 0;

  const sf::Vector2f getPosition();
  void setPosition(const sf::Vector2f &pos);
  void move(const sf::Vector2f &pos);
};
```

#### Smart Pointers

An important difference here is that the introduction of smart pointers (std::unique_ptr). So far we have been using "Raw" pointers, and creating them with New(), and deleting them with Delete().

Smart pointers use reference counting to know where a pointer is stored. If it isn't stored anywhere, the reference count is 0 and the smart pointer deletes the memory associated with the pointer. This means that we don't have to call Delete() ourselves, as we would with raw pointers.

This is similar but different to "Garbage Collection" in Java. Smart pointers de-allocates memory when the last reference goes out of scope. This is deterministic, and so you can know when it will happen. With Java, the garbage collection process runs separately and deallocated memory based on internal algorithms -- separate from your code.

### Entity.cpp

The definitions in the .cpp don't do anything fancy. We are just covering the basic movement functions that we no longer have from inhering from sf::sprite. Notice the Update function in particular. This is how we synchronise the state of the entity to SFML.

```cpp
//entity.cpp
#include "entity.h"
using namespace std;
using namespace sf;

const Vector2f Entity::getPosition() { return _position;}

void Entity::setPosition(const Vector2f &pos) { _position = pos;}

void Entity::move(const Vector2f &pos) { _position += pos;}

void Entity::Update(const double dt) {
    _shape->setPosition(_position);
}

Entity::Entity(unique_ptr<Shape> s) : _shape(std::move(s)) {}
```

### Player.h and Player.cpp

Nothing really to see here for the player class, just a standard
implementation of an Entity.

```cpp
//player.h
#pragma once
#include "entity.h"

class Player : public Entity {
 private:
  float _speed;

 public:
  void update(double dt) override;
  Player();
  void render(sf::RenderWindow &window) const override;
};
```

For the .cpp, for now we will keep this basic and come back to it.
Implement code that will move the player around on screen in the update.

```cpp
//player.cpp
#include "player.h"
using namespace sf;
using namespace std;

void Player::update(double dt) {
  //Move in four directions based on keys
  ...
  
  Entity::update(dt);
}

Player::Player()
    : _speed(200.0f), Entity(make_unique<CircleShape>(25.f)) {
  _shape->setFillColor(Color::Magenta);
  _shape->setOrigin(Vector2f(25.f, 25.f));
}

void Player::render(sf::RenderWindow &window) const {
  window.draw(*_shape);
}
```

Finally, you will need to create a player object, and link it into your update and render code from your game loop in order for it to show!


{:class="important"}
**At this point you should have a magenta circle moving around a screen. Do not continue on if you haven't got everything working so far.**


Previous step: [Space Invaders 2](space_invaders_2)

Next step: [Tile Engine 2](tile_engine_2)
