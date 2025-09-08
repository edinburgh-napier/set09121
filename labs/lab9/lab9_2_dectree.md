---
title: "Lab 9 - Decision Trees"
keywords: lab9_2
tags: [lab9]
permalink:  lab9_2.html
summary: lab9_2
sidebar: home_sidebar
---

## Building a Decision Tree System

<video class="middle" width="300" height="300" loop autoplay>
  <source src="assets/videos/dectree.mp4" type="video/mp4">
</video>


Our decision tree system will follow the same model. First we will implement the system and then we will test it. To do this, we will combine the steering, state, and decision systems together to make a (slightly better) mobbing system from the last lab.

### Decision Tree Fundamentals

The clue to decision trees is in the name -- it is a tree-like structure. As such, we need to define a node for the tree. However, depending on the decision point it maybe a leaf-node, a binary decision, or a multi-decision. As such, we will keep our basic decision tree node type simple. It is given below. The key behaviour is in `makeDecision`. Here, code will determine what should happen. It may be that we traverse a branch, or that we run some behaviour code. We will look at examples shortly.

```cpp
//cmp_decision_tree.h
#pragma once

#include <ecm.h>
#include <memory>
#include <vector>

class DecisionTreeNode
{
public:
    virtual ~DecisionTreeNode() = default;
    virtual void make_decision(Entity *owner) = 0;
};
```

### Defining Some Decision Types

With the basic type in place we will now define two decision types: binary and multi. The former will have a true-or-false decision. Multi-decision will examine a list of nodes.

#### Binary Decision

Binary decisions are our simplest decision behaviour, although the nature of the decision is still abstract. The aim is to traverse either down the true-branch or false-branch of the tree depending on some condition. As such our binary decision needs to have two attributes:

`true_node` :   the branch followed on a "true" decision.
`false_node` :   the branch followed on a "false" decision.

As such, we can actually define `make_decision` for a binary decision point -- we just get the correct branch and call `make_decision` on it. So we can implement `make_decision` we provide a new method: `get_branch`. This new method will be the one we use to define specific decisions as needed. The code is below:

```cpp
//decision_tree.hpp
class BinaryDecision : public DecisionTreeNode
{
protected:
    std::shared_ptr<DecisionTreeNode> _true_node;
    std::shared_ptr<DecisionTreeNode> _false_node;
    virtual std::shared_ptr<DecisionTreeNode> get_branch(Entity *owner) = 0;
public:
    BinaryDecision(std::shared_ptr<DecisionTreeNode> true_node, std::shared_ptr<DecisionTreeNode> false_node) 
        : _true_node(true_node), _false_node(false_node) { }

    void make_decision(Entity *owner) {
        get_branch(owner)->make_decision(owner);
    }
};
```

#### Multiple Decision

Multi-decision points are just the same as binary decisions, except we will have a list of nodes. So we can follow the same model as binary decision, we again provide a `get_branch` method to be implemented.
```cpp
//decision_tree.hpp
class MultiDecision : public DecisionTreeNode
{
protected:
    std::vector<std::shared_ptr<DecisionTreeNode>> _child_nodes;
    virtual std::shared_ptr<DecisionTreeNode> get_branch(Entity *owner) = 0;
public:
    MultiDecision(const std::vector<std::shared_ptr<DecisionTreeNode>> &child_nodes) : _child_nodes(child_nodes) { }

    void make_decision(Entity *owner) {
        get_branch(owner)->make_decision(owner);
    }
};

```

#### Random Decisions

Let us now implement a couple of decision types: random decision, and random multi-decision. These are effectively the same apart from the number of options they choose from. The declaration for the random decision is below.

```cpp
//decision_tree.hpp
class RandomDecision : public BinaryDecision
{
protected:
    std::shared_ptr<DecisionTreeNode> get_branch(Entity *owner);
public:
    RandomDecision(std::shared_ptr<DecisionTreeNode> true_node, std::shared_ptr<DecisionTreeNode> false_node) : BinaryDecision(true_node, false_node) { }
};
```

