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

The first item we will add is a `MenuSystem` class. First we will add a `scene_menu.h` file to our `scenes` folder of our project. This is below. The definition is just a standard scene.

```cpp
//"scene_menu.h"
#pragma once

#include "engine.h"

class MenuScene : public Scene {
public:
  MenuScene() = default;
  ~MenuScene() = default;

  void Load() override;

  void Update(const double& dt) override;
};
```

Our implementation (`scene_menu.cpp` in the `scene` folder) is below.

```cpp
//"scene_menu.cpp"
#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>

using namespace std;
using namespace sf;

void MenuScene::Load() {
  auto txt = makeEntity();
  auto t = txt->addComponent<TextComponent>( 
      "Movement Demos\nPress 1 for Steering\nPress 2 for Pathfinding");
  setLoaded(true);
}

void MenuScene::Update(const double& dt) {
  if (sf::Keyboard::isKeyPressed(Keyboard::Num1)) {
    ...
  } else if (sf::Keyboard::isKeyPressed(Keyboard::Num2)) {
   ...
  }
  Scene::Update(dt);
}
```

The `MenuScene` will display a `TextComponent` which will give two options:
1.  Steering Behaviour Demo
2.  Pathfinding Demo

The `Update` checks which button is pressed and has the outline for the code to handle this change. Once we have our other scenes in place we will add the new lines of code.

We will use a `game.h` file to allow easy access to the scenes. This is given below.

```cpp
//"game.h"
#pragma once

#include "scenes/scene_menu.h"

extern MenuScene menu;
```

And finally we need our `main.h` file. This is below.

```cpp
//"main.h"
#include "engine.h"
#include "game.h"

using namespace std;

MenuScene menu;
SteeringScene steeringScene;
PathfindingScene pathfindingScene;

int main() {
  Engine::Start(1280, 720, "Steering", &menu);
  return 0;
}
```

You should be able to run this now and get the output shown
![Steering Lab Menu](assets/images/screen-steer-start.png)
### SteeringScene

Before moving onto steering behaviours properly let us set up our `SteeringScene`. We are going to have an object on the screen (represented by a red dot) that the player will move around using the arrow keys. To do this we will create a basic `BasicMovementComponent`. You can replace this with the physics movement component from last practical if you wish. The `cmp_basic_movement.h` and `cmp_basic_movement.cpp` files are provided below. You should be able to understand the basic premise by now.

```cpp
//"cmp_basic_movement.h"
#pragma once
#include <ecm.h>

// A component to allow basic movement behaviour
class BasicMovementComponent : public Component {
protected:
  // Speed we can travel
  float _speed;
  // Checks if the move is valid.
  bool validMove(const sf::Vector2f&);

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
  explicit BasicMovementComponent(Entity* p);

  BasicMovementComponent() = delete;
};
```

```cpp
//"cmp_basic_movement.cpp"
#include "cmp_basic_movement.h"
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>

using namespace sf;
using namespace std;

// Checks keyboard and moves the player.
void BasicMovementComponent::update(double dt) {
  Vector2f direction(0.0f, 0.0f);
  if (Keyboard::isKeyPressed(Keyboard::Left)) {
    direction.x -= 1.0f;
  }
  if (Keyboard::isKeyPressed(Keyboard::Right)) {
    direction.x += 1.0f;
  }
  if (Keyboard::isKeyPressed(Keyboard::Up)) {
    direction.y -= 1.0f;
  }
  if (Keyboard::isKeyPressed(Keyboard::Down)) {
    direction.y += 1.0f;
  }

  move(normalize(direction) * _speed * (float)dt);
}

// Initialises the component.
BasicMovementComponent::BasicMovementComponent(Entity* p)
    : _speed(100.0f), Component(p) {}

// Checks if the proposed move is valid.
bool BasicMovementComponent::validMove(const sf::Vector2f& pos) {
  if (pos.x < 0.0f || pos.x > Engine::GetWindow().getSize().x ||
      pos.y < 0.0f || pos.y > Engine::GetWindow().getSize().y) {
    return false;
  }
  return true;
}

// Moves the component's parent.
void BasicMovementComponent::move(const sf::Vector2f& p) {
  auto new_pos = _parent->getPosition() + p;
  if (validMove(new_pos)) {
    _parent->setPosition(new_pos);
  }
}

// Moves the component's parent.
void BasicMovementComponent::move(float x, float y) {
  move(Vector2f(x, y));
}
```

