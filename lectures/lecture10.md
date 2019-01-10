---
title: "Lecture10"
keywords: Lecture
tags: [Lecture]
permalink:  lecture10.html
summary: lecture10
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---

Requirements of a Game
======================

What does a game need? From a Programmers point of view:

-   **Content**\
    ~~3d Models~~, ~~Shaders~~, Textures, Text, Fonts, Music, Video,
    Saves, levels/gamestate.

-   **Processing & io**\
    Rendering, User input, Networking, Audio,
    loading/unloading/streaming

-   **Logic and Mechanics**\
    Physics, AI, Gameplay rules.\

-   **Question**:

-   When does a game need any of the above?

-   **A: Right Now** (and without warning)

Game Engine Architecture
========================

Complexity

![image](2d_engine_architecture){width="60%"}

Combating the Complexity

-   Game Codebases Get Big Fast

-   Taming and maintaing it tests your Ability as a Software Engineer

-   We've covered some Software patterns that you can pull out of your
    toolbox to help.\
    These help solve small isolated design problems.

-   When it comes to pulling it all together as one giant moving thing,
    you need to think about the grand design of your *Engine*

-   This means separating your gameplay logic from the Generic Engine
    logic.

Build The Wall ![image](api_wall){width="\textwidth"}

Abstraction

-   And so we build Games Engines

-   But do we need them? \....\
    \...We didn't always have them.

-   Q: How complex do you think a game needs to before you think you
    need to write Engine Code?

When to start the engine

-   A:\

-   A: Once your code gets abstract enough

-   A: From the start

-   A: Never, and write some crazy \[fast/bad\] code

Not all Games Need an 'engine'\
Some are simplistic enough to not need it.\
We already have an engine somewhat: SFML. This is already isolated form
our code. But it doesn't do Everything we need.

Build The Wall ![image](api_wall2){width="\textwidth"}

Software techniques
===================

Object Orientation.

OO is Hammered into you since 1^st^ year, as the solution to software
complexity.

![image](software_development){width="42%"}

\... But it's not perfect.

The Evil Tree Problem

![image](oo_strcuture){width="65%"}

Possible Evil Tree Solutions To Fix this We need either:

-   Multiple Inheritance\
    (Which c++ doesn't have)

-   Or Interfaces\
    (Which c++ doesn't have)

C++ as a language doesn't have these natively, but it doesn't stop us
from adding it ourselves.

The Evil Tree Solutions The Entity Component Model

ECM ![image](ecm_strcuture){width="\textwidth"}

ECM ECM enables Data Orientated design.
![image](ecs2){width="\textwidth"}

ECM Code

``` {language="c++" breaklines="false" basicstyle="\tiny"}
class Entity {

protected:
  std::vector<std::shared_ptr<Component>> _components;

public:
  virtual void update(double dt);
  virtual void render();

  template <typename T, typename... Targs>
  std::shared_ptr<T> addComponent(Targs... params)

  template <typename T>
  const std::vector<std::shared_ptr<T>>& getComponents() const 
  
  void removeComponent(std::shared_ptr<Component>);
};


class Component {
  Entity* const _parent;
  virtual void update(double dt) = 0;
  virtual void render() = 0;
};
```

ECM Code

``` {language="c++" breaklines="false" basicstyle="\tiny"}
auto pl = make_shared<Entity>();

auto s = pl->addComponent<ShapeComponent>();
s->setShape<sf::CircleShape>(12.f);
s->getShape().setFillColor(Color::Yellow);

pl->addComponent<PlayerMovementComponent>();

// later on...
pl->getComponents<PlayerMovementComponent>()[0]->setSpeed(150.f);
```