The implementation we will put in a new code file. We just need to provide the `get_branch` method. As we are defining a random choice, we will just flip a coin (so to speak). The code is below.

```cpp
//decision_tree.cpp
#include "decision_tree.hpp"
#include <random>
#include <chrono>

std::shared_ptr<DecisionTreeNode> RandomDecision::get_branch(Entity *owner) {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    static std::uniform_int_distribution<int> dist(0, 1);
    bool choice = dist(e) == 0;
    if (choice)
        return _true_node;
    else
        return _false_node;
}
```

Random multi-decision follows the same pattern but the random choice has to be one from its list. The code for the declaration and implementation are below.
```cpp
//deciision_tree.hpp
class RandomMultiDecision : public MultiDecision
{
protected:
    std::shared_ptr<DecisionTreeNode> get_branch(Entity *owner) override;
public:
    RandomMultiDecision(const std::vector<std::shared_ptr<DecisionTreeNode>> &child_nodes) : MultiDecision(child_nodes) { }
};
```

```cpp
//decision_tree.cpp
std::shared_ptr<DecisionTreeNode> RandomMultiDecision::get_branch(Entity *owner)
{
    static std::random_device rd;
    static std::default_random_engine e(rd());
    static std::uniform_int_distribution<size_t> dist(0, _child_nodes.size());
    return _child_nodes[dist(e)];
}
```

### Decision Tree Component

As always, we want to implement our new behaviour in a way that can be plugged into an entity. Therefore, we need a new component. The `DecisionTreeComponent` is defined below and follows our standard model.

```cpp
//ai_cmps.hpp
class DecisionTreeComponent : public Component
{
private:
    std::shared_ptr<DecisionTreeNode> _decision_tree;
public:
    void update(const float &) override;
    void render() override { }
    explicit DecisionTreeComponent(Entity *p, std::shared_ptr<DecisionTreeNode> decision_tree);
    DecisionTreeComponent() = delete;
};
```

The implementation is actually very easy. The component has a decision tree -- it just calls `make_decision` on it. Job done. The code is below.
```cpp
//ai_cmps.cpp
DecisionTreeComponent::DecisionTreeComponent(Entity *p, shared_ptr<DecisionTreeNode> decisionTree) : _decision_tree(decision_tree), Component(p) { }

void DecisionTreeComponent::update(const float &dt) {
    _decisionTree->make_decision(_parent);
}
```

## Testing Decision Trees -- Combining Steering, State, and Decisions


OK time to combine everything we have done over the last two labs into an AI behaviour. We will create a system that uses states to control objects. Those states will execute steering behaviours as appropriate. Then we will use a decision tree to control when to change to a particular state.

### Our Decision Tree and State Diagram

To give an idea of the behaviour we are going to implement see this Figure: 

![Combined State and Decision Diagram](assets/images/combined-state-decision.png)

This combines the decisions and states into a single representation.

### Defining Steering States

Our first task is to implement some steering states. We will need three: stationary, seek, and flee. To do so, we will define a new clas `SteeringState` that will take as parameters: a color and a steering function. This two parameters will define in which state we are.  These will just call the respective steering behaviours from our previous lab and change colours accordingly. The code is defined below:

```cpp
//states.hpp
class SteeringState : public State
{
private:
    SteeringFunction _steering;
    std::shared_ptr<Entity> _player;
    float _max_speed;
    sf::Color _color;
public:
    SteeringState(SteeringFunction steering,std::shared_ptr<Entity> player, float max_speed, sf::Color color);
    void execute(Entity*, const float &) override;
};
```
You would have notice that the member `_steering` is of type `SteeringFunction`. We have not defined this type yet. We will define it in the *ai_cmps.hpp* by modifying our previous implementation of the `steering functions` from the previous lab.
```cpp
//ai_cmps.hpp
#include <functional>
...
using SteeringFunction = std::function<SteeringOutput(const sf::Vector2f &,const sf::Vector2f &)>;

struct SteeringBehaviours{
    static SteeringFunction seek;
    static SteeringFunction flee;
    static SteeringFunction stationary;
};
```
Here, we use the type `std::function` to define variables that will be function taking as argument two `sf::Vector2f` and return a `SteeringOutput`. We use the key word `using` to rename the lengthy type `std::function<SteeringOutput(const sf::Vector2f &,const sf::Vector2f &)>` into `SteeringFunction`. Also, from the last lab we defined a new steering behaviour called *stationary*.