Our `SteeringScene` will add an `Entity` with this component attached. We first need to define the `scene_steering.h` file which is given below.

```cpp
//"scene_steering.h"

#pragma once

#include "engine.h"

class SteeringScene : public Scene {
public:
  void Load() override;
  void UnLoad() override;
  void Update(const double& dt) override;
  void Render() override;
};
```

The key behaviour we can add at the moment will be defined in the `Load` method. Here we will create an `Entity` and add a `BasicMovementComponent`. `scene_steering.cpp` is given below.

```cpp
//"scene_steering.cpp"

#include "scene_steering.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_basic_movement.h"
#include <LevelSystem.h>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace sf;

void SteeringScene::Load(){
  auto player = makeEntity();
  player->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2,
                               Engine::GetWindow().getSize().y / 2));
  auto s = player->addComponent<ShapeComponent>();
  s->setShape<CircleShape>(10.0f);
  s->getShape().setFillColor(Color::Red);
  player->addComponent<BasicMovementComponent>();
}

void SteeringScene::UnLoad() { Scene::UnLoad(); }

void SteeringScene::Update(const double& dt) { Scene::Update(dt); }

void SteeringScene::Render() { Scene::Render(); }
```

All we have to do now is add to `game.h` as below.

```cpp
//"game.h"
...
extern SteeringScene steeringScene;
```

Update `MenuScene::Update` as below.

```cpp
void MenuScene::Update(const double &dt){
    if (sf::Keyboard::isKeyPressed(Keyboard::Num1)){
        // New line
        Engine::ChangeScene(&steeringScene);
    }
    else if (sf::Keyboard::isKeyPressed(Keyboard::Num2)){
    
    }
    Scene::Update(dt);
}
```

And then add a line `SteeringScene steeringScene;` to `main.cpp`. You should now be able to run the game, select option 1 and control the red dot


### SteeringOutput
As discussed in the lecture, the first part we need is the `SteeringOutput` type. We will define this in `steering.h`. This is given below.

```cpp
//"steering.h"
#pragma once
#include <engine.h>

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

### SteeringBehaviour

`SteeringBehaviour` we also defined in the lecture. This is a `virtual` class. The important part is the `getSteering` method. Add the below to the `steering.h` file.

```cpp
//"steering.h"
// Base class for steering behaviour
class SteeringBehaviour{
public:
    virtual ~SteeringBehaviour() = default;
    
    // Gets the output from a steering behaviour.
    virtual SteeringOutput getSteering() const noexcept = 0;
};
```

We will build two steering behaviours -- seek and flee. They are fairly easy as the following sections will show.

#### Seek

`Seek` is the behaviour to move an object to a target. We described the general idea in the lecture. The following is the definition to add to `steering.h`.

```cpp
//"steering.h"
// Seek steering behaviour
class Seek : public SteeringBehaviour{
private:
    Entity* _character;
    Entity* _target;
    float _maxSpeed;
    public:
    Seek() = delete;
    Seek(Entity *character, Entity *target, float maxSpeed)
    : _character(character), _target(target), _maxSpeed(maxSpeed) { }
    SteeringOutput getSteering() const noexcept;
};
```

The only part we need to define is the `Seek::getSteering` method. We will do this in a new file -- `steering.cpp`. This is given below.

```cpp
//"steering.cpp"
#include "steering.h"

using namespace sf;

