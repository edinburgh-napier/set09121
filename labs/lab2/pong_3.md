---
title: "Pong 3 - Moving the Ball"
keywords: pong
tags: [pong]
permalink:  pong3.html
summary: Bounce and move
sidebar: home_sidebar
---

# Moving the ball 
Add the following to the declarations at the start fo the file.
```cpp
Vector2f ballVelocity;
bool server = false;
```

Add this to the Load() function
```cpp
ballVelocity = {(server ? 100.0f : -100.0f), 60.0f};
```

Add this to the Update() function
```cpp
ball.move(ballVelocity * dt);
```

What we have done here is store a 2D vector of the **velocity** (speed+direction) of the ball. SFML stores the **position** of the ball internally so we don't need an extra variable for that. In our update function we then move the ball by it's velocity. If we were coding the move function manually it would look like this:

```newBallPosition = oldBallPosition + (ballVelocity * TimePassed)```

Which is just simple physics.

I've introduced another piece of logic, the 'server' boolean. This decided which direction the ball starts moving at the start of the game. 
The weird piece of code that you added to the load function is an 'inline IF statement'. It's just the same as a IF block, but in one line. If the statement before the ? is true, then ballVelocity.x is set to 100.0f, if server is false, ballVelocity.x = -100.0f. Note that ballVelocity.y is always the same at 60.0f!


## Ball collision

"Bouncing" the ball is one of those cheap tricks that looks harder than it is. As we are storing the ball velocity as a vector. Negating it with respect to wall it bounced off of is as easy as multiplying the right element (X or Y) by -1. We actually multiply by -1.1, and 1.1 here so the ball not only bounces, but gets faster. All in two lines of code. Maths *is* fun. 

In a perfect word that would be fine, but if the ball was flying super fast, in might get "stuck within the wall" where the collision code will constantly negate it's velocity, while speeding it up. This will cause the ball to stop,  wiggle, then form a black hole of big numbers. This is bad. To get around this we cheat and teleport the ball out of the colliding surface by 10 units. Pong is a fast paced game so no one will notice. (This foreshadows some of the nastiness that happens when trying to write good physics code, we will talk about this much later)

Add the following to the Update(). There is no functional purpose for the *bx* and *by* variables, but as we are going to use them a lot it's nicer to have them to keep our code small.
```cpp
  // check ball collision
  const float bx = ball.getPosition().x;
  const float by = ball.getPosition().y;
  if (by > gameHeight) { //bottom wall
    // bottom wall
    ballVelocity.x *= 1.1f;
    ballVelocity.y *= -1.1f;
    ball.move(0, -10);
  } else if (by < 0) { //top wall
    // top wall
    ballVelocity.x *= 1.1f;
    ballVelocity.y *= -1.1f;
    ball.move(0, 10);
  } 
```

### Left and right "Score walls"

This is a very easy check. We will be extending on the previous section of code with more else if statements.
What is new here is that if we have collided with these walls, then we don't bounce. We reset the balls and the paddles, and increment the score. Ignore score for now, but you should implement the reset function. 
You should pull out some of the logic form the load() function and then call Reset() at the beginning of the game. Repeated code is bad code.

```cpp
  else if (bx > gameWidth) {
    // right wall
    reset();
  } else if (bx < 0) {
    // left wall
    reset();
  }
```

### Collision with paddles

This is a simple "Circle - Rectangle" collision check. But as we know that the paddles only move in the Y axis, we can take some shortcuts, we only need to check if the ball is within the top and bottom edge of the paddle.

```cpp
else if (
	//ball is inline or behind paddle
	bx < paddleSize.x && 
	//AND ball is below top edge of paddle
	by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
	//AND ball is above bottom edge of paddle
	by < paddles[0].getPosition().y + (paddleSize.y * 0.5)
	) {
    // bounce off left paddle
  } else if (...) {
    // bounce off right paddle
  }
```


## Two Player and Validated moves
You should now extend your game logic code to allow for moving both paddles (use the controls array defined at the top of the file). This should simply be a case of adding two extra IF statements to the Update().
Further to this, players shouldn't be able to move off of the screen. Given you have already done collision code, this should be a simple addition for you to complete.

## AI
At this stage, we want to keep the code simple, so for AI, the AI paddle will try to match it's position to be the same height as the ball.
However, keep it fair, AI should always play by the same rules as the player. The AI paddles should move at the same speed as the player, and not teleport to the correct position.

I've not given you any code for this, you should try to implement this yourself.

## Adding score text
Loading and displaying text is super easy with SFML. Firstly you will need to find a font.ttf file somewhere, and use the following code

Add the following to Load()
```cpp
 // Load font-face from res dir
font.loadFromFile("res/fonts/RobotoMono-Regular.ttf");
// Set text element to use font
text.setFont(font);
// set the character size to 24 pixels
text.setCharacterSize(24);
```

Add the following to your Reset()
```cpp
 // Update Score Text
text.setString(...);
// Keep Score Text Centered
text.setPosition((gameWidth * .5f) - (text.getLocalBounds().width * .5f),0);
```

Finally, you need to add to your Render() function. I'll let you figure out what.

## Runtime Resources
If you have written your code correctly, you will get an error during runtime, that it can't find the font file specified.
You will need to have your font file relative to the *working directory*"* of your game. Traditionally this is wherever the .exe is (In the build folder), but IDEs can change this to be other places.

Take a look here  [Runtime Resources tips and tricks](resources)


## Done
At this point you should have a fully featured PONG game, feel free to add more features, but the point of this exercise is to make a simple game with as few lines as possible of simple code. It's refreshing to be able to work in these conditions, without having to think about software design and large scale functionality. The next project we will work though will get progressively more complex until we are building an entire games engine. So keep you simple PONG game around, as something to look back on fondly as a statement of how easy it can be sometime to make great games.

---
Previous step: [Pong 2](pong2)

Next step: [Space Invaders! pew pew pew](SpaceInvaders)
