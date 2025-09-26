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
Leni Le Goff
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

<a href="assets/images/2d_engine_architecture.png">![image](assets/images/2d_engine_architecture_a.png)</a> <!-- .element height="30%"  -->

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

![image](assets/images/ecm_bricks.png)

---

# ECM 

ECM enables Data Oriented design.

![image](assets/images/ecs2.png)

---

# Example: The Legend of Zelda: Breath Of the Wild

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
    virtual void update(const float &dt);
    virtual void render();

    template <typename T, typename... Targs>
    std::shared_ptr<T> add_component(Targs... params);

    template <typename T>
    const std::vector<std::shared_ptr<T>>& get_components() const;
    
    void remove_component(std::shared_ptr<Component>);
};

class Component {
private:
  Entity* const _parent;//link to the parent entity
public:
  virtual void update(const float &dt) = 0;
  virtual void render() = 0;
};
``` 
<!-- .element: class="stretch" -->

---

# Component Class

```cpp
class Component {
protected:
  Entity* const _parent;//link to the parent entity
public:
  virtual void update(const float &dt) = 0; //to update the component states
  virtual void render() = 0; //to display the component
  Component(Entity *const p);
  Component() = delete;
};
```
In the Component class we have:
- *_parent* a link to the parent entity.
- The default constructor is marked as `= delete` which means it is not usable.
- A Component *needs* a link to a parent to be instantiated.
- An *update* and *render* function that will be called by the entity update and render methods.

---

# ECM Code

```cpp
std::shared_ptr<Entity> player = std::make_shared<Entity>();

std::shared_ptr<ShapeComponent> s = player->add_component<ShapeComponent>();
s->set_shape<sf::CircleShape>(12.f);
s->get_shape().setFillColor(Color::Yellow);

player->add_component<MovementComponent>();

// later on...
player->get_components<MovementComponent>()[0]->setSpeed(150.f);
```


---

## C++ TEMPLATES have arrived!
`template <  typename HELP  >!`


---

# Add component without templates

Without template the code would look like this:
```cpp
//To add a ShapeComponent
std::shared_ptr<ShapeComponent> sp = 
  std::make_shared<ShapeComponent>(this, params...));
player->_components.push_back(sp); //we would need to make _components public
```
- If we want to encapsulate this we will need to implement a specific `add_component` function for each type of components: `add_shape_component`, `add_player_movement_component`, `add_texture_component`, `add_chase_component` etc...
- Or we can just copy paste this code every time. 
- In this situation, we cannot abstract this routine. 

---

# Get a particular component without templates

```cpp
for(size_t i=0;i < player->_components.size(); ++i)
{
    std::shared_ptr<Component>& comp = player->_components[i]; 
    if( std::dynamic_pointer_cast<MovementComponent>(comp) != nullptr)
    {
        std::dynamic_pointer_cast<MovementComponent>(comp)->setSpeed(150.0f);
        break;
    }
}
```
- This is impossible to encapsulate.
- Then, you will have to use similar code everytime you want to access a component.

---

# Generalising with Template

Add a component
```cpp
template <typename T, typename... Targs>
std::shared_ptr<T> add_component(Targs... params) {
  std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
  _components.push_back(sp);
  return sp;
}
```
Get the components of a certain type
```cpp
template < typename T >
const std::vector<std::shared_ptr<T>> get_components() const {
  std::vector<std::shared_ptr<T>> ret;
  for (const std::shared_ptr<Component>& c : _components) {
    if (typeid(*c) == typeid(T)) {
      ret.push_back(std::dynamic_pointer_cast<T>(c));
    }
  }
  return std::move(ret);
}
```

---

# Generalising with Template (cont.)

The compilator will do the substitution for us at compile time.
For instance, with a `PickupComponent`.

Add a component
```cpp
std::shared_ptr<PickupComponent> add_component(/*some params*/) {
  std::shared_ptr<PickupComponent> sp(std::make_shared<T>(this, params...));
  _components.push_back(sp);
  return sp;
}
```
Get the components of a certain type
```cpp
const std::vector<std::shared_ptr<PickupComponent>> get_components() const {
  std::vector<std::shared_ptr<PickupComponent>> ret;
  for (const std::shared_ptr<Component>& c : _components) {
    if (typeid(*c) == typeid(PickupComponent)) {
      ret.push_back(std::dynamic_pointer_cast<PickupComponent>(c));
    }
  }
  return std::move(ret);
}
```

---

# Things to know about templates

- It is a powerful tool for abstraction but can get tricky to use. 
- There are class template and function template
```cpp
template< typename T >
class A{};
```
- Template classes and functions has to be defined in headers.
- All code from templates will be duplicated everywhere it is used at compile time.
- A heavily templated code will need long compile (because of generation of code) and can be very slow unless compiled with optimisation options (-O3)

---

# How the templates are compiled

```cpp
//defined in headers
template <typename T, typename... Targs>
std::shared_ptr<T> add_component(Targs... params) {
  std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
  _components.push_back(sp);
  return sp;
}
```
We call this function
```cpp
//in the source file
std::shared_ptr<ShapeComponent> s = player->add_component<ShapeComponent>();
s->set_shape<sf::CircleShape>(12.f);
s->get_shape().setFillColor(Color::Yellow);
```
The compilator will generate this code before compiling.
```cpp 
std::shared_ptr<ShapeComponent> add_component() {
  std::shared_ptr<PickupComponent> sp(std::make_shared<T>(this, params...));
  _components.push_back(sp);
  return sp;
}
```
And use the generated function where we call it.

---

# Summary

- Games are complicated systems!
- This is why we use clever tricks to help simplify the problem
- ECM is a great way to reuse code
- But we do have to use C++ templates to do these nicely
