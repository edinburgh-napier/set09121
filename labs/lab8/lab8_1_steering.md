---
title: "Lab 8 - AI: Steering"
keywords: lab8
tags: [lab8]
permalink:  lab8_1.html
summary: lab8
sidebar: home_sidebar
---


## AI: Steering and Pathfinding

<video class="middle" width="300" height="300" loop autoplay>
  <source src="assets/videos/steering.mp4" type="video/mp4">
</video>

We are now going to start the AI labs. The first one will focus on movement and the decision making around movement. This will involve defining steering behaviours and pathfinding. This lab will use the engine as it stands at the end of practical 7. That is, we will be using *scenes* and *components* to build our solution. You will need the standard set up for a new lab. Our addition to the CMakeLists.txt is as follows.

```cmake
file(GLOB_RECURSE SOURCES lab_7_steering/*.cpp lab_7_steering/*.h)
file(GLOB_RECURSE CMPNTS lab_7_steering/components/*.cpp lab_7_steering/components/*.h)
file(GLOB_RECURSE SCENES lab_7_steering/scenes/*.cpp lab_7_steering/scenes/*.h)
add_executable(LAB_7_STEERING ${SOURCES} ${CMPNTS} ${SCENES} ${RESOURCE_FILES})
target_link_libraries(LAB_7_STEERING lib_engine)
set(EXECUTABLES ${EXECUTABLES} LAB_7_STEERING)
```

You will also need the components from practical 7 copied across.

## Steering Behaviours


We are going to split the lab into two parts. First we will focus on steering behaviours. We will need to do some setup first to get a menu system up and running and using that to drive our high-level game logic.

### Setting Up

First, we will implement a menu scene like in the previous lab.

The `MenuScene` will display a text which will give two options:
1.  Steering Behaviour Demo
2.  Pathfinding Demo

The `update` checks which button is pressed and has the outline for the code to handle this change. Once we have our other scenes in place we will add the new lines of code.

Don't forget to add the `struct Scenes` with the list of scene available. For the moment, we will have only the menu.

And finally we need our `main.cpp` file. This is below.

```cpp
//"main.cpp"
#include "game_parameters.hpp"
#include "scenes.hpp"

using param = Parameters;

int main(){
    Scenes::menu = std::make_shared<MenuScene>();
    Scenes::menu->load();
    GameSystem::set_active_scene(Scenes::menu);
    GameSystem::start(param::game_width,param::game_height,"AI Movement",param::time_step,false);
    return 0;
}
```

You should be able to run this now and get the output shown
![Steering Lab Menu](assets/images/screen-steer-start.png)

### SteeringScene

Before moving onto steering behaviours properly let us set up our `SteeringScene`. We are going to have an object on the screen (represented by a red dot) that the player will move around using the arrow keys. To do this we will create a `KeyboardMovementComponent`. We will add this component in new files *ctrl_cmps.hpp* and *ctrl_cmps.cpp*. The definition and implementation are provided below. You should be able to understand the basic premise by now.

```cpp
//"ctrl_cmps.hpp"
#pragma once
#include "ecm.hpp"

// A component to allow basic movement behaviour
class KeyboardMovementComponent : public Component {
protected:
  // Speed we can travel
  float _speed;
  // Checks if the move is valid.
  bool valid_move(const sf::Vector2f&);

public:
  // Will check the keyboard and move the component's parent.
  void update(double) override;
  // Moves the component's parent.
  void move(const sf::Vector2f&);
  // Moves the component's parent.
  void move(float x, float y);
  // Component does not need rendered.
  void render() override {}
  // Used to create the component for an entity
  explicit KeyboardMovementComponent(Entity* p);

  KeyboardMovementComponent() = delete;
};
```

