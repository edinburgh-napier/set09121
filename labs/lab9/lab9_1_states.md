---
title: "Lab 9 - State Machines"
keywords: lab9
tags: [lab9]
permalink:  lab9_1.html
summary: lab9
sidebar: home_sidebar
---

## AI: Making Decisions

<video class="middle" width="300" height="300" loop autoplay>
  <source src="assets/videos/states.mp4" type="video/mp4">
</video>


We have steering and movement in place, but that is really only a simple part of our AI behaviour. We are now going to look at how we can simplify decision making and behaviour management by implementing a state machine component and a decision tree component. These are both simple ideas, but take a bit of code to set-up. However, once in place you will be able to implement behaviours into your applications very rapidly.

## Review -- What is a State Machine?

We have mentioned states, state diagrams and state machines before. A state machine is just a technique for describing and modelling the state (behaviour, control, etc.) of a system in a mathematical manner. The system is modelled with a number of states, and the transitions between those states. As an example, we can model a game as a state machine such as this figure:

![State Machine](assets/images/state-machine.png)

We have four states (active, inactive, paused, exit) and transitions between these states. Modelling our system in this manner makes it quite easy to understand. Let us look at a more concrete example.

### State Machine Example

This is an example of a state machine for how a character might behave:

![State Machine Example](assets/images/state-machine-example.png)

If you plot out how the behaviour works, starting from Idle at the top, you will see that this behaviour maps how an enemy guard might behave in a game. This is what we are aiming for in a state machine. It might be that you have no idea yet how to implement what Patrol means, but that doesn't matter. At this level, you want to map high level behaviour, not low level implementation.

### State Diagrams

We have already looked at state diagrams from a UML point of view, and state diagrams are useful in our games for this very reason. You should start becoming familiar with state diagrams from now on, and map applications in this manner (games or otherwise).

An example of a state diagram:

![State Diagram](assets/images/state-diagram.png)

Individual states are mapped as ovals, and transitions between states are mapped using arrows between states. Again, we don't worry about how we implement the individual states; we are simply identifying behaviours and worrying about implementing the low level behaviour later.

### State Design Pattern

We have also already looked at the state design pattern. This is what we are going to implement in this lab. 

As a reminder the state design pattern:

![State Design Pattern](assets/images/state-design-pattern.png)


## Building a State Machine System

We have now reviewed the general ideas that we are going to implement in this practical. Let us look at what our class diagram looks like in our engine.

### Class Diagram

This illustrates the system we are going to implement:

![State Machine System Class Diagram](assets/images/state-class-diagram.png)

A `State` just has one method -- `execute`. This method will process the behaviour of a given state.

The `StateMachineComponent` is where the management work happens. It combines ideas from the `Component` model, the state design pattern (obviously), and the manager pattern. A state machine will manage its states accordingly. All of its behaviour is actually quite simple. We can add states, remove states, change state, and update. The update will just call execute on the current state.

So with all this in hand, let us implement our state machine system and get a simple test example working.

### Initial Setup

We need to do a little bit of set-up to get our system in place. You will need the components and steering behaviour code from last week for this lab. The point we are aiming for at the end of the lab is to combine steering, state, and decisions. So take everything you can from the last lab except for the scenes -- we will implement new ones.


#### The Scenes

Our scenes are pretty much the same as last week. We will include our (yet-to-be-implemented) scene headers just for completeness. We will be defining them soon.

```cpp
//"scenes.hpp"
#pragma once


struct Scenes{
    static std::shared_ptr<Scene> menu;
    static std::shared_ptr<Scene> state;
    static std::shared_ptr<Scene> decision;
};
```

#### Creating Scenes

As you saw, we will have three scenes: a menu, a state scene, and a decision scene. Let us implement each of these in turn.

The menu scene is similar to the one from the previous lab except for the text:
```
"Decision Demos
Press 1 for States Machines
Press 2 for Decision Trees"
```

##### State Machine Scene

We will provide a shell for the state scene at this point. The main part of the work comes in the `Load` method we will define later. The starting header and code files are below.

