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

The code we are about to write will be generic in nature, and we will want to use it again, so we will add it to the engine library.

{:class="important"}
**Check that everything still compiles and works before continuing**

### Entity Additions
We're going to add some new stuff to the basic Entity:
Here's what your entity class should look like:

```cpp
//"ecm.hpp"
#pragma once
#include "maths.h"
#include <algorithm>
#include <memory>
#include <typeindex>
#include <vector>

class Component; //forward declare

class Entity {
public:
  Entity();
  virtual ~Entity();
  virtual void update(const float &dt);
  virtual void render();

  const sf::Vector2f &get_position() const;
  void set_position(const sf::Vector2f &_position);
  bool is_for_deletion() const;
  float get_rotation() const;
  void set_rotation(float _rotation);
  bool is_alive() const;
  void set_alive(bool _alive);
  void set_for_delete();
  bool is_visible() const;
  void set_visible(bool _visible);
protected:
  std::vector<std::shared_ptr<Component>> _components;
  sf::Vector2f _position;
  float _rotation;
  bool _alive;       // should be updated
  bool _visible;     // should be rendered
  bool _for_deletion; // should be deleted
};
```

You will have already had some of this, but there are a few additions you won't have had. Note there is no property for a sprite or a shape. What's new is the declaration of a Component, and a vector of components stored privately.

{:class="important"}
Remember, you will have to ensure that the Entity calls the Render method of all the components! You should check visibility here too...

### Component
In the same header file, we are now going to define the component class. The code is remarkably simple.

```cpp
//"ecm.hpp"
class Component { 
public:
   Component() = delete;
   bool is_for_deletion() const;
   virtual void update(const float &dt) = 0;
   virtual void render() = 0;
   virtual ~Component();
protected:
   Entity *const _parent;
   bool _for_deletion; // should be removed
   explicit Component(Entity *const p);
};
```

When a component is constructed, an Entity must be passed to the constructor. This is so each component knows who it's parent is. Other than that, it's just our usual two friendly update and render() functions again. There is also a _for_deletion flag, we'll come back to this later.

## Shape component

Before we add the rest of the functionality it would be useful to work with an example of a component. Here is a ShapeComponent. Add it to the Pacman code folder for now. While it may be a good idea to have some generic components in the ecm library, we're not sure what we are going to need in the future. So we will keep components in the pacman code for now and have the library just be the definitions for the base Entity and Component.
```cpp
//"components.hpp"
class ShapeComponent : public Component {
public:
  ShapeComponent() = delete;
  explicit ShapeComponent(Entity *const p);

  void update(const float &dt) override;
  void render() override;

  sf::Shape &get_shape() const;

  template <typename T, typename... Targs>
  void set_shape(Targs... params) {
    _shape.reset(new T(params...));
  }
protected:
  std::shared_ptr<sf::Shape> _shape;
};

```

We'll talk about that set_shape template in a bit.

There's nothing funky in the definition .cpp. Components are remarkably simple when built correctly.

```cpp
//"components.cpp"
void ShapeComponent::update(const float &dt) {
  _shape->setPosition(_parent->get_position());
}

void ShapeComponent::render() { Renderer::queue(_shape.get()); }

sf::Shape& ShapeComponent::get_shape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p) : Component(p), _shape(std::make_shared<sf::CircleShape>()) {}
```

The only complexity to note is the constructor -- which passes the calling entity to the base `Component()` constructor, and also constructs the `_shape` to a sfml circle.

## Adding a component

So how do we add a shape component to an entity? There are many different approaches to this, the key is to to remember this happens at runtime. Components can be dynamically added and removed to entities. Therefore some of the usual methods you may think won't work.

The approach we will take is to go down (but not too far) the templated code route. Take a gander at this crazy thing:
```cpp
//"ecm.hpp"
template <typename T, typename... Targs>
std::shared_ptr<T> add_component(Targs... params) {
  static_assert(std::is_base_of<Component, T>::value, "T != component");
  std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
  _components.push_back(sp);
  return sp;
}
```

