---
title: "Space Invaders 2 - Writing Some Real Code"
keywords: SpaceInvaders
tags: [SpaceInvaders]
permalink:  spaceinvaders2.html
summary: OO
sidebar: home_sidebar
---

## Creating the Ship Class

Okay, now you have your sprite loaded (right?) it's time to start on the actual game. As with all software projects, as the complexity of the software grows, so do the potential different ways to implement it. That is to say, while this may not be the best way to implement space invaders, we like to think it's at least a 'good' way. The point of building it this way is to expose you to many different aspects of C++ OO. Any programmer worth their salt will have an opinion on how they could improve someone elses code, and if you feel at the end of this that you have some ideas, then this lesson was successful.

### Let's go OO

We are going to need at least two different entities for our invaders game. Invaders and the Player. Invaders are all identical other than their starting position and sprite. They also exhibit some non-trivial individual logic. Your software engineering brain should be starting to form the basis for properties and methods of the invader class by this point. To add to the fun, consider the player, and how similar it is also to the invader. They both shoot bullets, move, and can explode. It sounds like inheritance should be joining this party.

The way we are going to go about this to have an *abstract base class*
**Ship**, which is inherited from by a **Player** class and an
**Invader** Class.

### Functionality of the Ship

The ship class will contain all logic that is common for both the player and invaders. Primarily this will be \"moving around\". We could go with the full entity model and have Ship be a base class, with variables for it's position and rotation and such. We would then also have a sf::Sprite member attached where we would call upon all the SFML render logic. This is a good idea -- for a larger game. For space invaders that would involve lots of code to keep the sprite in sync with the ship Entity. Instead we are going to take a super short cut, and inherit from sf::sprite.

This means that Ship will have all the same methods as a sf::sprite, including all the usual 'SetPostition()' and 'move()' commands we have been using already. It also means we can pass a ship object directly to window.draw().

### Create Ship.h

Create a file inside the invaders source folder called "ship.h". **Remember to ensure this new file ends up in your source directory, not the build directory. Check out Part 1 of this lab for a reminder. You will need to reconfigure CMake again to have it show up in VS too!**.

This will be our Header file for the Ship class. Header files contain the declaration of our class, i.e only the function declarations. Headers shouldn't contain any code (some common exemptions apply). The reason we do this is to keep the logic of the class stored inside a .cpp file, while any piece of code that wants to access this functionality only needs the header. This concept does not exist inside Java or C#, wherein you provide the full definition of a function inside a class, in the one file. The is parsed for you, and allows other classes to link to it. C++ is not so nice! For example, you might have already noticed in the Pong example that if you put functions *after* those that call them, it will not work. It is totally possible to work in a more C# fashion but, we get into issues regarding name-space collisions, scope issues, multiple declarations, and code bloat. Instead, we are going to work in the more industry standard style that is likely older than most of you!


Anyway, Inside ship.h write this:

```cpp 
//ship.h
#pragma once
#include <SFML/Graphics.hpp>

class Ship : public sf::Sprite {
protected:
    sf::IntRect _sprite;
    //Default constructor is hidden
    Ship();
public:
    //Constructor that takes a sprite
    explicit Ship(sf::IntRect ir);
    //Pure virtual deconstructor -- makes this an abstract class and avoids undefined behaviour!
    virtual ~Ship() = 0;
    //Update, virtual so can be overridden, but not pure virtual
    virtual void Update(const float &dt);
};
```

### Create Ship.cpp

Next to our ship.h, create ship.cpp - this will cause errors for now, but we'll fix them in a second!

```cpp 
//ship.cpp
#include "ship.h"
using namespace sf;
using namespace std;

Ship::Ship() {};

Ship::Ship(IntRect ir) : Sprite() {
  _sprite = ir;
  setTexture(spritesheet);
  setTextureRect(_sprite);
};

void Ship::Update(const float &dt) {}

//Define the ship deconstructor. 
//Although we set this to pure virtual, we still have to define it.
Ship::~Ship() = default;
```

