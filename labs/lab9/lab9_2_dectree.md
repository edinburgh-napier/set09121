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
    virtual void makeDecision(Entity *owner) = 0;
};
```

### Defining Some Decision Types

With the basic type in place we will now define two decision types: binary and multi. The former will have a true-or-false decision. Multi-decision will examine a list of nodes.

#### Binary Decision

Binary decisions are our simplest decision behaviour, although the nature of the decision is still abstract. The aim is to traverse either down the true-branch or false-branch of the tree depending on some condition. As such our binary decision needs to have two attributes:

`trueNode` :   the branch followed on a "true" decision.
`falseNode` :   the branch followed on a "false" decision.

As such, we can actually define `makeDecision` for a binary decision point -- we just get the correct branch and call `makeDecision` on it. So we can implement `makeDecision` we provide a new method: `getBranch`. This new method will be the one we use to define specific decisions as needed. The code is below:

```cpp
//cmp_decision_tree.h
class Decision : public DecisionTreeNode
{
protected:
    std::shared_ptr<DecisionTreeNode> _trueNode;
    std::shared_ptr<DecisionTreeNode> _falseNode;
    virtual std::shared_ptr<DecisionTreeNode> getBranch(Entity *owner) = 0;
public:
    Decision(std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode) : _trueNode(trueNode), _falseNode(falseNode) { }

    void makeDecision(Entity *owner) {
        getBranch(owner)->makeDecision(owner);
    }
};
```

#### Multiple Decision

Multi-decision points are just the same as binary decisions, except we will have a list of nodes. So we can follow the same model as binary decision, we again provide a `getBranch` method to be implemented.
```cpp
//cmp_decision_tree.h
class MultiDecision : public DecisionTreeNode
{
protected:
    std::vector<std::shared_ptr<DecisionTreeNode>> _childNodes;
    virtual std::shared_ptr<DecisionTreeNode> getBranch(Entity *owner) = 0;
public:
    MultiDecision(const std::vector<std::shared_ptr<DecisionTreeNode>> &childNodes) : _childNodes(childNodes) { }

    void makeDecision(Entity *owner) {
        getBranch(owner)->makeDecision(owner);
    }
};
```

#### Random Decisions

Let us now implement a couple of decision types: random decision, and random multi-decision. These are effectively the same apart from the number of options they choose from. The declaration for the random decision is below.

```cpp
//cmp_decision_tree.h
class RandomDecision : public Decision
{
protected:
    std::shared_ptr<DecisionTreeNode> getBranch(Entity *owner) final;
public:
    RandomDecision(std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode) : Decision(trueNode, falseNode) { }
};
```

The implementation we will put in a new code file. We just need to provide the `getBranch` method. As we are defining a random choice, we will just flip a coin (so to speak). The code is below.

```cpp
//cmp_decision_tree.cpp
#include "cmp_decision_tree.h"
#include <random>
#include <chrono>

using namespace std;

