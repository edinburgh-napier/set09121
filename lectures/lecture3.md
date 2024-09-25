---
title: "Lecture 3 - Formal Elements"
keywords: Lecture
tags: [Lecture]
permalink:  lecture3.html
summary: lecture3
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 3 - Formal Elements of Games
### SET09121 - Games Engineering

<br><br>
Babis Koniaris
<br>


School of Computing. Edinburgh Napier University


---

# Recommended Reading

Game Design Workshop. 4th Edition. Tracy Fullerton (2019).

- Read Chapter 3 on Formal Elements.
- Digital copies are available in the library.

![GameDesignWorkshopBook](assets/images/gdw_book.jpg)

---

# What are the Formal Elements of Games?

- Structure of the game. Without them, the game is not a game anymore.
- Fullerton defines eight elements:
    -  Players
    -  Objectives
    -  Procedures
    -  Rules
    -  Resources
    -  Conflict
    -  Boundaries
    -  Outcome
- The Formal Elements help us define games from a design perspective
- ** We can also use these elements to scope the technical features**

---

# Players

---

# Players: who plays and why?

- Games are designed for the players.
    - You must consider your game genre and target audience.
- A game should start with an invitation to play. You want to entice the player.
    - Cinemas dim their lights. <!-- .element: class="fragment" -->
    - A book uses a cover. <!-- .element: class="fragment" -->
    - A game has a title screen or introduction video. <!-- .element: class="fragment" -->


---

# Multiplayer games

- The invitation to play can become quite complicated in multiplayer games.
- Can you name unique challenges when inviting players to a multiplayer online game?
    - You need to check how many players want to join and how many are needed. <!-- .element: class="fragment" -->
    - Are all of them ready? <!-- .element: class="fragment" -->
    - Which role is assigned to which player? <!-- .element: class="fragment" -->
    - Are there any teams? <!-- .element: class="fragment" -->

---

# Player Interaction Patterns

- A **Player Interaction Pattern** helps us define how players play the games.
- There are seven key types:
    - Single-player versus the game.
    - Multiple individual players versus the game.
    - Player versus player.
    - Unilateral competition.
    - Multilateral competition.
    - Cooperative play.
    - Team competition.

---

# Single-player versus the game

- A single player competes against the game system.

![SinglePlayerVersusGame](assets/images/SinglePlayerVersusGame.png)

(Image source: T.Fullerton. Game Design Workshop. 4th Edition, 2019.)

---

# Multiple individual players versus the game

- Multiple players competes against the game system.
- They do not compete against each other and the action is not directed at other players.

![MultipleIndividualsVsGame](assets/images/MultipleIndividualsVsGame.png)

(Image source: T.Fullerton. Game Design Workshop. 4th Edition, 2019.)

---

# Player versus player

- Two players directly compete.
- Within games, the term "Player versus Player" might be used differently.

![PlayerVsPlayer](assets/images/PlayerVsPlayer.png) 

(Image source: T.Fullerton. Game Design Workshop. 4th Edition, 2019.)

---

# Unilateral competition

- Two or more players compete against one single player.

![UnilateralCompetition](assets/images/UnilateralCompetition.png)

(Image source: T.Fullerton. Game Design Workshop. 4th Edition, 2019.)

---

# Multilateral competition

- Three or more players compete against each other.

![MultilateralCompetition](assets/images/MultilateralCompetition.png)

(Image source: T.Fullerton. Game Design Workshop. 4th Edition, 2019.)

---

# Cooperative play.

- Two or more players cooperate against the game system.

![CooperativePlay](assets/images/CooperativePlay.png)

(Image source: T.Fullerton. Game Design Workshop. 4th Edition, 2019.)

---

# Team competition.

- Two or more groups compete against each other.

![TeamCompetition](assets/images/TeamCompetition.png)

(Image source: T.Fullerton. Game Design Workshop. 4th Edition, 2019.)

---

# Example: Witcher 3

- What is the Player Interaction Pattern?

![Wither3](assets/images/witcher3.jpg)<!-- .element height="60%" width="45%" -->

- Single-player versus the game <!-- .element: class="fragment" -->

---

# Example: World of Warcraft

- What is the Player Interaction Pattern?

![WorldOfWarcraft](assets/images/wow.jpg)<!-- .element height="60%" width="45%" -->

- Multiple individual players versus the game. <!-- .element: class="fragment" -->
- Player versus player. <!-- .element: class="fragment" -->
- Cooperative play. <!-- .element: class="fragment" -->
- Team competition. <!-- .element: class="fragment" -->

---

# Example: Fortnite

- What is the Player Interaction Pattern?

![Fortnite](assets/images/fortnite_win.jpg)<!-- .element height="60%" width="45%" -->

- Multilateral competition <!-- .element: class="fragment" -->
- Team competition <!-- .element: class="fragment" -->

