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

 Game Design Workshop. 3rd Edition. Fullerton (2014).
- Read Chapter 5 on System Dynamics.

 ![image](assets/images/gdw_book.jpg)


---

# Example -- Donkey Kong: what do you see? 


<iframe width="760" height="515" src="https://www.youtube.com/embed/Pp2aMs38ERY" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

---

# What is a System?


- An example of a system is an engine.
 - Real, or a software one, like what we are going to build in the module.  <!-- .element: class="fragment" -->
- An engine is a system with a particular objective.  <!-- .element: class="fragment" -->
 - That objective is to power some form of manual action.  <!-- .element: class="fragment" -->
- We can apply our formal elements into thinking about systems.  <!-- .element: class="fragment" -->

 ![image](assets/images/engine.jpg)  <!-- .element width="50%"  -->


---

# What is a System? Defining Objects

**Objects**

- Objects are the basic elements of a system. Consider the objects you define in object-oriented development.
 - A system is the interaction between a collection of objects.  <!-- .element: class="fragment" -->
- For example: an engine, steering wheel, and wheels interact together to allow a car to operate.  <!-- .element: class="fragment" -->
- The complexity of the underlying system is hidden behind an interface. For a car, this is the steering wheels and pedals.  <!-- .element: class="fragment" -->
- Objects are defined by their properties and behaviors. <!-- .element: class="fragment" -->
- What objects can you see in other systems? <!-- .element: class="fragment" -->


---

# What is a System? Object Properties

**Properties**

- Properties are the attributes of the objects in a system.  <!-- .element: class="fragment" -->
- From an OO point of view, properties are values stored in the object, which may themselves be objects.  <!-- .element: class="fragment" -->
- Properties are the descriptive data that can be analysed, extracted or modified.  <!-- .element: class="fragment" -->
- Properties are also a method to allow unique identification of individual objects.  <!-- .element: class="fragment" -->
- What are the properties of objects you see in other systems?  <!-- .element: class="fragment" -->
- From our rules (Formal Elements) we can extract most of our objects and their properties.  <!-- .element: class="fragment" -->


---

# What is a System? Object Behaviours

**Behaviours**

- Behaviours are the actions that an object undertakes.  <!-- .element: class="fragment" -->
 - Object-orientation calls these methods.  <!-- .element: class="fragment" -->
 - Many of the behaviours are interlinked with the state of the object.  <!-- .element: class="fragment" -->
  - An engine requires fuel to be turned on.  <!-- .element: class="fragment" -->
  - We will look at state and state management later in the module.  <!-- .element: class="fragment" -->
  - Remember: our formal rules can restrict our formal procedures.  <!-- .element: class="fragment" -->
 - Behaviours can be complicated.  <!-- .element: class="fragment" -->
- What object behaviours of objects do you see in other systems?  <!-- .element: class="fragment" -->
- Rules and Procedures of Formal Elements can help us extract some of the object behaviours.  <!-- .element: class="fragment" -->


---

# What is a System? Relationships Between Objects

**Relationships**

- Once we have a collection of objects we can define some rudimentary systems.
- Objects themselves do not form a system. Systems are the relationships between the objects.  <!-- .element: class="fragment" -->
 - Depend on the context of use.
     - A car engine turns wheels.
     - A jet engine propels a plane.
 - We must determine the relationships between our objects to create a system.
- A game is a system!  <!-- .element: class="fragment" -->
 - With a shiny graphical front end.
 - Remember: the graphical presentation is almost separate from the system operation.


---

# Objects in games


---

# Games as Systems: Objects

**Objects**

 - Objects are the basic elements of a system.
 - In games the most basic of object is usually called an **Entity**. <!-- .element: class="fragment" -->
 - Almost everything in a game is an object.  <!-- .element: class="fragment" -->
     - For example: players, opponents, environment.  <!-- .element: class="fragment" -->
 - As developers we must identify these objects and develop them.  <!-- .element: class="fragment" -->
 - Games are similar to standard applications although the object relationship are probably more dynamic.  <!-- .element: class="fragment" -->
