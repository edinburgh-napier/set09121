---
title: "PacMan "
keywords: PACMAN
tags: [PACMAN]
permalink:  pacman1.html
summary: PACMAN
sidebar: home_sidebar
---

# Pacman
or: Engine Abstraction and the Entity Component Model
<video class="middle" width="400" height="300" loop autoplay>
  <source src="assets/videos/pacman.mp4" type="video/mp4">
</video>


## First Steps

1.  Create a new project folder and add to CMake
2.  Start with the usual blank gameloop example.
3.  Create an abstract Entity Class.
4.  Create a player class that inherits from Entity
    - moves around screen with keyboard keys
     should be drawn as a yellow sf::shape
5.  Create a \"ghost\" class, inherits from Entity.
    - should move around the screen randomly
    - should be a different colour to the player.
6.  Main.cpp should create one player, and four ghosts, and store all in a `vector<Entity*>`
    - should only call Update() and Render() on vector
    - all entity logic should be inside the entity classes.

The player and ghosts don't need any functional game logic behind them right now. Just make sure they are rendering and moving about before continuing on.

## Engine Abstraction

This lab is where our game code starts to look more like a game *engine*. This is inevitable once a game reaches a certain level of complexity. Pacman just tips over that threshold where we can justify using some major-league game software architecture.

### Entity Management

Games will have thousands of Entity's in flight, keeping them all in one global vector is not a good idea. Having multiple lists - relevant to their use is a better idea. This is a topic that strays instantly into optimizations, which really depend on the game you are making.

What we are going to go for in this lab is collating all the entities by the scene/level they are in. for pacman, we will just have two scenes: `Game` and `Menu`.

To move pacman to this paradigm is not going to take much of a change in code. Here's it is:

```cpp
//Entity.h
struct EntityManager {
  std::vector<std::shared_ptr<Entity>> list;
  void update(double dt);
  void render(sf::RenderWindow &window);
};
```

The implementation of the two functions do exactly what you would expect, loop through the vector and update/render all Entities. Replace your Global entity vector in main.cpp with an `EntityManager` called 'em', and insert both the player and ghosts into via `em.list.push_back()`. Swap out the calls to Update and Render to the pass through the manager instead.

You may be wondering why we even bothered doing this. We took simple code and made it more complex.

The point is to move Entity management logic to a more appropriate place. This allows us to split our code into that we can expand upon and re-use later. This is the first small step on a big journey.

### Render system

An annoying feature of our code right now is how we render Entities. Passing a reference around to the windows seems a little wrong seeing as the variable never changes. Let's fix this and pave the way for a new cool system.

#### The Renderer

In most game engines, the system that handles rendering things is usually the largest and arguably the most important. For us, we only need to pass what we want to draw to SFML and it handles it all. You can bet the internals window.draw() function is pretty damn impressive (it is, go and look). We don't *need* a complex rendering system on-top of, but we'll build something anyway, if only to point out how a more complex system would do things. If we weren't using SFML this is where things would get real complicated quickfast.

Our Render system will have a simplified Render() function that will take in a sf::Drawable object (e.g sprite,shape,text), and add this to a list of things to render.

The big difference here is that things won't be rendered immediately. The list will be built up of objects as each Render() function is called on all the Entities.

Once this process completed, we sent it all to SFML all at once.

The benefits to this is that we can keep track of how many things we are rendering per frame easily. More importantly it allows us to do optimisations. If z-order were important we could re-order the list to draw background objects first. Or do some form of debug-culling to stop certain object types of rendering. All useful stuff.

Again, if we were working with OpenGL or a more complex render system, this is were we would do some serious work. The reality is that SFML does almost everything for us so we don' actually have much to do here.

```cpp
//system_renderer.h
#pragma once
#include <SFML/Graphics.hpp>

namespace Renderer {
    void initialise(sf::RenderWindow &);
    sf::RenderWindow &getWindow();
    
    void shutdown();
    void update(const double &);
    void queue(const sf::Drawable *s);
    void render();
}; 
```

```cpp
//system_renderer.cpp
#include "system_renderer.h"
#include <queue>

using namespace std;
using namespace sf;

static queue<const Drawable *> sprites;
static RenderWindow *rw;

void Renderer::initialise(sf::RenderWindow &r) { rw = &r; }

sf::RenderWindow &Renderer::getWindow() { return *rw; }

void Renderer::shutdown() {
  while (!sprites.empty())
    sprites.pop();
}

void Renderer::update(const double &) {}

void Renderer::render() {
  if (rw == nullptr) {
    throw("No render window set! ");
  }
  while (!sprites.empty()) {
    rw->draw(*sprites.front());
    sprites.pop();
  }
}

void Renderer::queue(const sf::Drawable *s) { sprites.push(s); }
```

All that's left to do is Initialise the system from main.cpp, and call Renderer::render(); a the end of the main render() call.

Now whenever we need to render anything we can call something like.

```cpp
Renderer::queue(&text);
```

{:class="important"}
You might need to use .get() on the unique_ptr in your player or ghost.cpp if you are following along correctly!