### Access to global variables

The code above needs access to some variables we have in our main.cpp (spritesheet). There are multiple ways to go about this. A rather simple yet hacky way is to have these variables as 'extern' in a header file. To do this, create another header, called "game.h" and insert:

```cpp 
//game.h
#pragma once
#include <SFML/Graphics.hpp>
constexpr uint16_t gameWidth = 800;
constexpr uint16_t gameHeight = 600;
constexpr uint16_t invaders_rows = 5;
constexpr uint16_t invaders_columns = 12;

extern sf::Texture spritesheet;
```

We are defining some common variables here as constant, which is fine. The interesting bit is the 'extern spritesheet', this tells anyone that includes game.h that a sprite-sheet exists 'somewhere". That somewhere is main.cpp, and the compiler will figure this out for us when we need to access it from ship.cpp. What do you think you need to do now to fix the errors in the ship.cpp file?

**Remember to reload CMake via ZERO\_CHECK to add our new files to the build - if this doesn't work you can always do it via the CMake GUI too**

### Test out the code

As Ship is an abstract class, we can't create one. We can only create a concrete class derived from it. We can reference it as pointer however, due to how C++ polymorphism works. Add the following to the top of your main.cpp

```cpp 
//main.cpp
#include "ship.h"
//...
std::vector<Ship *> ships;
```

This should compile without errors.

{:class="important"}
**Do not move on until all your files appear in VS and it compiles!** 

## Making the Invader class


We could create a new invader.h and invader.cpp to house the invader class. Generally speaking separate files for separate classes is a good idea, although unlike Java we don't *have* to do this. In some situations when certain classes are very similar or just slightly different version of each other it makes sense to host them in the same Header file.

Add the following code to ship.h and ship.cpp:

```cpp 
//ship.h
class Invader : public Ship {
public:
    Invader(sf::IntRect ir, sf::Vector2f pos);
    Invader();
    void Update(const float &dt) override;
};
```

```cpp 
//ship.cpp
Invader::Invader() : Ship() {}

Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir) {
    setOrigin(16, 16);
    setPosition(pos);
}

void Invader::Update(const float &dt) {
    Ship::Update(dt);
}
```

Now that we have a concrete implementation of a Ship we can create one.

```cpp
//Main.cpp

Load(){
...
Invader* inv = new Invader(sf::IntRect(0, 0, 32, 32), {100,100});
ships.push_back(inv);
```

Up to here, I've been including the include statements you'll need too - from here on out, you should be able to work them out for yourself, so don't always expect to see them!

Important note, we used the New() operator here, which created the ship on the heap. If we wanted a stack version, we omit the new New and would use Invader 'inv = Invader()'.

As we are going to be storing the invader in a vector that will also later contain the player, this vector itself cannot be of type Invader. Instead, we need to use the base class that both inherit from - the Ship class. The way we have set this up, however, we can't create a vector<Ship>, as that would try to construct an abstract class. Instead, we create a vector of pointers to the objects!

### Calling Update and Render

Now we need to call the update function for all of our ships every frame. Due to polymorphism this is very simple: as *Update()* is a virtual function, when we call *Update()* on a Ship pointer it will run the *Update()* function of whatever is being pointed to. In other words, if we call *Update()* on an item in a *vector<\*Ship>* collection, and the Ship object that is pointed to is an Invader, then the *Update()* function in the Invader class is called. If it is a Player object, the *Update()* function in the Player class is called. Got that? 

Yeah... this can be confusing, especially if you are new to this! Do your best to get your head around it though, as this is core to how much of game engines work. We have a collection of things we need to update in some fashion, so we iterate through that collection and tell each thing to do what they are supposed to. **If you are completely befuddled, please ask in the lab!**

On the plus side, look how simple the code actually is to do the updating of all the ships:

```cpp
//Main.cpp
Update(){
...
  for (auto &s : ships) {
    s->Update(dt);
  };
```

The same ease goes for rendering, with the additional bonus that as we have inherited from sprite, SFML can render our ships natively! Nice!

