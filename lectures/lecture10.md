---
title: "Lecture10"
keywords: Lecture
tags: [Lecture]
permalink:  lecture10.html
summary: lecture10
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 10 - Game Engines and ECM
### SET09121 - Games Engineering

<br><br>
Kevin Chalmers and Sam Serrels

School of Computing. Edinburgh Napier University


---

# Requirements of a Game


---

# What does a game need? From a Programmers point of view:

-   **Content**
 - *3d Models*, *Shaders*, Textures, Text, Fonts, Music, Video, Saves, levels/gamestate.
-   **Processing & io**
 - Rendering, User input, Networking, Audio, loading/unloading/streaming
- **Logic and Mechanics**
 - Physics, AI, Gameplay rules.


**Question**:  When does a game need any of the above?
- **A: Right Now** (and without warning)


---

# Game Engine Architecture


---

# Complexity

![image](2d_engine_architecture)


---

# Combating the Complexity

-   Game Codebases Get Big Fast
-   Taming and maintaing it tests your Ability as a Software Engineer
-   We've covered some Software patterns that you can pull out of your toolbox to help.These help solve small isolated design problems.
-   When it comes to pulling it all together as one giant moving thing, you need to think about the grand design of your *Engine*
-   This means separating your gameplay logic from the Generic Engine logic.


---

# Build The Wall 

![image](api_wall)


---

# Abstraction

---

# Abstraction - And so we build Games Engines

-   But do we need them?
 - ...We didn't always have them.

Q: How complex do you think a game needs to before you think you need to sperate Engine Code?

-   A: `¯\_(ツ)_/¯`
-   A: Once your code gets abstract enough
-   A: From the start
-   A: Never, and write some crazy fast/bad code

Not all Games Need an 'engine'

- Some are simplistic enough to not need it.
- We already have an engine somewhat: SFML. 
 - This is already isolated from our code. But it doesn't do Everything we need.

---

# Build The Wall 

![image](api_wall2)

---

# Software Abstraction techniques

---

# Object Orientation.

OO is Hammered into you since 1st year, as the solution to software complexity.

![image](software_development)

... But it's not perfect.

Enter: The Evil Tree Problem

---

# ![image](oo_strcuture)

---

# Possible Evil Tree Solutions

To Fix this We need either:
-   Multiple Inheritance (Which c++ doesn't have)
-   Or Interfaces (Which c++ doesn't have)

C++ as a language doesn't have these natively, but it doesn't stop us from adding it ourselves.


---

# The Evil Tree Solutions The Entity Component Model

ECM ![image](ecm_strcuture)


---

# ECM 

ECM enables Data Orientated design.

![image](ecs2)


---

# ECM PseudoCode

```cpp
class Entity {

  protected:
    List_of_components;

  public:
    update(delta_time);
    render();

    addComponent(Component);
    getComponents();
    
    removeComponent(Component);
};

class Component {
  Entity* _parent;
  update(delta_time);
  render();
};
``` 
<!-- .element: class="stretch" -->


---

# ECM Code


```cpp
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
<!-- .element: class="stretch" -->


---

# ECM Code

```cpp
auto pl = make_shared<Entity>();

auto s = pl->addComponent<ShapeComponent>();
s->setShape<sf::CircleShape>(12.f);
s->getShape().setFillColor(Color::Yellow);

pl->addComponent<PlayerMovementComponent>();

// later on...
pl->getComponents<PlayerMovementComponent>()[0]->setSpeed(150.f);
```


---

## C++ TEMPLATES!
`template <typename HELP>!`


---

# Why not just use classes?

```cpp
class Component {
};

class ShapeComponent : public Component{
} //no change

class Entity {

  protected:
    std::vector<std::shared_ptr<Component>> _components; //No change

  public:
    //templated:
    std::shared_ptr<T> addComponent(Targs... params){}
    //Or no templates:
    Component* addComponent(Component*){}
}
```
<!-- .element: class="stretch" -->


---

# ECM Code

```cpp
auto pl = make_shared<Entity>();

ShapeComponent* sc = new ShapeComponent();
auto s = pl->addComponent<sc>();

// later on...
//Uh oh
pl->getComponents<PlayerMovementComponent>()[0]->setSpeed(150.f);
//We would have to do something lik this:
pl->getComponentsOftype(PlayerMovementComponent)[0]->setSpeed(150.f);
//Not *So* bad, but how would that function work?
```


---

# ECM without tempaltes

```cpp
getComponentsOftype(ComponentType CT){
  foreach(c in component){
    attempt to cast C to CT;
    did it work?
      return C; 
  }
}
```

OR

Save a static string in each component class and compare this way at runtime.

Either way : Lots of Icky Code, and we have less functionality: 

No parameter passing, no constructing with one method.

Templates give us more for less code. It's worth learning the weird syntax.