Then, in *ai_cmps.cpp*, we can modify our implementation to match this new definition.
```cpp
SteeringFunction SteeringBehaviours::seek = [](const sf::Vector2f &target,const sf::Vector2f &self) -> SteeringOutput{
    auto length = [](const sf::Vector2f &v) -> float{
        return std::sqrt(v.x*v.x+v.y*v.y);
    };
    SteeringOutput steering;
    steering.direction = target - self;
    steering.direction = steering.direction/length(steering.direction) ;
    steering.rotation = 0.0f;
    return steering;
};


SteeringFunction SteeringBehaviours::flee = [](const sf::Vector2f &target,const sf::Vector2f &self) -> SteeringOutput{
    auto length = [](const sf::Vector2f &v) -> float{
        return std::sqrt(v.x*v.x+v.y*v.y);
    };
    SteeringOutput steering;
    steering.direction = self - target;
    steering.direction = steering.direction/length(steering.direction) ;
    steering.rotation = 0.0f;
    return steering;
};


SteeringFunction SteeringBehaviours::stationary = [](const sf::Vector2f &target,const sf::Vector2f &self) -> SteeringOutput{
    auto length = [](const sf::Vector2f &v) -> float{
        return std::sqrt(v.x*v.x+v.y*v.y);
    };
    SteeringOutput steering;
    steering.direction = {0,0};
    steering.rotation = 0.0f;
    return steering;
};
```
The implementation is almost identical with our static functions from the previous lab except from two important points:
- we are delaring variables `seek`, `flee` and `stationary` of type `SteeringFunction`. So, we use the syntax `type var = ...;`
- To define the function stored in the variable, we use the c++ *lambda function* syntax: `[](...) -> ... {...}`. We alreadt used this syntax to define temporary function like `length`.

To be noted, the type `std::function` allow the programmer to use a **functional**  approach instead of a **object-oriented** approach. To defined our steering behaviours and store them in a variable, we could have define an abstract class `SteeringBehaviour` and define three daughter classes inheriting from this abstract class. This works perfectly but use inheritance and more redundant repeated code. 

Finally, our `execute` of `SteeringState` is implemented as follow:
```cpp
//states.cpp
SteeringState::SteeringState(SteeringFunction steering,std::shared_ptr<Entity> player,float max_speed,sf::Color color)
    : _steering(steering),_player(player),_max_speed(max_speed), _color(color){}

void SteeringState::execute(Entity *owner, const float &dt){
    std::shared_ptr<ShapeComponent> s = owner->get_components<ShapeComponent>()[0];
    s->get_shape().setFillColor(_color);
    SteeringOutput output = _steering(_player->get_position(),owner->get_position());
    owner->set_position(owner->get_position() + (output.direction*_max_speed * dt));
}
```

### Defining Steering Decisions

We also need to define some decisions. We need a distance one which will be used to determine if the player is within a certain range. We will also need decisions to change our state to stationary, seek, or flee. These are all relatively trivial and are defined below.

