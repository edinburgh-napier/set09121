---
title: "Lecture 7 - Game Design Document"
keywords: Lecture
tags: [Lecture]
permalink:  lecture7.html
summary: lecture7
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 7 - Game Design Document
### SET09121 - Games Engineering

<br><br>
Babis Koniaris
<br>


School of Computing. Edinburgh Napier University


---

# Recommended Reading

 Game Design Workshop. 3rd Edition. Fullerton (2014).

- Read Chapter 14 on Design Documentation.

![image](assets/images/gdw_book.jpg)


---

# What is a Game Design Document?


---

# What is the Game Design Document?

- A Game Design Document (GDD) is the template for the game that is to be created.
- The GDD is the main reference when determining how things should be done when developing the game.
- However, it is not a sacred document.
    - It is a living, constantly changing document.
    - Updates are made as the development progresses.
- It can get quite large.
    - Over 1,000 pages in some circumstances.
- Team size and document size leads to using online collaboration approaches.
    - Wiki usage common.


---

# General Content

- There is no standard format. It can be very company specific.
- Some guidelines are out there (for example Fullerton).
- Sections we will produce:
    - Overview and vision statement.
        - Broad overview of the game. Game designer's vision.
    - Audience, platform, and marketing.
        - Who is the game aimed at? What is the core demographic?
        - Marketing is generally responsibility of the marketing department.
    - Gameplay.
        - Large important section detailing how the game plays. Game designer puts together their vision for this section.


---

# General Content 2

- Sections we will produce (continued):
    - Characters.
        - What characters exist in the game? Consider both playable and non-playable characters.
    - Story.
        - What is the back story and the story told during the game?
    - World.
        - Description of the world where the game exists.



---

# General Content 3

- Sections we will produce (continued):
    - Media list.
        - Assets to be created for the game.
    - Technical specification.
        - May be a separate document.
        - Defines the technical aspects of the project.

---

# GDD Exercise

Research Design Documents! Use Google to search for game design documents. There are quite a few out there and some templates. 
You are unlikely to find modern AAA games due to the collaborative approach and NDA requirements. Also there is some argument about the utility of them these days!

---

# Contents


---

# Contents - Design History

- Design History.
    - GDD is a living document.
    - Updates to the document should be described at the beginning of the document.
    - Version history should be maintained.

 ![image](assets/images/version.png)


---

# Contents - Audience, Platform, and Marketing

- Market research must be undertaken to determine the likely return to the publisher.
    - Unit sales are the most important factor for a publisher.
    - The quality of the game comes second.
    - Catering for a small/niche market is unlikely to win over a publisher (although this depends on initial outlay).

---

# Contents - Audience, Platform, and Marketing 2

- You need to define the target audience.
    - Who will buy the game?
    - What is the core demographic?
    - How large is the audience and how likely are they to buy?
- You also need to choose the target platform(s).
    - What are the target platforms?
    - Why have these been chosen?
    - Ensure this matches with the target audience.
- You also need to define the hardware requirements.

---

# Contents - Legal Analysis

- Important that you ensure everything is covered from a legal point of view.
- Are there any legal or financial implications that come with the game production.
    - Licence agreements.
    - Copyright considerations.
    - Engine/library terms and conditions.
- Typically, Intellectual Property (IP) will be with the publisher, not the studio.
- Game engines typically require statements of use, are expensive, and may require a per unit sale cost (for big games).
- Legal & IP agreements with your Team/Contractors also needs to be considered. 

---

# Copyright Issues


- IP owners are very protective.
    - Game companies included.
- When creating your game consider:
    - Where you sourced your content from.
    - If the content is very similar to existing content.
    - If the game is very similar to others on the market.
    - If you have given due credit to any external tools, libraries, etc.
- Paying homage to something is all well and good but can get you into trouble.

![image](assets/images/sonic_anim.gif)


---

# Gameplay


---

# Contents - Gameplay

- The description of gameplay is initially one of the largest sections in the game.
- The game designer puts most of their focus into this section at the start.
- This section will focus on how the game plays.
- The Formal Elements and systems view will be useful here.
- Overview section:
    - Description of the core functionality.
    - Good practice is to base this on a physical or digital prototype.
- Gameplay description section:
    - A detailed description of how the game plays.
- Controls section:
    - User interfaces.
    - Rules (including some from Formal Elements).
    - Scoring / winning conditions.


---

# Contents - Gameplay 2


- Modes and other features.
    - Single player, multiplayer, etc.
    - Other features that may affect gameplay.
- Levels.
    - Designs for any levels for the game.
    - The more detailed the level descriptions the better.