```cpp
//"ctrl_cmps.cpp"
#include "ai_cmps.hpp"
#include "game_parameters.hpp"
#include "game_system.hpp"

using gs = GameSystem;
using param = Parameters;

// Checks keyboard and moves the player.
void KeyboardMovementComponent::update(const float &dt) {
  sf::Vector2f direction(0.0f, 0.0f);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    direction.x -= 1.0f;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    direction.x += 1.0f;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    direction.y -= 1.0f;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    direction.y += 1.0f;
  }

  move(normalize(direction) * _speed * (float)dt);
}

// Initialises the component.
KeyboardMovementComponent::BasicMovementComponent(Entity* p)
    : _speed(100.0f), Component(p) {}

// Checks if the proposed move is valid.
bool KeyboardMovementComponent::valid_move(const sf::Vector2f& pos) {
  if (pos.x < 0.0f || pos.x > param::game_width ||
      pos.y < 0.0f || pos.y > param::game_height) {
    return false;
  }
  return true;
}

// Moves the component's parent.
void KeyboardMovementComponent::move(const sf::Vector2f& p) {
  auto new_pos = _parent->get_position() + p;
  if (valid_move(new_pos)) {
    _parent->set_position(new_pos);
  }
}

// Moves the component's parent.
void KeyboardMovementComponent::move(float x, float y) {
  move(sf::Vector2f(x, y));
}
```

Our `SteeringScene` will add an `Entity` with this component attached.

```cpp
//"scenes.hpp"
#pragma once
#include "game_system.hpp"

class SteeringScene: public Scene{
public:
    SteeringScene() = default;
    void update(const float &dt) override;
    void render() override;
    void load()override;
    void unload() override;
};
```

The key behaviour we can add at the moment will be defined in the `Load` method. Here we will create an `Entity` and add a `BasicMovementComponent`. `scene_steering.cpp` is given below.

```cpp
//"scene_steering.cpp"
#include "scenes.hpp"
#include "renderer.hpp"
#include "graphics_cmps.hpp"
#include "ai_cmps.hpp"
#include "game_parameters.hpp"

using param = Parameters;

std::shared_ptr<Scene> Scenes::menu;

void MenuScene::update(const float &dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){

    }
    Scene::update(dt);
}

void MenuScene::render() {
    Renderer::queue(&_text);
    Scene::render();
}

void MenuScene::load() {
    _font.loadFromFile("resources/fonts/RobotoMono-Regular.ttf");
    _text.setFont(_font);
    _text.setCharacterSize(60);
    _text.setString("Movement Demos\nPress 1 for Steering\nPress 2 for Pathfinding");
}

void MenuScene::unload(){}


void SteeringScene::load(){
  std::shared_ptr<Entity> player = make_entity();
  player->setPosition(sf::Vector2f(param::game_width / 2,
                               param::game_height / 2));
  std::share_ptr<ShapeComponent> s = player->addComponent<ShapeComponent>();
  s->set_shape<sf::CircleShape>(10.0f);
  s->get_shape().setFillColor(Color::Red);
  player->addComponent<KeyboardMovementComponent>();
}

void SteeringScene::unLoad() { 
    Scene::unload(); 
}

void SteeringScene::update(const float& dt) { 
    Scene::update(dt); 
}

void SteeringScene::render() { 
    Scene::render(); 
}
```

Then, we need to:
- add the new scene to our `Scenes` structure; 
- initialise the steering scene in main;
- and update the menu scene `update` function to switch to this scene when pressed 1.

Don't forget to update the source files in the CMakeLists.txt

You should now be able to run the game, select option 1 and control the red dot


### SteeringOutput
As discussed in the lecture, the first part we need is the `SteeringOutput` type. We will define this in `ai_cmps.hpp`. This file will contain all the components and elements related to AI. This is given below.

```cpp
//"ai_cmps.hpp"
#pragma once
#include <SFML/Graphics.hpp>

// Output from a steering behaviour.
struct SteeringOutput
{
    // Direction of travel.
    sf::Vector2f direction;
    // Rotation of travel.
    float rotation;
};
```

We defined these values in the lecture. They allow us to move or rotate on the basis of a steering behaviour.

### SteeringBehaviours

As we saw in the lecture, steering involves two behaviours: *seek* and *flee*. We are going to define them as static function in a struct so they can be reused easily.

```cpp
//"ai_cmps.hpp"
struct SteeringBehaviours{
    static SteeringOutput seek(const sf::Vector2f &,const sf::Vector2f &);
    static SteeringOutput flee(const sf::Vector2f &,const sf::Vector2f &);
};
```

The implementation of these two function are very easy. Below are their implementation, they are identical except for one line. I leave to you to fill the blank.

