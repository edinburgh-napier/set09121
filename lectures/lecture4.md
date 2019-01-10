---
title: "Lecture4"
keywords: Lecture
tags: [Lecture]
permalink:  lecture4.html
summary: lecture4
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
Outline

Recommended Reading

.5 Game Design Workshop. 3rd Edition. Fullerton (2014).

-   Read Chapter 5 on System Dynamics.

.5 ![image](gdw_book){width="\textwidth"}

Example -- Donkey Kong: what do you see? [YouTube Video
Link](https://www.youtube.com/watch?v=Pp2aMs38ERY)

What is a System?
=================

What is a System?

.5

-   An example of a system is an engine.

    -   This is why we are building an engine in the module.

-   An engine is a system with a particular objective.

    -   That objective is to power some form of manual action.

-   We can apply our formal elements into thinking about systems.

.5 ![image](engine){width="\textwidth"}

What is a System? Defining Objects

-   **Objects**

    -   Objects are the basic elements of a system. Consider the objects
        you define in object-oriented development.

    -   A system is the interaction between the defined objects.

    -   For example, an engine, steering wheel, and wheels interact
        together to allow a car to operate.

    -   The complexity of the underlying system is hidden behind an
        interface. For a car, this is the steering wheels and pedals.

    -   Objects are defined by their properties and behaviours. The
        Relationships between objects completes our view.

-   What objects can you see in other systems?

What is a System? Object Properties

-   **Properties**

    -   Properties are the attributes of the objects in a system.

    -   From an object-oriented point of view, properties are the values
        stored in the object, which may themselves be objects.

    -   Properties are the descriptive data that can be analysed,
        extracted or modified.

    -   Properties are also a method to allow unique identification of
        individual objects.

-   What are the properties of objects you see in other systems?

-   From our rules (see Formal Elements) we have extracted some (if not
    all) of our objects and their properties.

    -   Rules (Formal Elements) $\Rightarrow$ Objects (System) and
        Properties (System).

What is a System? Object Behaviours

-   **Behaviours**

    -   Behaviours are the actions that an object undertakes.

        -   Object-orientation calls these methods.

    -   Some of the behaviours are on the state of the object.

        -   An engine requires fuel to be turned on.

        -   We will look at state and state management later in the
            module.

        -   Remember: our formal rules can restrict our formal
            procedures.

    -   Behaviours can be complicated.

-   What object behaviours of objects do you see in other systems?

-   Rules and Procedures of Formal Elements can help us extract some of
    the object behaviours.

    -   Rules (Formal Elements) and Procedures (Formal Elements)
        $\Rightarrow$ Behaviours (System).

What is a System? Relationships Between Objects

-   Once we have a collection of components we can define some
    rudimentary systems.

-   Objects themselves do not form a system. The key aspect is that
    there is a relationship between the objects.

-   **Relationships**

    -   Depend on the context of use.

        -   A car engine turns wheels.

        -   A jet engine propels a plane.

    -   We must determine the relationships between our objects to
        create a system.

-   A game is a system!

    -   With a shiny graphical front end.

    -   Remember: the graphical presentation is almost separate from the
        system operation.

Objects
-------

Games as Systems: Objects

-   Objects

    -   Objects are the basic elements of a system.

    -   This week we will define entities in the practical.

        -   This will form the basis of our game engine.

    -   Almost everything in a game is an object.

        -   For example: players, opponents, environment.

    -   As developers we must identify these objects and develop them.

    -   Games are similar to standard applications although the object
        relationship are probably more dynamic.

-   What objects can you identify in the games you play?

What Objects Are Here? ![image](mario){width="\textwidth"}

Properties
----------

Games as Systems: Properties

-   Properties

    -   Define the individuality of an object.

    -   Some common properties include:

        -   Position.

        -   Graphic.

        -   A flag (bool) to indicate whether the object is alive.

-   A basic entity in a game can have numerous properties and
    sub-components.

    -   RPG in particular has lots of properties.

    -   We will examine how we compose objects later.

-   What properties can you identify in the games you play?

What Properties are Here? ![image](witcher3){width="\textwidth"}

Behaviours
----------

Games as Systems: Behaviours

-   Behaviours

    -   Define what our objects can do.

    -   Behaviours are the basic actions within the game world.

        -   Update and draw are core features of the game, so it is
            normally other actions we are concerned with.

    -   We are normally thinking about what can happen during the update
        step.

        -   For example, when B is pressed the player should jump.

    -   Depending on the entity there could be many behaviours involved.

        -   Player's actions are normally based on the controls
            (Procedures from Formal Elements).

        -   Computer controlled actions are normally supplied via some
            form of Artificial Intelligence.

        -   The physical objects are normally controlled by the physics
            system.

-   What behaviours do you see in the games that you play?

What Behaviours are Here? ![image](minecraft){width="\textwidth"}

**Describe your favourite game as a system.** Pick one of your favourite
games and described the objects, their properties and their behaviours.
Keep it abstract, but consider how you would program these.

Relationships
-------------

Games as Systems: Relationships

-   Relationships

    -   How to make a collection of discrete objects into a system.

    -   Games have complex and dynamic relationships based on the game
        state.

    -   Some relationships are simple.

        -   The game character relates to the world via a position.

    -   Some relationships can be based on the current state.

        -   If character is evil then guards will chase on sight.

    -   Relationships can also be defined by the rules of the game.

    -   Relationships are possibly the hardest concept to understand in
        software development.

    -   They may also cause difficulties later in development when
        considering optimisation.

De-constructing Games
=====================

De-constructing Tic-Tac-Toe

-   A system is more than the objects that make it up.

    -   Dynamic relationships cause unforeseen interactions and Conflict
        (Formal Elements).

    -   Small changes in object properties can have a dramatic effect.

-   Tic-Tac-Toe (noughts and crosses) is a simple game.

    Objects:

    :   squares.

    Properties:

    :   symbol within the square (`O`, `X`, or empty).

    Behaviours:

    :   set symbol in square.

    Relationships:

    :   based on location of matching symbols. Important considering the
        gameplay mechanic.

Tic-Tac-Toe Game States ![image](tic-tac-toe){width="\textwidth"}

De-constructing Chess

-   Chess is a significantly more complex and strategic game than
    Tic-Tac-Toe.

-   From a system's point of view, chess and tic-tac-toe are very
    similar.

    Objects:

    :   squares, pieces.

    Properties:

    :   colour, rank, location of piece.

    Behaviours:

    :   move piece.

    Relationships:

    :   location of piece to other pieces.

-   Why is the outcome so different?

Game Flow

.5

-   The key difference is the range of possibilities.

-   Games should offer possibility; unpredictability.

-   A game also needs to be simple.

-   Games can be defined by the flow of possibilities or structure
    through the game.

-   As stated: objects are simple; the relationships between the objects
    lead to the complexity.

.5 ![image](gameflow){width="\textwidth"}

Example Systems
===============

Economics

.5

-   Many games have a simple economy where resources can be exchanged.

    -   Remember that Resources are a Formal Element.

-   The use of currency is seen in a wide range of games:

    -   RPGs.

    -   Assassin's Creed series.

    -   Batman: Arkham series.

    -   etc.

-   What does an economy require from a systems point of view?

.5 ![image](economics){width="\textwidth"}

Emergent Systems

.5

-   Emergent systems exhibit behaviour not explicitly programmed. The
    behaviour emerges from the rules placed on the objects.

    -   Birds flocking is such a behaviour.

    -   Very relative to game AI.

-   The system here could be quite simple.

-   Nature is full of such systems.

-   Examples include: Game of Life; Spore; The Sims (sort of).

.5 ![image](flocking){width="\textwidth"}

Software System

.5

-   The common approach to writing software is through
    object-orientation.

    -   Standard model in C\#, Java, and used in C++.

-   With object-orientation we have:

    Objects:

    :   which are instances of classes.

    Attributes:

    :   which are the properties of the objects.

    Methods:

    :   which are behaviours associated with objects.

.5 ![image](uml){width="\textwidth"}

Back to Donkey Kong [YouTube Video
Link](https://www.youtube.com/watch?v=Pp2aMs38ERY)

System Interaction and Control

-   A system does not always need interaction.

    -   A game definitely does.

-   Some things to consider:

    -   What information is provided to the player about the system?

    -   What can the player control?

    -   How does the player control these objects?

    -   What feedback occurs within the system?

    -   How do these considerations affect the gameplay?

-   A game will require tuning and polish to get all of these factors
    correct.

Summary
=======

Summary

-   From this lecture you should understand:

    Objects:

    :   the parts that make up a system.

    Properties:

    :   the values that define the objects of the system.

    Behaviours:

    :   what functions does an object perform in a system.

    Relationships:

    :   how do the objects interact with each other.

-   You should use these principles to try and de-construct games that
    you are familiar with.
