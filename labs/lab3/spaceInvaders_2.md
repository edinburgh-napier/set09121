---
title: "Space Invaders 2 - Writing Some Real Code"
keywords: SpaceInvaders
tags: [SpaceInvaders]
permalink:  spaceinvaders2.html
summary: OO
sidebar: home_sidebar
---

## Creating the Ship Class

It's been a long journey since we wrote some space invaders code, let's get back to it. As with all software projects, as the complexity of the software grows, so do the potential different ways to implement it. That is to say, this may not be the best way to implement space invaders, we like to think it's at least a 'good' way. The point of building it this way is to expose you to many different aspects of C++ OO. Any programmer worth their salt will have an opinion on how they could improve someone elses code, and if you feel at the end of this that you have some ideas, then this lesson was successful.

### Let's go OO

We are going to need at least two different entities for our invaders game. Invaders and the player. Invaders are all identical other than their starting position and sprite. They also exhibit some non-trivial individual logic. Your software engineering brain should be starting to form the basis for properties and methods of the invader class by this point. To add to the fun, consider the player, and how similar it is also to the invader. they both shoot bullets, move, and can explode. This sounds like inheritance should be joining this party.

The way we are going to go about this to have an *abstract base class*
**Ship**, which is inherited from by a **Player** class and an
**Invader** Class.

### Functionality of the Ship

The ship class will contain all logic that is common for both the player and invaders. Primarily this will be \"moving around\". We could go with the full entity model and have Ship be a base class, with variables for it's position and rotation and such. We would then also have a sf::Sprite member attached where we would call upon all the SFML render logic. This is a good idea -- for a larger game. For space invaders that would involve lot's of code to keep the sprite in sync with the ship Entity. Instead we are going to take a super short cut, and inherit for sf::sprite.

This means that Ship will have all teh same methods as a sf::sprite, including all the usual 'SetPostition()' and 'move()' commands we have been using already. It also means we can pass a ship object directly to window.draw().

### Create Ship.h

Create a file inside the invaders source folder called "ship.h". This will be our Header file for the Ship class. Header files contain the declaration of our class, i.e only the function declarations. Headers shouldn't contain any code (some common exemptions apply). the reason we do this is to keep the logic of the class stored inside a .cpp file, any piece of code that want's to access this functionality only needs the header. This concept does not exist inside java or C#, wherein you provide the full definition of a function inside a class in one file. the code runtime parses this and allows other classes to link to it. C++ is not so nice, and while this is totally possible to work in this fashion, we get into issues regarding name-space collisions, scope issues, multiple declarations, and code bloat. Ask in the lab if you would like to know more.

Anyway, Inside Ship.h get this written down:

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
    //Pure virtual deconstructor -- makes this an abstract class
    virtual ~Ship() = 0;
    //Update, virtual so can be overrided, but not pure virtual
    virtual void Update(const float &dt);
};
```

### Create Ship.cpp

Next to our ship.h, create ship.cpp

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

We are defining some common variables here as constant, which is fine. The interesting bit is the 'extern spritesheet', this tells anyone that includes game.h that a sprite-sheet exists 'somewhere". That somewhere is main.cpp, and the compiler will figure this out for us when we need to access it from ship.cpp.

**Remember to reload CMake via Zero\_check to add our new files to the build**

### Test out the code

As Ship is an abstract class, we can't create one. Ee can only create a concrete class derived from it. We can reference it as pointer however, due to how c++ polymorphism works. Add the following to the top of your main.cpp

```cpp 
//main.cpp
#include "ship.h"
//...
std::vector<Ship *> ships;
```

This should compile without errors.

## Making the Invader class


We could create a new invader.h and invader .cpp to house the invader class. Generally speaking separate files for separate classes is a good idea, although unlike Java we don't *have* to do this. In some situations when certain classes are very similar or just slightly different version of each other it makes sense to host them in the same Header file.

Add the following to code

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
Load(){...
Invader* inv = new Invader(sf::IntRect(0, 0, 32, 32), {100,100});
ships.push_back(inv);
```

Important note, we used the New() operator here, which created the ship on the heap. If we wanted a stack version, we omit the new New and would use Invader 'inv = Invader()'.

As we are storing the invader into a vector of ships, which will also later contain the player, this vector be . The way we have set this up we couldn't even create a vector$<$ship$>$, as that would try to construct an abstract class.

We need to call the update function of all our ships every frame, due to polymorphism this is very simple. Update() is a virtual function so when we call update() on a ship pointer that points to an invader, the invader's update() is called.

```cpp
//Main.cpp
Update(){...
  for (auto &s : ships) {
    s->Update(dt);
  };
```

The same goes for rendering, as we have inherited from sprite, SFML can
render our ships natively.

```cpp
//Main.cpp
Render() {...
  for (const auto s : ships) {
    window.draw(*s);
  }
```

### Invader movement

