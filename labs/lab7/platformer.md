---
title: "Platformer"
keywords: platformer
tags: [platformer]
permalink:  platformer.html
summary: platformer
sidebar: home_sidebar
---

<video class="middle" width="640" height="360" loop autoplay>
  <source src="assets/videos/platformer.mp4" type="video/mp4">
</video>

## Get the Code

There is a lot of code behind this practical, it would not be a good use of your time to get you to copy it all manually, so for this practical. You will be provided with most of the code, with some sections left for you to complete.

Get it here : [Repo (code)](https://github.com/edinburgh-napier/set09121/tree/master/code)

**You don't need to fork the repo or anything special, just download it in some way and then copy the code over to your usual lab repo.**

The code you have been given contains:
-   The skeleton code for lab7
-   The three libraries we have already written. Ecm, LevelLoader, Maths.
-   A new Library, the "Engine\". (Fully complete, no code to edit)
-   My completed CMakeLists.txt - for you to check against

## The Engine

The Engine library is built upon the knowledge that we gained working on Pacman. Where we started to separate out the engine logic from the game logic and build an abstracted API to interface with SFML. 
Most of the code within the engine should be familiar and understandable to you, it's mostly code we have written before. There are some scary new things, which we will cover in due time, like asynchronous level loading.

Don't see the engine code as something you should shy away from just because I've given it to you completed. Have a read, get stuck in, poke about and change some things.

### Changes

There have been some changes to the other libraries, some bugfixes, and some small additions that wouldn't have made sense when we first built them. However it was necessary to do some tweaks to help build the engine. This is why I have also given you the code to the three helper libraries. Feel free to copy them wholesale to keep in sync.

Summary of big changes
-   ECM: Entities can have String Tags associated with them, scenes can be searched for entities with tags.
-   LevelSystem: A compression stage happens before generating sprites. This groups similar tiles into one large sprite. This help massively with rendering and physics performance.
-   LevelSystem: Can now read, parse, and lookup any character from a level file. The ENUM is still there and working to give helpful names to certain values


## The Game Loop

Let's step through our new execution environment, starting with `platformer.cpp`

```cpp
//platformer.cpp
#include "engine.h"
#include "game.h"

using namespace std;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;

int main() {
  Engine::Start(1280, 720, "Platformer",&menu);
}
```

Wow, now that's simple. Obviously we have 'magic'd' away all the work we used to have to do. It's all down in the Engine.cpp. The point here is that we are in **Game** code, which shouldn't need to care about the implementation underneath. Game code shouldn't care if we are using SFML or some other rendering framework, game code should only care about Game logic.

In practise there isn't this perfect abstraction, we still include plenty of SFML headers throughout the game code. We could replace all this with an engine abstraction layer, but that would be over-engineering and possibly a performance hit. It's that balance of clean-software engineering versus optimal and fast code again, only your experience building these systems can guide you to making the right architecture decisions.

Let's take a peak at what that Engine::Start call did down in the Engine:

```cpp
//"engine.cpp"

void Engine::Start(unsigned int width, unsigned int height,
                   const std::string& gameName, Scene* scn) {
  RenderWindow window(VideoMode(width, height), gameName);
  _gameName = gameName;
  _window = &window;
  Renderer::initialise(window);
  Physics::initialise();
  ChangeScene(scn);
  while (window.isOpen()) {
   // Usual Game loop stuff
  }
  if (_activeScene != nullptr) {
    _activeScene->UnLoad();
    _activeScene = nullptr;
  }
  window.close();
  Physics::shutdown();
}
```

There's our usual game loop! It's never that far away.

Some new init and shut-down code has been added, and we have `_activeScene` to keep track of the current level that should be updated and rendered.

That `ChangeScene` call is new, let's take a look at what that does:

```cpp
//"engine.cpp"
void Engine::ChangeScene(Scene* s) {
  cout << "Eng: changing scene: " << s << endl;
  auto old = _activeScene;
  _activeScene = s;

  if (old != nullptr) {
    old->UnLoad();
  }

  if (!s->isLoaded()) {
    cout << "Eng: Entering Loading Screen\n";
    loadingTime =0;
    _activeScene->LoadAsync();
    loading = true;
  }
}
```

What we have here is a system to switch scenes, first it un-loads the current scene, then loads the new scene. Scenes have two loading methods, Load() and LoadAsync(). Let's talk about that.

## Scene Loading

### Synchronous loading

`Scene.Load()` Does what you would expect. It's a normal function doing normal things for normal people. It's basically a constructor for a scene, that can be called repeatedly (note: scene's constructors are deleted, you can't call them). A scene shouldn't be `Updated()` or `Rendered()` until `Load()` has completed and `Scene.isLoaded() == true`;

### Asynchronous loading

`Scene.LoadAsync()` Shifts us into another dimension of software engineering: Concurrency. Simply put, this function calls the standard `Load()` function, *but in the background*. This means we can do something else while it is loading, like update and render a loading screen!


### Games and Concurrency

This is a big and complicated topic, and we have a separate module just for this
[Concurrent and Parallel Systems](https://github.com/edinburgh-napier/set10108)

This is just a small toe-dip into the topic, and the code is already all written for you. What we are dealing with here is Multithreading. Using more than just one of those CPU cores that we have to gain us some more performance.

Multi-threading for games is a bit tricky, some things can be parallelised easy(background asset loading), some things are almost impossible(Updating a complex set of interconnected Entities). It's not a silver bullet to give us more FPS. But for developers on current-gen consoles, it's a must-have to squeeze out every last drop of performance out of the hardware.

#### Checking up on the background task

While the level is loading in the background, we need to know when it is finished. For this I have used a `std::future` guarded by a `std::mutex` inside `Scene::isLoaded()`. Don't worry about what that means or how that works for now, just know why that code is there.

#### Debugging

Important tip!

If your game crashes or throws an exception while loading Asynchronously, it can be difficult to debug. In this case, switch out the line in `Engine::ChangeScene` from

`_activeScene->LoadAsync();`

to

`_activeScene->Load();`

To temporarily disable background loading (Loading screen will never show, and game will hang as the scene loads).


## Scene 1

With *how* Levels are loaded covered, take a look at scene_level1.cpp. This is where our Game Kicks off (scene_menu is first, but that's boring).

```cpp
//"scene_level1.cpp"
void Level1Scene::Load() {
  ls::loadLevelFile("res/level_1.txt", 40.0f);
  ...

  // Create player
  {
    player = makeEntity();
    ...
    player->addComponent<ShapeComponent>();
    player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
  }

  // Add physics colliders to level tiles.
  {
    auto walls = ls::findTiles(ls::WALL);
    for (auto w : walls) {
      ...
      auto e = makeEntity();
      e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
    }
  }

  //Simulate long loading times for this scene, so we see the loading screen
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  setLoaded(true);
}
```


All the work is in the Load(). The Update() does very little -- only checking to see if the player has reached the end. This is the beautify of the Entity Component Model. Just build our level from your box of parts(components) and the game logic makes itself (almost Emergent behaviour).


## Components

We have some familiar components in use: ShapeComponent, which is unchanged. The two new components on the scene are PhysicsComponent and it's child: PlayerPhysicsComponent. This is how we are going to integrate Box2d into our engine, as components.

There is a PhysicsSystem located in the engine/system\_physics.cpp. This doesn't do much other than house some maths functions and the world.Step(); Most of our Box2D code lies in the components.

### Physics Component

The base PhysicsComponent in cmp\_physics.cpp is perhaps our most complex component yet. Many of it's function are getters and setters to interface correctly with SFML and Box2d maths, and as an interface to change physics properties.

The constructor does the same logic that we had in the physics practical, creating either a static or dynamic body.

Then there are some new functions such as isTouching() and getTouching(), which we can use to interface gamelogic with collisions. Frustratingly the B2D API flips interchangeably between C and C++.

```cpp
//Some of the B2D code is done in a C++ way:  
_fixture->getSomething()->doSomething();

// and some code is bit oldschool
Thing thing;
_fixture->getSomething(&thing);
thing.doSomething();
```

Keep a lookout for this when dealing with B2D directly, and take care with raw pointers.

#### Movement Gotcha

Take a look at the Update():

```cpp
void PhysicsComponent::update(double dt) {
  _parent->setPosition(invert_height(bv2_to_sv2(_body->GetPosition())));
  _parent->setRotation((180 / b2_pi) * _body->GetAngle());
}
```

This is how the physics world is linked to the SFML/Entity world. This happens every frame. There is one huge problem with this:
-   The PhysicsComponent is now in charge of the Entities position.
-   If anything modifies the Entities position (i.e another component), the physics component will overwrite this change with it's own value copied form the physics world.

We *can* still move Entities manually, by calling PhysicsComponent.teleport(), but we have to *know* that an Entity has a physics object beforehand. 

This gets complex if we wanted this logic inside a component. What we have done is made certain component *incompatible*. I haven't included a solution to this in the current Engine, as we don't need it. For your coursework you may run into this problem, there are a few solutions, but I'll leave that to you to figure out.

So that's the Physics Component, it does a lot, but nothing complicated. It's Child PlayerPhysicsComponent is where things get weird.


### Player Physics Component
Inheriting from PhysicsComponent, the PlayerPhysicsComponent is what drives the player.

Inside you will find:
-   A constructor - Set's some relevant physics flags, nothing fancy
-   A rather complicated Update()
-   bool isGrounded() - detect if the player is standing on something.

#### The Player Update

As described in the lectures, players characters usually don't obey the rules of physics. They *look* like they do, but they cheat and bend physics to make the game *feel* responsive and fast.

See: [Tommy Refenes' on the physics of Super Meat Boy](https://youtu.be/QVpSIdWE0do?t=43m55s)

To make the player *feel* right, we do several things:
-   Dampen X Movement if not moving left or right (essentially apply handbrake to player if no keys are pressed)
-   Apply impulse to move left or right, only if currently going slower than a max velocity.
-   Jump, only if on the ground.
-   Kill all Y velocity at the start of the jump
-   Jump teleports slightly upwards first, then applies an impulse
-   If not on the ground - player has no friction
-   After everything - clamp velocity to a maximum value


None of these things are an industry standard, it a method that I've adopted after doing this a few times. There are better ways, but this way *works*, but may not work for your game.


## Run The Scene

You shouldn't need to edit anything to get the Menu and first level running. Give it a go and bounce around.

## Scene 2

There is some code missing form the scene\_level2.cpp file for you to complete.
This scene incorporates a non-physics moving object (using EnemyAIComponent), and a cool new Turret (EnemyTurretComponent) that fires physical bullets. There are two more new components in use here:
-   The BulletComponent is just a countdown timer that deletes \_parent when it reaches 0;
-   The HurtComponent, check for (fake) collision with the player and kills the player.

## Scene 3

The same goes for Scene 3. This scene is easier than Scene 2, just the player and physics components to add.

## Phew!

Look at that, you've now going a working game, with multiple levels. Cool, huh? Remember: all of what is here is designed to be modified, broken, fixed, used, altered, changed, tweaked, and generally messed around with. Want a different type of movement for your game? Go add it then! Want new physics objects? Well, you can use Box2D can't ya?

The ball really is in your court from now on: go make something awesome!

Previous step: [Physics](physics)

Next step: [Steering 1](lab8_1)