- What objects can you identify in the games you play?  <!-- .element: class="fragment" -->

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

# Games as Systems: Properties

**Properties**

- Define the data attached to an object.
- Some common properties include:
    - Position.  <!-- .element: class="fragment" -->
    - Graphic.  <!-- .element: class="fragment" -->
    - A flag (bool) to indicate whether the object is alive.  <!-- .element: class="fragment" -->
- A basic entity in a game can have numerous properties and sub-components.  <!-- .element: class="fragment" -->
 - RPG in particular has lots of properties.
 - We will examine how we compose objects later.
- What properties can you identify in the games you play?  <!-- .element: class="fragment" -->


---

# What Properties are Here? 

![image](assets/images/dragon_age.jpg)


---

# Behaviours

---

# Games as Systems: Behaviours

**Behaviours**
- Define what our objects can do.
- We are normally thinking about what can happen during the update step.  <!-- .element: class="fragment" -->
 - For example, when B is pressed the player should jump.  <!-- .element: class="fragment" -->
 - Player's actions are normally based on the controls (Procedures from Formal Elements).  <!-- .element: class="fragment" -->
 - Computer controlled actions are normally supplied via some form of Artificial Intelligence (System procedures, objectives, rules).  <!-- .element: class="fragment" -->
- The physical objects are normally controlled by the physics system.  <!-- .element: class="fragment" -->
- What behaviours do you see in the games that you play?  <!-- .element: class="fragment" -->


---

# What Behaviours are Here? 

![image](assets/images/minecraft.jpg)


---

# What Behaviours are Here? 

![image](assets/images/pacman.gif)

