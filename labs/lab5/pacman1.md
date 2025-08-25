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


The goal of this practical is create our first a engine library and and then apply the entity component model to implement pacman.


## Part I: Engine Abstraction


This lab is where our game code starts to look more like a game *engine*. This is inevitable once a game reaches a certain level of complexity. Pacman just tips over that threshold where we can justify using some major-league game software architecture.

The first step will be to create a new folder *engine* and move the *game_system.hpp* and *cpp* in it. Then, you will create a new library called engine in the CMakeLists.txt. Exaclty like we did with the tile level library in the previous lab. 

Then, we will add two new elements: an **entity manager** and a **renderer**

### Entity Management

Games will have thousands of Entity's in flight, keeping them all in one global vector is not a good idea. Having multiple lists - relevant to their use is a better idea. This is a topic that strays instantly into optimizations, which really depend on the game you are making.

What we are going to go for in this lab is collating all the entities by the scene/level they are in. for pacman, we will just have two scenes: `Game` and `Menu`.

To move pacman to this paradigm is not going to take much of a change in code. Here's it is:

```cpp
//entity.hpp
struct EntityManager {
  std::vector<std::shared_ptr<Entity>> list;
  void update(double dt);
  void render(sf::RenderWindow &window);
};
```

The implementation of the two functions do exactly what you would expect, loop through the vector and update/render all Entities. Replace your entity vector *_entities* in the *Scene* class of  game_system.hpp with an `EntityManager` called 'em', and insert both the player and ghosts into via `em.list.push_back()`. Swap out the calls to Update and Render to the pass through the manager instead.

You may be wondering why we even bothered doing this. We took simple code and made it more complex.

The point is to move Entity management logic to a more appropriate place. This allows us to split our code into that we can expand upon and re-use later. This is the first small step on a big journey.

### Render system

An annoying feature of our code right now is how we render Entities. Passing a reference around to the windows seems a little wrong seeing as the variable never changes. Let's fix this and pave the way for a new cool system.

#### The Renderer

In most game engines, the system that handles rendering things is usually the largest and arguably the most important. For us, we only need to pass what we want to draw to SFML and it handles it all. You can bet the internals window.draw() function is pretty damn impressive (it is, go and look). We don't *need* a complex rendering system on-top of, but we'll build something anyway, if only to point out how a more complex system would do things. If we weren't using SFML this is where things would get real complicated quickfast.

Our Render system will have a simplified render() function that will take in a sf::Drawable object (e.g sprite,shape,text), and add this to a list of things to render.

The big difference here is that things won't be rendered immediately. The list will be built up of objects as each render() function is called on all the Entities.

Once this process completed, we sent it all to SFML all at once.

The benefits to this is that we can keep track of how many things we are rendering per frame easily. More importantly it allows us to do optimisations. If z-order were important we could re-order the list to draw background objects first. Or do some form of debug-culling to stop certain object types of rendering. All useful stuff.

Again, if we were working with OpenGL or a more complex render system, this is were we would do some serious work. The reality is that SFML does almost everything for us so we don't actually have much to do here.

```cpp
//renderer.hpp
#pragma once
#include <SFML/Graphics.hpp>

namespace Renderer {
    void initialise(sf::RenderWindow &);
    sf::RenderWindow &get_window();
    
    void shutdown();
    void update(const float &);
    void queue(const sf::Drawable *s);
    void render();
}; 
```

```cpp
//renderer.cpp
#include "renderer.hpp"
#include <queue>

using namespace std;
using namespace sf;

static queue<const Drawable *> sprites;
static RenderWindow *rw;

void Renderer::initialise(sf::RenderWindow &r) { rw = &r; }

sf::RenderWindow &Renderer::get_window() { return *rw; }

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

Now we have our *renderer* and *entity manager*, we need to modify our *Scene* and *GameSystem* classes to use them. the render and update function from Renderer need to be called in the update and render functions of the game system. The render functions of Scene and GameSystem does not need to take the window as argument. I let you find all the other changes.

### The Menu scene

We've declared the two scenes we need as pointers to the base Scene class, when it comes to implementation, they will actually be two different classes. We should declare them in pacman.h and define them in pacman.cpp.

We will bring along any gameplay variables that would normally be in the global scope of main.cpp, and have them as private properties in each scene. For the Menu, this is just a single sf::Text.

```cpp
//scenes.hpp
class MenuScene : public Scene {
private:
  sf::Text text;

public:
  MenuScene() = default;
  void update(const float &dt) override;
  void render() override;
  void load()override;
};
```

```cpp
//scenes.cpp
void MenuScene::update(const float &dt) {
  Scene::update(dt);
  text.setString("Almost Pacman");
}

