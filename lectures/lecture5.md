---
title: "Lecture5"
keywords: Lecture
tags: [Lecture]
permalink:  lecture5.html
summary: lecture5
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture X - 
### SET09121 - Games Engineering

<br><br>
Kevin Chalmers and Sam Serrels

School of Computing. Edinburgh Napier University


---

# Recommended Reading

Game Design Workshop. 3rd Edition. Fullerton (2014).

-   Read Chapter 5 on System Dynamics.

![image](gdw_book)


---

# Review - System Dynamics

-   Last lecture we examined systems and how games can be viewed as systems.
-   We incorporated our existing Formal Elements of game design into a system model view of a game.
-   We also showed how we can define a game as a system, and some of the similarities with software development.
-   In this lecture we will go into detail about how we can use this model to develop our based game objects.
-   What were the parts of a system we defined?


---

# Review -- Objects

-   A system is made up of a collection of objects.
    -   Examples: Mario, mushroom, blocks.
-   Objects are the key component of interest -- from a game point of view we want to work on the level of objects.

![image](mario)


---

# Review -- Properties

-   Objects will have properties associated with them.
-   The properties determine the individual values associated with an object.
-   Examples include: health; armour; position; etc.

 ![image](witcher3)


---

# Review -- Behaviours

-   Objects also have behaviours which define what actions an object performs.
    -   In object-orientation we call these behaviours methods.
-   Example behaviours include: jumping; running; shooting; etc.

![image](minecraft)


---

# Review -- Relationships

-   The most difficult aspect of a system to define is the relationship
    between the objects.

-   Relationships allow the objects to interact and therefore provide a
    more complex system.

    -   Remember Tic-Tac-Toe versus chess.

-   From a game point of view relationships can include:

    -   Position of a character in the game world.

    -   The type of weapon the character currently holds.

    -   The enemy the character is currently attacking.

    -   The armour the enemy is currently wearing.

    -   How the weapon reacts to that type of armour.


---

# Examples - Entities


---

# Example Game -- Starcraft II 
[YouTube VideoLink](https://www.youtube.com/watch?v=yaqeZ9Snt4E)


---

# Engineering a Game -- Objects as Entities

-   The first thing to do when considering Starcraft II as a system is
    to consider the objects.
-   There are numerous items in the game:
    -   Soldiers.
    -   Vehicles.
    -   Buildings.
    -   Scenery.
    -   etc.
-   We need a simple method to think of these objects so we can manage
    and control them.
-   To do this we will define a basic `entity` class.


---

# Example Game -- Pong 

![image](pong){width=".9\textwidth"}


---

# Properties of a Pong Entity

-   What do you see in Pong?
    -   What are the objects/entities involved in Pong?
-   What are the attributes/properties of these entities?
-   What are the common properties across all the entities?


---

# The Properties of a Basic Entity

-   We can identify four basic properties of a game entity.
-   Position of the object in the game world.
    -   Store as a 2-dimensional of 3-dimensional vector.
-   A graphic representing the entity.
    -   Texture for 2D game.
    -   Geometry for 3D game.
-   We also need basic state information of the entity.
    -   We need a flag to indicate if the game entity is active/up-datable: alive.
    -   We also need a flag to indicate is visible: visible.

Possible Other Properties

-   There are numerous other properties a game entity could have:
    -   Velocity.
    -   Sound effect.
    -   Hit points.
    -   etc.
-   The key is to keep the game entity abstract and simple.
    -   We can extend the abstract class.
    -   We can add extra properties as needed.
    -   We can also add components to add properties.


---

# Back to Pong -- Behaviours

-   What sort of behaviours do the Pong entities have?
    -   What actions do they perform during the game?
-   Are there any similarities between these behaviours?

 ![image](pong)


---

# Think About the Game

-   What are the main stages that a game goes through?
    -   Initialise.
    -   Load Content.
    -   Update.
    -   Render.
    -   Shutdown.

-   From an entity point of view we can operate on the same basic
    principles.

![image](gameloop)


---

# An Entity's Behaviours

-   We will have four key behaviours for a game entity.
 - Initialise - will be managed by the constructor of the entity.
 - Update - will update the game entity based on its logic.
 -  Render - will display the game entity on the screen.
 -  Destructor - will shutdown/destroy the entity.

-   There are numerous other methods possible:
    -   Detect collision.
    -   Update AI.
    -   Update physics.
-   Again, we are going to keep it abstract and simple.
    -   Constructor.
    -   Update.
    -   Render.
    -   Destructor.


---

# Back to System Dynamics

-   From a system point of view, we need to define the following:
    -   Objects in the system.
        -   Game entities.
    -   Properties of these objects.
        -   Abstract level -- position, graphic, alive, visible.
    -   Behaviours of these objects.
        -   Abstract level - constructor, update, render, destructor.
    -   Relationships between these objects.
        -   More on this shortly.
    -   Interaction with the system.
        -   External system control -- not modelled.

Our Entity Class Definition ![image](entity)


---

# Managing Entities


---

# Managing Entities

-   Our game will have multiple entities within it.
    -   Player controlled characters.
    -   Computer controlled characters.
    -   Pick-ups.
    -   Scenery
-   We need some method of managing all these entities without putting
    too much thought into it.

![image](lego_marvel)


---

# Data Structure Approach

-   The simplest method would be to create a suitable data structure to
    store all of our entities.

    -   Array, list, or map.

-   Then we can just tell them all to update and draw as required.

    -   For each entity in the list call update.

    -   For each entity in the list call render.

-   However, this has limitations.

    -   How do we find a single entity in the list?

    -   How do we remove entities from the list if we want to?

    -   How do we pass the list around the program?

    -   What about general efficiency?

-   Therefore we will adopt a software engineering approach to the
    problem by using a manager class.


---

# Software Engineering Approach


-   By using a manager component we can:

    -   Manage the logic of our application in discrete parts.

    -   Manage access to our collection of entities.

    -   Allow communication between the entities.

 ![image](game_operation)


---

# Defining our Manager Class
![image](entity_manager){height=".8\textheight"}


---

# Summary


---

# Summary

-   Think about the elements of your game as a collection of entities game objects.
    -   Use the entity base class.
    -   Add required properties.
    -   Add required behaviours.
    -   Add required relationships.
    -   Add the entity to the game world.

-   By using an entity based approach and a manager we can focus on individual entity behaviour in the first instance.