---

# Player Roles

- You need to define what the player's task is in the game. <!-- .element: class="fragment" -->
- Some games offer different roles for the player to choose from. <!-- .element: class="fragment" -->
    - A Game Master in Dungeons and Dragons has a different role than the other players. <!-- .element: class="fragment" -->
    - One team member might be the party leader. <!-- .element: class="fragment" -->
- Different roles might have different rules associated with them. <!-- .element: class="fragment" -->

---

# Objectives

---

# Objectives: providing drive and challenge to the player

- Players are the heart of the game experience, objectives drive the experience. <!-- .element: class="fragment" -->
- Objectives provide a challenge to the player that should be achievable. <!-- .element: class="fragment" -->
- Objectives also set the overall feel of the game: <!-- .element: class="fragment" -->
    - FPS: killing and survival. <!-- .element: class="fragment" -->
    - The Sims: Manage the lives of simulated people. <!-- .element: class="fragment" -->
- Objectives may also be made up of sub-objectives. <!-- .element: class="fragment" -->
    - Side quests are similar, but normally distinct. <!-- .element: class="fragment" -->

---

# Common Objective Types

- Capture  <!-- .element: class="fragment" -->
    - Take or destroy something from the opponent.  <!-- .element: class="fragment" -->
- Chase  <!-- .element: class="fragment" -->
    - Catch an opponent or elude one.  <!-- .element: class="fragment" -->
- Race <!-- .element: class="fragment" -->
    - Reach the goal before all other players. <!-- .element: class="fragment" -->
- Alignment <!-- .element: class="fragment" -->
    - Arrange pieces in certain configurations or categories. <!-- .element: class="fragment" -->
- Rescue / escape <!-- .element: class="fragment" -->
    - Get a designated unit to safety. <!-- .element: class="fragment" -->
- Forbidden act <!-- .element: class="fragment" -->
    - Try to make other players perform a forbidden action (laughing, say a word, etc.). <!-- .element: class="fragment" -->

---

# Common Objective Types (cont.)

- Construction <!-- .element: class="fragment" -->
    - Build, maintain, and manage objects. <!-- .element: class="fragment" -->
- Exploration <!-- .element: class="fragment" -->
    - Explore different game areas. <!-- .element: class="fragment" -->
- Solution <!-- .element: class="fragment" -->
    - Solve a problem or puzzle. <!-- .element: class="fragment" -->
- Outwit <!-- .element: class="fragment" -->
    - Gain and use knowledge to defeat the other players. <!-- .element: class="fragment" -->

---

# Exercise

- What is the objective in Super Mario Bros?

![SuperMarioBros](assets/images/mario.jpg) <!-- .element height="60%" width="45%" -->

- Saving the princess?
- Reaching the end of the level?
- Jumping on enemies?

---

# Procedures

---

# Procedures: How we Interact with and Control the Game

- **Procedures** describe how we interact with the game world and how we can act within the game world.
- Video games typically define procedures via the input control mechanisms.
- Procedures can be broadly broken down into four categories:
    - Starting actions.
    - Progression of action.
    - Special actions.
    - Resolving actions.

---

# Example: Mario's Jump

- Mario's jump is one of the best examples of a 'simple' procedure <!-- .element: class="fragment" -->
- But it gives a huge amount of freedom and flexibility <!-- .element: class="fragment" -->

<iframe width="560" height="315" src="https://www.youtube.com/embed/7daTGyVZ60I" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe> <!-- .element: class="fragment" -->

---

# System Procedures

- Video games are complex systems.
- Numerous procedures are running behind the scenes to respond to player actions.
    - Compare Dungeons and Dragons to a video game RPG.
- Example: Calculating the damage based on attributes and random rolls.

![image](assets/images/dnd.jpg) <!-- .element width="40%"  -->
![image](assets/images/neverwinter.jpg) <!-- .element width="40%"  -->


---

# Rules

---

# Rules: Defining Objects and Restrict Actions

- **Rules** define the objects and restrict actions in the game.
- In chess, the rules define different objects: <!-- .element: class="fragment" -->
    - King: Can only move one tile in any direction. <!-- .element: class="fragment" -->
    - Bishop: Can only move diagonal. <!-- .element: class="fragment" -->
- In a RTS game, rules define different unit types: <!-- .element: class="fragment" -->
    - A tank might have more HP than infantry. <!-- .element: class="fragment" -->
    - The type and amount of damage might be different. <!-- .element: class="fragment" -->
- A rule can restrict what you can do in a city builder: <!-- .element: class="fragment" -->
    - You can only build a building if you have enough money. <!-- .element: class="fragment" -->
    - You can only build an airport when you reached a population of 100 000. <!-- .element: class="fragment" -->

---

# Example: Rules Defining a shotgun

