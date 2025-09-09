---
title: "Lecture 5 - Entities"
keywords: Lecture
tags: [Lecture]
permalink:  lecture5.html
summary: lecture5
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 5 -  Game Entities and Entity Management
### SET09121 - Games Engineering

<br><br>
Leni Le Goff
<br>


School of Computing. Edinburgh Napier University


---

# Recommended Reading

Game Design Workshop. 3rd Edition. Fullerton (2014).

- Read Chapter 5 on System Dynamics.

 ![image](assets/images/gdw_book.jpg)


---

# Review - System Dynamics

- Last lecture we examined systems and how games can be viewed as systems.
- We incorporated our existing Formal Elements of game design into a system. <!-- .element: class="fragment" -->
- We also showed how we can define a game as a system, and some of the similarities with software development. <!-- .element: class="fragment" -->
- In this lecture we will go into detail about how we can use this model to develop our game objects. <!-- .element: class="fragment" -->
- What were the parts of a system we defined? <!-- .element: class="fragment" -->


---

# Review - Objects

- A system is made up of a collection of objects.
 - Examples: Mario, mushroom, blocks.
- Objects are the key component of interest - from a game point of view we want to work at the level of objects.


![image](assets/images/mario.jpg) <!-- .element width="70%"  -->


---

# Review - Properties

- Objects will have properties associated with them.
- The properties determine the values associated with individual objects.
- Examples include: health; armour; position; etc.

 ![image](assets/images/witcher3.jpg) <!-- .element width="70%"  -->


---

# Review - Behaviours

- Objects also have behaviours which define what actions an object performs.
 - In object-orientation we call these behaviours methods.
- Example behaviours include: jumping; running; shooting; etc.

![image](assets/images/minecraft.jpg) <!-- .element width="70%"  -->


---

# Review - Relationships

- The most difficult aspect of a system to define is the relationship between the objects.
- Relationships allow the objects to interact and therefore provide a more complex system. <!-- .element: class="fragment" -->
 - Remember Tic-Tac-Toe versus chess.
- From a game point of view relationships can include: <!-- .element: class="fragment" -->
 - Position of a character in the game world. <!-- .element: class="fragment" -->
 - The type of weapon the character currently holds. <!-- .element: class="fragment" -->
 - The enemy the character is currently attacking. <!-- .element: class="fragment" -->
 - The armour the enemy is currently wearing. <!-- .element: class="fragment" -->
 - How the weapon reacts to that type of armour. <!-- .element: class="fragment" -->


---

# Examples - Entities


---

# Example Game - Starcraft II 

<iframe width="760" height="515" src="https://www.youtube.com/embed/yaqeZ9Snt4E" frameborder="0" allow="autoplay; encrypted-media; picture-in-picture" allowfullscreen></iframe>


---

# Engineering a Game - Objects as Entities
- The first thing to do when considering Starcraft II as a system is to consider the objects.
 - Soldiers. <!-- .element: class="fragment" -->
 - Vehicles. <!-- .element: class="fragment" -->
 - Buildings. <!-- .element: class="fragment" -->
 - Scenery. <!-- .element: class="fragment" -->
 - etc. <!-- .element: class="fragment" -->
- We need a simple method to think of these objects so we can manage and control them. <!-- .element: class="fragment" -->


To do this we will define a basic <!-- .element: class="fragment" -->`entity` class.


---

# Example Game - Pong 

![image](assets/images/pong.gif)


---

# Properties of a Pong Entity

- What do you see in Pong?
 - What are the objects/entities involved in Pong? <!-- .element: class="fragment" -->
- What are the attributes/properties of these entities? <!-- .element: class="fragment" -->
- What are the common properties across all the entities? <!-- .element: class="fragment" -->


---

# The Properties of a Basic Entity

- We can identify four basic properties of a game entity.
- Position of the object in the game world. <!-- .element: class="fragment" -->
    - Store as a 2-dimensional or 3-dimensional vector. <!-- .element: class="fragment" -->
- A graphic representing the entity. <!-- .element: class="fragment" -->
    - Texture for 2D game. <!-- .element: class="fragment" -->
    - Geometry for 3D game. <!-- .element: class="fragment" -->
