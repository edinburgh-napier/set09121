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
//"pacman.cpp"
void GameScene::load() {
...
 ls::loadLevelFile("res/pacman.txt", 25.0f);
...
}

void GameScene::render() {
  ls::render(Renderer::getWindow());
  ...
}
```

Easily done, thanks to our well built level system. What we can do now is use this to change the spawn positions of our Entities

```cpp
//"pacman.cpp"
void GameScene::respawn() {
 player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
 player->GetCompatibleComponent<ActorMovementComponent>()[0]
            ->setSpeed(150.f);

 auto ghost_spawns = ls::findTiles(ls::ENEMY);
 for (auto& g : ghosts) {
   g->setPosition(
        ls::getTilePosition(ghost_spawns[rand() % ghost_spawns.size()]));
   g->GetCompatibleComponent<ActorMovementComponent>()[0]->setSpeed(100.0f);
 }
 ...
}
```

This makes use of a new function `findTiles()` which we haven't written yet, go an implement it into the level system library. Here's the declaration, you must figure out the implementation.

```
//"LevelSystem.h"
static std::vector<sf::Vector2ul> findTiles(TILE);
```

With this done, the player should be spawning at the bottom, and the ghosts randomly in the middle. The ActorMovementComponent has a `validMove()` function which should stop both Entity types from moving into a wall. We are getting pretty close to a game now.


## Ghost Movement


We could have the Ghosts chasing the player, for now we will do something easier. The ghosts will move along the level continuously, until they reach a corner or junction tile. They will then pick a random direction to turn and move in, they will never turn and move back the way they came.

For this to work we only need to store two additional properties in the ghost component: The current state, and current direction.

```cpp
//cmp_enemy_ai.h
class EnemyAIComponent : public ActorMovementComponent {
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
//cmp_enemy_ai.cpp
static const Vector2i directions[] = {{1, 0}, {0, 1}, {0, -1}, {-1, 0}};

void EnemyAIComponent::update(double dt) {
  //amount to move
  const auto mva = (float)(dt * _speed); 
  //Curent position
  const Vector2f pos = _parent->getPosition();
  //Next position
  const Vector2f newpos = pos + _direction * mva;
  //Inverse of our current direction
  const Vector2i baddir = -1 * Vector2i(_direction);
  //Random new direction
  Vector2i newdir = directions[(rand() % 4)];
  
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
     _direction = Vector2f(newdir);
     _state = ROTATED;
     break;
      
   case ROTATED:
     //have we left the waypoint?
     if (LevelSystem::getTileAt(pos) != LevelSystem::WAYPOINT) {
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

The approach we are going to take is the most simple, doing the check in the pacman.cpp Update().

For this to work we need to keep a reference to both the player and the ghosts. the game scene does have an EntityList which contains both, and so we could iterate through that. But wouldn't it just be easier if we did this?

```cpp
//"pacman.cpp"
vector<shared_ptr<Entity>> ghosts;
shared_ptr<Entity> player;

void GameScene::load() {
...
  {
    ...
    _ents.list.push_back(pl);
    player = pl;
  }

  for (int i = 0; i < GHOSTS_COUNT; ++i) {
    ...
    ghosts.push_back(ghost);
    _ents.list.push_back(ghost);
  }
```

Sometimes it's best to fall back on the simplest solutions. And here is
our collision check.


```cpp
//"pacman.cpp"
for (auto& g : ghosts) {
    if (length(g->getPosition() - player->getPosition()) < 30.0f) {
      respawn();
    }
}
```

You'll have to build up the respawn() code to reset everything.

## Nibbles

One last thing to add, food for pacman.

We're deviating quite far from the proper game rules and going to have the pickup 'nibbles' speed Pacman(and the ghosts) up. We'll place a small white nibble on every EMPTY tile, and larger blue nibble on all the WAYPOINTs. the larger blue nibbles will speed the eater up more than the white nibbles.
For this we will need a new component, a PickupComponent.

### PickupComponent

The class declaration is very simple so I've left it out, the only non standard property is a float `points` which is how much to speed the eater up.

The `PickupComponent::update` looks like this:

```cpp
//"cmp_pickup.cpp.cpp"
void PickupComponent::update(double) {
  for (...) {       //every entity in the scene
    if (...) {      //within 30.f unit of me
      ...               //get the entity ActorMovementComponent, if it has one
      if (...) {        //if it has one
        // nom nom
        ...                      //speed the entity up
        _parent->setForDelete(); //delete myself
        break;                   //stop looking
      }
    }
  }
}
```

.. and here is how we create the nibbles..

```cpp
//"pacman.cpp"

vector<shared_ptr<Entity>> nibbles;

shared_ptr<Entity> makeNibble(const Vector2ul& nl, bool big) {
  auto cherry = make_shared<Entity>();
  auto s = cherry->addComponent<ShapeComponent>();
  //set colour
  ...
  
  cherry->addComponent<PickupComponent>(big);
  cherry->setPosition(ls::getTilePosition(nl) + Vector2f(10.f, 10.f));
  return cherry;
}
```

... and here's where we call that function.

```cpp
//"pacman.cpp"

void GameScene::respawn() {
  ...
  //clear any remaining nibbles
  for (auto n : nibbles) {
    n->setForDelete();
    n.reset();
  }
  nibbles.clear();
  
  //white nibbles
  auto nibbleLoc = LevelSystem::findTiles(LevelSystem::EMPTY);
  for (const auto& nl : nibbleLoc) {
    auto cherry = makeNibble(nl, false);
    //add to _wnts and nibbles list
    ...
  }
  //blue nibbles
  nibbleLoc = LevelSystem::findTiles(LevelSystem::WAYPOINT);
  for (const auto& nl : nibbleLoc) {
    ...
  }
  ...
}
```

## Last steps

All that remains now is some form of high score system, and we are done. I'll leave this one up to you. 
