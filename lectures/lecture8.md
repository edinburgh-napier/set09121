---
title: "Lecture 8 - Design Patterns"
keywords: Lecture
tags: [Lecture]
permalink:  lecture8.html
summary: lecture8
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 8 - Design Patterns
### SET09121 - Games Engineering

<br><br>
Leni Le Goff
<br>


School of Computing. Edinburgh Napier University


---

# Recommended Reading

Game Programming Patterns - Robert Nystrom

![image](assets/images/game_patterns_book.jpg)



---

# Review of UML


---

# What is UML?

- UML stands for the Unified Modelling Language.
- UML allows us to model software from various viewpoints. <!-- .element: class="fragment" -->
    - The structure of the software.
        - Class diagram.
    - The behaviour of the software.
        - Use case diagram.
        - Activity diagram.
        - State diagram.
    - The interaction within the software.
        - Sequence diagram.
- UML can be integrated into any software development process. <!-- .element: class="fragment" -->
    - Analysis and requirements gathering.
    - System design.
- UML essentially provides a schematic of our software. <!-- .element: class="fragment" -->

---

# When to Use UML

- Five useful diagrams:
    - **Use case** diagrams: overall requirements gathering.
    - **Activity** diagrams: flow chart of behaviour.
    - **Class** diagrams: main system design.
    - **Sequence** diagrams: individual steps and interaction between components.
    - **State** diagrams: model object or system state.
