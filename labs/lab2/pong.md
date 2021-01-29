---
title: "Pong 1 - Game Programming Fundamentals"
keywords: pong
tags: [pong]
permalink:  pong.html
summary: Let's start at the beginning
sidebar: home_sidebar
---

For your first practical, we will be going back to the most basic of basic. Creating the classic arcade game PONG. For this we will be chucking some of the software practices you may be used to out the window. We will be using one single main.cpp file, no classes, no OO at all. You should be able to have a working pong game, with AI, in under 200 lines of code. 

<video class="middle" width="400" height="300" loop autoplay>
  <source src="assets/videos/pong.mp4" type="video/mp4">
</video>


The purpose of this exercise is to get you acquainted with SFML. We will come back to the exercise often as an example of simple software design, as it useful as a base to compare more complex approaches.

Before we get stuck in, let's cover some of the fundamentals.

{:class="important"}
**You don't need to do any code in this page, but it is vital you understand the concepts here before you move on!**

## The Game Loop

The fundamental core of all games, is the game loop. While some engines may hide this away, behind the scenes you can be sure that the code for any game can be stripped away to the fundamental game loop. It looks like this.
```Cpp
void Update(double dt) {
	// Update Everything
}

void Render() {
	// Draw Everything
}

int main () {
	//initialise and load
	while(!shouldQuit){
		//Caluclate DT
		Update(dt);
		Render();
		//Wait for Vsync
	}
	//Unload and shutdown
}
```

The program starts as all programs do, with the main() function. 
Here we would load in anything we need at the start of the game, create the window, load settings, and all the usual start-up things you can imagine. If we weren't using SFML we would have to detect the capabilities of the system and enable or disable certain code paths accordingly (or throw an error and quit). SFML does all this work for us, and so we only need to care about loading things directly relevant to our game.

From there we enter a while loop of some kind. This will be based on some boolean value that determines if the game should quit or not. The game logic will set this from true to false when some event has happened (e.g, ESC key pressed, or player presses quit button).  This loop will continuously run two functions, Update and Render.
 
The rate at which we loop through this loop is the games framerate.

### The Update Function
Update is where all game-logic code will go. This includes input processing, physics, content loading/unloading, networking.. etc.

This is also commonly called the game "Tick". While in our games we only do one "Tick" per frame, we could do more. If the game's logic could be executed quickly, and the game relies on fast action, it may be beneficial to do as many updates as you can between frames. While we aren't going to implement this, what you should take away is that:
The Update function should be decoupled from Render(), so that multiple calls to Update() before a call to  Render() should not break your game.

Once the Game update has been completed, the game can render a frame.  No rendering will take place during the update function. The simple way of thinking is that the Update function determines where everything is and what it's doing. The render function then draws the results of the update.   

#### Delta Time  - (Δt)

{:tip="deltatime" class="tip"}
DetaTime Covered is in more detail here

Before calling Update, the Delta-Time (DT) is calculated. This is the amount of time that has passed between now and the previous frame. With a game updating at a steady 60fps, dt should be approximately 16ms (1/60).
To actually calculate DT, you can use inbuilt C++ timers, or just use the handy SFML Clock.
```cpp
static sf::Clock clock;
const float dt = clock.restart().asSeconds();
```
But why do we need this?

Anything in the game logic that does anything with time needs DT to work. Imagine a weapon cooldown, a player can only use it every 10 seconds. A naive approach would be to assume a constant 60fps, so 10 seconds = 600 frames. Then you could have an integer set to 600 and decrement it every frame. This wouldn't work due to small fluctuations in the fps, and while 600 frames have passed, that may not equate to 10 seconds in the real world. If we instead do ```counter -= DT``` each update. now we are relying on time, rather than frames, This is called **frame-rate independence**.

Anytime you are doing game logic that has any link to time, you must use DT. This includes moving the player. This may not seem to be time-based at first. But consider what speed is: Distance over *time*. You don't want the player to move from one side fo the screen to the other in less time if the framerate is higher.

It's a tricky concept but easy to implement. Usually, just multiply whatever by DT.
```Cpp
float playerSpeed = 10.0f;
void Update(double dt) {
  //bad!
  if(moveButton.pressed){
    player.move(playerSpeed);
  }
  //Good!
  if(moveButton.pressed){
    player.move(playerSpeed * dt);
  }
}
```

### Render()

The render function does what you would expect, renders everything in the game to screen. There may be additional logic that goes on to do with optimization and sending things back and forth between the GPU, but we are not dealing with any of this for a 2D game in SFML. To us, the render function is simply where we tell SFML to draw to the screen. In a multi-threaded engine, this could be happening alongside an update function (more on this much later). 

#### Vsync
One other piece of logic that is important the game loop is the "buffer-swap" or "swap-chain" or "Vsync". This is a function that we can call that let's the GPU know that the rendering has finished, and therefore  it's time to send the completed rendered frame to the monitor.

```Cpp
//End the frame - send to monitor - do this every frame
window.display();
```

If we have enabled Vertical-sync (Vsync), the game will limit itself to the refresh rate of the connected monitor (usually 60hz, so 60fps). In this scenario once we have finished rendering everything in under 16ms, and we call window.display(), the game will wait for the remaining time of the frame before continuing. This is a carry-over from low level graphics programming where you don't want to send a new image to the monitor before it has finished drawing the previous (this causes visual Tearing). So if we are rendering faster than 60fps, the game will wait at the end of the render function while the monitor catches up. Before starting again the next frame.
With vsync disabled, once we have finished rendering a frame, window.display() does not pause after sending the image and we continue to render the next frame immediately.

```Cpp
// enable or disable vsync - do at start of game, or via options menu
window.setVerticalSyncEnabled(true/false);	
```

An important gotcha that can happen here  is that the graphics drivers can manually override and forcefully enable or disable Vsync. So don't depend on it always being in the state that you set it.

**Why do we care about this?**
If you are measuring the performance of your game, and it seems to be stuck to 30 or 60fps. Vsync is on. Turn it off to see the true performance of your game.


## Next Steps
You may want to come back here and re-read this page later on. For now, with the basics covered let's get some code written!

Head over to [chapter 2: The Mechanics of Pong](pong2)




---
Previous step: [Build setup](build_setup)
Next step: [Pong 2](pong2)