```cpp
SteeringOutput SteeringBehaviours::seek(const sf::Vector2f &target,const sf::Vector2f &self){
    auto length = [](const sf::Vector2f &v) -> float{
        return std::sqrt(v.x*v.x+v.y*v.y);
    };
    SteeringOutput steering;
    steering.direction = ...;
    steering.direction = steering.direction/length(steering.direction) ;
    steering.rotation = 0.0f;
    return steering;
}


SteeringOutput SteeringBehaviours::flee(const sf::Vector2f &target,const sf::Vector2f &self){
    auto length = [](const sf::Vector2f &v) -> float{
        return std::sqrt(v.x*v.x+v.y*v.y);
    };
    SteeringOutput steering;
    steering.direction = ...;
    steering.direction = steering.direction/length(steering.direction) ;
    steering.rotation = 0.0f;
    return steering;
}
```


### SteeringComponent

We will now add a new component that will control the AI movement. This we will call `SteeringComponent` and we will define it in `ai_cmps.hpp` as below.

```cpp
//"ai_cmps.h"
...

class SteeringComponent : public Component{
protected:
    Entity *_player;
    float _max_speed;
    bool valid_move(const sf::Vector2f&) const;
public:
    void update(const float &) override;
    void move(const sf::Vector2f&);
    void move(float x, float y);
    void render() override { }
    explicit SteeringComponent(Entity *p, Entity *player, float _max_speed);
    SteeringComponent() = delete;
};
```

The component is similar to the other movement ones we have defined and you could reuse those as well if you wanted. The key behaviour is really in `update`. This is defined in `ai_cmps.cpp` (as well as other methods). See this below.

```cpp
//"ai_cmps.cpp"
...

void SteeringComponent::update(const float &dt) {

    auto distance = [](const sf::Vector2f &a,const sf::Vector2f &b) -> float{
        return std::sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
    };
  // If target (player) is more than 100 pixels away seek
  if (distance(_parent->get_position(), _player->get_position()) > 100.0f) {
    SteeringOutput output = SteeringBehaviours::seek(_player->get_position(),_parent->get_position());
    move(output.direction*_max_speed * dt);
  }
  // If target (player) is less than 50 pixels away flee
  else if (distance(_parent->get_position(), _player->get_position()) <
           50.0f) {
    SteeringOutput output = SteeringBehaviours::flee(_player->get_position(),_parent->get_position());
    move(output.direction*_max_speed * dt);
  }
}

SteeringComponent::SteeringComponent(Entity* p, Entity* player, float max_speed)
    : _player(player),_max_speed(max_speed) , Component(p) {}

bool SteeringComponent::valid_move(const sf::Vector2f& pos) const {
  if (pos.x < 0.0f || pos.x > param::game_width ||
      pos.y < 0.0f || pos.y > param::game_height) {
    return false;
  }
  return true;
}

void SteeringComponent::move(const sf::Vector2f &p){
    sf::Vector2f new_pos = _parent->get_position() + p;
    if (valid_move(new_pos)){
        _parent->set_position(new_pos);
    }
}

void SteeringComponent::move(float x, float y){ 
    move(sf::Vector2f(x, y));
}
```

The key behaviour in `update` is a check on how far the `entity` (`_parent`) is from the player. If it is more than 100 pixels the component will use seek. If it is less than 50 it will flee. Now all we need to do is add some enemies with this behaviour. We do this back in the `SteeringScene::Load` method as below.

```cpp
...
#include <random>
...
//"Updated SteeringScene::Load"
void SteeringScene::load()
{
  ...
    
  // Setup C++ random number generation
  std::random_device dev;
  std::default_random_engine engine(dev());
  std::uniform_real_distribution<float> x_dist(0.0f,
                                          param::game_width);
  std::uniform_real_distribution<float> y_dist(0.0f,
                                          param::game_height);
  for (size_t n = 0; n < 100; ++n) {
    std::shared_ptr<Entity> enemy = make_entity();
    enemy->set_position(sf::Vector2f(x_dist(engine), y_dist(engine)));
    std::shared_ptr<ShapeComponent> s = enemy->add_component<ShapeComponent>();
    s->set_shape<sf::RectangleShape>(sf::Vector2f(10.0f, 10.0f));
    s->get_shape().setFillColor(sf::Color::Blue);
    enemy->add_component<SteeringComponent>(player.get(),50.f);
  }
}
```

You can now run the application and you will have the enemies chase and run away from you accordingly. It should like the video at the top.

## Advanced task

To continue you implement the rotation as well. The ai face  the player when seeking and turn over when fleeing. Check the lecture! Your AI should change rotation progressively. Change the shape to a triangle to get a direction.

Previous step: [Platformer](platformer)

Next step: [Pathfinding](lab8_2)