---
title: "Lecture 4 - Systems"
keywords: Lecture
tags: [Lecture]
permalink:  lecture4.html
summary: lecture4
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
--- 
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 4 - System Dynamics
### SET09121 - Games Engineering

<br><br>
Babis Koniaris
<br>


School of Computing. Edinburgh Napier University


---

# Recommended Reading

Game Design Workshop. 4th Edition. Tracy Fullerton (2019).

- Read Chapter 5 on System Dynamics.
- Digital copies are available in the library.

![GameDesignWorkshopBook](assets/images/gdw_book.jpg)

---

# What is a System?


- An example of a system is an engine. <!-- .element: class="fragment" -->
    - Real, or a software one, like what we are going to build in the module.
- An engine is a system with a particular objective.  <!-- .element: class="fragment" -->
    - That objective is to power some form of manual action.
- We can apply our formal elements when thinking about systems. <!-- .element: class="fragment" -->

 ![image](assets/images/engine.jpg)  <!-- .element width="50%"  -->

---

# Games as Systems

- When we consider games as systems, we focuse on the following elements:
    - **Objects** that interact with each other according to their **properties**, **behaviors**, **relationships**.

---

# Objects

- Objects are the basic elements of a system. Consider the objects you define in object-oriented development.
- For example: an engine, steering wheel, and wheels interact together to allow a car to operate. <!-- .element: class="fragment" -->
- The complexity of the underlying system is hidden behind an interface. For a car, this is the steering wheels and pedals. <!-- .element: class="fragment" -->
- Objects are defined by their properties, behaviors, and relationship with each other. <!-- .element: class="fragment" -->
- In games the most basic of object is usually called an Entity, Actor, or Game Object. <!-- .element: class="fragment" -->
- Almost everything in a game is an object. <!-- .element: class="fragment" -->
    - For example: players, opponents, environment. <!-- .element: class="fragment" -->

---

# What Objects Are Here? 

![image](assets/images/mario.jpg)

---

# Spelunky

- Spelunky is one of the best (?) examples of inheritance in games

- Olmec, a boss, inherits from a push block

- The ghost can be killed because of inheritance!

- More info: https://www.rockpapershotgun.com/2016/03/04/making-of-spelunky/

---

# Properties

- Properties are the attributes of the objects in a system. <!-- .element: class="fragment" -->
- From an OO point of view, properties are values stored in the object. <!-- .element: class="fragment" -->
- Some properties might change over the course of a game while others remain constant. <!-- .element: class="fragment" -->
    - In checkers, the color of a piece remain constant.
    - The position of a piece might change at every turn.
- More properties make the game more complex. <!-- .element: class="fragment" -->
    - More complex does not always mean better.
- Some common properties include: <!-- .element: class="fragment" -->
    - Position.
    - Appearance.
    - A flag (bool) to indicate whether the object is alive.

---

# What Properties are Here? 

![image](assets/images/dragon_age.jpg)

---

# Behaviours

- Behaviours are the actions that an object undertakes. <!-- .element: class="fragment" -->
- From an OO point of view, behaviours are implemented as methods of an object. <!-- .element: class="fragment" -->
- Many of the behaviours are interlinked with the state of the object. <!-- .element: class="fragment" -->
    - An engine requires fuel to be turned on.
- Adding more behaviors to a game makes it harder to predict. <!-- .element: class="fragment" -->
    - Depending on the kind of game you want to create, this might be desirable or not.

---

# Behaviours (cont.)

- We are normally thinking about what can happen during the update step. <!-- .element: class="fragment" -->
 - For example, when B is pressed the player should jump. <!-- .element: class="fragment" -->
 - Player's actions are normally based on the controls (Procedures from Formal Elements). <!-- .element: class="fragment" -->
 - Computer controlled actions are normally supplied via some form of Artificial Intelligence (System procedures, objectives, rules).  <!-- .element: class="fragment" -->
- The physical objects are normally controlled by the physics system. <!-- .element: class="fragment" -->

---

# What Behaviours are Here? 

![image](assets/images/minecraft.jpg)


---

# What Behaviours are Here? 

![image](assets/images/pacman.gif)

---

# Behaviours in PacMan.

- There are three major behaviours:
	- Chase / Scatter / Frightened <!-- .element: class="fragment" -->
- Each ghost has it's own chase behaviour <!-- .element: class="fragment" -->
	- Blinky always goes for you <!-- .element: class="fragment" -->
	- Pinky tries to get in front of you <!-- .element: class="fragment" -->
	- Inky targets a space based on Blinky and your position <!-- .element: class="fragment" -->
	- Clyde retreats when getting too close to the player. <!-- .element: class="fragment" -->
- Behaviours change based on game state. <!-- .element: class="fragment" -->

---

# More about PacMan

https://dev.to/code2bits/pac-man-patterns--ghost-movement-strategy-pattern-1k1a