```cpp
//"scenes.hpp"
class StateScene : public Scene
{
public:
    StateScene() = default;
    void load() override;
    void unload() override;
    void update(const float &dt) override;
    void render() override;
};
```

``` cpp
//scenes.cpp
void StateScene::load(){}

void StateScene::unLoad(){
    Scene::unload();
}

void StateScene::Update(const float &dt){
    Scene::update(dt);
}

void StateScene::render(){
    Scene::render();
}
```

##### Decision Tree Scene

As with the state scene, the decision scene will just be a skeleton to start with. It is given in the following two code listings.

```cpp
//scenes.hpp
class DecisionScene : public Scene
{
public:
    DecisionScene() = default;
    void load() override;
    void unload() override;
    void update(const float &dt) override;
    void render() override;
};
```

```cpp
//scenes.cpp
void DecisionScene::load(){}

void DecisionScene::unLoad(){
    Scene::unload();
}

void DecisionScene::Update(const float &dt){
    Scene::update(dt);
}

void DecisionScene::render(){
    Scene::render();
}
```

Then, we need to define our `struct Scenes` and initialise the scenes in main;

You should now be able to run the application to make sure it works. When you do, the menu should be presented as shown in this Figure: 

![Starting Menu Screen for Decision Demos](assets/images/screen-state-start.png)

### State Machine Component

Let us now implement our new state machine system. We already know our class definitions for the header files -- following this UML class diagram:

![State Machine System Class Diagram](assets/images/state-class-diagram.png)

#### Header File

The header file is just the definition as given in the class diagram.
The code is below.

```cpp
//ai_cmps.hpp
#pragma once

#include <ecm.h>
#include <unordered_map>
#include <string>

class State
{
public:
    virtual ~State() = default;
    virtual void execute(Entity*, double) = 0;
};

class StateMachineComponent : public Component
{
private:
    std::unordered_map<std::string, std::shared_ptr<State>> _states;
    std::shared_ptr<State> _current_state;
    std::string _current_state_name = "";
public:
    StateMachineComponent() = delete;    
    explicit StateMachineComponent(Entity *p);

    void update(const float &) override;
    void render() override { }
    void add_state(const std::string&, std::shared_ptr<State>);
    std::shared_ptr<State> get_state(const std::string&) const;
    void remove_state(const std::string&);
    void change_state(const std::string&);
    const std::string& current_state() const {return _current_state_name; }
};
```

You should (hopefully) be familiar with these ideas by now.

#### Implementation

The implementation of the state machine system is also easy. We need to implement the following:
-   The constructor -- it will be the same as previous components.
-   The update method -- will call execute on the current state.
-   Add state -- will add a value to the map.
-   Get state -- will retrieve a value from the map.
-   Remove state -- will remove a value from the map.
-   Change state -- will change the current state to the named state by retrieving it from the map.

That's it. The implementation file is below and you should be able to understand the few lines of code we are using.

```cpp
//ai_cmp.cpp
#include "ai_cmps.hpp"


StateMachineComponent::StateMachineComponent(Entity *p) : _current_state(nullptr), Component(p) { }

void StateMachineComponent::update(double dt)
{
    if (_current_state != nullptr){
        _current_state->execute(_parent, dt);
    }
}

void StateMachineComponent::addState(const string &name, shared_ptr<State> state) noexcept
{
    _states[name] = state;
}

shared_ptr<State> StateMachineComponent::getState(const string &name) const noexcept
{
    auto found = _states.find(name);
    if (found != _states.end()) {
        return found->second;
    } else {
        return nullptr;
    }
}

void StateMachineComponent::removeState(const string &name) noexcept
{
    _states.erase(name);
}

void StateMachineComponent::changeState(const string &name) noexcept
{
    auto found = _states.find(name);
    if (found != _states.end()) {
        _current_state = found->second;
        _current_state_name = name;
    }
}
```

## Testing the State Machine System


OK -- implementing the state machine was easy. But is it easy to use? Yes it is. All we need to do is implement some states and add them to the state machine. Let us do that now.

