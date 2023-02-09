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
Babis Koniaris
<br>


School of Computing. Edinburgh Napier University


---

# Recommended Reading

- Artificial Intelligence for Games. Second Edition. Millington and Funge, 2009.
    - A good resource for anyone interested in game AI.

![image](assets/images/ai_book.jpg)<!-- .element width="30%" -->


---

# History of Game AI

What is Artificial Intelligence?
- AI is the study of the intelligence of machines, and the attempt to replicate human-like intelligence in a machine.
- This is a very wide area of study, incorporating not only technical implementations of intelligence, but also ethics and philosophy.


---

# Our View of Game AI

- From a games point of view, we will take a very simple outlook:
    - AI is any algorithm or code that controls the behaviour of one of our game entities.
- Therefore, our Pong example back at the start of the module had a form of AI.
- It was dumb, but it still behaved in a way that would allow it to be competitive.

---

# Academic AI vs...

- Academic AI can be (very roughly) broken down into two phases:
    - Symbolic AI (early days)
        - Set of knowledge and reasoning algorithms.
    - Nature-Inspired and Learning AI (modern era)
        - Inspired by models in nature and statistical inference.
        - Used in learning, neural networks, genetic algorithms, etc.

---

# ...vs Game AI

- Game AI is still rooted in the deterministic, classical, symbolic AI era.
    - Symbolic AI still used extensively in games.
        - Path finding, state machines, etc.
    - Modern techniques have been tried, but are rarely successful.
    - Modern techniques have seen more success in content creation.

---

# In the beginning...PacMan

- PacMan is recognised as one of the earliest examples of AI in games.
- PacMan AI relied on state machine behaviour.
    - We will look at state machines in a separate lecture.
- This was in 1979...
- ...game AI did not change much until the mid 1990s.

![image](assets/images/pacman.gif) <!-- .element width="30%" -->


---

# Timeline

- In the mid-1990s AI started to become a selling point.
 - **1994**: Beneath a Steel Sky mentions AI on the box.
 - **1997**: GoldenEye 007 introduces world sensing, allowing enemies to see allies and notice when they were killed.
 - **1998**: Thief: The Dark Project and Metal Gear Solid expanded the world sensing concept.
- Real-time strategy games also emerged in the 1990s.
 - **1994**: Warcraft used noticeable path finding techniques.
 - **1998**: Warhammer: Dark Omen has robust formation motion.
- Games also start emerging where AI is the main game mechanic.
 - **1997**: Creatures.
 - **2000**: The Sims.
 - **2001**: Black and White.


---

# AI Techniques for Games

- There are numerous usable AI techniques applicable for games.
    - Classical AI techniques - common.
    - Newer academic techniques - uncommon.
- Different techniques accomplish different aspects of behaviour.
    - Movement of entities.
    - Decision making for entities.
    - Strategic (planning) decisions.
    - Learning from player behaviour.
- We will only look at the first two in detail as they are the most common. Other modules look at learning and planning in a general AI context.



---

## Movement


---

# Movement and Steering Behaviours

- Steering behaviours.
    - An algorithm which determines how an entity should move.
    - Can be goal based:
        - Seek, flee.
    - Or can be more general:
        - Patrol, wander.
- Steering behaviours are an the base of most game AI.
    - It allows us to move entities in a certain manner.
- Steering behaviours can also be combined to create more complex behaviour.
    - Look into flocking behaviour to get an idea.
- We will look at steering behaviours next week.


---

# Path Finding

- Path finding is the discovery of the route between two points in a game world.
- Information from path finding can be fed into steering behaviours.
- Two core techniques:
    Waypoints: most common.
    Navigation mesh: becoming more popular (see image).
- Two common algorithms:
    - Dijkstra (slow but checks everything).
    - A* (fast).

![image](assets/images/waypoint-mesh.jpg)

---

## Decision Making


---

# Decision Making

- We are going to look at two types.
	- State Machines
	- Decision Tress

---

# State machines

- Similar to the idea of state modelling in UML.
- Character has a number of possible states.
    - e.g. attack, hide, run, etc.
- Character determines current state based on any number of conditions.
- Character will change state when a particular action occurs.
    - If patrolling and player is spotted then change state to attacking.

---

# Decision trees
    
- Similar to activity diagrams in UML.
- Used to control characters decision making process.
- Can also be used to control animation.
- Very simple AI technique to implement, but it can be very powerful.

---



# Other Decision Making Techniques

- Fuzzy logic
    - Logic design with grey (fuzzy) areas.
    - Fuzzy logic applies some randomness to decisions.
    - It tries to be more in line with how people think.
    - Not very popular in academia any more - probability and statistics preferred.

![image](assets/images/fuzzy-logic.png) <!-- .element width="30%" -->


---

# Other Decision Making Techniques

- Behavioural trees
	- Chaining tasks and decisions together to form complicated behaviours.
- Markov systems
	- Similar to fuzzy logic but works with probability or priority of transitions.
- Goal-oriented behaviour
	- Character chooses an action based on its current goals.
- Rule-based systems
	- Database of "if" conditions to determine the behaviour to take.

---

# Strategy - World Data

- Modern game AI techniques rely on data from the game world.
    - We will see this more with Path Finding.
