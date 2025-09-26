---
title: "Lecture13"
keywords: Lecture
tags: [Lecture]
permalink:  lecture13.html
summary: lecture13
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---

<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 13 - Steering Behaviors
### SET09121 - Games Engineering

<br><br>
Leni Le Goff
<br>


School of Computing. Edinburgh Napier University


---

# Recommended Reading


- Artificial Intelligence for Games. Second Edition. Millington and Funge (2009).
- Whole chapter on steering behaviours.

![image](assets/images/ai_book.jpg)<!-- .element width="30%" -->


---

## Review - Background Knowledge


---

# Review - AI Techniques

- There are numerous usable AI techniques applicable to games development.
    - Classical, deterministic techniques - popular.
    - Academic, non-deterministic techniques - useful in some areas.
- Different techniques accomplish different aspects of game behaviour.
    - Movement.
    - Decision making.
    - Strategy.
    - Learning.
- Today we will look at the basics of movement via steering behaviours.


---

# Review - Working with Vectors

- We have dealt with vectors for a long time now.
    - Hopefully you understand them!
- Steering behaviours rely on vector operations.
    - We are generally trying to work out positions and velocity to move entities in a certain manner.
- We will be performing numerous vector operations to support our steering behaviours.
    - Adding and subtracting vectors.
    - Getting the length of a vector.
    - Normalizing a vector.
    - Converting vectors to angles.


---

# Review - Basic Physics/Movement

- Steering behaviours work with our physics engine.
- Steering behaviours output a direction of travel.
    - And a rotation if you want to use it.
- We use this output to influence our entities.
    - We can set the velocity directly.
    - We can apply the output as a force.
- Remember:
    - Our physics engine is concerned with object movement.
    - Our steering behaviours are also concerned with object movement.
    - Therefore, combining the two is a good idea.


---

## Steering Behaviours


---

# What are Steering Behaviours?

- Steering behaviours are an AI technique that lets us program basic movement.
    - Movement is often considered the base ability of a game AI.
- Steering behaviours are actually very simple.
    - They work on basic object positioning and rotation.
- They provide an output which tells a game character which way to move.
    - This can be considered the velocity of an entity.
- There are numerous examples (see the recommended reading):
    - Seek
    - Flee
    - Arrive
    - Avoid obstacle
    - etc.


---

# Example - Flocking



---

# Example - Flocking

<iframe width="1400" height="800" src="https://www.youtube.com/embed/QbUPfMXXQIY" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# Example - Game

<iframe width="1400" height="800" src="https://www.youtube.com/embed/J2hI_eGGmzg" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


---

# Steering Behaviours

- There are many steering behaviours out there.
    - Refer to the AI book for some of the most useful.
- You can even define your own if you like.
- We will only look at four:
 - **Seek** : move towards a target.
 - **Flee** : run away from a target.
 - **Arrive** :   move towards a target and stop within a certain range.
 - **Face** : face the target.


---

# Seek


- Very simple idea.
- Move towards a target.
- Calculation: 

$$ d = target - position $$

$$v = \frac{d}{||{d}||} \times v_{max}$$

![image](assets/images/seek.png)


---

# Flee

- Also simple - effectively the inverse of seek.
- Run away from a target.
- Calculation: 

$$d = position - target$$

$$v = \frac{d}{||{d}||} \times v_{max}$$

![image](assets/images/flee.png)


---

# Arrive

- Seek, but with a stopping distance to stop the wiggle.
- Move towards target and stop when within a given distance.
- Calculation: 

$$d = target - position $$

$$ \left\lVert d \right\rVert \leq radius \implies v = 0 $$

$$ \left\lVert d \right\rVert > radius \implies v = \frac{d}{||{d}||} \times v_{max}$$



![image](assets/images/arrive.png)


---

# Face

- A rotational steer.
- Turn to face a target.
- Calculation (simplified - there are more checks to do):

$$d = target - position $$

$$\theta = \arctan_2(d_y, d_x) $$

$$r = (\theta - orientation) * rot\_{speed}$$

![image](assets/images/face.png)


---

# Steering Behaviours in Our Engine

- We want to build a reusable technique for steering behaviours.
    - We want reusable so we can program as many steering behaviours as we like.