```cpp
//decision_tree.hpp
class DistanceDecision : public BinaryDecision
{
private:
    std::shared_ptr<Entity> _player;
    float _distance;
protected:
    std::shared_ptr<DecisionTreeNode> get_branch(Entity *owner);
public:
    DistanceDecision(std::shared_ptr<Entity> player, float distance, std::shared_ptr<DecisionTreeNode> true_node, std::shared_ptr<DecisionTreeNode> false_node) 
    : _player(player), _distance(distance), BinaryDecision(true_node, false_node) { }
};

class SteeringDecision : public DecisionTreeNode
{
public:
    SteeringDecision(const std::string& state): _state(state){}
    void make_decision(Entity *owner);
private:
    std::string _state;
};
```

```cpp
//decision_tree.cpp
std::shared_ptr<DecisionTreeNode> DistanceDecision::get_branch(Entity* owner)
    {
        auto length = [](const sf::Vector2f &v) -> float{
        return std::sqrt(v.x*v.x+v.y*v.y);
        };
        float dist = length(owner->get_position() - _player->get_position());
        if (dist < _distance)
            return _true_node;
        else
            return _false_node;
    }

void SteeringDecision::make_decision(Entity *owner)
{
    std::shared_ptr<StateMachineComponent> sm = owner->get_components<StateMachineComponent>()[0];
    sm->change_state(_state);
}
```

### Completing Decision Tree Scene

All we need to do now is complete the `DecisionTree::load` method. We will create 100 enemies as before. Each enemy will have a state machine and a decision tree attached. Then the behaviour will just take care of itself. The code is below. The state machine part you should recognise from the first half of the lesson. The decision tree part is new, but if you examine the structure you should recognise it from that presented in Figure [1.7](#fig:combined-state-decision){reference-type="ref" reference="fig:combined-state-decision"}.

```cpp
//scenes.cp
void DecisionScene::load() {
    std::shared_ptr<Entity> player = make_entity();
    player->set_position(sf::Vector2f(param::game_width / 2, 
        param::game_height / 2));
    std::shared_ptr<ShapeComponent> s = player->add_component<ShapeComponent>();
    s->set_shape<sf::CircleShape>(10.0f);
    s->get_shape().setFillColor(sf::Color::White);
    player->add_component<KeyboardMovementComponent>();
    
    std::random_device dev;
    std::default_random_engine engine(dev());
    std::uniform_real_distribution<float> x_dist(0.0f, param::game_width);
    std::uniform_real_distribution<float> y_dist(0.0f, param::game_height);
    
    for (size_t n = 0; n < 100; ++n)
    {
        std::shared_ptr<Entity> enemy = make_entity();
        enemy->set_position(sf::Vector2f(x_dist(engine), y_dist(engine)));
        std::shared_ptr<ShapeComponent> s = enemy->add_component<ShapeComponent>();
        s->set_shape<sf::CircleShape>(10.0f);
        s->get_shape().setFillColor(sf::Color::Blue);
        
        auto sm = enemy->add_component<StateMachineComponent>();
        sm->add_state("stationary", std::make_shared<SteeringState>(SteeringBehaviours::stationary,player,50.f,sf::Color::Blue));
        sm->add_state("seek", std::make_shared<SteeringState>(SteeringBehaviours::seek,player,50.f,sf::Color::Green));
        sm->add_state("flee", std::make_shared<SteeringState>(SteeringBehaviours::flee,player,50.f,sf::Color::Yellow));
        
        auto decision = std::make_shared<DistanceDecision>(
            player,
            50.0f,
            std::make_shared<SteeringDecision>("flee"),
            std::make_shared<DistanceDecision>(
                player,
                100.0f,
                std::make_shared<RandomDecision>(
                    std::make_shared<SteeringDecision>("seek"),
                    std::make_shared<SteeringDecision>("stationary")),
               std::make_shared<SteeringDecision>("seek")));
        
        enemy->add_component<DecisionTreeComponent>(decision);
    }
}
```

Running this example will give a new behaviour to our system. Try it out. Hopefully it looks like this:

<video class="middle" width="600" height="600" loop autoplay>
  <source src="assets/videos/dectree.mp4" type="video/mp4">
</video>

Previous step: [State Machines](lab9_1)

Next step: [Deploying](lab10_1)