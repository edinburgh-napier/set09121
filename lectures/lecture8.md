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
- UML allows us to model software from various viewpoints.
    - The structure of the software.
        - Class diagram.
    - The behaviour of the software.
        - Use case diagram.
        - Activity diagram.
        - State diagram.
    - The interaction within the software.
        - Sequence diagram.
- UML can be integrated into any software development process.
    - Analysis and requirements gathering.
    - System design.
- UML essentially provides a schematic of our software.

---

# When to Use UML

- There are five diagrams I find useful.
    - Use case diagrams - overall requirements gathering.
    - Activity diagrams - flow chart of behaviour.
    - Class diagrams - main system design.
    - Sequence diagrams - individual steps and interaction between components.
    - State diagrams - model object or system state.
- **Use diagrams whenever possible!** 
 - Working out on paper how something works is **always** easier than banging your head off your code.
 - Trust me, we've all been there.


---

# What are Design Patterns?


---

# What are Design Patterns?

- A design pattern is a reusable solution to a commonly occurring problem when designing software.
- Reusable is the key here.
    - Engineering is about reusing existing solutions whenever possible.
    - Other engineering disciplines have reusable solutions to given problems.
- When we look at our software development problems from a high enough abstraction level we will see lots of areas of reuse.

![image](assets/images/software_development.png) <!-- .element width="45%"  -->
 

---

# Reading UML for Patterns

- The important factor we are going to cover is our ability to recognise and implement patterns from UML.
- We're not expecting you to find patterns online, try to determine if they are useful, and then try and integrate them.

![image](assets/images/patterns.png) <!-- .element width="100%"  -->


---

# Useful Design Patterns for Games


---

# Types of Patterns

- Design patterns can be divided into a number of categories based on the type of problem they try and solve.
 - **Creational** patterns
    - Used for, or dictate, object creation mechanisms.
 - **Structural** patterns,
    - Allow control of the relationships between objects.
 - **Behavioural** patterns,
    - Used to control common communication patterns between objects.
- You'll likely already know at least one pattern from each of these categories.


---

# Singleton


---

# Singleton Pattern 
![image](assets/images/singleton.png) <!-- .element width="80%"  -->


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

# Composite Pattern 

![image](assets/images/composite.png) <!-- .element width="80%"  -->


---

# Composite Pattern

- The Composite pattern allows us to treat objects and compositions of objects in a uniform manner.
- We tell our game to update, it tells the entity manager to update, which tells the entities to update, etc.
- A pure compositional pattern has components and compositions of components that look the same.
    - In Object-Oriented Software Development you could add buttons to panels, and panels to other panels or windows. The window just treats the panel as any other component.
- We take a less pure approach but we are still composing groups of objects with uniform interfaces.


---

# Iterator Pattern 

![image](assets/images/iterator.png) <!-- .element width="80%"  -->


---

# Iterator Pattern

- The iterator pattern allows us to access the individual elements of a collection of objects (aggregate) in a uniform manner without exposing the collection structure underneath.
- One of the most useful (and oldest) patterns available.
    - Create a collection.
    - Add objects to collection.
    - Iterate through collection when needed and perform individual actions.


---

# Mediator Pattern 
![image](assets/images/mediator.png) <!-- .element width="80%"  -->


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

# State Pattern 
![image](assets/images/state.png) <!-- .element width="80%"  -->


---

# State Pattern

- Allows us to encapsulate an object's state within another object.
- We can switch the state object at any time during runtime.
    - Effectively changing the behaviour of the object.
- For example the ghost in PacMan.
    - Current state is chase PacMan.
    - When PacMan eats a power pill the ghost changes state to evade PacMan.
    - When power pill timer runs out state changes back to chase PacMan.
- The different behaviours are programmed in different objects. The ghost simply calls the state class when it updates.


---

# Strategy Pattern 

![image](assets/images/strategy.png) <!-- .element width="80%"  -->


---

# Strategy Pattern

- Sometimes we want to maintain a set of algorithms that an object can execute at given times.
- In general the algorithms that need to be used are not known until runtime.
    - The PacMan example previously code equally be achieved by the strategy pattern.
- Again this is a good AI pattern allowing us to interchange behaviours at given points.
- Notice that the state and strategy patterns are also very similar (structurally identical). They have different purposes but can be used fairly interchangeably.


---

# Manager Pattern 

![image](assets/images/manager.png) <!-- .element width="80%"  -->


---

# Manager Pattern

- We want to have a centralised repository and control point for a collection of objects.
- The manager object keeps track of objects of a particular type and maintains them in a data collection.
- The manager object also performs group operations on the collection of objects when required.
- The manager pattern is something we've identified when making the game engine.
    - There are similar patterns such as the Boss pattern.
    - We are free to define our own patterns.


---

# Data-driven Design Pattern 

![image](assets/images/data-driven-uml.png) <!-- .element width="95%"  -->


---

# Data-driven Design Pattern

- Having each object contain its own data is not-necessarily efficient.
- Data layout in memory means certain functions are going to jump around memory, which is expensive.
- Also, it can lead to costs in copying objects around.
- A better idea is to store data objects in data owning objects.
- A functional object (one that has actual methods) then has access to these objects as required.
- This makes everything much easier to manage - although some OO purists think it isn't OO any more


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
