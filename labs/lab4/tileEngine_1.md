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

**Create a new project, copy paste the useful files frome the previous lab, add the main.cpp file, and add the new project to CMake.**

## The Goal

The Game we are about to make is a maze game. The player moves a purple circle around from a starting point and tries to get to a determined end point in the shortest amount of time.

<video class="middle" width="400" height="300" loop autoplay>
  <source src="assets/videos/maze_game.mp4" type="video/mp4">
</video>

The game will have two levels, each with a different maze and an ending message. To switch between the maze and the ending screen, we will use a **Scene** system.

The Maze will be loaded from a text file with a super simple format. The logic for loading, rendering, and managing the maze data will be handled by a standalone \"LevelSystem\" library that we will build. This means we can use this code again in future projects. While we are building libraries, we will also make a small Maths helper library to cover some of the functions that SFML doesn't provide with its vector class.

The maze file, along with all other resources can be found in the [Repo (assets/levels/maze_2.txt)](https://github.com/edinburgh-napier/set09121/tree/master/assets)

```
wwwwwwww
wswe   w
w wwww w
w      w
wwwwwwww
```
Second maze:
```
wwwwwwww
wsw    w
w w w  w
w   w ew
wwwwwwww
```

## Refactor GameSystem

This step is not essential to make this game, we could keep the structure of GameSystem from the previous practical. But let's refactor it a bit to make it more easy to use and prepare for the extensions of the next practicals. See this as one more step toward a clean and functional game engine.

Here is the new interface of GameSystem:
```cpp
//game_system.hpp
class Scene; //forward definition

class GameSystem{
public:
    static void start(unsigned int width, unsigned int height, 
                      const std::string& name);
    static void clean();
    static void reset();
    static void set_active_scene(const std::shared_ptr<Scene>& act_sc);

private:
    static void _init();
    static void _update(const float &dt);
    static void _render(sf::RenderWindow &window);
    static std::shared_ptr<Scene> _active_scene;
};

```
First, we change it from a struct to a class. This is really just convention as struct and class are the same in C++. We add a start method, where will put all the code we had in the main. Then we put init, update and render in private. These will be only called from inside GameSystem. It is a simplification of the interface. ```GameSystem() = delete``` means that this class cannot be instantiated. We will be using only static members. Finally, we have a pointer to the active scene to be displayed. Here, we use a raw pointer because it is not an actual object but a link to the scene object that need to be rendered.

The start function implementation is pretty much the same as the what had in the main.cpp.
```cpp
//game_system.cpp
...
void GameSystem::start(unsigned int width, unsigned int height, 
                      const std::string& name,const float &time_step){
    sf::RenderWindow window(sf::VideoMode({width, height}), name);    
    _init();
    sf::Event event;
    while(window.isOpen()){
      static sf::Clock clock;
      float dt = clock.restart().asSeconds();
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
          clean();
          return;
      }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
    }
    window.clear();
    _update(dt);
    _render(window);
    sf::sleep(sf::seconds(time_step));
    //Wait for Vsync
    window.display();         
  }
  window.close();
  clean();
}
...
```
I leave to you to refactor the rest. It is not much different from the previous practical.

The main.cpp should look like this now:
```cpp
#include "game_parameters.hpp"
#include "game_system.hpp"

using param = Parameters;

int main(){
  GameSystem::start(param::game_width,param::game_height,"Tile Maze",param::time_step);
  return 0;
}
```
Much shorter, isn't it!

## Scene Management

As mentioned above, we will have two 'scenes': the maze scene and ending scene. What is a Scene? Mainly it's a collection of Entities. All of the game logic will mainly be inside the Entities, but there will be some global game logic that needs to run, and the scene class is where it shall be.

The scene should also be responsible for loading and unloading content that it needs. This is where we could implement a loading screen. This would run while a scene loads, and then display the scene once finished. We shouldn't be working with anything that will need a loading screen, simply freezing for a few frames while we transition scenes is acceptable (for now ... hint hint).

Here we have our scene class. It has our usual two update and render functions, and an internal list of entities. The entity class is described in the next section. Additionally we have a load() function and a public getter to the entity list. 

```cpp
//game_system.hpp
class Scene {
public:
  Scene() = default;

  virtual ~Scene() = default;
  virtual void update(const float &dt);
  virtual void render(sf::RenderWindow &window);
  virtual void load() = 0;
  virtual void unload();
  std::vector<std::shared_ptr<Entity>> &get_entities(){return _entities;}

protected:
  std::vector<std::shared_ptr<Entity>> _entities;
};
```
And the implementation of render and update in the game_system.cpp:

```cpp
//game_system.cpp
void Scene::update(const float dt&){
  for(std::shared_ptr<Entity> &ent: _entities){
    _entities.update(dt);
  }
}

void Scene::render(sf::RenderWindow &window){
    for(std::shared_ptr<Entity> &ent: _entities){
      _entities.render(window);
    }
}
```
Before we can define any scene we will need entities. So let's define the Entity class and then the Player class that will inherit from it.

## Player Entity

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
public:
  Entity(std::unique_ptr<sf::Shape> shp);
  Entity() = delete;
  virtual ~Entity() = default;

  virtual void update(const float &dt);
  virtual void render(sf::RenderWindow &window) const = 0;

  const sf::Vector2f get_position();
  void set_position(const sf::Vector2f &pos);
  void move(const sf::Vector2f &pos);

protected:
  std::unique_ptr<sf::Shape> _shape;
  sf::Vector2f _position;
};
```

### Entity.cpp

The definitions in the .cpp don't do anything fancy. We are just covering the basic movement functions that we no longer have from inhering from sf::sprite. 

```cpp
//entity.cpp
#include "entity.hpp"