<iframe width="560" height="315" src="https://www.youtube.com/embed/S4RHbnBkyh0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

---

# Relationships

- To turn a set of objects into a system, we need relationships between them. <!-- .element: class="fragment" -->
    - The steering wheel of a car is connected to the wheels.
    - The position of chess pieces on the board determines how pieces can interact with each other.
    - The position of Tetris pieces determine if lines are cleared, and where falling pieces stop.
- Some relationships between objects can be changed by the player. <!-- .element: class="fragment" -->
    - I can move the chess pieces to a different location.
- Some relationships can be based on the current state.  <!-- .element: class="fragment" -->
    - If a character is wanted then guards will chase them on sight.

---

# What Relationships are Here? 

![image](assets/images/cities_skylines.jpg)

---

**Describe your favourite game as a system.** 

Pick one of your favourite games and described the objects, their properties and their behaviours.

Keep it abstract, but consider how you would program these.

---

# System Dynamics

- A system is more than the objects that make it up. <!-- .element: class="fragment" -->
    - Dynamic relationships cause unforeseen interactions and Conflict. <!-- .element: class="fragment" -->
    - Small changes in object properties can have a dramatic effect. <!-- .element: class="fragment" -->
- To understand a game it is necessary to observe the dynamics of the system during play. <!-- .element: class="fragment" -->

---

# De-constructing Games


---

# De-constructing Tic-Tac-Toe

- Tic-Tac-Toe (noughts and crosses) is a simple game.  <!-- .element: class="fragment" -->
 - **Objects:** the nine squares. 
 - **Properties:** symbol within the square (`O`, `X`, or empty).
 - **Behaviours:** place a symbol inside a square.
 - **Relationships:** location of squares on the board.

---

# Tic-Tac-Toe Game States 

![image](assets/images/tic-tac-toe.jpg)

---

# De-constructing Chess

- Chess is a significantly more complex and strategic game than Tic-Tac-Toe.
 - **Objects**: board, pieces.
 - **Properties**: colour, rank, and location of a piece.
 - **Behaviours**: move a piece.
 - **Relationships**: location of pieces relative to other pieces.

---

# De-constructing Chess

- Why is chess so much more interesting than tic-tac-toe?
    - Simple but different behavior for different pieces. <!-- .element: class="fragment" -->
    - Much larger range of possibilities. <!-- .element: class="fragment" -->
    - Much more complex relationship between the pieces. <!-- .element: class="fragment" -->

---

# Example Systems

---

# Economies

- In-game economies are often simplified compared to the real-world.
- **Bartering Economy**: Exchange goods for other goods.
- **Market Economy**: Have a currency which can be used to buy any good.
- Some economies even have inflation (MMOs) as a result of how the economy is designed.

---

# Emergent Systems

- Emergent systems exhibit behavior not explicitly programmed. The behavior emerges from the rules placed on the objects. <!-- .element: class="fragment" -->
    - Birds flocking is such a behavior.
    - Very relevant to game AI.
- Nature is full of such systems. <!-- .element: class="fragment" -->
- Examples include: Game of Life, Spore, The Sims. <!-- .element: class="fragment" -->

 ![image](https://66.media.tumblr.com/303da0502e45b38484e73b174b3db9db/tumblr_nhte1rMwH01teec4eo2_500.gif)  <!-- .element width="35%"  -->
 ![image](https://media.indiedb.com/images/articles/1/182/181609/flock4.gif)  <!-- .element width="50%"  -->

---

# System Interaction

- What information is provided to the player about the system?  <!-- .element: class="fragment" -->
    - Hiding information encourages guessing, bluffing, deceiving.
- What can the player control?  <!-- .element: class="fragment" -->
    - This has a huge impact on the top-level experience of the game.
- What feedback occurs within the system?  <!-- .element: class="fragment" -->
    - Positive (reinforcing) feedback forces a system towards one extreme.
    - Negative (balancing) feedback forces a system towards equilibrium. 
- How do these considerations affect the gameplay?  <!-- .element: class="fragment" -->

---

# Tuning of Game Systems

- Make sure the system is internally complete. <!-- .element: class="fragment" -->
    - A loophole might allow a player to unintentionally skip a conflict.
    - It might not be possible to resolve a conflict.
- Make sure the game is fair and balanced. <!-- .element: class="fragment" -->
- Avoid dominant strategies (a strategy that is always the best irrespective of the game state) or overpowered items. <!-- .element: class="fragment" -->
- Make sure it is fun and challenging. <!-- .element: class="fragment" -->
    - Requires playtesting.

---

# Summary

---

# Summary
- From this lecture you should understand:
 - **Objects:** the parts that make up a system.
 - **Properties:** the values that define the objects of the system.
 - **Behaviours:** what functions does an object perform in a system.
 - **Relationships:** how do the objects interact with each other.
- You should use these principles to try and de-construct games that you are familiar with.
