---
title: "Lecture 1 - Overview"
keywords: Lecture
tags: [Lecture]
permalink:  lecture1.html
summary: lecture1
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---

<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 1 - Overview
### SET09121 Games Engineering
### Welcome

<br><br>
Babis Koniaris
<br>
(Original material by Kevin Chalmers/Sam Serrels/Thomas Methven)

School of Computing. Edinburgh Napier University

---

# What is Games Engineering
![Wikipedia for Engineering](assets/images/engineering.png)


* Engineering is design, building and use of engines. <!-- .element: class="fragment" -->
* Games Engineering is the application of this to video games - we are going to design, build and use an engine to make a game. <!-- .element: class="fragment" -->
* Many technical members of a games development team are called engineers. <!-- .element: class="fragment" -->
* We are using the same analogies that are applied to software engineers. <!-- .element: class="fragment" -->
* This module will focus on the development of a game engine <!-- .element: class="fragment" -->
 * Entity management, physics system, AI, other management functionality. <!-- .element: class="fragment" -->

---

# Module Aims


* Gain experience in games development, particularly the technical aspects of games development. <!-- .element: class="fragment" -->


* Examine the technical aspects of games and break these down to understand how games work. <!-- .element: class="fragment" -->


* Put together a basic game engine for 2D games. <!-- .element: class="fragment" -->


* Examine some of the fundamental technologies used in games development, such as artificial intelligence and physics. <!-- .element: class="fragment" -->

---

# Why Not Just Use Unity/Unreal?


* Because some people need to make Unity/Unreal! <!-- .element: class="fragment" -->


* No, really. Engine teams are still a huge employment area <!-- .element: class="fragment" -->


* Unity has an office in Edinburgh now! <!-- .element: class="fragment" -->


* It's important for everyone to understand how engines work <!-- .element: class="fragment" -->


* It will help you even if you use a pre-made engine <!-- .element: class="fragment" -->

---

# Learning Outcomes


- LO1: **Design, develop and evaluate** a games engine through *robust software engineering techniques* 

- LO2: **Examine and evaluate** modern software *development, deployment, and testing procedures*.

- LO3: **Examine game design principles and procedures** 

- LO4: **Demonstrate a working knowledge** of the *technical components of a games engine* 


---

## Teaching and Assessment


---

# Structure – Lectures

- Two one-hour lectures a week.  <!-- .element: class="fragment" -->
    - Friday @ 9am-11am in G21  
- Lectures will cover:  <!-- .element: class="fragment" -->
    - Game design concepts. 
    - Software engineering for games.  
    - Game technology.
    - How our game engine works.


---

# Structure – Practical Sessions

- Two-hour practical session. <!-- .element: class="fragment" -->
    - Friday @ 12pm (after the lectures)
- Practicals take place in D02. Bring your laptop or use the lab PCs. <!-- .element: class="fragment" -->
- Labs will help you develop the engine you need to use for the coursework. <!-- .element: class="fragment" -->
    - 2D development with SFML.
    - Physics with Box2D.
    - Management infrastructure for the engine.
- You need to do these practicals and keep up – they form the basis of the coursework submission. <!-- .element: class="fragment" -->


---

# Coursework

- Out Next Week (hopefully)!

- Develop a game using the game engine.
    - **You cannot use one of our samples and just change the graphics**, or likewise with samples for SFML, Box2D, etc. – this will be a **FAIL**!

- You will work with a partner, think who you might want to work with.

- Coursework has four submissions.
    - [Week ~4:] Game pitch presentation.
    - [Week ~7:] Game design document.
    - [Week ~13:] Implementation and report.
    - [Week ~13:] Demo and peer assessment.


---

# Support and Assessment

| **Learning Outcome** | **Supported By** | **Assessed In** |
| --- | --- |--- |
| LO1: Design, develop and evaluate a games engine through robust software engineering techniques. | Lectures, Practicals | Coursework |
| LO2: Examine and evaluate modern software development, deployment, and testing procedures. | Lectures, Practicals | Coursework |
| LO3: Examine game design principles and procedures. | Lectures| Coursework |
| LO4: Demonstrate a working knowledge of the technical components of a games engine. | Practicals | Coursework |


---

# Work Plan

- We know every lecturer says it, but if you took Programming Fundamentals, Computer Graphics, or Physics, you will know we mean it.
- You __**WILL**__ have to work around 14 hours per week on the module.
    - 4 hours of contact (lectures and practicals).
    - 10 hours of self-study.
- The coursework requires working in pairs which will require some organisation outside class time.
    - Use Discord/Slack/Whatsapp to communicate (make sure you book regular meetings).
- Keep up with the practical work!  There is one every week.
    - The practicals build the engine you need.  Falling behind one week means trying to catch up the next!


---

# Video

- This is an example of a previous project!
<br><br>
<iframe width="560" height="315" src="https://www.youtube.com/embed/3_m0fZHJiI8" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


--


## Module Outline


---

# Focus of the Module

- **Game design:** Initially we will focus on basic design ideas and show how we can use these to engineer our game.
- **Software modelling:** Applying common modelling techniques to games development.
- **Game mechanics:** Developing the underlying rules and procedure for our game (AI, physics, etc.).
- **Software development:** Programming our design, model, and mechanics to produce a working game.
- **Playing games:** Yes, you will have to play games.  However, you will start to analyse games more closely and understand more about what is happening within them.


