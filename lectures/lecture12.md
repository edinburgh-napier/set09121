---
title: "Lecture12"
keywords: Lecture
tags: [Lecture]
permalink:  lecture12.html
summary: lecture12
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 12 - AI
### SET09121 - Games Engineering

<br><br>
Kevin Chalmers and Sam Serrels

School of Computing. Edinburgh Napier University


---

# Recommended Reading

.5

- Artificial Intelligence for Games. Second Edition. Millington and
    Funge, 2009.

    - A good resource for anyone interested in game AI.

.5 ![image](ai_book)

History of Game AI
==================

What is Artificial Intelligence?

- Artificial Intelligence (AI) is the study of the intelligence of
    machines, and the attempt to replicate human-like intelligence in a
    machine.

    - This is a very wide area of study, incorporating not only
        technical implementations of intelligence, but also ethics and
        philosophy.

- From a games point of view, we will take a very simple outlook:

    - AI is any algorithm or code that controls the behaviour of one
        of our game entities.

- Therefore, our Pong example back at the start of the module had a
    form of AI.

    - It was dumb, but it still behaved in a way that would allow it
        to be competitive.

Academic AI vs. Game AI

- Academic AI and game AI have overlaps, but normally have a different
    goal today.

- Academic AI can be (very roughly) broken down into two phases:

    - Symbolic AI (early days)

        - Set of knowledge and reasoning algorithms.

    - Nature inspired and learning (modern era)

        - Techniques inspired by models in nature and statistical
            inference.

        - Used in learning, neural networks, genetic algorithms, etc.

- Game AI is still rooted in the deterministic, classical, symbolic AI
    era.

    - Symbolic AI still used extensively in games.

        - Path finding, state machines, etc.

    - Modern techniques have been tried, but are rarely successful.

    - Modern techniques have seen more success in content creation.

        - Do some research into procedural content generation if you
            are interested.

In the beginning...PacMan

.5

- PacMan is recognised as one of the earliest examples of AI in games.

- PacMan AI relied on state machine behaviour.

    - We will look at state machines in a separate lecture.

- This was in 1979...

- ...game AI did not change much until the mid 1990s.

.5 ![image](pacman)

Timeline

- In the mid-1990s AI started to become a selling point.

    1994:

    :   Beneath a Steel Sky mentions AI on the box.

    1997:

    :   GoldenEye 007 introduces world sensing, allowing enemies to see
        allies and notice when they were killed.

    1998:

    :   Thief: The Dark Project and Metal Gear Solid expanded the world
        sensing concept.

- Real-time strategy games also emerged in the 1990s.

    1994:

    :   Warcraft used noticeable path finding techniques.

    1998:

    :   Warhammer: Dark Omen has robust formation motion.

- Games also start emerging where AI is the main game mechanic.

    1997:

    :   Creatures.

    2000:

    :   The Sims.

    2001:

    :   Black and White.

AI Techniques for Games
=======================

AI Techniques for Games -- Overview

- There are numerous usable AI techniques applicable for games.

    - Classical AI techniques -- common.

    - Newer academic techniques -- uncommon.

- Different techniques accomplish different aspects of game behaviour.

    - Movement of entities.

    - Decision making for entities.

    - Strategic (planning) decisions.

    - Learning from player behaviour.

- We will only look at the first two in detail as they are the most
    common. Other modules look at learning and planning in a general AI
    context.

Movement
--------

Movement and Steering Behaviours

- Steering behaviours.

    - An algorithm which determines how an entity should move.

    - Can be goal based:

        - Seek, flee.

    - Or can be more general:

        - Patrol, wander.

- Steering behaviours are an important base concept of most game AI.

    - It allows us to move entities in a certain manner.

- Steering behaviours can also be combined to create more complex
    behaviour.

    - Look into flocking behaviour to get an idea.

- We will look at steering behaviours next week.

Path Finding

.5

- Path finding is the discovery of the route between two points in a
    game world.

- Information from path finding can be fed into steering behaviours.

- Two core techniques:

    Waypoints:

    :   most common.

    Navigation mesh:

    :   becoming more popular (see image).

- Two common algorithms:

    - Dijkstra (slow but checks everything).

    - A\* (fast).

.5 ![image](waypoint-mesh)

Decision Making
---------------

Decision Making

- We are going to look at two types.

- State machines.

    - Similar to the idea of state modelling in UML.

    - Character has a number of possible states.

        - e.g. attack, hide, run, etc.

    - Character determines current state based on any number of
        conditions.

    - Character will change state when a particular action occurs.

        - If patrolling and player is spotted then change state to
            attacking.

- Decision trees.

    - Similar to activity diagrams in UML.

    - Used to control characters decision making process.

    - Can also be used to control animation.

    - Very simply AI technique to implement but can be very powerful.

Decision Making

.5

- Fuzzy logic

    - Logic design with grey (fuzzy) areas.

    - Fuzzy logic applies some randomness to decisions.

    - It tries to be more in line with how people think.

    - Not very popular in academia any more -- probability and
        statistics preferred.

.5 ![image](fuzzy-logic)

Decision Making

