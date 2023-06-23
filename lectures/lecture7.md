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

Game Design Workshop. 4th Edition. Tracy Fullerton (2019).

- Read Chapter 14 on Game Design Documentation.
- Digital copies are available in the library.

![GameDesignWorkshopBook](assets/images/gdw_book.jpg)


---

# What is a Game Design Document?


---

# What is the Game Design Document?

- A Game Design Document (GDD) is the template for the game that is to be created.
- The GDD is the main reference when determining how things should be done when developing the game. <!-- .element: class="fragment" -->
- However, it is not a sacred document. <!-- .element: class="fragment" -->
    - It is a living, constantly changing document.
    - Updates are made as the development progresses.
- It can get quite large. <!-- .element: class="fragment" -->
    - Over 1,000 pages in some circumstances.
- Team size and document size leads to the use of online collaboration tool like a Wiki. <!-- .element: class="fragment" -->


---

# Game Design Document Overview

- Overview and vision statement.
- Audience, platform, and marketing.
- Legal Analysis.
- Gameplay.
- Characters.
- Story.
- World.
- Media list.
- Technical specification.

---

# GDD Exercise

Research Design Documents! Use Google to search for game design documents. There are quite a few out there and some templates. 
You are unlikely to find modern AAA games due to the collaborative approach and NDA requirements. 

---

# Contents


---

# Design History

- Design History.
    - GDD is a living document.
    - Updates to the document should be described at the beginning of the document.
    - Version history should be maintained.

 ![image](assets/images/version.png)


---

# Audience, Platform, and Marketing

- Market research must be undertaken to determine the likely return to the publisher.
    - Unit sales are the most important factor for a publisher.
    - The quality of the game comes second.
    - Catering for a small/niche market is unlikely to win over a publisher (although this depends on initial outlay).

---

# Audience, Platform, and Marketing (cont.)

- You need to define the target audience.
    - Who will buy the game?
    - What is the core demographic?
    - How large is the audience and how likely are they to buy?
- You also need to choose the target platform(s). <!-- .element: class="fragment" -->
    - What are the target platforms?
    - Why have these been chosen?
    - Ensure this matches with the target audience.
- You also need to define the hardware requirements. <!-- .element: class="fragment" -->

---

# Legal Analysis

- Important that you ensure everything is covered from a legal point of view.
- Are there any legal or financial implications that come with the game production. <!-- .element: class="fragment" -->
    - Licence agreements.
    - Copyright considerations.
    - Engine/library terms and conditions.
- Typically, Intellectual Property (IP) will be with the publisher, not the studio. <!-- .element: class="fragment" -->
- Game engines typically require statements of use, are expensive, and may require a per unit sale cost (for big games). <!-- .element: class="fragment" -->
- Legal & IP agreements with your Team/Contractors also needs to be considered. <!-- .element: class="fragment" -->

---

# Copyright Issues

- IP owners are very protective.
    - Game companies included.
- When creating your game consider: <!-- .element: class="fragment" -->
    - Where you sourced your content from.
    - If the content is very similar to existing content.
    - If the game is very similar to others on the market.
    - If you have given due credit to any external tools, libraries, etc.
- Paying homage to something is all well and good but can get you into trouble. <!-- .element: class="fragment" -->

![image](assets/images/sonic_anim.gif)

---

# Gameplay

- The description of gameplay is initially one of the largest sections.
- The game designer puts most of their focus into this section at the start. <!-- .element: class="fragment" -->
- This section will focus on how the game plays. <!-- .element: class="fragment" -->
- The Formal Elements and systems view will be useful here. <!-- .element: class="fragment" -->
- Overview section: <!-- .element: class="fragment" -->
    - Description of the core functionality.
    - Good practice is to base this on a physical or digital prototype.
- Gameplay description section: <!-- .element: class="fragment" -->
    - A detailed description of how the game plays.
- Controls section: <!-- .element: class="fragment" -->
    - User interfaces.
    - Rules and procedures (including some from Formal Elements).
    - Scoring / winning conditions.

---

# Gameplay (cont.)


- Modes and other features. <!-- .element: class="fragment" -->
    - Single player, multiplayer, etc.
    - Other features that may affect gameplay.
- Levels. <!-- .element: class="fragment" -->
    - Designs for any levels for the game.
    - The more detailed the level descriptions the better.
- Flowchart. <!-- .element: class="fragment" -->
    - Flowchart is needed to map out the gameplay.
