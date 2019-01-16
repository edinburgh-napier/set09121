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
- Paddles can't go past Horizontal edges of screen.
- Ball starts in center heading towards an edge
- Ball bounce off Paddles and Horizontal screen edges.
- Ball bounces perfectly: angle of incidence == angle of reflection.
- Ball speeds up after each bounce.
- If ball touches either vertical edge. Score is given, ball resets

## The Code

I'm going to give you the code here verbatim, with lines left for you to complete throughout **Remember the purpose of this exercise** 
1. Get acquainted with SFML and C++ game code.
2. See how to build a game with bare-minimum ultra simplistic code

This exercise isn't following best practices, and isn't how you or I would go about building a game in the future. This is a thought exercise in how basic a game can get.

### practical_1/main.cpp
I'll get you started off with the top of your file. It's the usual imports and namespaces, followed by some variables we will use for game rules, and then 3 shapes, 1 circle for the ball, and 2 rectangles stored in an array for the paddles.

We then move onto the Load() function, we would load in assets here if we had any, and then we set-up the game by resizing and moving our shapes.

The last two lines are left incomplete for you to complete later. 

```cpp
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

const Keyboard::Key controls[4] = {
    Keyboard::A,   // Player1 UP
    Keyboard::Z,   // Player1 Down
    Keyboard::Up,  // Player2 UP
    Keyboard::Down // Player2 Down
};
const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;

CircleShape ball;
RectangleShape paddles[2];

void Load() {
  // Set size and origin of paddles
  for (auto &p : paddles) {
    p.setSize(paddleSize - Vector2f(3, 3));
    p.setOrigin(paddleSize / 2.f);
  }
  // Set size and origin of ball
  ball.setRadius(ballRadius - 3);
  ball.setOrigin(ballRadius / 2, ballRadius / 2);
  // reset paddle position
  paddles[0].setPosition(10 + paddleSize.x / 2, gameHeight / 2);
  paddles[1].setPosition(...);
  // reset Ball Position
  ball.setPosition(...);
}
```

#### The Update
Here -- as we have covered previously -- is where our gamelogic goes. This runs every frame.
Firstly we calculate DT, then process any events that sfml passes to us.
From there we are free to do whatever we want, and what we want to do is make PONG. 
We will come back and add to this, you don't need to edit anything just now

```cpp
void Update(RenderWindow &window) {
  // Reset clock, recalculate deltatime
  static Clock clock;
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
  if (Keyboard::isKeyPressed(Keyboard::Escape)) {
    window.close();
  }

  // handle paddle movement
  float direction = 0.0f;
  if (Keyboard::isKeyPressed(controls[0])) {
    direction--;
  }
  if (Keyboard::isKeyPressed(controls[1])) {
    direction++;
  }
  paddles[0].move(0, direction * paddleSpeed * dt);
}
```

#### Render and Main
Our last section of the file is out super simple render function. I mean, just look at it. Isn't SFML awesome?
Then we have our standard Main entrypoint, with our gameloop code. Not much to see here.

```cpp
void Render(RenderWindow &window) {
  // Draw Everything
  window.draw(paddles[0]);
  window.draw(paddles[1]);
  window.draw(ball);
}

int main() {
  RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
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

Once you've typed all of the above in: you should have something like this:

{:class="small"}
![PONG screenshot](assets/images/pong_1.png)

Make sure  everything is working before continuing. If you find yourself a little lost now, have a re-read or ask for help. Skipping on through without a good understanding of the above is a one-way ticket to confusion town.

[Take the one-way ticket to Confusion town and carry onto chapter 3](pong3)

[Go back to Chapter 1 and prepare yourself](pong)