SteeringOutput Seek::getSteering() const noexcept{
    SteeringOutput steering;
    steering.direction = _target->getPosition() - _character->getPosition();
    steering.direction = normalize(steering.direction);
    steering.direction *= _maxSpeed;
    steering.rotation = 0.0f;
    return steering;
}
```

#### Flee

Flee is likewise simple to implement. We add the declaration to `steering.h` and then the definition to `steering.cpp`. These are given below.

```cpp
//"steering.h"
class Flee : public SteeringBehaviour{
private:
    Entity* _character;
    Entity* _target;
    float _maxSpeed;
    public:
    Flee() = delete;
    Flee(Entity *character, Entity *target, float maxSpeed)
    : _character(character), _target(target), _maxSpeed(maxSpeed) { }
    SteeringOutput getSteering() const noexcept;
};
```

```cpp
//"steering.cpp"
SteeringOutput Flee::getSteering() const noexcept{
    SteeringOutput steering;
    steering.direction = _character->getPosition() - _target->getPosition();
    steering.direction = normalize(steering.direction);
    steering.direction *= _maxSpeed;
    steering.rotation = 0.0f;
    return steering;
}
```

### `SteeringComponent`

We will now add a new component that will control the AI movement. This we will call `SteeringComponent` and we will define it in `cmp_ai_steering.h` as below.

```cpp
//"cmp_ai_steering.h"
#pragma once
#include <ecm.h>
#include "../steering.h"

class SteeringComponent : public Component{
protected:
    Seek _seek;
    Flee _flee;
    Entity *_player;
    bool validMove(const sf::Vector2f&) const;
public:
    void update(double) override;
    void move(const sf::Vector2f&);
    void move(float x, float y);
    void render() override { }
    explicit SteeringComponent(Entity *p, Entity *player);
    SteeringComponent() = delete;
};
```

The component is similar to the other movement ones we have defined and you could reuse those as well if you wanted. The key behaviour is really in `update`. This is defined in `cmp_ai_steering.cpp` (as well as other methods). See this below.

```cpp
//"cmp_ai_steering.cpp"
#include "cmp_ai_steering.h"

using namespace sf;

void SteeringComponent::update(double dt) {
  // If target (player) is more than 100 pixels away seek
  if (length(_parent->getPosition() - _player->getPosition()) > 100.0f) {
    auto output = _seek.getSteering();
    move(output.direction * (float)dt);
  }
  // If target (player) is less than 50 pixels away flee
  else if (length(_parent->getPosition() - _player->getPosition()) <
           50.0f) {
    auto output = _flee.getSteering();
    move(output.direction * (float)dt);
  }
}

SteeringComponent::SteeringComponent(Entity* p, Entity* player)
    : _player(player), _seek(Seek(p, player, 100.0f)),
      _flee(Flee(p, player, 100.0f)), Component(p) {}

bool SteeringComponent::validMove(const sf::Vector2f& pos) const {
  if (pos.x < 0.0f || pos.x > Engine::GetWindow().getSize().x ||
      pos.y < 0.0f || pos.y > Engine::GetWindow().getSize().y) {
    return false;
  }
  return true;
}

void SteeringComponent::move(const sf::Vector2f &p){
    auto new_pos = _parent->getPosition() + p;
    if (validMove(new_pos)){
        _parent->setPosition(new_pos);
    }
}

void SteeringComponent::move(float x, float y){ move(Vector2f(x, y));}
```

The key behaviour in `update` is a check on how far the `entity` (`_parent`) is from the player. If it is more than 100 pixels the component will use seek. If it is less than 50 it will flee. Now all we need to do is add some enemies with this behaviour. We do this back in the `SteeringScene::Load` method as below.

```cpp
//"Updated SteeringScene::Load"
void SteeringScene::Load()
{
  auto player...
  ...
    
  // New lines for here.
  // Setup C++ random number generation
  random_device dev;
  default_random_engine engine(dev());
  uniform_real_distribution<float> x_dist(0.0f,
                                          Engine::GetWindow().getSize().x);
  uniform_real_distribution<float> y_dist(0.0f,
                                          Engine::GetWindow().getSize().y);
  for (size_t n = 0; n < 100; ++n) {
    auto enemy = makeEntity();
    enemy->setPosition(Vector2f(x_dist(engine), y_dist(engine)));
    auto s = enemy->addComponent<ShapeComponent>();
    s->setShape<RectangleShape>(Vector2f(10.0f, 10.0f));
    s->getShape().setFillColor(Color::Blue);
    enemy->addComponent<SteeringComponent>(player.get());
  }
}
```

You can now run the application and you will have the enemies chase and run away from you accordingly. It should like the video at the top.

Previous step: [Platformer](platformer)

Next step: [Pathfinding 1](lab8_2)