### Enemy States

We are going to have a small enemy dot on the screen which will change colour based on how near the player is. We only need two states for this example -- a normal state for when the player is far away; and a near state for when the player is within a certain distance. The declarations are given below.

```cpp 
//states.h
#pragma once
#include "ia_cmps.hpp"

class NormalState : public State
{
private:
    std::shared_ptr<Entity> _player;
public:
    NormalState(std::shared_ptr<Entity> player) : _player(player) { }
    void execute(Entity*, const float &) override;
};

class NearState : public State
{
private:
    std::shared_ptr<Entity> _player;
public:
    NearState(std::shared_ptr<Entity> player) : _player(player) { }
    void execute(Entity*, const float &) override;
};
```

To implement these we need to decide what happens in each state. Remember, these states are executed by the enemy each frame. So we need to ask ourselves -- what is the behaviour we want the enemy to have at that frame. Let us make a decision:

-   If the player is far away, we will set our colour to blue.
    -   We will then need to check if we should change state.
-   If the player is near, we will set our colour to green.
    -   We will then need to check if we should change state.

This is surprisingly easy to do with the engine we have set-up so far. The implementation is below.

```cpp 
//states.cpp
#include "states.hpp"
#include "graphics_cmps.hpp"
#include <cmath>

void NormalState::execute(Entity *owner, const float &dt){
    auto length = [](sf::Vector2f v) -> float{
        return std::sqrt(v.x*v.x + v.y*v.y);
    };
    std::shared_ptr<ShapeComponent> s = owner->get_components<ShapeComponent>()[0];
    s->get_shape().setFillColor(sf::Color::Blue);
    
    if (length(owner->get_position() - _player->get_position()) < 100.0f){
        std::shared_ptr<StateMachineComponent> sm = owner->get_components<StateMachineComponent>()[0];
        sm->change_state("near");
    }
}

void NearState::execute(Entity *owner, const float &dt){
    auto length = [](sf::Vector2f v) -> float{
        return std::sqrt(v.x*v.x + v.y*v.y);
    };
    std::shared_ptr<ShapeComponent> s = owner->get_components<ShapeComponent>()[0];
    s->get_shape().setFillColor(sf::Color::Green);
    
    if (length(owner->get_position() - _player->get_position()) > 100.0f){
        std::shared_ptr<StateMachineComponent> sm = owner->get_components<StateMachineComponent>()[0];
        sm->change_state("normal");
    }
```

### Completing State Machine Scene

OK, we have ourselves a state machine system, and we have some states. How do we use it? Well, we need to create an entity ("the enemy") and attach a state machine component. We then use the component to add some states, and set our initial state. Sounds easy right? It is. The updated loading of the state scene is below.

```cpp
//scenes.cpp
void StateScene::load(){
    std::shared_ptr<Entity> player = make_entity();
    player->set_position(sf::Vector2f(param::game_width / 2.f,param::game_height / 2.f));
    auto s = player->add_component<ShapeComponent>();
    s->set_shape<sf::CircleShape>(10.0f);
    s->get_shape().setFillColor(sf::Color::Red);
    player->add_component<KeyboardMovementComponent>();
    
    std::shared_ptr<Entity> enemy = make_entity();
    enemy->set_position(sf::Vector2f(50.0f, 50.0f));
    s = enemy->add_component<ShapeComponent>();
    s->set_shape<sf::CircleShape>(10.0f);
    s->get_shape().setFillColor(sf::Color::Blue);
    
    std::shared_ptr<StateMachineComponent> sm = enemy->add_component<StateMachineComponent>();
    sm->add_state("normal", std::make_shared<NormalState>(player));
    sm->add_state("near", std::make_shared<NearState>(player));
    sm->change_state("normal");
}
```

The new lines of code are 16-19. If you run the application you will be able to move your player to the enemy dot and see them change colour. 

![](assets/images/screen-states.png)

Previous step: [Pathfinding](lab8_2)

Next step: [Decision Trees](lab9_2)