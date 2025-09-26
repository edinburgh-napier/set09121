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
Leni Le Goff
<br>


School of Computing. Edinburgh Napier University


---

# Recommended Reading (Optional)


- Game Physics Engine Development, Millington.
- If you ever want to build your own physics engine this is the book.
- It does introduce some of the physics concepts well.
- Not required for this module.

![image](assets/images/physics_book.jpg) <!-- .element width="30%" -->


---

# What do we mean by game physics?

<iframe width="1400" height="800" src="https://www.youtube.com/embed/xh6bhBAO7vQ" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# What do we mean by game physics?

<iframe width="1400" height="800" src="https://www.youtube.com/embed/tugbGpRqiFY" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# Game Physics

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
- There are numerous techniques to detect collisions in 2D and 3D - from fast and more approximate to slow and more accurate.

![image](assets/images/collision-detection.png) <!-- .element width="50%" -->


---

# Particle Simulation


- Particle simulation forms the basis of many physics engines.
- Particles are simply simulated elements (points) that we can apply the Laws of Motion to.
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

- Collision detection (intersection testing) lets us find out which bodies have come into contact.
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
    1.  Law of inertia
    2.  Force produces motion ($F = ma$)
    3.  Law of action and reaction
- There are also Euler's Two Laws of Rigid Body Motion which we won't discuss here.


---

# Newton's First Law of Motion


- Law of inertia:
    - A body remains at rest, or in motion at a constant speed in a straight line, unless acted upon by a force.
- Basically, if there is no force there is no change in velocity.

- If $F_{net} = 0$ then there is no **change** in motion.
- Where:
    - $F_{net}$ is the combined force applied to the object.
    - Opposite forces can cancel each other out.


---

# Newton's Second Law of Motion


- Force produces motion
    - When a body is acted upon by a force, it accelerates proportionally to its mass and the force applied, towards the direction of the force

- This is an important calculation, and normally underpins most of the force calculation work in a physics engine.

`$$ F = ma $$`

where: $m$ is the mass, $a$ is the acceleration.

Or:

`$$a = \frac{F}{m}$$`


---

# Newton's Third Law of Motion

- Law of action and reaction
    - If two bodies exert forces on each other, these forces have the same magnitude but opposite directions.
- The law comes into play when working with collision resolution.

![image](assets/images/normal-force.png)


---

# Equations of Motion

- These equations apply when `$$a$$` is constant.

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

- On a planet, gravity is a downward force applied to an object.
- Gravitational acceleration $g$ at sea level is equal to $9.82m/s^2$.
    - As a 2D vector this is $<0, -9.82>$.


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
- Like acceleration $g$, a force is also defined as a vector, having a direction and magnitude.
- A Newton is a standard unit of force applied to an object.

$$1N = 1kg \times m/s^2$$
On Earth: $$g = 9.8 m/s^2$$ so: $$\frac{1N}{g} = 0.102kg $$ $$ 1kg \times g = 9.8N $$


---

# Adding Forces


- When applying forces, we are typically concerned with accumulated force for a particular frame.
- Adding forces is just a case of adding the vector forces together and applying the resultant net force to the object.

![image](assets/images/adding-forces.png)


---

# Force Examples

- Springs are commonly used for a number of effects - they do exactly what you think.
- In games, springs are used for deformable shapes and balls.

Hooke's Law: $$F = -k\Delta s$$ where $k$ is the stiffness of the spring and $s$ the displacement from the resting length.

- Drag is another force that is caused by air resistance (or any other medium, e.g. liquid)
- Games will use a simplified model of drag:

$$F_{drag} = -\hat{\textbf{v}}(k_1\lVert\textbf{v}\rVert + k_2\lVert\textbf{v}\rVert^2)$$


---

# Impulses

- Impulses are changes in an object's momentum
    - "Force acting over time"
    - Momentum: $p = mv$
- We can use them instead of forces to handle collision resolution.
- This allows us to transfer momentum instantenously without dealing with infinit forces acting over an infinte small amount of time.

---

# Impulses over Forces

- The effect of a force is gradual, particularly in the time frames we are dealing with.
    - Force applied to an object.
    - Force divided by object's mass is added to object's acceleration.
    - Acceleration is then multiplied by time (typically a fraction of a second) and is added to the velocity of the object.
    - The new velocity, multiplied by time, is used to move the object.
- Exchange of impulses are instanteneous.
    - Impulse is calculated.
    - Impulse divided by mass is added to the velocity.
    - Use the new velocity multiplied by time to move the object.

---

# Summary

**Warning**

- Physics effects look good in your game, provide nicer looking movement, and can be used for gameplay. However...
- Physics calculations can be expensive.
    - They also don't always scale well due to the potential number of object interactions.
- Collision detection is also expensive.
- Be smart! Don't have lots of physical effects on the screen at one time - this can really hit performance!

---

**Warning (cont.)**

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