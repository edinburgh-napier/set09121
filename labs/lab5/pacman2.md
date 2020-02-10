---
title: "PacMan 2 - ECM"
keywords: PACMAN
tags: [PACMAN]
permalink:  pacman2.html
summary: PACMAN
sidebar: home_sidebar
---


## The Entity Component Model


It's time to bring in the big-guns, standard inheritance and OO can only get us so far. Having a huge inheritance tree for all of our entities would become infeasible to write and maintain, and such we will now adopt the ECM pattern. This will be covered a lecture, here we will focus only on the implementation.

### The ECM Library

The code we are about to write will be generic in nature, and we will want to use it again, so we will spin it out to it's own library.
In case you've forgotten, here's the CMake: Remember to add it to the linked libraries of our lab executable too.

```CMake
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm" )
target_link_libraries(lib_ecm PRIVATE lib_maths)
```

Inside the lib_ecm folder we will have ecm.h and ecm.cpp. Move your Entity class that you've already written into the ecm library, and include it in the relevant places in the pacman code. 

{:class="important"}
**Check that everything still compiles and works before continuing**

### Entity Additions
We're going to add some new stuff to the basic Entity:
Here's what your entity class should look like:

```cpp
//"ecm.h"
#pragma once
#include "maths.h"
#include <algorithm>
#include <memory>
#include <typeindex>
#include <vector>

class Component; //forward declare

class Entity {

protected:
  std::vector<std::shared_ptr<Component>> _components;
  sf::Vector2f _position;
  float _rotation;
  bool _alive;       // should be updated
  bool _visible;     // should be rendered
  bool _fordeletion; // should be deleted
public:
  Entity();
  virtual ~Entity();
  virtual void update(double dt);
  virtual void render();

  const sf::Vector2f &getPosition() const;
  void setPosition(const sf::Vector2f &_position);
  bool is_fordeletion() const;
  float getRotation() const;
  void setRotation(float _rotation);
  bool isAlive() const;
  void setAlive(bool _alive);
  void setForDelete();
  bool isVisible() const;
  void setVisible(bool _visible);
};
```

You will have already had some of this, but there are a few additions you won't have had. Note there is no property for a sprite or a shape. What's new is the declaration of a Component, and a vector of components stored privately.

{:class="important"}
Remember, you will have to ensure that the Entity calls the Render method of all the components! You should check visibility here too...

### Component
In the same header file, we are now going to define the component class. The code is remarkably simple.

```cpp
//"ecm.h"
class Component { 
 protected:
   Entity *const _parent;
   bool _fordeletion; // should be removed
   explicit Component(Entity *const p);
 
 public:
   Component() = delete;
   bool is_fordeletion() const;
   virtual void update(double dt) = 0;
   virtual void render() = 0;
   virtual ~Component();
};
```

When a component is constructed, an Entity must be passed to the constructor. This is so each component knows who it's parent is. Other than that, it's just our usual two friendly update and render() functions again. There is also a _fordeletion flag, we'll come back to this later.

## Shape component

Before we add the rest of the functionality it would be useful to work with an example of a component. Here is a ShapeComponent. Add it to the Pacman code folder for now. While it may be a good idea to have some generic components in the ecm library, we're not sure what we are going to need in the future. So we will keep components in the paceman code for now and have the library just be the definitions for the base Entity and Component.


```cpp
//"cmp_sprite.h"
class ShapeComponent : public Component {
protected:
  std::shared_ptr<sf::Shape> _shape;
public:
  ShapeComponent() = delete;
  explicit ShapeComponent(Entity *p);

  void update(double dt) override;
  void render() override;

  sf::Shape &getShape() const;

  template <typename T, typename... Targs>
  void setShape(Targs... params) {
    _shape.reset(new T(params...));
  }
};
```

We'll talk about that setShape template in a bit.


There's nothing funky in the definition .cpp. Components are remarkably simple when built correctly.

```cpp
//"cmp_sprite.cpp"

void ShapeComponent::update(double dt) {
  _shape->setPosition(_parent->getPosition());
}

void ShapeComponent::render() { Renderer::queue(_shape.get()); }

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p) : Component(p), _shape(std::make_shared<sf::CircleShape>()) {}
```

The only complexity to note is the constructor -- which passes the calling entity to the base `Component()` constructor, and also constructs the `_shape` to a sfml circle.

## Adding a component

So how do we add a shape component to an entity? There are many different approaches to this, the key is to to remember this happens at runtime. Components can be dynamically added and removed to entities. Therefore some of the usual methods you may think won't work.

The approach we will take is to go down (but not too far) the templated code route. Take a gander at this crazy thing:
```cpp
//"ecm.h"
template <typename T, typename... Targs>
std::shared_ptr<T> addComponent(Targs... params) {
  static_assert(std::is_base_of<Component, T>::value, "T != component");
  std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
  _components.push_back(sp);
  return sp;
}
```