```cpp
//Main.cpp
Render() {
...
  for (const auto s : ships) {
    window.draw(*s);
  }
```

**PAY ATTENTION TO THE DIFFERENCES HERE** For Update, we are calling something that will change the stage of what we call, and we're calling a function on the object - hence no const, using & and -> as we talked about in lectures. For Render we aren't changing the state of the sprite (so we can use const) and the draw() function requires us to pass a pointer variable, hence the *.

At this stage, you can now add additional Invaders to the screen, at different locations, and with different graphics thanks to the sprite sheet. Each one you add to the vector should be automagically rendered! Go on, you've probably hurt your brain by this point, so play around with it a bit. Remember you can look up the SFML API whenever you need to.

{:class="important"}
**Do not continue until you have multiple different sprites on your screen!** 

### Invader movement

A quirk of space invaders is that all the invaders move as one, and when any of the invaders touch the edge of the screen all invaders drop down and reverse direction. When invaders are killed, the remaining invaders speed up. From this we can gather that we need some form of communication medium between all the invaders so they can communicate when it's time to drop down and when to speed up. We are going to store these parameters as two variables: direction and speed. We could store these as properties in each invader, but as the contents will be identical for each invader we should do something better. The \"something better\" is static properties.

```cpp 
//ship.h
class Invader : public Ship {
public:
  static bool direction;
  static float speed;
  ...
```

**Top Hint:** Any declared static variable **must** be defined
somewhere. Which means we do the following in ship.cpp

```cpp 
//ship.cpp
bool Invader::direction;
float Invader::speed;
```

We can access these variables anywhere like so 'Invader::speed = 20.f' and it will update the speed of ALL invaders at the same time. Why not try putting this in your Load() or Reset() functions in main.cpp to see how it works?

### Invader Update

It's about time we had something moving on screen. We should modify the
Invaders Update() to include some movement code.

```cpp 
//ship.cpp
#include game.h

void Invader::Update(const float &dt) {
    Ship::Update(dt);

    move(dt * (direction ? 1.0f : -1.0f) * speed, 0);
    
    if ((direction && getPosition().x > gameWidth - 16) ||
        (!direction && getPosition().x < 16)) {
            direction = !direction;
                for (int i = 0; i < ships.size(); ++i) {
                    ships[i]->move(0, 24);
                }
    }
}
```

The first two lines are simple, we call the base ship::update() to run any logic that is generic for all ships (none right now). Then we move either left or right, at the speed dictated by the static speed variable. The next few lines of code is the logic to detect weather it's time to drop and reverse. Direction is involved in the check to stop a feedback loop occurring of one invader triggering the reverse, then in the same frame another invader re-reversing it. So long as the invaders are updated sequentially (i.e not in threads) then this will work. 