- We also need basic state information of the entity. <!-- .element: class="fragment" -->
    - We need a flag to indicate if the game entity is active/updatable: alive. <!-- .element: class="fragment" -->
    - We also need a flag to indicate if the entity is visible: visible. <!-- .element: class="fragment" -->


---

# Possible Other Properties

- There are numerous other properties a game entity could have:
    - Velocity.
    - Sound effect.
    - Hit points.
    - etc.
- The key is to keep the game entity abstract and simple. <!-- .element: class="fragment" -->
- We can extend the entity class using inheritance or composition. <!-- .element: class="fragment" -->


---

# Back to Pong - Behaviours

- What sort of behaviours do the Pong entities have?
    - What actions do they perform during the game?
- Are there any similarities between these behaviours?

 ![image](assets/images/pong.gif)


---

<!-- .slide: class="split" -->
# Think About the Game

- What are the main stages that a game goes through?
 - Initialise.
 - Load Content.
 - Update.
 - Render.
 - Shutdown.

- From an entity point of view we can operate on the same basic principles.

![image](assets/images/gameloop.png) <!-- .element height="750px"  -->


---

# An Entity's Behaviours

- We will have four key behaviours for a game entity.
 - <!-- .element: class="fragment" --> **Initialise** - will be managed by the constructor of the entity.
 - <!-- .element: class="fragment" --> **Update** - will update the game entity based on its logic. 
 - <!-- .element: class="fragment" --> **Render** - will display the game entity on the screen.
 - <!-- .element: class="fragment" --> **Destructor** - will shutdown/destroy the entity.

- There are numerous other methods possible: <!-- .element: class="fragment" -->
    - Detect collision. <!-- .element: class="fragment" -->
    - Update AI. <!-- .element: class="fragment" -->
    - Update physics. <!-- .element: class="fragment" -->


---


# Back to System Dynamics

- From a system point of view, we need to define the following:
- Objects / Entities
- Object Properties
 - Abstract level - position, graphic, alive, visible.
- Object Behaviours
 - Abstract level - constructor, update, render, destructor.
- Relationships between Objects.
 - More on this shortly.
- Interaction with the system.
 - External system control - not modelled.

---

# Our Entity Class

![image](assets/images/entity.png) 


---

# Managing Entities


---

# Managing Entities

- Our game will have multiple entities within it.
    - Player controlled characters.
    - Computer controlled characters.
    - Pick-ups.
    - Scenery
- We need some method of managing all these entities without putting
    too much thought into it.

---

# Managing Entities
Games have lots of Entities

![image](assets/images/factorio.jpg) 


---

# Data Structure Approach

- The simplest method: use a suitable data structure. <!-- .element: class="fragment" -->
    - Array, vector, list, or map. <!-- .element: class="fragment" -->
- Then we can just tell them all to update and draw as required. <!-- .element: class="fragment" -->
    - For each entity in the list call Update(). <!-- .element: class="fragment" -->
    - For each entity in the list call Render(). <!-- .element: class="fragment" -->
- However, this has limitations. <!-- .element: class="fragment" -->
    - How do we find a single entity in the list? <!-- .element: class="fragment" -->
    - How do we add or remove entities to/from the list if we want to? <!-- .element: class="fragment" -->
    - How do we access this list throughout the program? <!-- .element: class="fragment" -->
- Therefore we will adopt a software engineering approach to tackle this problem: we'll use a manager class. <!-- .element: class="fragment" -->



---

# Software Engineering Approach

- By using a manager class we can: <!-- .element: class="fragment" --> 
    - Manage the logic of our application in discrete parts, each with their own responsibility. <!-- .element: class="fragment" -->
    - Manage access to our collection of entities. <!-- .element: class="fragment" -->
    - Allow communication between the entities. <!-- .element: class="fragment" -->


---

# Defining our Manager Class
![image](assets/images/entity_manager.png)


---

# Summary


---

# Summary

- Think about the elements of your game as a collection of entities game objects.
    - Use the entity base class.
    - Add required properties.
    - Add required behaviours.
    - Add required relationships.
    - Add the entity to the game world.

- By using an entity based approach and a manager we can focus on individual entity behaviours.