This is added to the Entity class in ecm.h.

This is called like so:

```cpp
auto s = ghost->addComponent<ShapeComponent>();
s->setShape<sf::CircleShape>(12.f);
```

We use templates here to do four major things.

1.  Create a component of any `<specified>` type `T`(line 4).
    - consider this as simple algebra, T is a variable in our formula, and our formula is this whole function.
    - imagine any place that T is used, replace it with a type: like `int` or `movement_component`
2.  Check that the specified type is actually a component (line 3)
3.  Pass any parameters to the component constructor. (line 4)
4.  Add the built component into the entity component list (line 5)

The extra template we had in the ShapeComponent is unrelated to this process. That one just allows us to set the shape type with templates, rather than having a different shape component for each type of sf::shape.

#### Putting this to use

It's time to kill off our original Entity classes for Ghosts and the player that were in the pacman code. We may need some of the code in there, so instead of deleting the files, just change any `\#includes` pointing to them to point to `ecm.h` instead.

Creating Entities now follows this process:
{% raw  %}
```cpp
//pacman.cpp
#define GHOSTS_COUNT 4
...
void GameScene::load() {

  {
    auto pl = make_shared<Entity>();

    auto s = pl->addComponent<ShapeComponent>();
    s->setShape<sf::CircleShape>(12.f);
    s->getShape().setFillColor(Color::Yellow);
    s->getShape().setOrigin(Vector2f(12.f, 12.f));

    _ents.list.push_back(pl);
  }

  const sf::Color ghost_cols[]{{208, 62, 25},    // red Blinky
                               {219, 133, 28},   // orange Clyde
                               {70, 191, 238},   // cyan Inky
                               {234, 130, 229}}; // pink Pinky

  for (int i = 0; i < GHOSTS_COUNT; ++i) {
    auto ghost = make_shared<Entity>();
    auto s = ghost->addComponent<ShapeComponent>();
    s->setShape<sf::CircleShape>(12.f);
    s->getShape().setFillColor(ghost_cols[i % 4]);
    s->getShape().setOrigin(Vector2f(12.f, 12.f));
    
    _ents.list.push_back(ghost);
  }
 ...
```
{% endraw %}
This should be all we need to get the game running again, but with one
problem - things aren't moving any more.

{:class="important"}
**Do not continue until you are able to render things through the new ECM system!**

### Building More components

We've got a shape component that let's things be drawn. We need game
logic and movement next.

#### Actor Movement Component

For moving things around we will define 3 components. A base "Actor Movement" Component that has the generic methods and properties such as `Move()` and `_speed`. From there we will inherit to two seperate components `PlayerMovementComponent` and `EnemyAIComponent`. The first will contain the keyboard controls to move the play, the second will contain the AI for the ghosts.

I'll give you the complete listing for the base Component:

```cpp
//"cmp_actor_movement.h"
  #pragma once
  #include <ecm.h>
  
  class ActorMovementComponent : public Component {
  protected:
    bool validMove(const sf::Vector2f&);
    float _speed;
  
  public:
    explicit ActorMovementComponent(Entity* p);
    ActorMovementComponent() = delete;
  
    float getSpeed() const;
    void setSpeed(float _speed);
  
    void move(const sf::Vector2f&);
    void move(float x, float y);
  
    void render() override {}
    void update(double dt) override;
  };
```

Some lines are missing from the definition, for you to fill in.

```cpp
//"cmp_actor_movement.cpp"
#include "cmp_actor_movement.h"
#include <LevelSystem.h>

using namespace sf;

void ActorMovementComponent::update(double dt) {}

ActorMovementComponent::ActorMovementComponent(Entity* p)
    : _speed(100.0f), Component(p) {}

bool ActorMovementComponent::validMove(const sf::Vector2f& pos) {
  return (LevelSystem::getTileAt(pos) != LevelSystem::WALL);
}

void ActorMovementComponent::move(const sf::Vector2f& p) {
  auto pp = _parent->getPosition() + p;
  if (validMove(pp)) {
   ...
  }
}

void ActorMovementComponent::move(float x, float y) {
  move(Vector2f(x, y));
}
float ActorMovementComponent::getSpeed() const { ... }
void ActorMovementComponent::setSpeed(float speed) { ... }
```

{:class="important"}
You will need to comment out the validMove test for now! We will come back to this...

#### Player Movement Component

This is super simple, inherit from ActorMovementComponent and add the usual keyboard controls to the Update();

You will also have to ensure you add the constructor in, so it works correctly. You will need to add it into the .h file, and the .cpp should have something like this in it, calling the parent class:

```

PlayerMovementComponent::PlayerMovementComponent(Entity* p)
	: ActorMovementComponent(p) {}

```