## Scene Management

As mentioned above, we will have two 'scenes'. What is a Scene? Mainly it's a collection of Entities. All of the game logic will mainly be inside the Entities, but there will be some global game logic that needs to run, and the scene class is where it shall be.

The scene should also be responsible for loading and unloading content that it needs. This is where we could implement a loading screen. This would run while a scene loads, and then display the scene once finished. We shouldn't be working with anything that will need a loading screen, simply freezing for a few frames while we transition scenes is acceptable (for now ... hint hint).

Here we have our scene class. It has our usual two Update and Render functions, and an internal EntityManager. Additionally we have a Load() function and a public getter to the entity list.

```cpp
//scene.h
#include "Entity.h"
class Scene {
public:
  Scene() = default;

  virtual ~Scene() = default;
  virtual void update(double dt);
  virtual void render();
  virtual void load() = 0;
  std::vector<std::shared_ptr<Entity>> &getEnts();

protected:
  EntityManager _ents;
};
```

hint: Here's the base Scene render(). You should be able to define the other functions yourself.

```cpp
//pacman.cpp
void Scene::render() { _ents.render(); }
```

We should instantiate our two scenes. We should do this away from main.cpp -- in a separate header that contains only Pacman related things: `pacman.h`. As these are extern'd, define them in `pacman.cpp`.

```cpp
//pacman.h
#include <memory>
#include "scene.h"

extern std::shared_ptr<Scene> gameScene;
extern std::shared_ptr<Scene> menuScene;
extern std::shared_ptr<Scene> activeScene;
```

### The Menu scene

We've declared the two scenes we need as pointers to the base Scene class, when it comes to implementation, they will actually be two different classes. We should declare them in pacman.h and define them in pacman.cpp.

We will bring along any gameplay variables that would normally be in the global scope of main.cpp, and have them as private properties in each scene. For the Menu, this is just a single sf::Text.

```cpp
//pacman.h

class MenuScene : public Scene {
private:
  sf::Text text;

public:
  MenuScene() = default;
  void update(double dt) override;
  void render() override;
  void load()override;
};
```

```cpp
//pacman.cpp
void MenuScene::update(double dt) {
  Scene::update(dt);
  text.setString("Almost Pacman");
}

void MenuScene::render() {
  Renderer::queue(&text);
  Scene::render();
}

void MenuScene::load() {
//Set up the text element here!
}
```

{:class="important"}
Remember! For the text to show you will have to load and assign a font! Remember Pong?

### The Game scene

For the main game-play scene, we will have an extra method: Respawn() and a scoreClock. This is all we need for global game logic in the scene, the entities handle everything else.

```cpp
//pacman.h
class GameScene : public Scene {
private:
  sf::Text text;
  sf::Clock scoreClock;
  void respawn();

public:
  GameScene() = default;
  void update(double dt) override;
  void render() override;
  void load() override;
};
```

The ghosts and player that are still stored in a global EntityManager should now be moved into the GameScene. Each scene has it's own EntityManager, stored privately as `_ents`. Do the Entity creation in the Load() function. I.e:

```cpp
void GameScene::load() {
  ...
  auto player = ...
  _ents.list.push_back(player);
  for(4 ghosts){
    auto ghost = ...
    _ents.list.push_back(ghost);
  }
  ...
}
```
### Instantiating the scenes

All that's left to do is actually Instantiate the two scenes, we do this in the main Load function. We can really start to see how we are separating out the logic to the different systems, with main.cpp becoming a small part that glues it all together.

```cpp
//main.cpp
void Load() {
  // Load Scene-Local Assets
  gameScene.reset(new GameScene());
  menuScene.reset(new MenuScene());
  gameScene->load();
  menuScene->load();
  // Start at main menu
  activeScene = menuScene;
}
...
void Update() {
  static Clock clock;
  float dt = clock.restart().asSeconds();
  activeScene->update(dt);
}
...
void Render(RenderWindow &window) {
  activeScene->render();
  // flush to screen
  Renderer::render();
}
```

### Changing scenes

Switching between the scenes is done with the variable 'activeScene'.

```cpp
//pacman.cpp
void MenuScene::update(double dt) {
    if (Keyboard::isKeyPressed(Keyboard::Space)) {
        activeScene = gameScene;
    }
    Scene::update(dt);
    text.setString("Almost Pacman");
}
...
void GameScene::update(double dt) {
    if (Keyboard::isKeyPressed(Keyboard::Tab)) {
        activeScene = menuScene;
    }
    Scene::update(dt);  
    ...
}
```

### Checkpoint

After this long round trip, implementing a new render system, An entity manager class, a Scene class, and creating two scenes, we should be back to where we begun. The game will start to the menu scene, where you should see the text "Almost Pacman" Drawn. Pressing Space will take you into the game scene, where our player and 4 ghosts will be on screen and moving around. Pressing Tab will take us back to the menu. Pressing Escape will close the game down.

{:class="important"}
Make sure you have got here, and everything is working so far without any errors. Things are going to get a bit wild next. You should commit your code now.