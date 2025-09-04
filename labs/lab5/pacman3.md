---
title: "PacMan 3 - AI"
keywords: PACMAN
tags: [PACMAN]
permalink:  pacman3.html
summary: PACMAN
sidebar: home_sidebar
---

AI will be covered in detail in later labs, for this we will be using a very basic state-machine and super simple path-finding. You may have noticed that we have brought in the level system library within the ActorMovementComponent. You should have already altered the CMake to link against this. We will be using the Level system to feed the Ghost AI with information about the level.

## Pacman Level

First up, let's get the level loaded and rendered. You can find it [here (assets/levels/maze_2.txt)](https://github.com/edinburgh-napier/set09121/tree/master/assets/levels).

```cpp
//"scenes.cpp"
void GameScene::load() {
...
 ls::load_level("resouces/pacman.txt", 25.0f);
...
}

void GameScene::render() {
  ls::render(Renderer::get_window());
  ...
}
```

Easily done, thanks to our well built level system. What we can do now is use this to change the spawn positions of our Entities.

**Before you can do this, you need to store a reference in the .h file for the player object, and some form of list for the ghosts so we can access it from other methods!**

```cpp
//"scenes.cpp"
void GameScene::respawn() {
 _player->set_position(ls::get_start_position());
 _player->get_compatible_components<ActorMovementComponent>()[0]
            ->set_speed(param::player_speed);

 std::vector<sf::Vector2i> ghost_spawns = ls::find_tiles(ls::ENEMY);
 for (size_t i = 1; i < _entities.list.size(); i++) {
    std::shared_ptr<Entity> &ghost = _entities.list[i];
    ghost->set_position(
        ls::get_tile_position(ghost_spawns[rand() % ghost_spawns.size()]));
    ghost->get_compatible_components<ActorMovementComponent>()[0]->set_speed(param::ghost_speed);
 }
 ...
}
```

This makes use of a new function `find_tiles()` which we haven't written yet, go an implement it into the level system library. Here's the declaration, you must figure out the implementation.

```cpp
//"level_system.h"
static std::vector<sf::Vector2i> find_tiles(Tile);
```

With this done, the player should be spawning at the bottom, and the ghosts randomly in the middle. The ActorMovementComponent has a `_valid_ove()` function which should stop both Entity types from moving into a wall. We are getting pretty close to a game now.

**Remember: a) you need to actually call respawn() somewhere and b) you might have to uncomment the _valid_move() check from earlier!**


## Ghost Movement


We could have the Ghosts chasing the player, for now we will do something easier. The ghosts will move along the level continuously, until they reach a corner or junction tile. They will then pick a random direction to turn and move in, they will never turn and move back the way they came.

For this to work we only need to store two additional properties in the ghost component: The current state, and current direction.

```cpp
//components.hpp
class EnemyAIComponent : public ActorMovementComponent {
  ...
protected:
  sf::Vector2f _direction;
  enum state {ROAMING, ROTATING, ROTATED };
  state _state;
...
}
```

The three states a ghost can be are as follows:

-   **ROAMING** - Happily moving along - not at waypoint

-   **ROTATING** - Currently Choosing a different direction to move

-   **ROTATED** - Keep moving until out of waypoint.

You may think that this is a little over-complicated, why do we need two different rotating states? The reason is that as we are moving as float coordinates -- not teleporting from tile to tile, a ghost will be inside a way-point for more than one frame. For this reason we need to store a flag to know that we have already rotated, and should not rotate again until the ghost exits the tile. There are many approaches to this, this is just my implementation, feel free to do something alternative.

### Picking a Direction

With our states figured out, we now move onto the code that transitions between them, the ghost 'AI'. Each update() the ghost will need to evaluate if it's time to change state, we will need a few variables to accomplish this (Lines 4 to 13). A switch statement forms the logic flow, starting with the current state. I've left two of the states to complete. 

{% raw %}
```cpp
//components.cpp
static const Vector2i directions[] = {{1, 0}, {0, 1}, {0, -1}, {-1, 0}};

void EnemyAIComponent::update(const float &dt) {
  //amount to move
  const float mva = static_cast<float>(dt * _speed);
  //Curent position
  const sf::Vector2f pos = _parent->get_position();
  //Next position
  const sf::Vector2f newpos = pos + _direction * mva;
  //Inverse of our current direction
  const sf::Vector2i baddir = -1 * sf::Vector2i(_direction);
  //Random new direction
  sf::Vector2i newdir = directions[(rand() % 4)];
  
 switch (_state) {
   case ROAMING:
     if (... )// Wall in front or at waypoint
     {
       .. // start rotate
     } else {
      ... //keep moving
     }
     break;
    
   case ROTATING:
     while (
        // Don't reverse
          ...
        // and Don't pick a direction that will lead to a wall
          ...
        ) {
          ... // pick new direction
        }
     _direction = sf::Vector2f(newdir);
     _state = ROTATED;
     break;
      
   case ROTATED:
     //have we left the waypoint?
     if (ls::get_tile_at(pos) != ls::WAYPOINT) {
        _state = ROAMING; //yes
     }
     move(_direction * mva); //No
     break;
 }
 ActorMovementComponent::update(dt);
}
```
{% endraw %}