- Consider a shotgun in a FPS:
    - Cost: $500, Damage: 20 <!-- .element: class="fragment" -->
    - Spread: 10, Range: 5 <!-- .element: class="fragment" -->
    - Ammo: 2, Magazine: 12 <!-- .element: class="fragment" -->
- We can also use rules to restrict certain actions for progression purposes. <!-- .element: class="fragment" -->
    - Special items in Legend of Zelda series
- Providing all capabilities at once can confuse and frustrate the player. <!-- .element: class="fragment" -->
    - Same applies with tutorials, and any form of learning

---

# Rules Determining Effects

- Rules that trigger events or effects are very useful when considering the procedures of our game.
- We can boil down such rules to a collection of `if` statements:
    - `if player’s health == 0 then player dies.`
    - `if player picks up apple then health += 10.`
    - etc.

---

# Resources

---

# Resources: providing in-game assets


A **resource** is an asset that provides the player with an advantage, allows the player to reach certain objectives, or allows the player to perform certain procedures.


- Resources are ubiquitous in games:
    - Monopoly has money and property.
    - Command & Conquer has Tiberium.
    - Halo has health, ammunition, and shields.

 ![BorderlandsLootBox](http://lootmaster.weebly.com/uploads/3/8/1/3/38139257/7052736.jpg) <!-- .element height="250px" -->
 ![UThealthPack](assets/images/uthealthpack.jpg) <!-- .element height="250px" -->

---

# Examples of Resource Types

Try to name some examples of resources.

- Lives <!-- .element: class="fragment" -->
- Units <!-- .element: class="fragment" -->
- Health <!-- .element: class="fragment" -->
- Currency <!-- .element: class="fragment" -->
- Inventory <!-- .element: class="fragment" -->
- Special terrain <!-- .element: class="fragment" -->
- Actions <!-- .element: class="fragment" -->
- Power-ups <!-- .element: class="fragment" -->
- Time <!-- .element: class="fragment" -->

---

# Conflict

---

# Conflict: Clash between Objectives, Procedures, Rules, and Resources

- **Conflict** occurs due to a clash between the objectives, the procedures, the rules, and the resources.
- A great example is golf. <!-- .element: class="fragment" -->
    - The objective is simple: put the ball in the hole ... <!-- .element: class="fragment" -->
    - ...but the hole and ball are small ... <!-- .element: class="fragment" -->
    - ...and you can only move the ball with a little stick ... <!-- .element: class="fragment" -->
    - ...and the hole is away over there! <!-- .element: class="fragment" -->
- Conflict increases the challenge, which increases the fun. <!-- .element: class="fragment" -->
- Some Conflict examples: <!-- .element: class="fragment" -->
    - Obstacles: physical or conceptual.
    - Opponents: NPCs or other players.
    - Dilemmas: putting real choice in a game.

---

# Example: Starcraft 2

- What is the conflict here?

 ![Starcraft2](assets/images/Starcraf2.png) <!-- .element height="60%" width="45%" -->

 - Obstacles: Limits on how many units you can build. <!-- .element: class="fragment" -->
 - Opponents: AI or other players. <!-- .element: class="fragment" -->
 - Dilemmas: On what unit or tech should I spend my resources? <!-- .element: class="fragment" -->

---

# Boundaries

---

# Boundaries

- **Boundaries** define where the game is taking place.
- Games exist in a world where the formal elements exist. <!-- .element: class="fragment" -->
    - Sometimes called the magic circle.
- Boundaries can be physical. <!-- .element: class="fragment" -->
    - The normal approach in video games.
    - Screen boundaries, 3D world boundaries, etc. are all examples of this.
- Boundaries can be conceptual. <!-- .element: class="fragment" -->
    - The players make an agreement to stay within the rules of the game.


---

# Outcome

---

# Outcome

- The **outcome** is the expected payoff from playing the game.
- How do we resolve the objectives that the game sets out?
- What, if any, is the end state of the game?
- Winning is a traditional end point.
    - The game state where one of the winning conditions (objectives) has been met.
- Some games continue without end points.
    - Sim City, Factorio (if you ignore the rocket), The Sims


---

# Summary


---

# Try Describing a Game Using the Formal Elements

- After the class, try and describe a game (board, video, or social) using the vocabulary of formal elements.
- Try to go into some real depth: it will help you understand games more.
- Think about the formal elements whenever you play a game. Try and pick apart the individual elements of the game to get a better understanding of how it is put together.


---

# Summary

- **Players**: Who is playing our game?
- **Objectives**: What is the goal of playing the game?
- **Procedures**: How is the game played?
- **Rules**: What are the restrictions on how the game is played?
- **Resources**: What assets are available in the game?
- **Conflict**: How do the objectives, procedures, rules, and resources constrain each other?
- **Boundaries**: Where is the game played?
- **Outcome**: How is the game resolved?