- If you like you can go further and combine steering behaviours within a single steering behaviour.
    - See weighted/combined behaviours in the recommended reading.
    - Read this article [Steering Behaviors For Autonomous Characters by C.W. Reynolds](https://www.red3d.com/cwr/steer/gdc99/)




---


# Steering Output struct

- `SteeringOutput` contains two values.
    - `direction`:   the vector we want to travel in.
    - `rotation`:   the angle we want to turn.
- We will not use rotation in the practical, but it is there if you need it.

```cpp
struct SteeringOutput
{
    sf::Vector2f direction;
    float rotation;
};
```

---

# Steering Behaviour Implementation

- There are different ways to implement steering behaviours.
- OO approach:
    - Define a `SteeringBehaviour` abstract base class 
    - With one pure virtual method: `get_steering`
    - `get_steering` performs the necessary calculation for the defined steering behaviour and outputs a `steering_output`.

```cpp
class SteeringBehaviour{
public:
   virtual SteeringOutput get_steering() = 0;
};
```

Then, one class per type of steering behaviour.

---

# Steering Behaviour Implementation

The problem with the OO approach:
- A lot of repeated code
- Have to define a new class just to implement one function
- Classes should be used when data needs to be carried with the procedure. 

![image](assets/images/seek_class.png)
![image](assets/images/flee_class.png)


---

# Steering Behaviour Implementation

Functional approach using std::function.

- The only information needed for a steering behaviour is the target and agent positions. 
- So we can define a new type of function:
```cpp
using SteeringFunction = 
std::function<SteeringOutput(const sf::Vector2f &,const sf::Vector2f &)>;
```
- Then define the steering behaviours as static variable:
```cpp
struct SteeringBehaviours{
    static SteeringFunction seek;
    static SteeringFunction flee;
};
```

Then, we just need to implement the functions.

This is similar to the *strategy design pattern*.

---

# Steering Behaviour Implementation

Seek

```cpp
SteeringFunction SteeringBehaviours::seek = 
[](const sf::Vector2f &target,const sf::Vector2f &self) -> SteeringOutput{
    SteeringOutput steering;
    steering.direction = normalize(target - self);
    steering.rotation = 0.0f;
    return steering;
};
```

Flee


```cpp
SteeringFunction SteeringBehaviours::seek = 
[](const sf::Vector2f &target,const sf::Vector2f &self) -> SteeringOutput{
    SteeringOutput steering;
    steering.direction = normalize(self - target);
    steering.rotation = 0.0f;
    return steering;
};
```

---

# Steering Behaviour Implementation

Then, we can use them in a steering component for instance:
```cpp
SteeringOutput output = 
SteeringBehaviours::seek(_player->get_position(),_parent->get_position());

move(output.direction*_max_speed * dt);
```

Where, 
- `_player` is a pointer to the player entity 
- `_parent` is a pointer to the parent entity of this component
- `move` is a function to move the parent enity.

---

# Combining Steering, Decisions, and State

- Next we are going to discuss decision making and behaviour control using state machines and decision trees.
- We will be looking at combining these ideas to create a sophisticated looking AI.
    - We will look at this in more detail next week.
- The idea we will look at is when we make a decision (via a decision tree) we will change state.
    - For example, if we decide we are under attack we change our state to engage.
- We can consider that the behavioural states also contain a steering behaviour if necessary.
    - For example having a seek state.


---

# Example - The Sophisticated Guard

- The guard has some basic actions:
    - The guard patrols between point A and point B.
    - The guard has a 20% chance of stopping while patrolling.
    - If the guard is shot at, the guard will stop patrolling, engage the player, and fire back.
    - If the guard sees the player, the guard will engage the player.
    - If engaged and the player is far away, the guard will seek the player.
    - If health is low, the guard will flee from the player.
    - If the guard loses sight of the player, the guard will return to patrolling between point A and point B.


---

# Example - The Sophisticated Guard Diagram
![image](assets/images/sophisticated_guard.png)


---

# Combining Steering Behaviours

- We can also combine steering behaviours to create more elaborate movement.
    - This is how flocking works.
- Remember that we can add vectors together quite happily.
    - This will give us a mean direction of travel.
- We can combine steering behaviours normally.
    - For example combined seek and face.
- Or we can weight the steering behaviours.
    - 0.8 seek.
    - 0.1 align.
    - 0.1 obstacle avoidance.


---

# Comments on Steering

- Steering behaviours are very simple.
    - They are also very fast to calculate.
- They can also be very powerful.
    - Combining steering behaviours can lead to rich, complicated movement.
- They also underpin the basis of many AI techniques.
    - Path finding uses a path following steering behaviour.
    - State machines and decision trees can determine which steering behaviour to perform.
- Steering behaviours by themselves can lead to weird behaviour.
    - Remember some of the path finding examples.


---

# Summary

- As always, we have only really scratched the surface of steering behaviours.
    - There are numerous other behaviours out there.
- Basic steering is good, but quite simple.
- We normally want to combine behaviours.
    - Weighted.
    - Flocking.
- Consider what behaviour you want, and just program the movement.
    - Do not worry about complexities.
