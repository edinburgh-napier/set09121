---
title: "Pong 3 - Moving the Ball"
keywords: pong
tags: [pong]
permalink:  pong3.html
summary: Bounce and move
sidebar: home_sidebar
---

# Moving the ball 
Add the following to the declarations at the start of the file.
```cpp
Vector2f ballVelocity;
bool server = false;
```

Add this to the Load() function
```cpp
ballVelocity = {(server ? 100.0f : -100.0f), 60.0f};
```

Add this to the Update() function (after the event handling)
```cpp
ball.move(ballVelocity * dt);
```

What we have done here is store a 2D vector of the **velocity** (speed & direction) of the ball. SFML stores the **position** of the ball internally, so we don't need an extra variable for that. In our update function we then move the ball by it's velocity. If we were coding the move function manually it would look like this:

```newBallPosition = oldBallPosition + (ballVelocity * TimePassed)```

This is just simple physics, but remember that we've used Delta Time to ensure the ball moves at a constant rate.

I've introduced another piece of logic, the 'server' boolean. This decided which direction the ball starts moving at the start of the game. 
The weird piece of code that you added to the load function is an 'inline if statement'. It's just the same as a if block, but in one line. If the statement before the ? is true, then ballVelocity.x is set to 100.0f, if server is false, ballVelocity.x = -100.0f. Note that ballVelocity.y is always the same at 60.0f!


## Ball collision

"Bouncing" the ball is one of those cheap tricks that looks harder than it is. As we are storing the ball velocity as a vector, negating it with respect to the wall it bounced off is as easy as multiplying the right element (X or Y) by -1. We actually multiply by -1.1, and 1.1 here so the ball not only bounces, but gets faster. All in two lines of code. Maths *is* fun. Note, this trick only works for perfectly horizontal or vertical walls!

In a perfect word that would be fine, but if the ball was flying super fast, it might get "stuck within the wall" where the collision code will constantly negate it's velocity, while speeding it up. This will cause the ball to stop, wiggle, then form a black hole of big numbers, usually resulting in it firing out like an angry bee from a cannon! This is bad. To get around this, we cheat and teleport the ball out of the colliding surface by 10 units. Pong is a fast paced game so no one will notice. (This foreshadows some of the nastiness that happens when trying to write good physics code, which we will talk about much later)

Add the following to the Update(). There is no functional purpose for the *bx* and *by* variables, but as we are going to use them a lot it's nicer to have them to keep our code small. Oh, you probably want to make sure you add any collision code **after** the movement code too, otherwise people might get upset that the ball can miss paddles that look like they should hit!
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
What is different here, is that if we have collided with these walls, then we don't bounce. We reset the balls and the paddles, and increment the score. Ignore score for now, but **you now need to implement a reset function.** 

You should pull out some of the logic form the Load() function and then call Reset() at the beginning of the game. Repeated code is bad code! Remember, you DON'T need all of the code from Load() in Reset(). Only take what you need.

Once your ready, add this code:

```cpp
  else if (bx > gameWidth) {
    // right wall
    Reset();
  } else if (bx < 0) {
    // left wall
    Reset();
  }
```

### Collision with paddles

This is a simple "Circle - Rectangle" collision check. But as we know that the paddles only move in the Y axis, we can take some shortcuts! We always know a paddle's x position, so we only need to check if the ball is within the top and bottom edge of the paddle.

Add, and then complete, the code below. Remember, you might need the collision trick we talked about previously!

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

{:class="important"}
**Spend some time here making sure you can get things feeling right! You might need to adjust some of the values you've been given.**

## Two Player and Validated Moves
You should now extend your game logic code to allow for moving both paddles (use the controls array defined at the top of the file). This should simply be a case of adding two extra if statements to the Update() function.

Further to this, players shouldn't be able to move their paddles off of the screen. Given you've already done collision code for the ball, this should be a simple addition for you to complete! (While your here, you might want to make it so that the serving direction is linked to who just lost a point, too!)

## AI
At this stage, we want to keep the code simple, so for AI, the AI paddle will try to match it's position to be the same height as the ball.
However, to keep it fair, the AI should always play by the same rules as the player. The AI paddle should move at the same speed as the player, and not teleport to the correct position!

I've not given you any code for this, you should try to implement this yourself. I'd recommend adding a toggle so you can switch between 1P and 2P modes too! Oh, and remember to use delta time!

## Adding score text
Loading and displaying text is super easy with SFML. First, you will need to find a font.ttf file somewhere. Google Fonts is a great source of fonts with permissive licenses. Remember, anything you use in your game project for this module **must be used legally and within its license**.

Anyway, add the following to Load()
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

Finally, you need to actually keep track of the score, and then you need to add some code to your Render() function. I'll let you figure out what!

## Runtime Resources
If you have written your code correctly, you will get an error during runtime. That is because your game can't find the font file specified.
You will need to have your font file relative to the **working directory** of your game. Traditionally this is wherever the .exe is (In the build folder), but IDEs can change this to be other places when they run, and VS certainly does..

Take a look here for how to fix this [Runtime Resources tips and tricks](resources)


## Done
At this point you should have a fully featured Pong game. If you've got here quickly, you should spend some time adding more features to get used to SFML and C++. Just remember, the point of this exercise was to make a simple game with as few lines code as possible. It's refreshing to be able to work in these conditions, without having to think about software design and large scale functionality. The next projects we will work though will get progressively more complex until we are building an entire games engine. So keep your simple Pong game around, as something to look back on fondly as a statement of how easy it can be sometime to make great games!

---
Previous step: [Pong 2](pong2)

Next step: [Space Invaders! pew pew pew](SpaceInvaders)
