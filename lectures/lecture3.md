---
title: "Lecture3"
keywords: Lecture
tags: [Lecture]
permalink:  lecture3.html
summary: lecture3
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---

Outline

Recommended Reading

.5 Game Design Workshop. 3rd Edition. Fullerton (2014).

-   Read Chapter 3 on Formal Elements.

.5 ![image](gdw_book){width="\textwidth"}

What are the Formal Elements of Games?

-   Fullerton defines eight elements.

    -   Players

    -   Objectives

    -   Procedures

    -   Rules

    -   Resources

    -   Conflict

    -   Boundaries

    -   Outcome

-   The Formal Elements held us define a game from a design perspective,
    but we can use these elements to scope the technical game aspects.

-   We will look at each of the Formal Elements in turn during this
    lecture.

Players
=======

Players: who plays and why?

.5

-   Games are designed for the players.

    -   You must consider your game genre and target audience.

-   A game should start with an invitation to play. You want to entice
    the player.

    -   Cinemas dim their lights.

    -   A book uses a cover.

    -   A game has a title screen or introduction video.

-   What about the games you play? What made you buy them?

.5 ![image](players){width="\textwidth"}

Player Interaction Patterns

-   A **Player Interaction Pattern** helps us define how players play
    the games.

-   There are seven key types:

    -   Single-player versus the game.

    -   Multiple individual players versus the game.

    -   Player versus player.

    -   Unilateral competition.

    -   Multilateral competition.

    -   Cooperative play.

    -   Team competition.

-   What about the games you normally play? What are the common
    interaction patterns that you engage with?

Guess the Interaction Pattern

.5 ![image](witcher3){width="\textwidth"}
![image](wow){width="\textwidth"}

.5 ![image](hearthstone){width="\textwidth"}
![image](overwatch){width="\textwidth"}

Player Roles

-   You need to define what the player's task is in the game.

-   Traditional board and card games had everyone with the same role,
    more or less.

    -   Some players had multiple roles -- Dungeons and Dragons,
        Monopoly, etc.

-   Video games follow this tradition.

    -   Again, there are exceptions such as Team Fortress 2, Neverwinter
        Nights, etc.

-   Roles can also be defined by how the players react to the rules and
    procedures of the game.

    -   Players can be collectors, explorers, killers, etc.

    -   See Fullerton for further discussion.

    -   Consider the type of player that you are aiming for.

Objectives
==========

Objectives: providing drive and challenge to the player

-   If players are the heart of the game experience, then objectives
    drive the heart.

-   **Objectives** provide a challenge to the player that should be
    achievable.

-   Objectives also set the overall feel of the game:

    -   FPS: killing and survival.

    -   RPG: exploration.

-   Objectives may also be made up of sub-objectives.

    -   Side quests are similar, but normally distinct.

Fullerton: six questions for objectives

-   Fullerton poses six questions to help determine the objectives of a
    game:

    -   What are some of the objectives of the games you have played?

    -   What impact do these objectives have on the tone of the game?

    -   Do certain genres of play lend themselves to certain objectives?

    -   What about multiple objectives?

    -   Do objectives have to be explicit?

    -   What about player determined objectives?

-   You should pose these questions to yourself when coming up with your
    game idea.

Common Objective Types

-   There are a collection of common objective types for you to select
    from:

    -   Capture / destroy

    -   Chase

    -   Race

    -   Alignment

    -   Rescue / escape

    -   Forbidden act

    -   Construction

    -   Exploration

    -   Solution

    -   Outwit

-   Again, see Fullerton for more information.

Exercise

-   Describe the objectives in the games you normally play. Think about
    your favourite games in particular.

-   Discuss any similarities you find.

Procedures
==========

Procedures: how we interact with and control the game

-   **Procedures** describe how we interact with the game world and how
    we can act within the game world.

-   Video games typically define procedures via the input control
    mechanisms.

-   Procedures can be broadly broken down into four categories:

    -   Starting actions.

    -   Progression of action.

    -   Special actions.

    -   Resolving actions.

System Procedures

.5

-   Video games are complex systems.

-   Numerous background processes are running in a game.

    -   Compare Dungeons and Dragons to a video game RPG.

-   Physics, AI, etc. are all procedures within a game.

-   The game engine we are developing will feature these background
    procedures.

-   Systems are examined in the next lecture.

.5 ![image](dnd){width="\textwidth"}
![image](neverwinter){width="\textwidth"}

Rules
=====

