---
title: "Pong"
keywords: pong
tags: [pong]
permalink:  pong.html
summary: Let's start at the beginning
sidebar: home_sidebar
---

For your first practical, we will be going back to the most basic of basic. Creating the classic arcade game PONG. For this we will be chucking some of the software practices you may be used to out the window. We will be using one single main.cpp file, no classes, no OO at all. You should be able to have a working pong game, with AI, in under 200 lines of code. 

<video class="middle" width="400" height="300" loop autoplay>
  <source src="/assets/videos/pong.gif" type="video/mp4">
</video>


The purpose of this exercise is to get you acquainted with SFML. We will come back to the exercise often as an example of simple software design, as it useful as a base to compare more complex approaches.

Before we get stuck in, let's cover some of the fundamentals.

# The Game Loop

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
Update is where all game-logic code will go. This includes input processing, physics, content loading/unloading, networking.. etc. 

Once the Game update has been completed, the game can render a frame.  No rendering will take place during the update function. The simple way of thinking is that the Update function determines where everything is and what it's doing. The render function then draws the results of the update.   

The rate at which we loop through this loop is the games framerate.

Before calling Update, the Delta-Time (DT) is calculated. This is the amount of time that has passed between now and the previous frame. With a game updating at a steady 60fps, dt should be approximately 16ms (1/60).
To actually calculate DT, you can use inbuilt C++ timers, or just use the handy SFML Clock.
```cpp
static sf::Clock clock;
const float dt = clock.restart().asSeconds();
```



---
Previous step: [Build setup](build_setup)
Next step: [Pong](pong)