- Depending on the technique used, different types of data are required.
    - Movement may need to know about obstacles, jump points.
    - Knowledge of cover.
    - Knowledge of other characters in the game world.
    - etc.
- Most of the techniques requiring world data are referred to as strategic techniques.


---

# Strategy - Waypoint tactics.

- Areas of the map are marked for tactical significance.
    - For example cover positions, sniper positions, etc.
- The AI determines which waypoint to head for based on an algorithm.
- This allows strategic looking behaviour from the game characters.
- This technique is used extensively in FPS and similar games.
- Tactical path finding.
    - Takes consideration of the surroundings when determining how to move between waypoints.


---

# Strategy - Waypoint tactics.

<iframe width="1400" height="800" src="https://www.youtube.com/embed/0i7SMSdwbLI" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# Tactical Analysis

- Essentially a more complicated version of the previous idea.
- The map is marked up with values to help determine where characters should move to.
- An algorithm analyses the data to determine the action to take.
- For example:
    - Consider a RTS game.
    - There is a base located on the map.
    - The base area is given a high target value.
    - This makes the game AI try to control this base area.
- Adding dynamic data means that the AI will dynamically respond to the player's actions.


---

## Learning

---

# Neural Networks

- Neural networks are a popular nature inspired technique. 
- They are modelled on a simplified idea of the brain.
    - Neurons input a signal.
    - Output signal to other neurons.
- Can have learning developed over time.
- Academia and hobbyists see it as a method of believable learning.
- So far fallen short on game projects (see Creatures, Fable II).


![image](assets/images/neural-network.png) 


---

# Other Techniques

- Emergent behaviour.
    - Evolutionary algorithms.
        - AI is given a set of values (genes) to determine the likelihood of performing an action.
        - If the AI is successful the genes are used for breeding the next iteration.
    - Bio-inspired algorithms.
        - Ant colony optimisation.
        - Artificial immune systems.
- Depending on your programme you might study some of these:
    - Multi-agent systems.
    - Computational intelligence.
    - Emergent computing for optimisation.


---

# Issues With These Techniques

- The learning techniques often lack designer control
	- They are 'black boxes'
- This makes it hard to tune the AI agents in your systems
- So how do we guarantee that the AI make the game *better*?

---

# Goal of AI - Realism

- One goal of game AI is to provide a challenge to the player and create a realistic, living world.
- Examples:
    - GTA V/RDR 2
    - Assassin's Creed Origins
- Notice:
    - People walking around.
    - Reaction to environment effects (e.g. rain).
    - Enemies attacking in realistic looking manners.
    - etc.

---

## Bad AI


---

# Bad AI

<iframe width="1400" height="800" src="https://www.youtube.com/embed/Vk3aruZIZyo" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# Bad AI
<iframe width="1400" height="800" src="https://www.youtube.com/embed/bc5BwK3iNh0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# Bad AI

- When the game designer comes up with an idea, they have a vision of how the game will play.
- A game is meant to be a carefully crafted experience for the player.
- AI can and does bring unpredictability to the game.
    - The game designer might not want unpredictable behaviour.
- Developers may override the AI decisions for the sake of gameplay.


---

# Bad AI

- There are lots of examples of bad AI breaking immersion.
- Stupid NPCs getting stuck in corners or getting in your way.
- The design of the level needs to take account of the NPCs' ability to navigate it.
    - Hence you normally get large doorways and corridors.
- Unfortunately, the AI, NPCs, and levels are normally designed by different teams.
    - So it doesn't normally work first time.
    - At some point comprises have to be made.
- If the NPC is on the player's side you need to make sure it does not let them down.
    - Halo was notorious for this. Fallout 4 has problems also.

---

# 'Protect Natalya'

These words still make me grumpy...

![image](assets/images/GoldenEye-Natalya.png) 

---

# AI Doesn't Exist in a Vacuum

- Many examples of 'great AI' don't have very complicated AI...
- How effective the AI appears is down to more than just the algorithm
	- How well tagged is the level?
	- How is the level designed? (F.E.A.R. / Half-Life)
	- Do the mechnics support the AI? (Civ V)
	- How much does the player see?

---

# Cheating AI

- Many game AI systems cheat.
    - Pro Evolution Soccer
- Sometimes this is referred to as rubber band AI.
    - A term originally coined in racing games.
    - Opposition always appeared to keep up with you no matter how well you drove.
    - Opposition was considered to be on a rubber band attached to the player.


---

# Cheating AI

<iframe width="1400" height="800" src="https://www.youtube.com/embed/HIZmQ7F1EZg" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# Non-Cheating AI

- This can also happen in reverse!
- If your AI is too good, people will think it cheats.
	- This was the case in Farcry 1
	- Most Backgammon games get accused of this too
- This is partly why most FPS baddies shout what they are doing
	- (Although this is often also a lie, see F.E.A.R. 1)

---

# Summary

- We have looked at a very broad picture of what game AI is.
    - Essentially, use classical, deterministic techniques.
- We also introduced some techniques that are used in games.
	- Movement
	- Decision making
	- Strategy
	- Learning

---

# Remember...

- AI can sometimes not work how the player (or designer) wants.
    - Bad AI
    - Cheating AI
	- Unfun AI
- All these areas are worth considering depending on your game. Just think about the time you have and the experience you are aiming for.