- Flowchart.
    - Flowchart is needed to map out the gameplay.
- Editor.
    - Does the game require the development of an editor?
    - What are its features?


---

# Contents - Gameplay - Flowcharts are great

![image](assets/images/flowchart.png)


---

# Characters


---

# Contents - Character Design

- Optional part of the design document.
    - Crucial part in AAA game design.
- Character work can be a long, iterative process.
- Very important to increase engagement with the player.
    - Kids love dark-haired 30-something males (or so it is claimed).
	- Try thinking about less overused character archetypes?
- Will include concept design and description.

![image](assets/images/game_characters.jpg) <!-- .element width="40%"  -->



---

# Contents - Character Types

- Generally two types of character in a game.
- Playable characters.
    - Can have a lot of work spent on them.
    - Generally very detailed - particularly in games where you see the character all the time.
- Non-playable characters.
    - Treat these as objects in your game with properties and functionality (see 'baguette boy').
    - Includes:
        - Monsters and enemies.
        - Friends and allies.
        - Neutral characters.
    - Think about which traits you want these characters to have.
    - Think about the behaviours these characters need.
    - AI is a core concern in this section.


---

# Story


---

# Contents - Story

- Another optional section of the GDD.
- Can become very large for story driven games.
 - e.g. RPGs.
- There has to be a link between the gameplay and story.
 - Your story should unfold through the game.
- Game writers do exist and are not usually at fault for the bad stories.
 - Books are also written on game storylines.
- Topics to think about:
 - Synopsis.
 - Complete story.
 - Back story.
 - Narrative details.
 - Sub-plots.


---

# World


---

# Contents - Game World

If required, a description of the game world should be detailed.
 - Overview.
 - Key locations.
 - Travel.
 - Map.
 - Scale.
 - Physical objects.
 - Weather conditions.
 - Day and night cycle.
 - Time/era.
 - Physics.
 - Society/culture.


---

# Game World - Maps are great!

![image](assets/images/game-world.png)


---

# Contents - Media List

- You will need a list of the assets that are required by the art and sound team.
- Descriptions of these assets will need to be provided.
- You should adopt some form of naming convention to make life easier.
- Types of assets you may need to produce include:
 - User interface assets.
 - Environmental assets.
 - Character-based assets.
 - Animations.
 - Music.
 - Sound effects.


---

# Technical Specification


---

# Contents - Technical Specification

- The most important section for the technical team.
- Can sometimes be a separate document.
- Developed by the technical lead.
 - The technical lead or director heads the technical team.
- Can be quite large and detailed.
  - The more detailed the better.

---

# Contents - Technical Specification 2

- Key area is a technical analysis of the game.
 - What are the technical challenges of the project?
  - Important for costing the project.
 - Is any new technology required?
 - What are the major software development taks?
 - What are the risks involved in developing the game?
  - And how do you mitigate these risks?
 - What are the estimated resources required to deliver the game?


---

# Contents - Technical Specification 3

- Some technical requirements to consider:
- Development platform and tools.
 - e.g. Visual Studio, Unity.
- Delivery mechanism / distribution.
 - Steam / Humble / Itch / Epic / GoG
- Game engine
 - Technical specifications.
 - Design.
- Interface technical specification.
- Controls technical specification.
- Lighting models.
- Rendering system.
- Internet / network requirements.
- System parameters.
 - e.g. max players, connectivity, etc

---

# Contents - Flow Chart's are still great!

![image](assets/images/flowchart2.png)


---

# Maintenance


---

# Contents - Maintenance


- Game Design Document (GDD) is not a sacred object.
    - Living document.
    - Updates made as development progresses.
- GDD can also get quite large.
    - Thousands of pages in some circumstances.
- Game development can have large teams.
    - Over a thousand people in some circumstances.

![image](assets/images/script.jpg) 


---

# Using Wikis and Repositories

- Some are using a wiki to coordinate and communicate between team members.
 - Allows multiple users to update.
 - Allows all users to access the most up to date documentation.
 - Software development uses these often too - see wikis on GitHub.
- Repository systems can also be used.
 - Combine document maintenance with code maintenance.
 - Could also be storing assets.
- Using tools allows for a structured and managed approach to the development process.
 - Collaboration.
 - Communication.
- Use tools as necessary throughout your development process.


---

# Summary


---

# Summary

- Game Design Document is a very important piece of work to put together at the start of your development process.
 - Blueprint for your game.
 - Communication with the team.
- It is a living document, so keep it up to date.
 - Content and media lists especially.
- Use collaboration tools as much as possible.
 - Team communication.
 - Document control.