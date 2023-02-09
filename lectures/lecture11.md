---
title: "Lecture 11 - Physics"
keywords: Lecture
tags: [Lecture]
permalink:  lecture11.html
summary: lecture1
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>
{% raw  %}
# Lecture 11 - 2D Physics
### SET09121 - Games Engineering

<br><br>
Babis Koniaris
<br>


School of Computing. Edinburgh Napier University


---

# Recommended Reading


- Game Physics Engine Development, Millington.
- If you ever want to build your own physics engine this is the book.
- It does introduce some of the physics concepts well.
- Unless you need to know this information, it isn't necessary.

![image](assets/images/physics_book.jpg) <!-- .element width="30%" -->


---

# What do we mean by game physics?

<iframe width="1400" height="800" src="https://www.youtube.com/embed/xh6bhBAO7vQ" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# What do we mean by game physics?

<iframe width="1400" height="800" src="https://www.youtube.com/embed/tugbGpRqiFY" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# Game Physics

- Showing a couple of examples is fine, but what is game physics and how do we use it?
- Game physics is really only a small subset of academic physics - it is not theoretical.
- Game physics uses classical mechanics to provide the basic movement of objects in the game world.
    - We will look at the Laws of Motion for example.
- Other physics models are used in some areas of games.
    - Fluid dynamics for example.
- Most advanced rendering techniques rely on physics concepts.
    - Optics, etc.


---

# Physics Engine



- Game physics is almost always provided by a third-party engine.
- It used to be that a game studio required someone with a Masters or PhD in Physics.
- We will use Box2D as our middleware.
- There are a number of industry used, free, physics engines out there.

![image](assets/images/box2d.png) <!-- .element width="30%" -->
![image](assets/images/havok.png) <!-- .element width="30%" -->
![image](assets/images/euphoria.jpg) <!-- .element width="30%" -->


---

# Collision Detection

- Collision detection is not strictly part of a physics engine.
- The physics engine concerns itself with resolving collisions.
    - So we need to be able to detect them in the first place.
- There are numerous techniques to detect collisions in 2D and 3D - from fast and course-grained to slow and fine-grained.

![image](assets/images/collision-detection.png) <!-- .element width="50%" -->


---

# Particle Simulation


- Particle simulation forms the basis of many physics engines.
- Particles are simply simulated elements that we can apply the Laws of Motion to.
    - The have a position, velocity, acceleration, etc.
- Particles are use for numerous graphical effects.
    - For example, smoke, fire, explosions, water, etc.

![image](assets/images/particle.jpg)<!-- .element width="60%" -->


---

# Rigid Body Dynamics


- Rigid bodies is about how geometric objects move and interact.
- Unlike particles, rigid bodies have a shape. This means they not only move in space, but can also rotate.
- The bodies are rigid as they do not change shape.

![image](assets/images/rigid-body.jpg)


---

# Bringing them Together

- Collision detection (intersection testing) lets us find our which bodies have come into contact.
- Particle physics allow us to control the motion of objects without taking into account their size and shape.
- Rigid body dynamics let us model how geometric objects interact.
- Rigid body dynamics also let us determine what happens when two objects collide: **Collision resolution**
- Box2D provides these core features (and a bit more) so we can do almost any type of 2D physical effect you can think of.


---

# Example - Collision Detection
<iframe width="1400" height="800" src="https://www.youtube.com/embed/qTV3ZQgTnkg" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# Example - Particle Simulation
<iframe width="1400" height="800" src="https://www.youtube.com/embed/YeNeod0qfPY" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# Example - Rigid Bodies
<iframe width="1400" height="800" src="https://www.youtube.com/embed/LnvtZn2agmA" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# Example - Water Simulation
<iframe width="1400" height="800" src="https://www.youtube.com/embed/zMTzWLGcPEk" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

## Fundamentals - Laws of Motion


---

# What are the Laws of Motion?

- Game physics are underpinned by Newton's Three Laws of Motion.
    - First described by Isaac Newton in the 17th century.
- Newton's laws are:
    1.  An object in motion stays in motion unless an external force is applied to it.
    2.  A force applied to an object causes an acceleration in that direction, multiplied by the inverse mass of the object.
    3.  For any action there is an equal, but opposite reaction.
- There are also Euler's Two Laws of Rigid Body Motion which we won't discuss here.


---

# Newton's First Law of Motion


- An object in motion stays in motion unless a force is applied to it.
- Basically, if there is no force there is no change in acceleration or velocity.

- If $F_{net} = 0$ then there is no **change** in motion.
- Where:
    - $F_{net}$ is the combined force applied to the object.
    - Opposite forces can cancel each other out.


---

# Newton's Second Law of Motion


- A force applied to an object causes an acceleration in that direction multiplied by the inverse mass of the object.

- This is an important calculation, and normally underpins most of the force calculation work in a physics engine.

`$$ F = ma $$` Where: $m$ is the mass, $a$ is the acceleration.

Or: `$$a = \frac{F}{m}$$`


---

# Newton's Third Law of Motion


- For any action, there is an equal but opposite reaction.
- The law comes into play when working with collision resolution.
- A similar looking force is the normal force which cancels out the force of gravity on a resting object.

![image](assets/images/normal-force.png)


---

# Equations of Motion


<div style="float: left;width: 40%;" > 