(I bet it's more complicated than you think)

---

# Seriously, do you know about the Ghosts?

- There are three major behaviours:
	- Chase / Scatter / Frightened <!-- .element: class="fragment" -->
- Each ghost has it's own chase behaviour <!-- .element: class="fragment" -->
	- Blinky always goes for you <!-- .element: class="fragment" -->
	- Pinky tries to get in front of you <!-- .element: class="fragment" -->
	- Inky targets a space based on Blinky and your position <!-- .element: class="fragment" -->
	- Clyde is there too (he gets scared!) <!-- .element: class="fragment" -->
- Behaviours change based on game state <!-- .element: class="fragment" -->

---

# You should read more!

https://dev.to/code2bits/pac-man-patterns--ghost-movement-strategy-pattern-1k1a


<iframe width="560" height="315" src="https://www.youtube.com/embed/S4RHbnBkyh0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

---


**Describe your favourite game as a system.** 

Pick one of your favourite games and described the objects, their properties and their behaviours.


Keep it abstract, but consider how you would program these.


---

# Relationships


---

# Games as Systems: Relationships

**Relationships**

- Games have complex and dynamic relationships based on the game state.
- Some relationships are simple.  <!-- .element: class="fragment" -->
    - The game character relates to the world via a position.
- Some relationships can be based on the current state.  <!-- .element: class="fragment" -->
    - If character is evil then guards will chase on sight.
- Relationships can also be defined by the rules of the game.  <!-- .element: class="fragment" -->
- Possibly the hardest concept to understand in software development.  <!-- .element: class="fragment" -->
- May also cause difficulties later in development when considering optimisation.  <!-- .element: class="fragment" -->

---

# What Relationships are Here? 

![image](assets/images/cities_skylines.jpg)

---

# De-constructing Games


---

# De-constructing Tic-Tac-Toe
- A system is more than the objects that make it up.  <!-- .element: class="fragment" -->
 - Dynamic relationships cause unforeseen interactions and Conflict.  <!-- .element: class="fragment" -->
 - Small changes in object properties can have a dramatic effect.  <!-- .element: class="fragment" -->
- Tic-Tac-Toe (noughts and crosses) is a simple game.  <!-- .element: class="fragment" -->
 - **Objects:** squares. 
 - **Properties:** symbol within the square (`O`, `X`, or empty).
 - **Behaviours:** set symbol in square.
 - **Relationships:** based on location of matching symbols. Important considering the gameplay mechanic.


---

# Tic-Tac-Toe Game States 

![image](assets/images/tic-tac-toe.jpg)


---

# De-constructing Chess
- Chess is a significantly more complex and strategic game than Tic-Tac-Toe.
- From a system's point of view, chess and tic-tac-toe are very similar.
 - **Objects**: squares, pieces.
 - **Properties**: colour, rank, location of piece.
 - **Behaviours**: move piece.
 - **Relationships**: location of piece to other pieces.
- Why is the outcome so different?  <!-- .element: class="fragment" -->


---

# Game Flow

- The key difference is the range of possibilities.
- Games should offer possibility; unpredictability. <!-- .element: class="fragment" -->
- A game also needs to be simple.  <!-- .element: class="fragment" -->
- Games can be defined by the flow of possibilities or structure through the game.  <!-- .element: class="fragment" -->
- As stated: objects are simple; the relationships between the objects lead to the complexity.  <!-- .element: class="fragment" -->


---

# Game Flow

 ![image](assets/images/gameflow.png)

---

# Question Time!

- Should games always be unpredictable?
	- Why should they?
	- Why shouldn't they?

---

# Example Systems


---

# Economics
- Many games have a simple economy where resources can be exchanged.
 - Remember that Resources are a Formal Element.
- The use of currency is seen in a wide range of games:  <!-- .element: class="fragment" -->
 - RPGs.
 - Assassin's Creed series.
 - Batman: Arkham series.
 - etc.
- What does an economy require from a systems point of view?  <!-- .element: class="fragment" -->


---

# Emergent Systems

- Emergent systems exhibit behavior not explicitly programmed. The behavior emerges from the rules placed on the objects.
 - Birds flocking is such a behavior.
 - Very relevant to game AI.
- Nature is full of such systems.
- Examples include: Game of Life; Spore; The Sims (sort of).

 ![image](https://66.media.tumblr.com/303da0502e45b38484e73b174b3db9db/tumblr_nhte1rMwH01teec4eo2_500.gif)  <!-- .element width="35%"  -->
 ![image](https://media.indiedb.com/images/articles/1/182/181609/flock4.gif)  <!-- .element width="50%"  -->


---

# Software System
- The common approach to writing software is object-orientation.
 - Standard model in C#, Java, and used in C++.
- With object-orientation we have:
 -  **Objects**: which are instances of classes.
 -  **Attributes**: which are the properties of the objects.
 -  **Methods**: which are behaviours associated with objects.




---

# Software System

 ![image](assets/images/uml.png) 

---

# System Interaction and Control
- A system does not always need interaction.
 - A game definitely does (You can stretch that definition though!) <!-- .element: class="fragment" -->
- Some things to consider:  <!-- .element: class="fragment" -->
 - What information is provided to the player about the system?  <!-- .element: class="fragment" -->
 - What can the player control?  <!-- .element: class="fragment" -->
 - How does the player control these objects?  <!-- .element: class="fragment" -->
 - What feedback occurs within the system?  <!-- .element: class="fragment" -->
 - How do these considerations affect the gameplay?  <!-- .element: class="fragment" -->
- A game will require tuning and polish to get all of these correct.  <!-- .element: class="fragment" -->

**So much tuning**<!-- .element: class="fragment" -->

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


---

# Summary

- **Objects** 
- **Properties**
- **Behaviours**
- **Relationships**

** This is how Object Orientation Works **  <!-- .element: class="fragment" -->

OO is a great way of describing and building systems of objects (games)  <!-- .element: class="fragment" -->

However; in practice, actually programming these systems can be tricky. But designing and planning the OO way is a great way to design systems that will exist as software.  <!-- .element: class="fragment" -->

---

# BONUS SLIDE

**How do you find inspiration for game ideas?**

People are often tempted to look only at other games... <!-- .element: class="fragment" -->

**This is how you make bad games!** <!-- .element: class="fragment" -->

For example Pac Man was inspired by:<br /> <!-- .element: class="fragment" -->
Tom and Jerry<br /> <!-- .element: class="fragment" -->
Popeye<br /> <!-- .element: class="fragment" --> 
Manga & Casper<br /> <!-- .element: class="fragment" -->