std::shared_ptr<DecisionTreeNode> RandomDecision::getBranch(Entity *owner) {
    static random_device rd;
    static default_random_engine e(rd());
    static uniform_int_distribution<int> dist(0, 1);
    bool choice = dist(e) == 0;
    if (choice)
        return _trueNode;
    else
        return _falseNode;
}
```

Random multi-decision follows the same pattern but the random choice has to be one from its list. The code for the declaration and implementation are below.
```cpp
//RandomMultiDecision class -- add to {cmp_decision_tree.h
class RandomMultiDecision : public MultiDecision
{
protected:
    std::shared_ptr<DecisionTreeNode> getBranch(Entity *owner) override final;
public:
    RandomMultiDecision(const std::vector<std::shared_ptr<DecisionTreeNode>> &childNodes) : MultiDecision(childNodes) { }
};
```

```cpp
//RandomMultiDecision::getBranch -- add to cmp_decision_tree.cpp
std::shared_ptr<DecisionTreeNode> RandomMultiDecision::getBranch(Entity *owner)
{
    static random_device rd;
    static default_random_engine e(rd());
    static uniform_int_distribution<size_t> dist(0, _childNodes.size());
    return _childNodes[dist(e)];
}
```

### Decision Tree Component

As always, we want to implement our new behaviour in a way that can be plugged into an entity. Therefore, we need a new component. The `DecisionTreeComponent` is defined below and follows our standard model.

```cpp
//DecisionTreeComponent class -- add to cmp_decision_tree.h
class DecisionTreeComponent : public Component
{
private:
    std::shared_ptr<DecisionTreeNode> _decisionTree;
public:
    void update(double) override;
    void render() override { }
    explicit DecisionTreeComponent(Entity *p, std::shared_ptr<DecisionTreeNode> decisionTree);
    DecisionTreeComponent() = delete;
};
```

The implementation is actually very easy. The component has a decision tree -- it just calls `makeDecision` on it. Job done. The code is below.
```cpp
//Definition of DecisionTreeComponent -- add to cmp_decision_tree.cpp
DecisionTreeComponent::DecisionTreeComponent(Entity *p, shared_ptr<DecisionTreeNode> decisionTree) : _decisionTree(decisionTree), Component(p) { }

void DecisionTreeComponent::update(double dt) {
    _decisionTree->makeDecision(_parent);
}
```

## Testing Decision Trees -- Combining Steering, State, and Decisions


OK time to combine everything we have done over the last two labs into an AI behaviour. We will create a system that uses states to control objects. Those states will execute steering behaviours as appropriate. Then we will use a decision tree to control when to change to a particular state.

### Our Decision Tree and State Diagram

To give an idea of the behaviour we are going to implement see this Figure: 

![Combined State and Decision Diagram](assets/images/combined-state-decision.png)

This combines the decisions and states into a single representation.

### Defining Steering States

Our first task is to implement some steering states. We will need three: stationary, seek, and flee. These will just call the respective steering behaviours from our previous lab and change colours accordingly. The code is defined below.

```cpp
//steering_states.h
#pragma once

#include "steering.h"
#include "components/cmp_state_machine.h"

class StationaryState : public State
{
public:
    StationaryState() = default;
    void execute(Entity*, double) noexcept override;
};

class SeekState : public State
{
private:
    Seek _steering;
public:
    SeekState(std::shared_ptr<Entity> owner, std::shared_ptr<Entity> player) : _steering(owner.get(), player.get(), 50.0f) { }
    void execute(Entity*, double) noexcept override;
};

class FleeState : public State
{
private:
    Flee _steering;
public:
    FleeState(std::shared_ptr<Entity> owner, std::shared_ptr<Entity> player) : _steering(owner.get(), player.get(), 50.0f) { }
    void execute(Entity*, double) noexcept override;
};
```

```cpp
//steering_states.cpp
#include "steering_states.h"
#include "components/cmp_sprite.h"

using namespace sf;

void StationaryState::execute(Entity *owner, double dt) noexcept {
    auto s = owner->get_components<ShapeComponent>();
    s[0]->getShape().setFillColor(Color::Blue);
}

void SeekState::execute(Entity *owner, double dt) noexcept {
    auto s = owner->get_components<ShapeComponent>();
    s[0]->getShape().setFillColor(Color::Green);
    auto output = _steering.getSteering();
    owner->setPosition(owner->getPosition() + (output.direction * (float)dt));
}

void FleeState::execute(Entity *owner, double dt) noexcept {
    auto s = owner->get_components<ShapeComponent>();
    s[0]->getShape().setFillColor(Color::Yellow);
    auto output = _steering.getSteering();
    owner->setPosition(owner->getPosition() + (output.direction * (float)dt));
}
```

### Defining Steering Decisions

We also need to define some decisions. We need a distance one which will be used to determine if the player is within a certain range. We will also need decisions to change our state to stationary, seek, or flee. These are all relatively trivial and are defined below.

```cpp
//steering_decisions.h
#pragma once