`$$ v = u + at $$`<br>
`$$ s = \frac{1}{2}(u + v)t $$`<br>
`$$ s = ut + \frac{1}{2}at^2 $$`<br>
`$$ s = vt - \frac{1}{2}at^2 $$`<br>
`$$ v^2 = u^2 + 2as $$`<br>
`$$ a = \frac{v - u}{t} $$`<br>

</div>


<div style="float: right;width: 60%;text-align: left;" > 
\\\(s\\) : object displacement<br>
\\\(u\\) : the initial velocity<br>
\\\(v\\) : the final velocity<br>
\\\(a\\) : the acceleration<br>
\\\(t\\) :  (or \\(\Delta t\\)) : the time passed 
</div>


---

# SUVAT

![image](assets/images/suvat.jpg) <!-- .element width="100%"  -->


---

# Simple Gravity

- We will define some basic values and principles that are useful when considering motion.
- The first value we shall define is gravity, $g$.
- On Earth, $g$ is a downward force applied to an object.
- $g$ at sea level is equal to $9.82m/s$.
    - As a 2D vector this is $<0, -9.82>$.
- This value for $g$ is commonly low in a 2D game world (as pixels are do not represent metres) so you will probably want to increase it.


---

# Weight and Mass

- We use the terms weight and mass interchangeably in everyday language.
- In physics, weight and mass are different.
- Weight is the downward force applied to an object due to gravity and the object's mass.
- Mass is the measure of how much matter is in an object. 
- We use $kg$ for mass. Less gravity means less weight, but the mass will remain the same.

$w = mg$ 

Where: $w$ is weight, $m$ is mass, $g$ is gravity.


---

# Forces


- Weight is a force. A force is considered to be any influence that can affect the velocity of an object.
- As we saw with $g$, a force is defined as a vector, having a direction and magnitude.
- A Newton is a standard unit of force applied to an object.
- Many physic engines will try and deal in Newtons to ensure calculations are uniform.

$$1N = 1kg \times m/s^2$$
On Earth: $$g = 9.8 m/s^2$$ so: $$1N = 0.102kg $$ $$ 1kg = 9.8N $$


---

# Adding Forces


- When applying forces, we are typically concerned with accumulated force for a particular frame.
- Adding forces is just a case of adding the vector forces together and applying the resultant net force to the object.

![image](assets/images/adding-forces.png)


---

# Force Examples

- Springs are commonly used for a number of effects - they do exactly what you think.
- In games, springs are used for deformable shapes and balls.

Hook's Law: $$F = -k\Delta l$$ where $k$ is the stiffness of the spring and $l$ the length.

- Drag is another force that is caused by air resistance.
- Games will use a simplified model of drag, such as shown.

Simplified drag:
$$F_{drag} = \hat{\textbf{v}}(k_1\lVert\textbf{v}\rVert + k_2\lVert\textbf{v}\rVert^2)$$


---

# Impulses

- Forces are a simple way of managing object movement.
    - A force is applied to the object.
    - The force affects the object's acceleration.
    - The acceleration affects the velocity of the object.
- Sometimes we want to modify velocity directly, for example in collision resolution.
    - The amount of force applied after collision may not be enough to move the object.
- Therefore we use impulses to calculate direct changes in velocity.

Impulses are Cheat forces. We implement them by directly modifying velocity. You can't do this in real life.


---

# Impulses over Forces

- The effect of a force is gradual, particularly in the time frames we are dealing with.
    - Force applied to an object.
    - Force divided by object's mass is added to object's acceleration.
    - Acceleration is then multiplied by time (typically a fraction of a second) and is added to the velocity of the object.
    - The new velocity, multiplied by time, is used to move the object.
- Impulse forces are far more sudden.
    - Impulse is calculated.
    - Impulse divided by mass is added to the velocity.
    - Use the new velocity multiplied by time to move the object.


---

# Impulse Example

- Normally we would calculate a force as follows: $$ F_{net} \mathrel{{+}{=}} F $$
- For an impulse, the change is far more sudden, and we just add a value directly to the velocity. $$ p.v \mathrel{{+}{=}} I $$
- Particle uses new velocity at next update.

---

# Summary

**Warning**

- Physics effects look good in your game, provide nicer looking movement, and can be used for gameplay. However...
- Physics calculations can be expensive.
    - They also don't always scale well also due to the object interactions.
- Collision detection is also expensive.
- Be smart! Don't have lots of physical effects on the screen at one time - this can really hit performance!

---

**Warning Pt 2**

- Physics engines are not always the best way to make your game *fun*.
- This is particularly true if your engine is not deterministic. <!-- .element: class="fragment" -->
- Think about games like Sonic the Hedgehog: <!-- .element: class="fragment" -->
 - Movement should feel good <!-- .element: class="fragment" -->
 - Movement should be repeatable <!-- .element: class="fragment" -->
 - Sonic isn't controlled by a physics engine <!-- .element: class="fragment" -->
 - (But gravity is applied!) <!-- .element: class="fragment" -->

---

# Summary

- We have taken a very broad overview of what we mean by game physics.
    - Laws of Motion.
    - Particles.
    - Rigid bodies.
    - Collisions.
- Box2D will provide us with all these features and more - you just need to explore it.
- The physics lab will introduce most of these ideas practically.

{% endraw %}