- Some others...

    - Behavioural trees

        - Chaining tasks and decisions together to form complicated
            behaviours.

    - Markov systems

        - Similar to fuzzy logic but works with probability or
            priority of transitions.

    - Goal-oriented behaviour

        - Character chooses an action based on its current goals.

    - Rule-based systems

        - Database of "if" conditions to determine the behaviour to
            take.

Strategy
--------

World Data

- Modern game AI techniques rely on data from the game world to be
    effective.

    - We have already seen this with path finding.

- Depending on the AI technique used, different types of data can be
    required.

    - Movement may need to know about obstacles, jump points.

    - Knowledge of cover.

    - Knowledge of other characters in the game world to launch
        attacks.

    - etc.

- Most of the techniques requiring world data are referred to as
    strategic techniques.

Strategy

- Waypoint tactics.

    - Areas of the map are marked for tactical significance.

        - For example cover positions, sniper positions, etc.

    - The AI determines which waypoint to head for based on an
        algorithm.

    - This allows strategic looking behaviour from the game
        characters.

    - This technique is used extensively in FPS and similar games.

- Tactical path finding.

    - Takes consideration of the surroundings when determining how to
        move between waypoints.

    - [[example]{style="color: blue"}](https://youtu.be/0i7SMSdwbLI)

Tactical Analysis

- Essentially a more complicated version of the previous ideas.

- The map is marked up with values to help determine where characters
    should move to.

- An algorithm analyses the date to determine the action to take.

- For example:

    - Consider a RTS game.

    - There is a base located on the map.

    - The base area is given a high target value.

    - This makes the game AI try to control this base area.

- Adding dynamic data means that the AI will dynamically respond to
    the player's actions.

Learning
--------

Learning

.5

- Neural networks are a popular nature inspired technique.

- They are modelled on a simplified idea of the brain.

    - Neurons input a signal.

    - Output signal to other neurons.

    - etc.

- Can have learning developed over time.

- Academia and hobbyists see it as a method to implement believable
    learning.

- So far has fallen short on large scale game projects (see Creatures,
    Fable II).

.5 ![image](neural-network)

Learning

- Emergent behaviour.

    - Evolutionary algorithms.

        - AI is given a set of values (genes) to determine the
            likelihood of performing an action.

        - If the AI is successful the genes are used for breeding the
            next iteration.

    - Bio-inspired algorithms.

        - Ant colony optimisation.

        - Artificial immune systems.

- Depending on your programme you might study some of these topics:

    - Multi-agent systems.

    - Computational intelligence.

    - Emergent computing for optimisation.

Bad and Cheating AI
===================

Goal of AI -- Realism

- The goal of game AI is to provide a challenge to the player and
    create a realistic, living world.

- Examples:

    - [[GTA IV]{style="color: blue"}](https://youtu.be/zaqBBD3QQ_I)

    - [[Assassin's Creed
        Origins]{style="color: blue"}](https://youtu.be/LRUcpaIAKos)

- Notice:

    - People walking around.

    - Reaction to environment effects (e.g. rain).

    - Enemies attacking in realistic looking manners.

    - etc.

Bad AI

- [[Half-Life 2]{style="color: blue"}](https://youtu.be/WzYEZVI46Uw)

- [[Halo 3]{style="color: blue"}](https://youtu.be/bc5BwK3iNh0)

- [[Path finding
    problems]{style="color: blue"}](https://youtu.be/lw9G-8gL5o0)

- Just search on YouTube -- there are lots of modern examples.

Bad AI

- When the game designer comes up with an idea, they have a vision of
    how the game will play.

- A game is meant to be a carefully crafted experience for the player.

- AI can and does bring unpredictability to the game.

    - The game designer might not want unpredictable behaviour.

- Developers may override the AI decisions for the sake of gameplay.

Bad AI

- There are lots of examples of bad AI breaking immersion.

- Stupid NPCs getting stuck in corners or getting in your way.

- The design of the level needs to take account of the NPCs' ability
    to navigate it.

    - Hence you normally get large doorways and corridors.

- Unfortunately, the AI, NPCs, and levels are normally designed by
    different teams.

    - So it doesn't normally work first time.

    - At some point comprises have to be made.

- If the NPC is on the player's side you need to make sure it does not
    let them down.

    - Halo was notorious for this. Fallout 4 has problems also.

Cheating AI

- Many game AI systems cheat.

    - [[Pro Evolution
        Soccer]{style="color: blue"}](https://youtu.be/HIZmQ7F1EZg)

- Sometimes this is referred to as rubber band AI.

    - A term originally coined in racing games.

    - Opposition always appeared to keep up with you no matter how
        well you drove.

    - Opposition was considered to be on a rubber band attached to the
        player.

Summary
=======

Summary

- We have looked at a very broad picture of what game AI is.

    - Essentially, use classical, deterministic techniques.

- We also introduced some techniques that are used in games.

    - Movement

    - Decision making

    - Strategy

    - Learning

- And how AI can sometimes not work how the player wants.

    - Bad AI

    - Cheating AI

- All these areas are worth considering depending on your game. Just
    think about the time you have and the experience you are aiming for.