- **Use diagrams whenever possible!**
Useful article: [geeksforgeeks.com - UML introduction](https://www.geeksforgeeks.org/system-design/unified-modeling-language-uml-introduction/)

---

# Class diagram

![image](assets/images/Class-Diagram-Example.webp)

---

# What are Design Patterns?

- A design pattern is a **reusable solution** to a **commonly occurring problem** when designing software.
- Reusable is the key here. <!-- .element: class="fragment" -->
    - Engineering is about reusing existing solutions whenever possible.
    - Other engineering disciplines have reusable solutions to given problems.
- When we look at our software development problems from a high enough abstraction level we will see lots of areas of reuse. <!-- .element: class="fragment" -->

![image](assets/images/software_development.png) <!-- .element width="45%"  -->

---

# Useful Design Patterns for Games

---

# Types of Patterns

- Design patterns can be divided into a number of categories based on the type of problem they try and solve.
 - **Creational** patterns
    - Used for, or dictate, object creation mechanisms.
 - **Structural** patterns,
    - Used to dictate how objects are composed to form larger structures.
 - **Behavioural** patterns,
    - Used to control common communication patterns between objects.
- You'll likely already know at least one pattern from each of these categories.

---

# Singleton

---

# Singleton Pattern

**Problem**: How to ensure that only one instance of a given class ever exists?

- The pattern is good when we want to control and coordinate particular operations in our system.
    - E.g. A game only has one GameController for tracking gamestate and flow.
    - Our game engine wants to ensure control over game entities at particular stages.
- Singletons are good for providing a centralised approach to access a particular part of the system.
    - Almost like providing a global attribute.
- There are numerous approaches to ensure Singleton behaviour.

---

# Singleton Pattern 

![image](assets/images/singleton.png) <!-- .element width="80%"  -->

(Source: https://en.wikipedia.org/wiki/Singleton_pattern)

---

# Singleton Pattern 

```cpp
class EntityManager
{
public:
    static EntityManager& getInstance()
    {
        static EntityManager instance;
        return instance;
    }
private:
    EntityManager(){}
    EntityManager(const EntityManager&) = delete;
};
```

---

# Composite Pattern

**Problem:** How to use in **the same way** **objects** and **compositions** of objects? 

- Think of a tree: the whole tree, a node or a leaf should be usable the same way (recursive structure)
- For example, elements in a UI can be a single element, called a **leaf** (e.g., a button), or a collection of other Elements, called a **composite** (e.g., a panel with buttons, etc.)
    - This creates a **hierarchy** of UI elements.
    - We tell the top UI element to update.
    - The top UI element will tell the child elements to update, if it is a composite.
    - Thanks to a common interface, we don't need to know whether we are dealing with a composite or leaf.

---

# Composite Pattern 

![image](assets/images/composite.png) <!-- .element width="80%"  -->

(Source: https://en.wikipedia.org/wiki/Composite_pattern)

---

# Composite Pattern 

```cpp
class UIElement { // Component
public:
    virtual void update()=0; // Operation
};

class Panel : public UIElement { // Composite
    std::vector<std::shared_ptr<UIElement>> panelElements;

    void update() override {
        for (std::shared_ptr<UIElement>& element : panelElements) {
            element->update();
        }
    }

    // add(), remove(), getChild()...
};

class Button : public UIElement { // Leaf
    void update() override {
        ...
    }
};
```

---

# Iterator Pattern

**Problem:** How to iterate through a collection of elements in a **linear** way for **any type** of collection while protecting it.

- One of the most useful (and oldest) patterns available.
    - Create a collection.
    - Add objects to collection.
    - Iterate through collection when needed and perform individual actions.
- If you don't create your own collection, you most likely do not have to create your own iterator.

---

# Iterator Pattern 

![image](assets/images/iterator.png) <!-- .element width="80%"  -->

(Source: https://en.wikipedia.org/wiki/Iterator_pattern)

---

# Iterator Pattern 

```cpp
// Iterator interface
class Iterator {
public:
    virtual int next() = 0;
    virtual bool hasNext() = 0;
};

// Concrete Iterator
class ConcreteIterator : public Iterator {
public:
    ConcreteIterator(const std::vector<int>& items) : items(items), position(0) {}

    int next() override {
        if (hasNext()) {
            return items[position++];
        }
        throw std::out_of_range("No more elements.");
    }

    bool hasNext() override {
        return position < items.size();
    }
private:
    const std::vector<int>& items;
    size_t position;
};
```

---

# Iterator Pattern 

```cpp
// Aggregate interface
class Aggregate {
public:
    virtual std::unique_ptr<Iterator> createIterator() const = 0;
    virtual ~Aggregate() = default;
};

// Concrete Aggregate
class ConcreteAggregate : public Aggregate {
public:
    void addItem(int item) {
        items.push_back(item);
    }

    std::unique_ptr<Iterator> createIterator() const override {
        return std::make_unique<ConcreteIterator>(items);
    }
private:
    std::vector<int> items;
};
```

---

# Iterator Pattern 

```cpp
ConcreteAggregate numbers;
numbers.addItem(10);
numbers.addItem(20);
numbers.addItem(30);
numbers.addItem(40);

std::unique_ptr<Iterator> iterator = numbers.createIterator();

while (iterator->hasNext()) {
    std::cout << iterator->next() << " ";
}
std::cout << std::endl;
return 0;
```

---

# Mediator Pattern

**Problem:** How to define a **common communication** protocol between objects? And, how to implement **new communiction** protocol **without having to change** the implementation of the objects?

- Objects no longer communicate directly with each other, but instead communicate through the mediator.
- This reduces the dependencies between communicating objects, thereby reducing coupling.
- Mediator is very common pattern in GUI systems.
    - Event based programming.
    - Message passing. 
- The mediator pattern is useful for building messaging systems as it detaches the components. It is a loose coupling approach.

---

# Mediator Pattern 

![image](assets/images/mediator.png) <!-- .element width="80%"  -->

(Source: https://javadevcentral.com/mediator-design-pattern)

---

# Mediator Pattern 

```cpp
class Colleague;

class Mediator {
public:
    virtual void broadcastMessage(const std::string& message, Colleague* sender) = 0;
};

class Colleague {
public:
    Colleague(Mediator* mediator, const std::string& name) : mediator(mediator), name(name) {}

    void sendMessage(const std::string& message) {
        mediator->broadcastMessage(message, this);
    }

    void receiveMessage(const std::string& message) {
        std::cout << name << " received: " << message << std::endl;
    }

    std::string getName() const {
        return name;
    }
private:
    Mediator* mediator;
    std::string name;
};
```

---

# Mediator Pattern 

```cpp
// Concrete Mediator (Chat Room)
class ChatRoom : public Mediator {
public:
    void addParticipant(Colleague* colleague) {
        participants.push_back(colleague);
    }

    void broadcastMessage(const std::string& message, Colleague* sender) override {
        for (Colleague* participant : participants) {
            if (participant != sender) {
                participant->receiveMessage(sender->getName() + ": " + message);
            }
        }
    }
private:
    std::vector<Colleague*> participants;
};
```

---

# Mediator Pattern 

```cpp
// Create a mediator (chat room)
ChatRoom chatRoom;

// Create participants (colleagues)
Colleague alice(&chatRoom, "Alice");
Colleague bob(&chatRoom, "Bob");
Colleague charlie(&chatRoom, "Charlie");

// Add participants to the chat room
chatRoom.addParticipant(&alice);
chatRoom.addParticipant(&bob);
chatRoom.addParticipant(&charlie);

// Participants send messages through the mediator
alice.sendMessage("Hello, everyone!");
bob.sendMessage("Hi Alice!");
charlie.sendMessage("Good morning, folks!");
```

---

# State Pattern

**Problem:** How to change an object's behaviour when its internal state change? How to implement new behaviour without altering the other behaviours and the object itself?

- For example the ghost in PacMan.
    - Current state is chase PacMan.
    - When PacMan eats a power pill the ghost changes state to evade PacMan.
    - When power pill timer runs out state changes back to chase PacMan.
- The different behaviours can be programmed in different objects. 
- The ghost uses the behaviour specified in the state object when it updates.

---

# State Pattern 

![image](assets/images/state.png) <!-- .element width="80%"  -->

(Source: https://en.wikipedia.org/wiki/State_pattern)


---

# State Pattern 

```cpp
class Context; // Forward declaration

// State Interface
class State {
public:
    virtual void handle(Context& context) = 0;
    virtual ~State() = default;
};

class ChaseState : public State { // Concrete state
public:
    void handle(Context& context) override;
};

class EvadeState : public State {
public:
    void handle(Context& context) override;
};

```

---

# State Pattern 

```cpp
class Context { // Context class that holds the current state
public:
    Context(State * initialState) : state(initialState) {}

    void setState(State* newState) {
        state = newState;
    }

    void tick() {
        state->handle(*this);  // Delegate behavior to the current state
    }
private:
    State * state;  // The current state
};

void ChaseState::handle(Context& context) {
    if (pacmanJustAtePowerPill)
            context.setState(new EvadeState());  // Transition to evade state
}

void EvadeState::handle(Context& context) {
    if(timeElapsedSinceEvadeStarted < evasionDuration)
        context.setState(new ChaseState());  // Transition to chase state
}
```

---

# State Pattern 

```cpp
 Context pacman(new PacmanState());
 Context ghost(new EvadeState());

 while (true)
 {
     pacman.tick();
     ghost.tick();
 }
```

---

# Strategy Pattern

**Problem:** How to switch between algorithms at run-time seamlessly without modifying the object?

- We use the strategy pattern when we want to use a different algorithm (strategy) to achieve **the same thing**.
- A good example of the strategy pattern is different numerical integration method for physics simulation.
    - They all achieve the same thing, but have different trade-offs in accuracy and performance.
    - Games like Universe Sandbox allow the player to change the integration method at run-time.

---

# Strategy Pattern

```cpp
class Integrator {
public:
    virtual void step(float h) = 0;
};

class LeapFrog : Integrator {
public:
    void step(float h) override {} // use leapfrog verlet
};

class Euler : Integrator {
public:
    void step(float h) override {} // use explicit euler
};

class Simulator {
public:
    void update(float h) {
        integrationMethod->step(h);
    }
private:
    Integrator * integrationMethod;
};
```

---

# Strategy Pattern with Lambda expression

```cpp

using Integrator = std::function<void(float)>;

struct Integrators{
    static Integrator leap_frog = [](float h){};
    static Integrator euler = [](float h){};
}

class Simulator {
public:
    void update(float h) {
        integrator(h);
    }
private:
    Integrator integrator;
};

```

---

# Observer Pattern

**Problem:** How to process/update a set of objects without having to go through all of them? 

- We want to have a centralised repository and control point for a collection of objects.
- The **subject** keeps track of all objects, the **observers**, and notifies them of any state changes.
- The subjects are registered at runtime. 
- Example: An entity manager that keeps track of all entities in a game.
    - Entity manager is the subject.
    - The entities are the observers.
    - The entity manager calls methods like `update()` and `render()` each frame

---

# Observer Pattern 

![image](assets/images/observerPattern.png) <!-- .element width="80%"  -->

(Source: https://en.wikipedia.org/wiki/Observer_pattern)

---

# Observer Pattern 

```cpp
class Entity { // Observer
public:
    void update(float dt) {}
    void render() {}
};

class EntityManager { // Subject
    std::vector<Entity*> entities;

    void update(float dt) {
        for (Entity* entity : entities) {
            entity->update(dt);
        }
    }

    void render() {
        for (Entity* entity : entities) {
            entity->render();
        }
    }

    void registerEntity(Entity * entity) {
        entities.push_back(entity);
    }

    void unregisterEntity(Entity* entity) {
        // ... remove entity from entities vector
    }
};
```

---

# Recognising Design Patterns

- You will be surprised how often design patterns pop-up when developing software.
- Some patterns are even built into languages and frameworks that you have used.
    - Range-based for-loop in C++ uses iterators provided by the collection
    - C# has events.
    - Python has decorators.
    - Java provides the observer pattern.
- To recognise a pattern you need to first recognise the problem you are trying to solve.
    - Again, this means doing a higher-level analysis.
- If you find you are doing the same thing over and over again, chances are you have a pattern.


---

# Summary

- Design patterns are a very important tool in the software engineer's toolbox.
    - Reusable solutions to particular problems.
    - Simplify existing solutions.
    - Patterns have proven usefulness.
- Understanding design patterns is probably the most important skill you can pick up at this stage of your programming education.
    - Object-oriented Software Development touched on these areas.
- Knowing when and where to use a design pattern can save you a lot of effort.
    - And there are a lot of potential patterns out there.