This is added to the Entity class in ecm.hpp.

This is called like so:

```cpp
std::shared_ptr<ShapeComponent> s = ghost->add_component<ShapeComponent>();
s->set_shape<sf::CircleShape>(12.f);
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

It's time to kill off our original Entity classes for Ghosts and the player that were in the pacman code. We may need some of the code in there, so instead of deleting the files, just change any `#includes` pointing to them to point to `ecm.h` instead.

Creating Entities now follows this process:
{% raw  %}
```cpp
//scenes.cpp
#define GHOSTS_COUNT 4
...
void GameScene::load() {
  {
    std::shared_ptr<Entity> player = std::make_shared<Entity>();
    std::shared_ptr<ShapeComponent> shape = player->add_component<ShapeComponent>();
    shape->set_shape<sf::CircleShape>(param::entity_size);
    shape->get_shape().setFillColor(sf::Color::Yellow);
    shape->get_shape().setOrigin(sf::Vector2f(param::entity_size, param::entity_size));  
    _entities.list.push_back(player);
  }

  const sf::Color ghost_cols[]{{208, 62, 25},    // red Blinky
                               {219, 133, 28},   // orange Clyde
                               {70, 191, 238},   // cyan Inky
                               {234, 130, 229}}; // pink Pinky

for(int i = 0; i < param::ghost_count; i++){
    std::shared_ptr<Entity> ghost = std::make_shared<Entity>();
    std::shared_ptr<ShapeComponent> shape = ghost->add_component<ShapeComponent>();
    shape->set_shape<sf::CircleShape>(param::entity_size);
    shape->get_shape().setFillColor(ghost_cols[i % param::number_ghosts]);
    shape->get_shape().setOrigin(
      sf::Vector2f(param::entity_size,param::entity_size));
    _entities.list.push_back(ghost);
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
//"components.hpp"
class ActorMovementComponent : public Component {
public:
  explicit ActorMovementComponent(Entity* p);
  ActorMovementComponent() = delete;

  float get_speed() const;
  void set_speed(float _speed);

  void move(const sf::Vector2f&);
  void move(float x, float y);

  void render() override {}
  void update(const float &dt) override;
protected:
  bool _valid_move(const sf::Vector2f&);
  float _speed;
};
```

Some lines are missing from the definition, for you to fill in.

```cpp
//"components.cpp"
void ActorMovementComponent::update(const float &dt) {}

ActorMovementComponent::ActorMovementComponent(Entity* p)
    : _speed(100.0f), Component(p) {}

bool ActorMovementComponent::_valid_move(const sf::Vector2f& pos) {
  return (ls::get_tile_at(pos) != ls::WALL);
}

void ActorMovementComponent::move(const sf::Vector2f& p) {
  sf::Vector2f new_pos = _parent->get_position() + p;
  if (valid_move(new_pos)) {
    _parent->set_position(new_pos);
  }
}

void ActorMovementComponent::move(float x, float y) {
  move(sf::Vector2f(x, y));
}
float ActorMovementComponent::get_speed() const { ... }
void ActorMovementComponent::set_speed(float speed) { ... }
```

{:class="important"}
You will need to comment out the _valid_mode test for now! We will come back to this...

#### Player Movement Component

This is super simple, inherit from ActorMovementComponent and add the usual keyboard controls to the update();

You will also have to ensure you add the constructor in, so it works correctly. You will need to add it into the .hpp file, and the .cpp should have something like this in it, calling the parent class:

```cpp
PlayerMovementComponent::PlayerMovementComponent(Entity* p)
	: ActorMovementComponent(p) {}
```

**REMEMBER: You have to call the update() method on all the components - you will likely want to check whether the Entity is alive here too!**



#### Enemy AI Component

This will get much more complex later on, as we dip into AI topics. For now, place your random movement code into the Update().

#### Add the new Components

Adding the new components we just made to our player and ghosts follows the same principle we use to add the shape component:

```cpp
//"scenes.cpp"
void GameScene::load() {
...
  {
    std::shared_ptr<Entity> player = std::make_shared<Entity>();
    ...
    
    player->addComponent<PlayerMovementComponent>();
    
    ...
  }

  ...

  for (int i = 0; i < param::ghost_count; ++i) {
    std::shared_ptr<Entity> ghost = std::make_shared<Entity>();
    ...
    
    ghost->add_component<EnemyAIComponent>();
    
   ...
  }
 ...
```

### Getting Components

We have already covered the templated code that creates a component, as a refresher here it is:

```cpp
//"ecm.h - addComponent"
template <typename T, typename... Targs>
std::shared_ptr<T> add_component(Targs... params) {
    static_assert(std::is_base_of<Component, T>::value, "T != component");
    std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
    _components.push_back(sp);
    return sp;
}
```

This function returns a `shared_ptr` to the newly created component, so we can access it and it's properties. There will be times later on in the code where we need to retrieve certain components from an Entity, unless we keep the original `shared_ptr` around, we need a new mechanism.

What we want is something like this:

```cpp
Entity->get_component<MovementComponent>();
```

It could be the case that there are multiple MovementComponent's on an Entity, so the function should probably return a vector. So what we are looking for is a function declaration like this:

```cpp
const std::vector<std::shared_ptr<T>> get_components() const;
```

But how do we build this?

#### Finding components

Retrieving or finding components of a certain type can be done in a few ways. To do this we need a way to tell what type a component is. The simple way is to have a string or numeric identifier as a property on each component type. Then a simple search through all the components on an entity for the value we need. This would require us adding code into every component class we have made -- not good (but not a bad idea).

Fortunately, the C++ runtime has us covered for this, with a very handy function: `typeid()`. This is part of the Run-Time-Type-Information (RTTI) capabilites of the compiler, there are arguments for and against the performance cost of enabling this. It makes our lives easy so we'll take it for now and make a fake promise to ourselves to come up with something better in the future.

`typeid()` returns an arbitrary number, so it can't tell us outright what type of class an object is, but we can compare it with the id of a known class to discover this. So our process to find components of a given type is to loop through all components on an entity and compare the `typeid()` with the `typeid()` of the type we want. When we find a component that matches, we add it to a vector that we will return.

#### get_components<T>()

Here it is:

```cpp
//ecm.hpp - get_components<T>()
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

More often than not, we may not know exactly which components are on an entity. A Player will have a PlayerMovement Component, which derives from ActorMovement. Calling get_components\<ActorMovement\> would return an empty vector. This will absolutely cause us headaches later on! We need a way of getting a T Component, or anything that derives from a T component. `typeid()` can't help us here.

RTTI has another method that can save us here, you may have seen it in use in other places -- the `dynamic_cast()`. This tries to convert one thing into another thing, and returns true or false depending on if this was successful. This does some complex things behind the scenes and is quite slow. So slow you might actually see it taking up some of your frametime when running in Debug. So we should use this sparingly in the code, and where possible: save the result so we don't have to call it every frame.

Here it is:

```cpp
//ecm.h - getCompatibleComponent$T>()"
// Will return a T component, or anything derived from a T component.
template <typename T>
const std::vector<std::shared_ptr<T>> get_compatible_component() {
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

That is a big change-up in code design. We are no longer using  classes inheriting from `Entity` to define our Player and Ghosts. Instead, we are constructing them from components. You can see how this approach lends itself to modular and "Game-like" design. We can easily add or remove components at runtime to any Entity. A logical extension of this would be to design a "Factory" that constructs pre-set Entities with specific components, rather than doing it in pacman.cpp. That's a job for another day, our way is fine for now.

{:class="important"}
Make sure you have got here, and everything is working, compiling and running so far without any errors. You should commit your code now. Have a well-earned break.

Previous step: [PacMan 2](pacman2)

Next step: [PacMan 3](pacman3)