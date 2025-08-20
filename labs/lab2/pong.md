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

## Structure of the main.cpp and the Game Loop

The fundamental core of all games, is the game loop. While some engines may hide this away, behind the scenes you can be sure that the code for any game can be stripped away to the fundamental game loop. It looks like this.
```Cpp
#include <SFML/Graphics.cpp>

void init(){
	// initialise all the objects needed for the game. 
}

void update(float dt) {
	// Update Everything
}

void render(sf::RenderWindow &window) {
	// Draw Everything
}

void clean(){
	//free up the memory if necessary.
}

int main () {
	//create the window
	sf::RenderWindow window(sf::VideoMode({game_width, game_height}), "PONG");
    //initialise and load
	init();
	while(!window.isOpen()){
		//Caluclate dt
		...
		window.clear();
		update(dt);
		render(window);
		//wait for the time_step to finish before displaying the next frame.
		sf::sleep(time_step);
		//Wait for Vsync
		window.display()
	}
	//Unload and shutdown
	clean();
}
```

The program starts as all programs do, with the main() function. 
Here we would load in anything we need at the start of the game, create the window, load game assets, initialise game objects, and all the usual start-up things you can imagine. If we weren't using SFML we would have to detect the capabilities of the system and enable or disable certain code paths accordingly (or throw an error and quit). SFML does all this work for us, and so we only need to care about loading things directly relevant to our game.

From there we enter a while loop. This will be based on some boolean value that determines if the game should quit or not (here with SFML: if the window is still open). The game logic will set this from false to true when some event has happened (e.g, ESC key pressed, or player presses quit button).  This loop will continuously run two functions, Update and Render.
 
The rate at which we loop through this loop is the game's framerate. The framerate is fixed by the sleep function with a fixed time step. This insure that regardless the system or number of calculation the game will run with a fixed framerate.

### The Update Function
Update is where all game's logic code will go. This includes input processing, physics, content loading/unloading, networking, etc.

This is also commonly called the game "Tick". While in our games we only do one "Tick" per frame, we could do more. If the game's logic could be executed quickly, and the game relies on fast action, it may be beneficial to do as many updates as you can between frames. While we aren't going to implement this, what you should take away is this: <br />
**The Update function should be decoupled from Render(), so that multiple calls to Update() before a call to  Render() should not break your game.**

In our case, we are doing it more simply. Once the update function has been completed, the game can render a frame.  No rendering will take place during the update function. The simple way of thinking of this is that the update function determines where everything is and what it's doing. The render function then draws the results of the update. Remember, we are decoupling the logic and underlying game state from the rendering itself. (This, incidentally, makes it much easier to deal with things we need in the scene but might not want to render, such as triggers)

#### Delta Time  - (Δt or dt)

{:tip="deltatime" class="tip"}
Delta Time Covered is in more detail here - you will want to read this!

Before calling Update, the Delta Time (dt) is calculated. This is the amount of time that has passed between now and the previous frame or tick. With a game updating at a steady 60fps, dt should be approximately 17ms (1/60).
To actually calculate dt, you can use inbuilt C++ timers, or just use the handy SFML Clock.
```cpp
static sf::Clock clock;
const float dt = clock.restart().asSeconds();
```
These two lines should go at the start of the while loop.

But why do we need this?

Anything in the game logic that does anything with time needs dt to work. Imagine a weapon cooldown, a player can only use it every 10 seconds. A naive approach would be to assume a constant 60fps, so 10 seconds = 600 frames. Then you could have an integer set to 600 and decrement it every frame. This wouldn't work due to small fluctuations in the FPS or if you want your game to run at different FPS settings, and while 600 frames have passed, that may not equate to 10 seconds in the real world. If we instead do ```counter -= dt``` each update, we are relying on time directly, rather than on the number of frames. This is called **framerate independence**.

Any time you are doing game logic that has any link to time, you must use dt. This includes moving the player. This may not seem to be time-based at first, but consider what speed is: Distance over *time*. You don't want the player to move further if framerate is higher.

It's a tricky concept but easy to implement. Usually, you can just multiply whatever it is by dt. If you've worked in Unity before, you'll have seen Time.deltaTime, and this is the same idea.
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
One thing you will need to remember, however, is that all speeds are now **per second**. In other words, in the good example above, the player will move 10 units per second. This actually makes a lot of things easier (e.g. rotating something at a set speed, you can now say "360 degrees per second") but if you are converting old, bad code you might find you need to update your speed values!

### Render()

The render function does what you would expect, renders everything in the game to screen. There may be additional logic that goes on to do with optimization and sending things back and forth between the GPU, but we are not dealing with any of this for a 2D game in SFML. To us, the render function is simply where we tell SFML to draw to the screen. In a multi-threaded engine, this could be happening alongside an update function (more on this much later). 

#### Blitting and VSync
One other piece of logic that is important to consider in the game loop is the "buffer-swap", "double buffering", "screen blitting", or "swap-chain". There are lots of names for this than all mean slightly different things, but for now you can think of it as a function that we can call that let's the GPU know that the rendering has finished, and therefore it's time to send the completed frame to the monitor.

Normally, we don't draw directly onto the screen because this is a) very slow, and b) can cause incomplete frames to appear on screen which looks horrible. Instead, we render in a memory location that is the same size and shape as the screen. This is much (much) quicker, and means we can wait until we've rendered everything we want before 'blitting' it to the screen itself. Remember, each draw call to the screen is relatively slow, so if we do it for each object our game will stutter to a crawl. This way, we instead only draw to the screen once per frame! With SFML we use the following command:

```Cpp
//End the frame - send to monitor - do this every frame
window.display();
```

If we have enabled Vertical-Sync (VSync), the game will limit itself to the refresh rate of the connected monitor (usually 60hz, so 60fps). In this scenario, once we have finished rendering everything in under 16ms, and we call window.display(), the game will wait for the remaining time of the frame before continuing. This comes from low level graphics programming where you don't want to send a new image to the monitor before it has finished drawing the previous (this causes visual tearing in the scene as you are seeing two different frames in the top and bottom half of the monitor). So, if we are rendering faster than 60fps, the game will wait at the end of the render function while the monitor catches up before starting again the next frame.
With VSync disabled, once we have finished rendering a frame, window.display() does not pause after sending the image and we continue to render the next frame immediately.

```Cpp
// enable or disable vsync - do at start of game, or via options menu
window.setVerticalSyncEnabled(true/false);	
```

An important gotcha that can happen here is that the graphics drivers can manually override and forcefully enable or disable VSync. As such, don't depend on it always being in the state that you set it!

**Why do we care about this?**
If you are measuring the performance of your game, and it seems to be stuck to 30 or 60fps. VSync is on. Turn it off to see a more accurate measure of the true performance of your game.


## Next Steps
You may want to come back here and re-read this page later on. For now, with the basics covered let's get some code written!

Head over to [Chapter 2: The Mechanics of Pong](pong2)




---
Previous step: [Build setup](build_setup)
Next step: [Pong 2](pong2)