Entity::Entity(std::unique_ptr<sf::Shape> s) : _shape(std::move(s)) {}

const sf::Vector2f Entity::get_position() { return _position;}

void Entity::set_position(const sf::Vector2f &pos) { 
  _position = pos;
  _shape->set_position(sf::Vector2f(_position));
}

void Entity::move(const sf::Vector2f &pos) { _position += pos;}

void Entity::update(const float &dt) {}
```

### Player.h and Player.cpp

Nothing really to see here for the player class, just a standard
implementation of an Entity.

```cpp
//player.h
#pragma once
#include "entity.h"

class Player : public Entity {
public:
  void update(const float &dt) override;
  Player();
  void render(sf::RenderWindow &window) const override;

private:
  static constexpr float _radius = 25.f;
  static constexpr float _speed = 200.f;
};
```

For the .cpp, for now we will keep this basic and come back to it!

**You now need to implement code that will move the player around on screen in the Update function.** (Hint: you'll need to define inputs, use the speed, and remember to use dt! You can use the functions from Entity, too)

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
    : Entity(make_unique<CircleShape>(_radius_)) {
  _shape->setFillColor(Color::Magenta);
  _shape->setOrigin(sf::Vector2f(_radius, _radius_));
}

void Player::render(sf::RenderWindow &window) const {
  window.draw(*_shape);
}
```

## MazeScene

First, create a new header file called *scenes.hpp* where we will put all our scenes.

```cpp
//scenes.hpp
#include <string>
#include "game_system.hpp"

class MazeScene: public Scene{
public:
  MazeScene() = default;
  void update(const float &dt) override;
  void render(sf::RenderWindow &window) override;
  void load() override;
  void reset();
  void set_file_path(const std::string &file_path);
private:
  std::string _file_path;
};
```

```
**MazeScene** class inherit from **Scene** and we will override all the methods from **Scene**. This class just has two extra methods called **reset** and **set_file_path** to switch between mazes. The _file_path will store the path the maze.txt file.


Now, we need to implement our load function to instantiate our player object. And the reset function will just set the position of the player.
```cpp
void MazeScene::load(){
    std::shared_ptr<Entity> player = std::make_shared<Player>();
    _entities.push_back(player);
    reset();
    
}
void MazeScene::reset(){
    _entities[0]->set_position(...);
}
```

For now, the update and render methods will just call the update and render methods from Scene.

To have access to our scenes from anywhere in the code base. We will define and declare our scene as a static shared pointer to the scene put in a structure.

```cpp
//scenes.hpp
struct Scenes{
  static std::shared_ptr<Scene> maze;
}
```
As maze is a static member of a structure. We need to declare it in the cpp.
```cpp
std::shared_ptr<Scene> Scenes::maze;
```

Finally, the main function should look like this.
```cpp
//main.cpp
int main(){
  Scenes::maze = std::make_shared<MazeScene>();
  std::static_pointer_cast<MazeScene>(Scenes::maze)->set_file_path(param::maze_1);
  Scenes::maze->load();
  GameSystem::set_active_scene(Scenes::maze);
  GameSystem::start(param::game_width,param::game_height,"tile_maze");
  return 0;
}
```

{:class="important"}
**At this point you should have a magenta circle moving around a screen. Do not continue on if you haven't got everything working so far.**

Oh, and you can move diagonally... right?

## Sanity Check

Okay... but do you get what is going on right now? Because I bet a few of you are utterly confused. So, let's summarise this process a little, and have a brief chat about why it's important. Second thing first: why is it important?

Put simply, we can now create and manage scenes incredibly easily. If you've every made anything in a games engine you'll know how important scenes are, almost every single game is broken up into distinct scenes which have their own entities, sounds, textures, models etc. Often this'll be a menu scene and one scene per level, but it depends on how the game is made. But, in the end, they all have the same basic loop: load things, loop through updating and rendering until some end point, unload the things we loaded. Sound familiar? What did we define in our scene.cpp file? load(), update(), render()... but don't forget we have a constructor and deconstuctor too where we can unload things.

What's nice about the way we've done it, however, is that the main gameplay loop doesn't have to care about what scene is currently running - it just calls the right functions at the right time on whatever scene is currently active. What is double nice, is that to change which scene is running (i.e. to change from the menu to the game, or between levels) we just swap out which scene is currently 'active'... and that's really it. Everything else just works because of clever use of inheritance, polymorphism, and all those other nice OO things.

Previous step: [Space Invaders 2](space_invaders_2)

Next step: [Tile Engine 2](tile_engine_2)
