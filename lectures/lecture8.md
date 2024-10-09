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
Babis Koniaris
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

---

# What are Design Patterns?

---

# What are Design Patterns?

- A design pattern is a reusable solution to a commonly occurring problem when designing software.
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

- The Singleton pattern lets us ensure that only one instance of a given class ever exists.
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

```CS
class EntityManager {
    static EntityManager instance;

    static EntityManager getInstance() {
        if (instance == null) {
            instance = new EntityManager();
        }
        return instance;
    }
}
```

---

# Composite Pattern

- The Composite pattern allows us to treat objects and compositions of objects in a uniform manner.
- For example, elements in a UI can be a single element, called a **leaf** (e.g., a button), or a collection of other Elements, called a **composite** (e.g., a panel with buttons, etc.)
    - This creates a **hierarchy** of UI elements.
    - We tell the top UI element to update.
    - The top UI element will tell the child elements to update, if it is a composite.
    - Thanks to a common interface, we don't need to know whether we are dealing with a composite or leaf.
- Do not confuse the component in the composite pattern with the component in a Entity Component Model!

---

# Composite Pattern 

![image](assets/images/composite.png) <!-- .element width="80%"  -->

(Source: https://en.wikipedia.org/wiki/Composite_pattern)

---

# Composite Pattern 

```CS
interface UIElement { // Component
    void update(); // Operation
}

class Panel : UIElement { // Composite
    List<UIElement> panelElements = new List<UIElement>();

    void update() { 
        for (UIElement element in panelElements) {
            element.update();
        }
    }
    
    // add(), remove(), getChild()...
}

class Button : UIElement { // Leaf
    void update() {
        ...
    }
}
```

---

# Iterator Pattern

- The iterator pattern allows us to access the individual elements of a collection of objects (aggregate) in a uniform manner without exposing the collection structure underneath.
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

```CS
QuerySolution solution = QueryEngine.Query("SELECT name, address FROM customers");

while (solution.hasNext()) {
    Bindings bindings = solution.next();

    // Do something with the bindings.
}
```

---

# Mediator Pattern

- The Mediator pattern provides a unified interface to a set of objects in a system.
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

```CS
class ChatRoom {
    List<User> users = new List<User>();

    void send(string message, string sender) {
        for (User user in users) {
            user.receive(message, sender);
        }
    }

    // register(), remove(), etc.
}

class User {
    void receive(string message, string sender) {
        // Display message
    }

    void send(string message) {
        mediator.send(message, Name);
    }
}
```

---

# State Pattern

- Allows us to change the behaviour of the object when its internal state changes
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

```CS
interface State {
    void handle();
}

class ChaseState : State {
    void handle() {
        // Chase PacMan
    }
}

class EvadeState : State {
    void handle() {
        // Evade PacMan
    }
}

class Enemy {
    State behaviourState;

    void update() {
        behaviourState.handle();
    }
}
```

---

# Strategy Pattern

- The strategy pattern has the identical structure as the state pattern.
- We use the strategy pattern when we want to use a different algorithm (strategy) to achieve **the same thing**.
    - State pattern: change to a **different behaviour** at run-time.
    - Strategy pattern: change the **implementation** of the **same behaviour** at run-time.
- A good example of the strategy pattern is different numerical integration method for physics simulation.
    - They all achieve the same thing, but have different trade-offs in accuracy and performance.
    - Games like Universe Sandbox allow the player to change the integration method at run-time.

---

# Strategy Pattern

```CS
interface Integrator {
    void step(float h);
}

class LeapFrog : Integrator {
    void step(float h) {
        // Calculate forces
    }
}

class Euler : Integrator {
    void step(float h) {
        // Calculate forces
    }
}

class Simulator {
    Integrator integrationMethod;

    void update(float h) {
        integrationMethod.step(h);
    }
}
```

---

# Observer Pattern

- We want to have a centralised repository and control point for a collection of objects.
- The **subject** keeps track of all objects, the **observers**, and performs operations on them.
- The subjects are registered at runtime. 
- Example: An entity manger that keeps track of all entities in a game.
    - Entity manager is the subject.
    - The entities are the observers.
    - The entity manger calls methods like `update()` and `render()` each frame. 

---

# Observer Pattern 

![image](assets/images/observerPattern.png) <!-- .element width="80%"  -->

(Source: https://en.wikipedia.org/wiki/Observer_pattern)

---

# Observer Pattern 

```CS
class EntityManager { // Subject
    List<Entity> entities = new List<Entity>();

    void update(float dt) {
        for (Entity entity in entities) {
            entity.update(dt);
        }
    } 

    // register(), unregister(), etc.
}

class Entity { // Observer
    void update(float dt) {
        // ...
    }
}
```

---

# Recognising Design Patterns

- You will be surprised how often design patterns pop-up when developing software.
- Some patterns are even built into languages and frameworks that you have used.
    - `foreach` in C# is an iterator.
    - C# also has delegates.
    - Python has decorators.
    - Java provides the observer pattern.
- To recognise a pattern you need to first recognise the problem you are trying to solve.
    - Again, this means doing a higher-level analysis.
- If you find you are doing the same thing over and over again, chances are you have a pattern.


---

# Summary


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