#include <engine.h>
#include "components/cmp_decision_tree.h"
#include "components/cmp_state_machine.h"

class DistanceDecision : public Decision
{
private:
    std::shared_ptr<Entity> _player;
    float _distance;
protected:
    std::shared_ptr<DecisionTreeNode> getBranch(Entity *owner) final
    {
        float dist = sf::length(owner->getPosition() - _player->getPosition());
        if (dist < _distance)
            return _trueNode;
        else
            return _falseNode;
    }
public:
    DistanceDecision(std::shared_ptr<Entity> player, float distance, std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode) : _player(player), _distance(distance), Decision(trueNode, falseNode) { }
};

class StationaryDecision : public DecisionTreeNode
{
public:
    void makeDecision(Entity *owner) final {
        auto sm = owner->get_components<StateMachineComponent>();
        sm[0]->changeState("stationary");
    }
};

class SeekDecision : public DecisionTreeNode
{
public:
    void makeDecision(Entity *owner) final {
        auto sm = owner->get_components<StateMachineComponent>();
        sm[0]->changeState("seek");
    }
};

class FleeDecision : public DecisionTreeNode
{
public:
    void makeDecision(Entity *owner) final {
        auto sm = owner->get_components<StateMachineComponent>();
        sm[0]->changeState("flee");
    }
};
```

### Completing Decision Tree Scene

All we need to do now is complete the `DecisionTree::Load` method. We will create 100 enemies as before. Each enemy will have a state machine and a decision tree attached. Then the behaviour will just take care of itself. The code is below. The state machine part you should recognise from the first half of the lesson. The decision tree part is new, but if you examine the structure you should recognise it from that presented in Figure [1.7](#fig:combined-state-decision){reference-type="ref" reference="fig:combined-state-decision"}.

```cpp
//Updated DecisionScene::Load
void DecisionScene::Load()
{
    auto player = makeEntity();
    player->addTag("player");
    player->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, Engine::GetWindow().getSize().y / 2));
    auto s = player->addComponent<ShapeComponent>();
    s->setShape<CircleShape>(10.0f);
    s->getShape().setFillColor(Color::Red);
    player->addComponent<BasicMovementComponent>();
    
    random_device dev;
    default_random_engine engine(dev());
    uniform_real_distribution<float> x_dist(0.0f, Engine::GetWindow().getSize().x);
    uniform_real_distribution<float> y_dist(0.0f, Engine::GetWindow().getSize().y);
    
    for (size_t n = 0; n < 100; ++n)
    {
        auto enemy = makeEntity();
        enemy->setPosition(Vector2f(x_dist(engine), y_dist(engine)));
        auto s = enemy->addComponent<ShapeComponent>();
        s->setShape<CircleShape>(10.0f);
        s->getShape().setFillColor(Color::Blue);
        
        auto sm = enemy->addComponent<StateMachineComponent>();
        sm->addState("stationary", make_shared<StationaryState>());
        sm->addState("seek", make_shared<SeekState>(enemy, player));
        sm->addState("flee", make_shared<FleeState>(enemy, player));
        
        auto decision = make_shared<DistanceDecision>(
            player,
            50.0f,
            make_shared<FleeDecision>(),
            make_shared<DistanceDecision>(
                player,
                100.0f,
                make_shared<RandomDecision>(
                    make_shared<SeekDecision>(),
                    make_shared<StationaryDecision>()),
                make_shared<SeekDecision>()));
        
        enemy->addComponent<DecisionTreeComponent>(decision);
    }
}
```

Running this example will give a new behaviour to our system. Try it out. Hopefully it looks like this:

<video class="middle" width="600" height="600" loop autoplay>
  <source src="assets/videos/dectree.mp4" type="video/mp4">
</video>

Previous step: [State Machines](lab9_1)

Next step: [Deploying](lab10_1)