- Editor. <!-- .element: class="fragment" -->
    - Does the game require the development of an editor?
    - What are its features?

---

# Flowcharts

Flowcharts can help visualise the gamplay

![image](assets/images/flowchart.png)

---

# Characters

- Optional part of the design document but often important.
- Character work can be a long, iterative process. <!-- .element: class="fragment" -->
- Very important to increase engagement with the player. <!-- .element: class="fragment" -->
- Will include concept design and description. <!-- .element: class="fragment" -->

![image](assets/images/game_characters.jpg) <!-- .element width="40%"  -->

---

# Contents - Character Types

- Playable characters. <!-- .element: class="fragment" -->
    - Can have a lot of work spent on them.
    - Generally very detailed - particularly in games where you see the character all the time.
- Non-playable characters. <!-- .element: class="fragment" -->
    - Treat these as objects in your game with properties and functionality.
    - Includes:
        - Monsters and enemies.
        - Friends and allies.
        - Neutral characters.
    - Think about which traits you want these characters to have.
    - Think about the behaviours these characters need.
    - AI is a core concern in this section.

---

# Story

- Another optional section of the GDD. <!-- .element: class="fragment" -->
- Can become very large for story driven games, like RPGs. <!-- .element: class="fragment" -->
- There has to be a link between the gameplay and story. <!-- .element: class="fragment" -->
    - Your story should unfold through the game.
- Important parts of the story: <!-- .element: class="fragment" -->
    - Synopsis.
    - Complete story.
    - Back story.
    - Narrative details.
    - Sub-plots.


---

# Game World

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

# Maps

Maps can be very helpful to get an overview of the gameworld.

![image](assets/images/game-world.png)


---

# Media List

- You will need a list of the assets that are required by the art and sound team. <!-- .element: class="fragment" -->
- Descriptions of these assets will need to be provided. <!-- .element: class="fragment" -->
- You should adopt some form of naming convention to make life easier. <!-- .element: class="fragment" -->
- Types of assets you may need to produce include: <!-- .element: class="fragment" -->
    - User interface assets.
    - Environmental assets.
    - Character-based assets.
    - Animations.
    - Music.
    - Sound effects.


---

# Technical Specification

- The most important section for the technical team.
- Can sometimes be a separate document. <!-- .element: class="fragment" -->
- Developed by the technical lead. <!-- .element: class="fragment" -->
- Can be quite large and detailed. <!-- .element: class="fragment" -->

---

# Technical Specification (cont.)

- What are the technical challenges of the project?
    - Important for costing the project.
- Is any new technology required? <!-- .element: class="fragment" -->
- What are the major software development taks? <!-- .element: class="fragment" -->
- What are the risks involved in developing the game? <!-- .element: class="fragment" -->
    - And how do you mitigate these risks?
- What are the estimated resources required to deliver the game? <!-- .element: class="fragment" -->

---

# Technical Specification (cont.)

- Development platform and tools.
    - e.g. Visual Studio, Unity.
- Delivery mechanism / distribution. <!-- .element: class="fragment" -->
    - Steam / Humble / Itch / Epic / GoG
- Game engine <!-- .element: class="fragment" -->
    - Technical specifications.
    - Design.
- Interface technical specification. <!-- .element: class="fragment" -->
- Controls technical specification. <!-- .element: class="fragment" -->
- Lighting models. <!-- .element: class="fragment" -->
- Rendering system. <!-- .element: class="fragment" -->
- Internet / network requirements. <!-- .element: class="fragment" -->
- System parameters. <!-- .element: class="fragment" -->
    - e.g. max players, connectivity, etc


---

# Maintenance

- Game Design Document (GDD) is not a carved in stone. <!-- .element: class="fragment" -->
    - Living document.
    - Updates made as development progresses.
- GDD can also get quite large. <!-- .element: class="fragment" -->
    - Thousands of pages in some circumstances.
- Game development can have large teams. <!-- .element: class="fragment" -->
    - Over a thousand people in some circumstances.

![image](assets/images/script.jpg) 


---

# Using Wikis and Repositories

- Some are using a wiki to coordinate and communicate between team members.
    - Allows multiple users to update.
    - Allows all users to access the most up to date documentation.
    - Software development uses these often too - see wikis on GitHub.
- Repository systems can also be used. <!-- .element: class="fragment" -->
    - Combine document maintenance with code maintenance.
    - Could also be storing assets.
- Tools provide a structured and managed approach to the development process. <!-- .element: class="fragment" -->
    - Collaboration.
    - Communication.


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