Rules: defining objects and actions

-   **Rules** define the objects in the game and the possible actions in
    the game.

-   Fullerton again poses some questions to ask yourself when defining
    rules:

    -   How do players learn the rules?

    -   How are the rules enforced?

    -   What kinds of rules work best in certain situations?

    -   Are there patterns to rule sets?

    -   What can we learn from these patterns?

-   Consider the rules for the game you want to develop:

    -   Is you game idea obvious to play, or do you have to provide the
        player with instructions?

    -   Are your rules correct for the type of gameplay experience you
        are aiming for?

Defining Objects and Actions

-   Rules can help us define the game objects within our game world.

    -   We will discuss entities next week.

-   Consider a shotgun in a FPS:

    -   Cost: \$ 500

    -   Damage: 20

    -   Spread: 10

    -   Range: 5

    -   Ammo: 2

-   We can directly implement this idea in our game -- it is just an
    object definition.

-   We can also use rules to restrict certain actions for progression
    purposes.

    -   For example: Assassin's Creed series, Batman Arkham series.

-   Providing all capabilities at once can confuse and frustrate the
    player.

Determining Effects

-   Rules that trigger events or effects are very useful when
    considering the procedures of our game.

-   We can boil down such rules to a collection of `if` statements:

    -   `if player’s health == 0 then player dies.`

    -   `if player picks up apple health increases by 10.`

    -   etc.

-   It is likely that most of your rules will be effect based.

Resources
=========

Resources: providing in-game assets

.5

-   A **resource** is an asset that provides the player with an
    advantage, allows the player to reach certain objectives, or allows
    the player to perform certain procedures.

-   Resources are ubiquitous in games:

    -   Monopoly has money and property.

    -   Command & Conquer has Timberium.

    -   Halo has health, ammunition, and weapons.

.5 ![image](resource){width="\textwidth"}

Resource Types

.5

**Obvious Resources**

-   Lives

-   Units

-   Health

-   Currency

-   Inventory

-   Special terrain

.5

**Not Obvious Resources**

-   Actions

-   Power-ups

-   Time

List Resources in the Games You Play List the five games you have played
most recently and list the resources available in each. Note any
similarities, particularly in games of the same genre.

Conflict
========

Conflict: the contradiction in objectives, procedures, rules, and
resources

-   **Conflict** occurs due to the contradiction between the objectives,
    the procedures, the rules, and the resources.

    -   A great example is golf.

    -   The objective is simple: put the ball in the hole ...

    -   ...but the hole and ball are small ...

    -   ...and you can only move the ball with a little stick ...

    -   ...and the hole is away over there!

-   There are techniques that can be used to add conflict to a game.

    -   Conflict increases challenge, which increases the fun.

-   Some examples:

    -   Obstacles: physical or conceptual.

    -   Opponents: NPCs or other players.

    -   Dilemmas: putting real choice in a game.

Boundaries
==========

Boundaries

-   **Boundaries** define where the game is taking place.

-   Games exist in a world where the formal elements exist.

    -   Sometimes called the magic circle.

-   Boundaries can be physical.

    -   The normal approach in video games.

    -   Screen boundaries, 3D world boundaries, etc. are all examples of
        this.

-   Boundaries can be conceptual.

    -   The players make an agreement to stay within the rules of the
        game.

Outcome
=======

Outcome

-   The **outcome** is the expected payoff from playing the game.

-   How do we resolve the objectives that the game sets out?

-   What, if any, is the end state of the game?

-   Winning is a traditional end point.

    -   The game state where one of the winning conditions (objectives)
        has been met.

-   Some games continue without end points.

    -   Sim City.

    -   Sims.

Summary
=======

Try Describing a Game Using the Formal Elements

-   After class, try and describe a game (board, video, or whatever)
    using the vocabulary of formal elements.

-   Try to go into some real depth: it will help you understand games
    more.

-   Think about the formal elements whenever you play a game. Try and
    pick apart the individual elements of the game to get a better
    understanding of how it is put together.

Summary

-   We have covered the eight formal elements:

    Players:

    :   who is playing our game?

    Objectives:

    :   what is the goal of playing the game?

    Procedures:

    :   how is the game played?

    Rules:

    :   what are the restrictions on how the game is played?

    Resources:

    :   what assets are available in the game?

    Conflict:

    :   how do the objectives, procedures, rules, and resources
        constrain each other?

    Boundaries:

    :   where is the game played?

    Outcome:

    :   how is the game resolved?