(Yes, we're being naughty and using hardcoded magic numbers in here which we maybe should have at least defined somewhere globally. What happens if we increase the size of our sprites?)

As we are now accessing the ships array (and gameWidth) we now need to include `game.h`, and we should put a handle to it in game.h, and fix any errors that causes.

```cpp 
//game.h
extern std::vector<Ship*> ships;
```

## Spawning Invaders

Now we need to see this in action, lets create some more invaders. I'll
start you off with this hint:

```cpp
//Main.cpp
Load(){...
   for (int r = 0; r < invaders_rows; ++r) {
    auto rect = IntRect(...);
    for (int c = 0; c < invaders_columns; ++c) {
        Vector2f position = ...;
        auto inv = new Invader(rect, position);
        ships.push_back(inv);
    }
 }
```

The rest of the spawning logic is up to you! Good luck!

{:class="important"}
**Once you get to this stage, show us so we can see that you got a cool thing working!** 

## The Player Class


Compared to the invader, the player is actually a very simple class. The only real logic it brings to the party is moving left and right based on keyboard inputs. Let's get to it by adding to the ship.h file

```cpp 
//ship.h
class Player : public Ship {
public:
  Player();
  void Update(const float &dt) override;
};
```

Pretty basic. Over in the ship.cpp we now define this code.

```cpp 
//ship.cpp
Player::Player() : Ship(IntRect(160, 32, 32, 32)) {
    setPosition({gameHeight * .5f, gameHeight - 32.f});
}

void Player::Update(const float &dt) {
    Ship::Update(dt);
    //Move left
    ...
    //Move Right
    ..
}
```

You should know how to add in the movement code, it's almost identical to Pong. Bonus points for not allowing it to move off-screen. You should construct one player at load time. You could add it to the vector of ships, but rember the hacky line in invader's update: `ships[i]->move(0, 24);`? This would also move the player. Not good. To solve this you can either 
 - A: Have the player seperate from the ship list, and manually update and render it. **Bad option**
 - B: Change the invader update to only move invaders down. **Good option!**
 
 As a hint: what if there was a function you could call on all ships that moved the Invaders down, but which Player ignored?
 
## Bullets

{:class="important"}
**This part is hard and requires you to do lots of refactoring of your code! Take it slow, and work with someone if you're really struggling.** 

The game wouldn't be very difficult (or possible) without bullets firing around. Let's look at our requirements:
- Invaders shoot green bullets downwards
- The player shoots white bullets upwards
- The bullets explode any ship they touch
- After exploding the bullets disappear

If we look at our sprite-sheet we have two different bullet sprites. SFML can do some colour replacement, so if we wanted we could use the same white sprites for both bullet types and get SFML to colour them differently. As we want the two bullet types to look physically different, however, we will use two different sprites. Whatever visuals we decide to go with, for our software design we will be inheriting from sf::Sprite again.

That's the plan for rendering out of the way, so now just movement and explosions to figure out! It would be tempting to do as we did with Ship and have two sublcasses for invader and player bullets. But as they are both so similar (the only difference being the direction they travel and who they collide with) having a three class structure would be overkill. Sometimes rigorously following OO patterns isn't the best way forward, especially with games. So instead we will build just bullet class.

Create a bullet.h and bullet.cpp and add the following:

```cpp 
//bullet.h
#pragma once
#include <SFML/Graphics.hpp>

class Bullet : public sf::Sprite {
public:
  void Update(const float &dt);
  Bullet(const sf::Vector2f &pos, const bool mode);
  ~Bullet()=default;
protected:
  Bullet();
  //false=player bullet, true=Enemy bullet
  bool _mode;
};
```

```cpp 
//bullet.cpp
#include "bullet.h"
#include "game.h"
using namespace sf;
using namespace std;

//Create definition for the constructor
//...

void Bullet::Update(const float &dt) {
    move(0, dt * 200.0f * (_mode ? 1.0f : -1.0f));
}
```

**Remember: if you want anything to happen, you need to call Update() and Render() from main.cpp!**
(It might also be sensible to create a static function to initialise the bullets too!)

### Firing bullets

Alrighty, that's a barebones Bullet created, now to spawn one. The simplest way to do this would be to do something like this:


```cpp 
//ship.cpp
Player::Update(){...
if (Keyboard::isKeyPressed(...)) {
  new Bullet(getPosition(), false);
}
```

This is not a good idea however, and there  are three major problems.

1. we will spawn thousands of bullets, we need a way to 'cooldown' the weapon of the player 
1. How do we update and render them? We should store our bullets somewhere. 
1. This is a big one: we put these bullets on the heap, and then forget about them. That's a classic **Memory Leak**.

### Storing our bullets

Let's get our bullets at least updating and rendering before we worry
about the other issues. What we could do is something like this:


```cpp 
//ship.cpp
Player::Update(){...
  static vector<bullet*> bullets;
  if (Keyboard::isKeyPressed(...)) {
    bullets.push_back(new Bullet(getPosition(), false));
  }
  for (const auto s : bullets) {
    bullets.Update(dt);
  }
```

So now the player is responsible for handling and keeping track of all the bullets it fires. We've stopped a runaway memory leak (for now, we still have to delete the bullets later). But how do we Render them? and while we're asking questions, won't this very get really, really big? We are going to fire a lot of bullets.

Yes, we will be firing loads of bullets, and we don't want to have keep track them all and delete them. In larger games this would cause performance issues. It won't here, but let's pretend we are running on original 80's hardware, we've gotta do better, or else the arcade will have to shut down and the kids will be sad. So will I!

### A different solution - Bullet Pools

How about instead of creating bullets as and when we need them, we allocate a whole bunch at the start, and put them into a \"pool of available bullets\". When a player or an invader fires, an inactive bullet in the pool gets initialised to the correct position and mode and goes about it's bullety business. After this bullet has exploded or moved off-screen, it is moved back into the pool (or just set to \"inactive\").

This is a very common technique used in games with lots of expensive things coming into and out of existence. Almost every AAA Unity3D game uses this with GameObjects -- which take forever to allocate and construct. It's much quicker to allocate loads at the start and re-use them.

#### Storing the Bullet Pool

Each Ship could have it's own pool of 3 or 4 bullets to re-use, but that's a lot of code to refactor. Instead let's store the bullet pool *inside* the bullet class.


```cpp 
//bullet.h
class Bullet : public sf::Sprite {

...
    protected:
    static unsigned char bulletPointer;
    static Bullet bullets[256];
...

}

```

We have statically allocated 256 bullets on the stack. We have brought along a sneaky unsigned char to do a clever trick to determine which bullet to use next. Unisgned chars go between 0 and 255, and then wrap round back to 0 and repeat. Therefore every time we Fire() a bullet we choose from the array like this \"bullets\[++bulletPointer\]\". If there were ever more than 256 bullets on screen we will run into trouble, but I won't worry about this if you don't. In fact, if we are clever, we would just reuse the oldest bullet and with so many of them around, players would likely never even notice!

We will need to change our Firing mechanism, we now don't want to ever construct a bullet, just Fire() one. We will have to change our class declaration around to suit this. Fire() will become a static function.


```cpp 
//bullet.h
class Bullet : public sf::Sprite {
public:
  //updates All bullets
  static void Update(const float &dt);
  //Render's All bullets
  static void Render(sf::RenderWindow &window);
  //Chose an inactive bullet and use it.
  static void Fire(const sf::Vector2f &pos, const bool mode);
  
  ~Bullet()=default;
protected:
  static unsigned char bulletPointer;
  static Bullet bullets[256];
  //Called by the static Update()
  void _Update(const float &dt);
  //Never called by our code
  Bullet();
  //false=player bullet, true=Enemy bullet
  bool _mode;
};
```

I'll let you figure out the changes to the bullet.cpp. Keep in mind the differences between static-and non static functions. The _update() function is given in the next section.

{:class="important"}
**If you are getting unresolved external symbol errors, remember the top hint from before! (Also, make you you have a constructor in bullet.cpp)**

#### Exploding Things


We will be using SFML to do the collision checks for us this time.


```cpp 
//bullet.cpp
void Bullet::_Update(const float &dt) {
    if (getPosition().y < -32 || getPosition().y > gameHeight + 32) {
        //off screen - do nothing
        return;
    } else {
        move(0, dt * 200.0f * (_mode ? 1.0f : -1.0f));
        const FloatRect boundingBox = getGlobalBounds();
        
        for (auto s : ships) {
            if (!_mode && s == player) {
                //player bulelts don't collide with player
                continue;
            }
            if (_mode && s != player) {
                //invader bullets don't collide with other invaders
                continue;
            }
            if (!s->is_exploded() && 
                s->getGlobalBounds().intersects(boundingBox)) {
                  //Explode the ship
                  s->Explode();
                  //warp bullet off-screen
                  setPosition(-100, -100);
                  return;
            }
        }
    }
};
```

This code will require modification to our ship class. Also we need access to a pointer to the player ship so we can determine the types of collisions. My way of doing this would be to add it as another extern in game.h. We need to introduce Explode behaviour into the ship classes. We will add the common functionality to the base Ship class - turning into the explosion sprite. The invader class will extend this by increasing the speed of other invaders, and removing the explosion sprite after a second. The player ship will end the game if explode is called on it which will trigger a game reset.


```cpp 
//ship.h
class Ship : public sf::Sprite {
    ...
    protected:
      ...
      bool _exploded = false;
    public:
      ...
      bool is_exploded() const;
      virtual void Explode();
};
```


```cpp 
//ship.cpp
void Ship::Explode() {
    setTextureRect(IntRect(  128, 32, 32, 32));
    _exploded = true;
}
```

**You will also have to define the is_exploded() getter function!**

#### A Quick Pause

Right, by now you are probably confused with bullet pools! So, let's have a quick rundown of how your code is supposed to work. The basic idea is that our Bullet class should just deal with anything bullety by itself, without external classes needing to know what it is doing. So, all calls to Bullet from outside should be to static funtions. This is so we can ensure we don't get memory leaks, and to make it easier for the rest of our code. An invader doesn't need to know it's shooting bullet 231, after all. Only _Update() is non-static, as that is the function which moves and checks collisions per bullet.

So the general structure should be:
1. Your main.cpp file should call Bullet::Update(dt) and Bullet::Render() in the appropriate places. These are static functions.
2. The Bullet class will the interact through all bullets in the array. When updating we call _update() on each bullet, for rendering we call window.draw()
3. _Update() checks if the bullet is on screen. If so, it moves it up or down as appropriate and looks for collisions with our Ships.
4. If it hits anything, it calls the Explode() function on it, which should... you know, make it explode.
5. The Player should check for a key press in it's Update function, and call Bullet::Fire to shoot a bullet.
6. Bullet:Fire will get the next bullet off the array, set it to the correct position and mode, and assign the correct sprite to it
7. Now it's on screen, Step 1 will make it move and render!

### Bullet Timing and Explosion fade


We noticed earlier that there is no limit to how fast a player could shoot -- let's remedy that now.

My favourite way of doing this is keeping a 'cooldown' timer.

```cpp 
//ship.cpp
void Player::Update(const float &dt) {
  ...
  static float firetime = 0.0f;
  firetime -= dt;
  ...
  if (firetime <= 0 && Keyboard::isKeyPressed(controls[2])) {
      Bullet::fire(getPosition(), false);
      firetime = 0.7f;
    }
}
```

Every time we fire, we put the timer up by .7 seconds. Every Update() we reduce this by dt. This means that we should only be able to fire from the player every .7 seconds. You could image some form of power-up that would modify this timer.

## Invader shooting

Invaders should shoot somewhat randomly. How you do this is up to you, when it comes to something like this, it's usually a case of trying out magic numbers until you find something that looks good.

My hacky / beautiful solution was this:

```cpp 
//sghip.cpp
void Invader::Update(const float &dt) {
  ...
  static float firetime = 0.0f;
  firetime -= dt;
  ...
  if (firetime <= 0 && rand() % 100 == 0) {
      Bullet::Fire(getPosition(), true);
      firetime = 4.0f + (rand() % 60);
  }
}
```

I've limited so an invader won't be able to fire more than once in four seconds. Bonus points for coming up with a solution wherein only the bottom row of invaders shoot, and as the invaders numbers dwindle, they fire more often.

## Fading the Explosion sprite

At the moment our implementation turns a invader into the explosion sprite when it explodes, an the explosion remains in space. We need it to fade out over time. To do ths we will use a similar technique as the bullet timer, where we will have a cooldown timer that starts when the ship explodes, and once the timer hit's 0, the invader is moved off the screen or turned invisible.

I'll leave the code for this up to you.

## Future / Advanced tasks.

-   Game over screen
-   Restarting the Game
-   Animated sprites
-   Green shield bases


---
Previous step: [Space Invaders 1](spaceinvaders_2)

Next step: [Tile Engine 1](tile_engine_1)
