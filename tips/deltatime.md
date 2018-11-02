---
title: "DT"
keywords: DT
tags: [DT]
permalink:  deltatime.html
summary: How Long has it been?
sidebar: home_sidebar
---
# Framerate Independence
The use of DT is to allow time related logic to take place (i.e a countdown timer) accurately, and for movement and physics code to work properly - independent of the framerate. A problem that arises with games is that the frametime can vary, sometime not by much, sometimes drastically (lower end PCs). Ideally we want a solid 16ms, but we won't always get that.
A stuttering game doesn't look good graphically, but if we don't account for it within the game-logic, it can actually cause major problems. Imagine a player moving left at a speed of 10 units a second. With an ideal frame-rate of 60fps. We are calling Update() 60 times per second. So we should move the player by 0.16 (10/60) units each update. 


```cpp
//This is Bad
const float playerSpeed = 10.f;
const float idealFps = 60.f;

void Update() {
 player.move(playerSpeed / idealFps);
}
```

This would work, but if the frametime suddenly drops to 30fps, or starts varying between 30 or 60, the update() function will be called less, and so the player will move slower. Conversely, if the fps skyrockets, then our player will start moving at light-speed. The solutions is to \textbf{Always include DT in calculations that involve time. i.e speed, movement, acceleration.} Here is the proper way to do it:

```cpp
const float playerSpeed = 10.f;

void Update(double dt) {
  player.move(playerSpeed * dt);
}

```

Let's look at the maths here.
* when the game is running slower -- DT gets larger (it's the time between frames). 
* When the game is running faster -- DT gets smaller.
* When the game is running at our ideal rate of 60fps -- DT = 60.
* If the game drops down to 30fps -- DT = 120
* When the game is running slower  --  the player moves faster.
* When the game is running faster  --  the player moves slower.