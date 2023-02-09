---
title: "Lecture15"
keywords: Lecture
tags: [Lecture]
permalink:  lecture15.html
summary: lecture15
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 15 - State Machines
### SET09121 - Games Engineering

<br><br>
Babis Koniaris
<br>


School of Computing. Edinburgh Napier University


---

# Recommended Reading



- Artificial Intelligence for Games. Second Edition. Millington and
    Funge (2009).


 ![image](assets/images/ai_book.jpg)


---

## Review - State and State Machines


---

# Review - State Diagrams

- State modelling is one of (if not the) most important aspect of computing!
    - Software development (object/component state).
    - AI (state machines).
    - Networking (protocol and hardware development).
    - Software verification (state-space search).
- Understanding and modelling application state is one of the most important skills and tasks you can do.
- State modelling also provides dynamic behaviour.


---

# Review - State Diagrams


![image](assets/images/state_diagram.png) <!-- .element width="85%"  -->


---

# Review - State Design Pattern

- The state design pattern allows us to encapsulate an object's state within another object.
- We can switch the state object at any time during runtime - changing the behaviour of the object.
- For example the ghosts in PacMan change behaviour.
- Different behaviours are programmed in different objects - the ghost simply calls the state class when it updates.


 ![image](assets/images/state.png)


---

# Review - AI Techniques

- There are numerous usable AI techniques applicable to games development.
    - Classical, deterministic techniques - popular.
    - Academic, non-deterministic techniques - useful in some areas.
- Different techniques accomplish different aspects of game behaviour.
    - Movement.
    - Decision making.
    - Strategy.
    - Learning.
- Today we will look at the basics of decisions via state machines.


---

# Example - Batman: Arkham Asylum

<iframe width="1400" height="800" src="https://www.youtube.com/embed/hNs-orQHaKU" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# What are State Machines?

- State machines (or specifically in our case Finite State Machines - FSM) are one of the most fundamental concepts and cornerstones of computer science.
- A state machine is a technique of describing and modelling the state (e.g. behaviour, control, etc.) of a system in a mathematical manner.
- The system is modelled with a number of states and the transitions between these states.
    - The idea of a graph of states can come into play here - remember our description of a graph last week.


---

# State Machines for AI
- Let us return to the guard concept we presented last week.
- We will take a simple view so we can just focus on state.
- The guard has some basic actions:
    - The guard patrols between point A and point B.
    - If the guard is shot at, the guard will stop patrolling, engage the player, and fire back.
    - If the guard loses sight of the player, the guard will return to patrolling between point A and point B.
    - If the guard is hit, the guard will fall onto the ground and die.



---

# State Machines for AI 

![image](assets/images/simple_state_guard.png) <!-- .element width="100%"  -->


---

# State Machines for AI 

![image](assets/images/sanctum_chart.png) <!-- .element width="100%"  -->


---

# Example - Maze Solving 

![image](assets/images/maze.png) <!-- .element width="100%"  -->


---

# Example - Maze Solving

- To solve a maze we can use a particular trick.
    - This only works if the maze two or more ways in and out of the maze.
- The basic algorithm is:
    - Walk forward from the entrance until you hit a wall.
    - Turn left.
    - Now keep your right hand on a wall at all times.
    - You will eventually reach the other exit.
        - Although it will not necessarily be the fastest route.


---

# Example - Maze Solving 


![image](assets/images/maze_solve.png) <!-- .element width="85%"  -->


---

# State Machines in Our Game Engine

- We will be implementing a basic, reusable state machine behaviour in our game engine.
    - We want reusable so that it is simple for us to extend functionality if required.
- We have already identified the state design pattern as a likely candidate for implementation of state machine behaviour.
- What we need to do is implement this pattern in a manner that works in our game engine.


---

# State Pattern in Our Engine

![image](assets/images/game_state_pattern.png) <!-- .element width="90%"  -->


---

# `State` Interface

- The `State` interface only defines one method:
    - `Execute`
- This method executes the behaviour associated with that state upon the owner of the state.
    - So the state needs to be told the entity to work on.
    - Allows simple state reuse if this is desired.


 ![image](assets/images/state_interface.png)


---

# `StateMachineComponent` Class

- The `StateMachineComponent` is a `Component` that we can attach to an `Entity`.
- The class also follows the manager pattern - it contains and manages a collection of states.
- The core difference is that `Update` does not apply to all states, just the current state.


 ![image](assets/images/state_machine.png)


---

# `Update`

- `Update` is where the main functionality of the state machine occurs.
- It is just one line of code:
    - Execute the current state.
- We call `ChangeState` to change the current state.
- We call `Update` to execute the current state.
- Although simple, the key work we have done is separate out and encapsulated the different object behaviours.


---

# Decomposing State Machines

- If a model has two or more properties it is worthwhile looking to see if they are independent.
- If the properties are independent, it simplifies the logic to separate them into different state machines.
    - You can do this - just have two `StateMachineComponent`s attached to an `Entity`.
- For example:
    - A ranger wanders in the wilderness.
    - If the ranger is hungry, the ranger eats.
    - If it is night, the ranger lights a torch to see.


---

# Decomposing State Machines


![image](assets/images/ranger_all_states.png) <!-- .element width="65%"  -->


![image](assets/images/ranger_decomposed_states.png) <!-- .element width="65%"  -->



---

# Comments on State Machines

- FSMs are simple to use and understand.
    - Advantageous in lots of circumstances.
    - If you require degrees of intensity or "fuzziness" you will require a different AI technique.
- FSMs are difficult to modify once in place.
    - Small changes usually affect the entire FSM.
    - You will generally need to rethink and rewrite your FSM code.


---

# State Machines for Game Control

- We can extend our state machine implementation to work as a game controller.
    - All you need is an update and render for state, and call these when in the main game's relevant method.
- This allows you to trivially implement game screens:
    - Menu.
    - Main gameplay.
    - etc.
- The main game only calls update and draw on these elements of the game based on the state.
- This is effectively what the scene management system is doing.


---

## Summary


---

# Summary

- We've taken a broad look at state machines and how they work.
- We have also taken a look at how we will implement them in our game engine.
- Really, the ideas here will be more understandable when you implement the system and play around with the functionality.
- This can be used to underpin much of the AI behaviour we will look at - much like steering behaviours.