A quirk of space invaders is that all the invaders move as one, when any of the invaders touches the edge of the screen: all invaders drop down and reverse direction. When invaders are killed, the remaining invaders speed up. From this we can gather that we need some form of communication medium between all the invaders so they can communicate when it's time to drop down and when to speed up. We are going to store these parameters as two variables: direction and speed. We could store these as properties in each invader, but as the contents will be identical for each invader we should do something better. The \"something better\" is static properties.

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

We can access these variables anywhere like so 'invader::speed = 20.f'.

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

As we are nowaccessing the ships array, we now need to include `game.h`, and we should put a handle to it in game.h.

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

You should know how to add in the movement code, it's almost identical to pong. Bonus points for not allowing it to move off-screen. You should construct one player at load time. You could add it to the vector of ships, but rember the hacky line in invader's update: `ships[i]->move(0, 24);`? This wuold also move the player. Not good. To solve this you can either 
 - A: Have the player seperate fomr the ship list, and manually update and render it.
 - B: Change the invader update to only move invaders down.
 
## Bullets

The game wouldn't be very difficult (or possible) without bullets firing around. Let's look at our requirements:
- Invaders shoot green bullets downwards
- The player shoots white bullets upwards
- The bullets explode any ship they touch
- After exploding the bullets disappear

If we look at our sprite-sheet we have two different bullet sprites. SFML can do some colour replacement, so if we wanted we could use the same white sprites for both bullet types and get SFML to colour tehm differently. However we want the two bullet types to look physically different so we will use two different sprites. So for wahtevr we decide to go with for our software design, we will be inheriting from sf::sprite again.

That's rendering out of the way, now just movement and explosions to figure out. It would be tempting to do as we did with Ship and have two sublcasses for invader and player bullets. But as they are both so similar, the only difference being the direction they travel and who they collide with, having a three class structure would be overkill. Sometimes rigorously following OO patterns isn't the best way forward. So instead we will build just bullet class.

Here we go, create a bullet.h and bullet.cpp

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

### Firing bullets

Alrighty, that's a barebones Bullet created, now to spawn one. The simplest way to do this would be to do something like this:


```cpp 
//ship.cpp
Player::Update(){...
if (Keyboard::isKeyPressed(...)) {
  new Bullet(getPosition(), false);
}
```

This is not a good idea however, there is three major problems.

1. we will spawn thousands of bullets, we need a way to 'cooldown' the weapon of the player 
1. How do we update and render them? We should store our bullets somewhere. 
1. This is a big one, we put these bullets on the heap, and then forget about them, ayy'oh that's a **Memory Leak**.

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

Yes, we will be firing loads of bullets, and we don't want to have keep track them all and delete them. In larger games this would cause performance issues. It won't here, but let's pretend we are running on original 80's hardware, we've gotta do better, or else the arcade will have to shut-down and the kids will be sad.

### A different solution - Bullet Pools

How about instead of creating bullets as and when we need them, we allocate a whole bunch at the start, and put them into a \"pool of available bullets\". When a player or an invader fires, an inactive bullet in the pool gets initialised to the correct position and mode and goes about it's bullet'y business. After this bullet has exploded or moved off-screen, it is moved back into the pool (or just set to \"inactive\").

This is a very common technique used in games with lots of expensive things coming into and out of existence. Almost every AAA Unity£D game uses this with GameObjects -- which take forever to allocate and construct. It's much quicker to allocate loads at the start and re-use them.

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

We have statically allocated 256 bullets on the stack. We have brought along a sneaky unsigned char to do a clever trick to determine which bullet to use next. Unisgned chars go between 0 and 255, and then wrap round back to 0 and repeat. Therefore every time we Fire() a bullet we choose from the array like this \"bullets\[++bulletPointer\]\". If there were ever more than 256 bullets on screen we will run into trouble, but I won't worry about this if you don't.

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
  static Fire(const sf::Vector2f &pos, const bool mode);
  
  ~Bullet()=default;
Protected:
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

This code will require modification to our ship class. Also we need access to a pointer to the player ship so we can determine the types of collisions. My way of doing this would be to add it as another extern in game.h. We need to introduce Explode behaviour into the ship classes. We will add the common functionality to the base Ship class - turning into the explosion sprite. The invader class will extend this by increasing the speed of other invaders, add removing the explosion sprite after a second. The player ship will end the game if explode is called on it. Which will trigger a game reset.


```cpp 
//ship.h
class Ship : public sf::Sprite {
    ...
    protected:
      ...
      bool _exploded;
    public:
      ...
      bool is_exploded() const;
      virtual void Explode();
};
```


```cpp 
//bullet.cpp
void Ship::Explode() {
    setTextureRect(IntRect(128, 32, 32, 32));
    _exploded = true;
}
```

### Bullet Timing and Explosion fade


We noticed earlier that there is no limit to how fast a player could shoot -- let's remedy that now.

My favourite way of doing this is keeping a 'cooldown' timer.

```cpp 
//sghip.cpp
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
  f (firetime <= 0 && rand() % 100 == 0) {
      Bullet::fire(getPosition(), true);
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

Next step: [Space Invaders 3](spaceinvaders_3)
