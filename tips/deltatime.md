---
title: "Delta Time"
keywords: Delta Time
tags: [DT]
permalink:  deltatime.html
summary: How Long has it been?
sidebar: home_sidebar
---
# Framerate Independence
The use of Delta Time is to allow time related logic to take place (i.e a countdown timer) accurately, and for movement and physics code to work properly - independent of the framerate. Delta Time is simply how long has it been since the last tick! This is often calculated as the time since the update function was last called, and normally is represented as a fraction of a second to make the calculations easier.

A problem that arises with games is that the frametime can vary, sometime not by much, sometimes drastically (such as on lower end PCs). Ideally we want a solid 16ms, but we won't always get that. A stuttering game doesn't look good graphically, but if we don't account for it within the game logic, it can actually cause major problems. (See some AAA games where the physics has been linked to the framerate.) Imagine a player moving left at a speed of 10 units a second. With an ideal framerate of 60fps. We are calling Update() 60 times per second. So we should move the player by 0.16 (10/60) units each update. 


```cpp
//This is Bad
const float playerSpeed = 10.f;
const float idealFps = 60.f;

void Update() {
 player.move(playerSpeed / idealFps);
}
```

This would work, but if the frametime suddenly drops to 30fps, or starts varying between 30 or 60, the update function will be called less, and so the player will move slower. Conversely, if the FPS skyrockets, then our player will start moving at light speed. The solutions is to **ALWAYS include Delta Time in calculations that involve time. e.g. speed, movement, and acceleration calculations.** Here is the proper way to do it:

```cpp
const float playerSpeed = 10.f;

void Update(double dt) {
  player.move(playerSpeed * dt);
}

```

Let's look at the maths here.
* When the game is running slower -- dt gets larger (as it's the time between frames). 
* When the game is running faster -- dt gets smaller.
* When the game is running at our ideal rate of 60fps -- dt = 1/60 (0.166...).
* If the game drops down to 30fps -- dt = 1/30 (0.033...)
* When the game is running slower  --  we move the player more per tick.
* When the game is running faster  --  we move the player less per tick.
* This means the final movement is the same no matter the framerate