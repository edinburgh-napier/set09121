---
title: "Pong 2 - The Mechanics of Pong"
keywords: pong
tags: [pong]
permalink:  pong2.html
summary: Paddle, Ball, Score ...
sidebar: home_sidebar
---

Let's have a look at that Pong capture again. Imagine how you would go about building it. What are the systems and mechanics?

<video class="middle" width="400" height="300" loop autoplay>
  <source src="assets/videos/pong.mp4" type="video/mp4">
</video>

- There are two Paddles, which move up and down.
- There is one Ball that bounces around
- Paddles can't go past Horizontal edges of screen.
- Ball starts in the center, heading towards an edge
- Ball bounces off Paddles and Horizontal screen edges.
- Ball bounces perfectly: angle of incidence == angle of reflection.
- Ball speeds up after each bounce.
- If Ball touches either Vertical edge, Score is given, Ball resets

## The Code

I'm going to give you the code here verbatim, with lines left for you to complete throughout (...).

**Remember the purpose of this exercise is to:** 
1. Get acquainted with SFML and C++ game code.
2. See how to build a game with the bare minimum (ultra simplistic) code

This exercise isn't following best practices, and isn't how you or I would go about building a game in the future. This is a thought exercise in how basic a game can get and to get you making something interactive quickly.

### practical_1/main.cpp
In this example, we will be overwriting the SFML hello world code you used in the last lab. This is to save you some faff time with CMake.

{:class="important"}
**Make sure you take a back up of that code, as it will be useful to test CMake when you create new projects in later labs!**

I'll get you started off with the top of your file. It's the usual imports and namespaces, followed by some variables we will use for game rules, and then 3 shapes, 1 circle for the ball, and 2 rectangles stored in an array for the paddles.

**I recommend you type the code you're given in manually, rather than just copying and pasting. This will get you used to SFML and C++ syntax and common mistakes!** You can, of course, compare your code to the original if Visual Studio complains, but it's a good habit to get in, especially if you've never written C++ before.

We then move onto the Load() function. This is where we would load in assets if we had any (remember this for future)! For now, we setup the game by resizing and moving our shapes.

The ... lines are left incomplete for you to complete. There are hints about what should go there, in comments and other code, but you should start thinking about what these lines should do, and how to use the SFML API!

```cpp
#include <SFML/Graphics.hpp>


const sf::Keyboard::Key controls[4] = {
    sf::Keyboard::A,   // Player1 UP
    sf::Keyboard::Z,   // Player1 Down
    sf::Keyboard::Up,  // Player2 UP
    sf::Keyboard::Down // Player2 Down
};
const sf::Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;

CircleShape ball;
RectangleShape paddles[2];

void Load() {
    // Set size and origin of paddles
    for (auto &p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }
    // Set size and origin of ball
    ball.setRadius(ballRadius);
    ball.setOrigin(...); //Should be half the ball width and height
    // reset paddle position
    paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));
    paddles[1].setPosition(...);
    // reset Ball Position
    ball.setPosition(...);
}
```

#### The Update
Here, as we have covered previously, is where our game logic goes. At the moment, this runs every frame.
First we calculate delta time, then process any events that SFML passes to us.
From there we are free to do whatever we want, and what we want to do is make Pong! 

We will come back and add to this, but you don't need to edit anything just now. Add this to your file, under the load function from before.

```cpp
void Update(RenderWindow &window) {
    // Reset clock, recalculate deltatime
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();
    // check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    // Quit Via ESC Key
    if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    // handle paddle movement
    float direction = 0.0f;
    if (sf::Keyboard::isKeyPressed(controls[0])) {
        direction--;
    }
    if (sf::Keyboard::isKeyPressed(controls[1])) {
        direction++;
    }
    paddles[0].move(Vector2f(0.f, direction * paddleSpeed * dt));
}
```

#### Render and Main
Our last section of the file is our super simple render function. I mean, just look at it! Isn't SFML awesome?
Then we have our standard main entry point, with our game loop. Not much to see here, it should be pretty obvious what it's doing by now.

Add this to the bottom of your main.cpp file:

```cpp
void Render(RenderWindow &window) {
    // Draw Everything
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
}

int main() {
    sf::RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
    Load();
    while (window.isOpen()) {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}
```

#### Filling in the Gaps
We left a few parts for you to fill in here, now is the time for you to go and fill them in. Hopefully by now you should be able to find errors within your code using Visual Studio. If you can't remember what you need to fill in, use those to hunt them down.

Add sensible code into those sections to do what makes sense as per how we defined Pong at the top of this page!

{:class="important"}
**Remember, you can (and should) look up the SFML API to make sure you understand what the different functions expect!**

#### Checking it Works!
Once you've typed all of the above in, filled in the gaps, and debugged any typing issues you should be able to run it. When you do, you should have something like this:

{:class="small"}
![PONG screenshot](assets/images/pong_1.png)

{:class="important"}
**Make sure everything is working before continuing!**

If it doesn't look like that image, then you should go back and figure out why and fix your code. Also, you'll find that the left paddle will move, but the right paddle and ball won't yet - don't worry, we'll get there in the next section!

If you find yourself a little lost now, re-read this page, look up the SFML API and/or ask for help. Skipping onwards without a good understanding of what we did here is a one-way ticket to confusion town!

#### An API Update

It seems that SFML have updated their  APIs so you can no longer pass separate x and y values into many of the calls – instead you need to pass sf::Vector2f() (or make sure you are using sf like we are in this file). This will no doubt be an optimisation, but it means that some 'legacy' code (like what you will get later in this module) will not work by default.

Thankfully it's an easy fix, and I'll be fixing it for most of the code we give you in the labs, but just remember this for if you find errors later. After all, I might miss one!

[When you are ready, carry on to Chapter 3](pong3)

[Go back to Chapter 1 and prepare yourself](pong)