**REMEMBER: You have to call the Update() method on all the components - you will likely want to check whether the Entity is alive here too!**



#### Enemy AI Component

This will get much more complex later on, as we dip into AI topics. For now, place your random movement code into the Update().

#### Add the new Components

Adding the new components we just made to our player and ghosts follows the same principle we use to add the shape component:

```cpp
//"pacman.cpp"
void GameScene::load() {
...
  {
    auto pl = make_shared<Entity>();
    ...
    
    pl->addComponent<PlayerMovementComponent>();
    
    ...
  }

  ...

  for (int i = 0; i < GHOSTS_COUNT; ++i) {
    ...
    
    ghost->addComponent<EnemyAIComponent>();
    
   ...
  }
 ...
```

### Getting Components

We have already covered the templated code that creates a component, as a refresher here it is:

```cpp
//"ecm.h - addComponent"
template <typename T, typename... Targs>
std::shared_ptr<T> addComponent(Targs... params) {
    static_assert(std::is_base_of<Component, T>::value, "T != component");
    std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
    _components.push_back(sp);
    return sp;
}
```

This function returns a `shared_ptr` to the newly created component, so we can access it and it's properties. There will be times later on in the code where we need to retrieve certain components from an Entity, unless we keep the original `shared_ptr` around, we need a new mechanism.

What we want is something like this:

```cpp
Entity-> get_component<MovementComponent>();
```

It could be the case that there are multiple MovementComponent's on an Entity, so the function should probably return a vector. So what we are looking for is a function declaration like this:

```cpp
const std::vector<std::shared_ptr<T>> get_components() const;
```

But how do we build this?

#### Finding components

Retrieving or finding components of a certain type can be done in a few ways. To do this we need a way to tell what type a component is. The simple way is to have a string or numeric identifier as a property on each component type. Then a simple search through all the components on an entity for the value we need. This would require us adding code into every component class we have made -- not good(but not a bad idea).

Fortunately, the c++ runtime has us covered for this, with a very handy function: `typeid()`. This is part of the Run-Time-Type-Information (RTTI) capabilites of the compiler, there are arguments for and against the performance cost of enabling this. It makes our lives easy so we'll take it for now and make a fake promise to ourselves to come up with something better in the future.

`typeid()` returns an arbitrary number, so it ca't tell us outright what type of class an object is, but we can compare it with the id of a known class. so our process to find components of a given type is to loop through all components on an entity and compare the `typeid()` with the `typeid()` of the type we want. When we find a component that matches, we add it to a vector that we will return.

#### get_components<T>()

Here it is, isn't it pretty:

```cpp
//ecm.h - get_components<T>()
template <typename T>
const std::vector<std::shared_ptr<T>> get_components() const {
  static_assert(std::is_base_of<Component, T>::value, "T != component");
  std::vector<std::shared_ptr<T>> ret;
  for (const auto c : _components) {
    if (typeid(*c) == typeid(T)) {
      ret.push_back(std::dynamic_pointer_cast<T>(c));
    }
  }
  return std::move(ret);
}
```

#### Getting Compatible Components

More often than not, we may not know exactly which components are on an entity. A Player will have a PlayerMovement Component, which derives from ActorMovement. Calling get_components<ActorMovement> would return an empty vector. This could cause us headaches. We need a way of getting a T Component, or anything that derives from a T component. `typeid()` can't help us here.

RTTI has another method that can save us here, you may have seen it in use in other places -- the `dynamic_cast()`. This tries to convert one thing into another thing, and returns true or false depending on if this was successful. This does some complex things behind the scenes and is quite slow. So slow you might actually see it taking up some of your frametime when running in Debug. So we should use this sparingly in the code, and where possible: save the result so we don't have to call it every frame.

Here it is:

```cpp
//ecm.h - GetCompatibleComponent$T>()"
// Will return a T component, or anything derived from a T component.
template <typename T>
const std::vector<std::shared_ptr<T>> GetCompatibleComponent() {
  static_assert(std::is_base_of<Component, T>::value, "T != component");
  std::vector<std::shared_ptr<T>> ret;
  for (auto c : _components) {
      auto dd = dynamic_cast<T*>(&(*c));
      if (dd) {
        ret.push_back(std::dynamic_pointer_cast<T>(c));
      }
    }
    return ret;
}
```

## Checkpoint

That was a big change-up in code design. We are no longer using a single classes for Player and Ghost. Instead we are constructing them from components. You can see how this approach lends itself to modular and "Game-like" design. We can easily add or remove components at runtime to any Entity. A logical extension of this would be to design a "Factory" that constructs pre-set Entities with specific components, rather than doing it in pacman.cpp. That's a job for another day, our way is fine for now.

{:class="important"}
Make sure you have got here, and everything is working, compiling and running so far without any errors. You should commit your code now. Have a well-earned break.