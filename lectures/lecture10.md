---
title: "Lecture 10 - Engines and ECM"
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
Babis Koniaris
<br>


School of Computing. Edinburgh Napier University


---

# Requirements of a Game

---

# What does a game need? From a Programmer's POV:

- **Content**
 - 3d Models, Shaders, Textures, Text, Fonts, Music, Video, Saves, Levels/Game State etc.. <!-- .element: class="fragment" -->
- **Processing & I/O**
 - Rendering, User input, Networking, Audio, Loading/Unloading/Streaming  <!-- .element: class="fragment" -->
- **Logic and Mechanics**
 - Physics, AI, Gameplay rules. <!-- .element: class="fragment" -->

---

# Game Engine Architecture


---

# Complexity

<a href="assets/images/2d_engine_architecture.png">![image](assets/images/2d_engine_architecture_b.png)</a> <!-- .element height="760px"  -->

---

# Complexity

<a href="assets/images/2d_engine_architecture.png">![image](assets/images/2d_engine_architecture_a.png)</a> <!-- .element height="760px"  -->

---

# Combating the Complexity

- Game Codebases Get Big Fast <!-- .element: class="fragment" -->
- Taming and maintaing it tests your ability as a Software Engineer <!-- .element: class="fragment" -->
- We've covered some Software Patterns that you can pull out of your toolbox to help. These help solve small isolated design problems. <!-- .element: class="fragment" -->
- Use standard approaches to deal with complexity: abstraction, decoupling, encapsulation <!-- .element: class="fragment" -->
- E.g. separating your gameplay logic (jump!) from the core engine logic (load a file!). <!-- .element: class="fragment" -->


---

# Build The Wall 

![image](assets/images/api_wall.png)


---

# Abstraction

---

# Abstraction - And so we build Games Engines

Do we need them?

After all we didn't always have them? <!-- .element: class="fragment" -->

Q: When do you think you need to separate engine code? <!-- .element: class="fragment" -->

- A: From the beginning <!-- .element: class="fragment" -->
- A: Later, when you identify things that can be abstracted <!-- .element: class="fragment" -->
- A: Never! Who's got the time for that <!-- .element: class="fragment" -->

Not all games need an 'engine' <!-- .element: class="fragment" -->

- Some are simplistic enough to not need it. <!-- .element: class="fragment" -->
- We already have an engine somewhat: SFML.  <!-- .element: class="fragment" -->
- This is already isolated from our code. But it doesn't do everything we need. <!-- .element: class="fragment" -->

---

# Build The Wall 

![image](assets/images/api_wall2.png)

---

# Software Abstraction techniques

---

# Object Orientation.

OO is hammered into you since 1st year as the solution to software complexity.

![image](assets/images/software_development.png)

... But it's not perfect. <!-- .element: class="fragment" -->

Enter: The Evil Tree Problem <!-- .element: class="fragment" -->

---

# Object Orientation & the Evil Tree

![image](assets/images/oo_strcuture.PNG)

---

# Possible Evil Tree Solutions

To fix this we need to either:
- Use multiple Inheritance (Danger Zone, bugs) <!-- .element: class="fragment" -->
- Use interfaces (which C++ can emulate, tediously) <!-- .element: class="fragment" -->
- Throw our design in the bin, unceremoniously, and... <!-- .element: class="fragment" -->
- **USE COMPOSITION, NOT INHERITANCE** <!-- .element: class="fragment" -->


---

# The Evil Tree Solution - The Entity Component Model

![image](assets/images/ecm_strcuture.png)


---

# ECM 

ECM enables Data Oriented design.

![image](assets/images/ecs2.png)


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
    std::shared_ptr<T> addComponent(Targs... params);

    template <typename T>
    const std::vector<std::shared_ptr<T>>& getComponents() const;
    
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

## C++ TEMPLATES have arrived!
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
	//template <typename T>
    //std::shared_ptr<T> addComponent(Targs... params){}
    //Or no templates:
    Component* addComponent(Component*){}
}
```

```cpp
auto pl = make_shared<Entity>();

ShapeComponent* sc = new ShapeComponent();
auto s = pl->addComponent(sc);

// later on...
//Uh oh
//pl->getComponents<PlayerMovementComponent>()[0]->setSpeed(150.f);
//We would have to do something like this:
pl->getComponentsOfType(PlayerMovementComponent)[0]->setSpeed(150.f);
//Not *So* bad, but how would that function work?
```


---

# ECM without templates

```cpp
getComponentsOftype(ComponentType CT){
  foreach(c in component){
    attempt to cast C to CT;
    did it work?
      return C; 
  }
}
```

That's what we have to do, due to C++ lack of reflection. <!-- .element: class="fragment" -->

It's verbose, ugly and slow. <!-- .element: class="fragment" -->

Templates are verbose, ugly and fast. <!-- .element: class="fragment" -->

They are worth delving deeper to, eventually. <!-- .element: class="fragment" -->

---

# ECM template Deep Dive 1

```cpp
class Entity {

  template <typename T, typename... Targs>
  std::shared_ptr<T> addComponent(Targs... params) {
    static_assert(std::is_base_of<Component, T>::value, "T != component");
    std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
    _components.push_back(sp);
    return sp;
  }

};
```


---

# ECM template Deep Dive 2

```cpp
class Entity {

  template <typename T, typename... Targs>
  std::shared_ptr<T> addComponent(Targs... params) {
    static_assert(std::is_base_of<Component, T>::value, "T != component");
    std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
    _components.push_back(sp);
    return sp;
  }

};
```


```cpp
class Component {
  private: 
    Entity* _parent;
  public:
    Component(Entity* const p);
};

class PickupComponent : public Component {
private: 
  bool _isBig;
public:
  PickupComponent() = delete;
  PickupComponent(Entity* p, bool big = false);
};
```
<!-- .element: class="fragment" -->


---

# ECM template Deep Dive 3

Replace T with PickupComponent

```cpp
class Entity {
  std::shared_ptr<PickupComponent> addPickupComponent(bool big) {
    std::shared_ptr<PickupComponent> sp(std::make_shared<PickupComponent>(this,big));
    _components.push_back(sp);
    return sp;
  }
};
```

```cpp
class Component {
  private: 
    Entity* _parent;
  public:
    Component(Entity* const p);
};

class PickupComponent : public Component {
private: 
  bool _isBig;
public:
  PickupComponent() = delete;
  PickupComponent(Entity* p, bool big = false);
};
```

---

# ECM template Deep Dive 4

With Old Raw Pointers
```cpp
class Entity {
  PickupComponent* addPickupComponent(bool big) {
    PickupComponent* sp = new PickupComponent(this, big);
    _components.push_back(sp);
    return sp;
  }
};
```

with New Safe Smart Pointers.

```cpp
class Entity {
  std::shared_ptr<PickupComponent> addPickupComponent(bool big) {
    std::shared_ptr<PickupComponent> sp(std::make_shared<PickupComponent>(this,big));
    _components.push_back(sp);
    return sp;
  }
};
```
<!-- .element: class="fragment" -->

---

# Summary


---

# Summary

- Games are complicated systems!
- This is why we use clever tricks to help simplify the problem
- ECM is a great way to reuse code
- But we do have to use C++ templates to do these nicely