---

# Not the Focus of the Module

- **Introduction to C++ programming:** If you do not have experience with C++ then get a book to learn the basics quickly.  We will cover some concepts, but not the basics.
- **Object-orientation:** We will refresh the basics in the context of C++ and games development, but if you require more knowledge then you are recommended to get a book.  You will likely learn more about object-orientation in this module by doing the work provided.
- **Graphics and sound creation:** Although some websites will be pointed to, you are generally expected to find or produce your own content. 


---

# Lecture Series – Part 1

- Overview.
- Workflow and repository management.
- Formal elements of games.
- Systems dynamics.
- Game entities and entity management.
- Object-orientation in C++.
- Game design documents.
- Design patterns for games.
- Memory and resource management.
- Game engine architecture and operation.
- 2D physics overview.


---

# Lecture Series – Part 2

- AI in games.
- Pathfinding.
- Steering behaviours.
- State machines.
- Decision trees.
- QA and TRC.
- Game testing.
- Performance testing.
- Performance optimisation.
- Basic networking principles.
- Scripting.
- Review.


---

# Practical Lab Series

- Git Workflow and CMake.
- Introduction to SFML.  **Pong**.
- Entity Management.  **Space Invaders**.
- Tile Engine 1.  **Maze game**.
- Tile Engine 2.  **PacMan**.
- Physics and Resource Management.
- AI: Steering Behaviours and Pathfinding.
- AI: Behaviour via State Machines and Decision Trees.
- Deployment and Testing.
- Performance Optimisation.
- Networking.
- Scripting.


---

## Recommended Reading

</textarea>
</section>

<section>
  <h1>Game Design Workshop</h1>
  <div style="float: left; width:60%">
    <p>Game Design Workshop: A Playcentric Approach to Creating Innovative Games.  Tracy Fullerton.</p>
    <p>Game design elements of the module come from here.</p>
    <p>This book will make you look differently at games. (See Reading list)</p>
  </div>
  <div>
    <img src="assets/images/gdw_book.jpg" alt="book" style="float: right; width:30%">
  </div>
</section>

<section>
  <h1>C++ Primer</h1>
  <div style="float: left; width:60%">
    <p>C++ Primer.  Lippman, Lajoie, and Moo.</p>
    <p>If your C++ needs some work this is a reasonable book to pick-up.</p>
    <p>Covers most aspects of C++ that you will require. (See Reading list)</p>
  </div>
  <div>
    <img src="assets/images/cpp_primer_book.jpg" alt="book" style="float: right;  width:30%">
  </div>
</section>

<section>
  <h1>Artificial Intelligence for Games</h1>
  <div style="float: left; width:60%">
    <p>Artificial Intelligence for Games. Millington and Funge.</p>
    <p>Very good book covering the algorithms and techniques used within game AI.</p>
    <p>Lots more in here than we cover in the module. (See Reading list)</p>
  </div>
  <div>
    <img src="assets/images/ai_book.jpg" alt="book" style="float: right;  width:30%">
  </div>
</section>

<section>
  <h1>Beginning Math and Physics for Game Programmers</h1>
  <div style="float: left; width:60%">
    <p>Beginning Math and Physics for Game Programmers.  Wendy Stahler.</p>
    <p>A basic book really, but if your maths is not great this should give you enough to solve common problems.</p>
    <p>Anyone with a reasonable grasp of "games" maths should instead look towards... (See Reading list)</p>
  </div>
  <div>
    <img src="assets/images/basic_math_book.jpg" alt="book" style="float: right;  width:30%">
  </div>
</section>

<section>
  <h1>Mathematics for 3D Game Programming and Computer Graphics</h1>
  <div style="float: left; width:60%">
    <p>Mathematics for 3D Game Programming and Computer Graphics. Eric Lengyel.</p>
    <p>Has a number of good bits of information and techniques. Easy enough to translate 3D to 2D.</p>
    <p>If you want to get better at "games" mathematics I recommend this book. (Don't currently have a digital copy)</p>
  </div>
  <div>
    <img src="assets/images/hard_math_book.jpg" alt="book" style="float: right;  width:30%">
  </div>
</section>

<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>
# Useful Websites

- **SFML:** https://www.sfml-dev.org

     SFML provides our graphics system for our game engine.  You will need to use this resource when working with SFML.

- **Box2D:** http://box2d.org

     Box2D provides the physics system for our game engine.  Again, you will need to use this resource when working with Box2D.

- **C++:** http://www.learncpp.com

     Modern C++ tutorials. Unofficial reference at http://www.cppreference.com

- **Game Programming Patterns:** http://gameprogrammingpatterns.com

     Want to know more about the patterns we use and other patterns useful for games?  This website provides a number of examples.



---


# Contacting the Module Team

### Contact details:

MY  EMAIL 

**b.koniaris@napier.ac.uk**


- The best time to ask for help is during the practical sessions.
- Try and complete the practical before the class and then ask questions during the lab time.
- In other words: make sure you work on this outside the assigned time!


---

# And finally...
		
- I hope you have fun during the module.
- The module has been designed to be challenging but with effort everyone can succeed.
- We brought this back as we thought it was fundamental for the Games Development students.
- But for everyone else we hope you will learn:
    - How games work and how you can put them together.
    - How to do software engineering properly (from our point of view!)

		

</textarea>
</section>