### Collision

Pacman just isn't Pacman without dangerous ghosts. Detecting when a ghost has collided with the player could be done in a number of places. A well-engineered solution would be to have a \"collidable\" interface on the player, with each ghost checking itself against the player. Another approach would be to ship this out to a standalone physics and collision system.

The approach we are going to take is the most simple, doing the check in the scenes.cpp Update().

For this to work we need to keep a reference to both the player and the ghosts. the game scene does have an EntityList which contains both, and so we could iterate through that. But wouldn't it just be easier if we did this?

```cpp
class GameScene : public Scene {
  ...
private:
  std::shared_ptr<Entity> _player;
  std::vector<std::shared_ptr<Entity>> _ghosts;
```

```cpp
//"scenes.cpp"
void GameScene::load() {
...
  {
    ...
    _ents.list.push_back(player);
    _player = player;
  }

  for (int i = 0; i < param::ghost_count; i++) {
    ...
    _ghosts.push_back(ghost);
    _ents.list.push_back(ghost);
  }
```

Sometimes it's best to fall back on the simplest solutions. And here is
our collision check.


```cpp
//"scenes.cpp"
//inline function to compute the distance
auto vect_distance = [](sf::Vector2f a,sf::Vector2f b) -> float{
  return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
};
for (const std::shared_ptr<Entity> &ghost: _ghosts) {
  if (vect_distance(ghost->get_position(), _player->get_position()) < 30.0f) {
        respawn();
  }
}
```

To compute the distance between the ghost and the player, we define an inline function using the C++ **lambda function** syntax: `[](arg1,arg2,...) -> return type`. Check the C++ documentation for more information. Lambda functions are a good way to define functions that will be needed only once and to have a more readable code.

You'll have to build up the respawn() code to reset everything.

## Nibbles

One last thing to add, food for pacman.

We're deviating quite far from the proper game rules and going to have the pickup 'nibbles' speed Pacman(and the ghosts) up. We'll place a small white nibble on every EMPTY tile, and larger blue nibble on all the WAYPOINTs. the larger blue nibbles will speed the eater up more than the white nibbles.
For this we will need a new component, a PickupComponent.

### PickupComponent

The class declaration is very simple so I've left it out, the only non standard property is a float `points` which is how much to speed the eater up.

The `PickupComponent::update` looks like this:

```cpp
//components.cpp
void PickupComponent::update(const float &) {
...
  for (...) {       //every entity in the scene
    if (...) {      //within 30.f unit of me
      std::vector<std::shared_ptr<ActorMovementComponent>> comp = 
        e->get_compatible_components<ActorMovementComponent>();               //get the entity ActorMovementComponent, if it has one
      if (!comp.empty()) {        //if it has one
        // nom nom
        ...
        _parent->set_for_delete(); //delete myself
        break;                   //stop looking
      }
    }
  }
}
```

.. and here is how we create the nibbles..

```cpp
//scenes.hpp
class GameScene: public Scene{
...
private:
  ... 
  EntityManager _nibbles;
  std::shared_ptr<Entity> _make_nibble(const sf::Vector2i& pos, bool big);
  ...
}
```  


```cpp
//scenes.cpp
std::shared_ptr<Entity> GameScene::_make_nibble(const sf::Vector2i& pos, bool big) {
  std::shared_ptr<Entity> cherry = std::make_shared<Entity>();
  std::shared_ptr<ShapeComponent> s = cherry->add_component<ShapeComponent>();
  //set colour
  ... 
  
  cherry->add_component<PickupComponent>(big);
  cherry->set_position(ls::get_tile_position(pos) + sf::Vector2f(10.f, 10.f));
  return cherry;
}
```

... and here's where we call that function.

```cpp
//"scenes.cpp"
void GameScene::respawn() {
  ...
  for (auto n : _nibbles.list) {
    n->set_for_delete();
    n.reset();
  }
  
  _nibbles.list.clear();
  //white nibbles
  std::vector<sf::Vector2i> nibbleLoc = ls::find_tiles(ls::EMPTY);
  for (const sf::Vector2i& nl : nibbleLoc) {
    std::shared_ptr<Entity> cherry = _make_nibble(nl, false);
    //add to nibbles list
    _nibbles.list.push_back(cherry);
  }
  //blue nibbles
  nibbleLoc = ls::find_tiles(ls::WAYPOINT);
  for (const sf::Vector2i& nl : nibbleLoc) {
    std::shared_ptr<Entity> cherry = _make_nibble(nl, true);
    //add to nibbles list
    _nibbles.list.push_back(cherry);
  }
...
}
```

## Last steps

All that remains now is some form of high score system, and we are done. I'll leave this one up to you. 

Previous step: [PacMan 2](pacman2)

Next step: [Physics](physics)