void MenuScene::render() {
  Renderer::queue(&text);
  Scene::render();
}

void MenuScene::load() {
//Set up the text element here!
...
}
```

{:class="important"}
Remember! For the text to show you will have to load and assign a font! Remember Pong?

### The Game scene

For the main game-play scene, we will have an extra method: Respawn() and a scoreClock. This is all we need for global game logic in the scene, the entities handle everything else.

```cpp
//scenes.hpp
class GameScene : public Scene {
private:
  sf::Text text;
  sf::Clock scoreClock;
  void respawn();

public:
  GameScene() = default;
  void update(const float &dt) override;
  void render() override;
  void load() override;
};
```

The ghosts and player that are still stored in a global EntityManager should now be moved into the GameScene. Each scene has it's own EntityManager, stored privately as *_entities*. Do the Entity creation in the Load() function. I.e:

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

All that's left to do is actually instantiate the two scenes, we do this like in the previous practical. We can really start to see how we are separating out the logic to the different systems, with main.cpp becoming a small part that glues it all together.

```cpp
//main.cpp
int main(){
  Scenes::menu = std::make_shared<MenuScene>();
  Scenes::menu->load();
  Scenes::game = std::make_shared<GameScene>();
  Scenes::game->load();
  GameSystem::set_active_scene(Scenes::menu);
  GameSystem::start(param::game_width,param::game_height,"pacman");
  return 0;
}
``` 

### Changing scenes

Like in the previous lab, switching between the scenes is done the setter *GameSystem::set_active_scene()*.

```cpp
//pacman.cpp
void MenuScene::update(double dt) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      gs::set_active_scene(Scenes::game);
  }
    Scene::update(dt);
    text.setString("Almost Pacman");
}
...
void GameScene::update(double dt) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
      gs::set_active_scene(Scenes::menu);
  }
    Scene::update(dt);  
    ...
}
```

### Checkpoint

After this long round trip, implementing a new render system, an entity manager class, and package it as a libarry, we should be back to where we begun. The game will start to the menu scene, where you should see the text "Almost Pacman" Drawn. Pressing Space will take you into the game scene, where our player and 4 ghosts will be on screen and moving around. Pressing Tab will take us back to the menu. Pressing Escape will close the game down.

{:class="important"}
Make sure you have got here, and everything is working so far without any errors. Things are going to get a bit wild next. You should commit your code now. **READ THE NEXT SECTION, IT'LL HELP I PROMISE**

### Sanity Check

Now the structure of our code is more complex and intricate. Probably, some of you are confused by it. So, let's breakdown what is happening when update() and render() from the game system are called.

1. All our scenes inherit from our parent scene class, which contains an `EntityManager` struct called `_entities`
2. Each `_entities` contains a list that stores `Entity` objects within it, and has it's own internal update() and render() functions
3. These functions both foreach through that list and call the update() and render() functions on each individual Entity (which must inherit from Entity)
4. The update() functions do our gameplay work on each Entity, the render() functions call the render function of each Entity which add the appropriate sf::Drawable objects to the `Renderer` queue
5. Our GameSystem uses the shared pointer to our active scene and calls update() on it which causes all Entities to be updated as per above
6. Our GameSystem then calls the `Renderer` render() function, which goes through everything queued up and renders them all!
7. When we change scene, we just update the active scene, and then a different `EntityManager` with different `_entities` is called... which means different update() functions are called, and different Entities are put on the `Renderer` queue.

Phew... yeah, it's a bit complicated, and it seems like a lot of work for now, but this will make creating more complicated games later way easier as we've decoupled lots of stuff like sensible developers!

Previous step: [Tile Engine 2](tile_engine_2)

Next step: [PacMan 2](pacman2)