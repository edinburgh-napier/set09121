---
title: "Lab 8_2 - AI: Pathfinding"
keywords: lab8_2
tags: [lab8_2]
permalink:  lab8_2.html
summary: lab8_2
sidebar: home_sidebar
---

## Pathfinding

<video class="middle" width="300" height="300" loop autoplay>
  <source src="assets/videos/pathfinding.mp4" type="video/mp4">
</video>

Now that the steering behaviour scene is working we can focus on pathfinding. Our aim here is to use the level system to provide information so we can tell a character how to move around a map. This requires implementation of the A\* algorithm. This will provide information we can send to a component so it can decide how to move. We will end by adding controls so you can click on the map to change the target to move towards.

### `PathFindingScene`

First we need to implement the `PathFindingScene`. The starting files are defined below and go into `scene_pathfinding.h` and `scene_pathfinding.cpp`.

```cpp
//"scene_pathfinding.h"
#pragma once

#include <engine.h>

class PathfindingScene : public Scene
{
public:
    void Load() override;
    void UnLoad() override;
    void Update(const double &dt) override;
    void Render() override;
};
```

```cpp
//scene_pathfinding.cpp
#include "scene_pathfinding.h"

#include <LevelSystem.h>
#include "../components/cmp_sprite.h"

using namespace std;
using namespace sf;

shared_ptr<Entity> character;

void PathfindingScene::Load() {
  ls::loadLevelFile("res/pacman.txt", 20.0f);

  character = makeEntity();
  Vector2f startPos(ls::getOffset().x + 1 * ls::getTileSize(),
                    ls::getOffset().y + 1 * ls::getTileSize());
  character->setPosition(startPos);
  auto s = character->addComponent<ShapeComponent>();
  s->setShape<CircleShape>(10.0f);
  s->getShape().setFillColor(Color::Red);
}

void PathfindingScene::UnLoad() { Scene::UnLoad(); }

void PathfindingScene::Update(const double &dt){
    Scene::Update(dt);
}

void PathfindingScene::Render(){
    ls::render(Engine::GetWindow());
    Scene::Render();
}
```

The `Load` method just loads a tile map (I will use the one from PacMan) and add a red circle to the screen. We then just update `game.h` to below.

```cpp
//game.h
#pragma once

#include "scenes/scene_menu.h"
#include "scenes/scene_steering.h"
#include "scenes/scene_pathfinding.h"

extern MenuScene menu;
extern SteeringScene steeringScene;
extern PathfindingScene pathfindingScene;
```

Update `MenuScene::update` so it will select the new `PathFindingScene`.

```cpp
//Updated MenuScene::update
void MenuScene::Update(const double &dt){
    if (sf::Keyboard::isKeyPressed(Keyboard::Num1)){
        Engine::ChangeScene(&steeringScene);
    }else if (sf::Keyboard::isKeyPressed(Keyboard::Num2)){
        Engine::ChangeScene(&pathfindingScene);
    }
    Scene::Update(dt);
}
```

And add the line `PathfindingScene pathfindingScene;` to our declarations in `main.cpp`. Then you should run the application and get the output shown:

![Start of Pathfinding Demo](/assets/images/screen-path-start.png)


## Implementing A\*

We discussed the basics of A\* in the lecture. We will define these in the files `astar.h` and `astar.cpp`. The latter is a long definition but the comments and the lecture slides will help you understand what is happening.

```cpp
//"astar.h"
#pragma once

#include <engine.h>
#include <vector>

std::vector<sf::Vector2i> pathFind(sf::Vector2i start, sf::Vector2i finish);
```

```cpp
//astar.cpp
#include "astar.h"
#include <LevelSystem.h>
#include <array>
#include <queue>
using namespace std;
using namespace sf;

// Node in the search graph.
class Node {
private:
  Vector2i _pos;
  int _level;
  int _priority;

public:
  Node() = default;
  Node(const Vector2i& pos, int level, int priority) : _pos(pos), _level(level), _priority(priority) {}

  const Vector2i& getPos() const { return _pos; }

  int getLevel() const { return _level; }

  int getPriority() const { return _priority; }

  unsigned int estimate(const Vector2i& dest) const {
    Vector2i delta = dest - _pos;
    return static_cast<unsigned int>(length(delta));
  }

  void updatePriority(const Vector2i& dest) {
    // Heuristic is just Euclidian distance.
    // Can be modified.
    _priority = _level + estimate(dest) * 10;
  }

  void nextLevel() { _level += 10; }

  // Used for priority ordering
  bool operator<(const Node& other) const {
    return _priority > other._priority;
  }
};

vector<Vector2i> pathFind(Vector2i start, Vector2i finish) {
  static std::array<sf::Vector2i, 4> directions = {
      Vector2i(1, 0), Vector2i(0, 1), Vector2i(-1, 0), Vector2i(0, -1)};

  // This may not be the most efficient mechanism, but should not be a // problem unless your maps get very big.
  vector<vector<bool>> closed_nodes_map(ls::getWidth());
  vector<vector<int>> open_nodes_map(ls::getWidth());
  vector<vector<Vector2i>> direction_map(ls::getWidth());
  // Queue of nodes to test.  Priority ordered.
  // We need two for when we redirect and copy the path.
  priority_queue<Node> queue[2];
  // Index of current queue
  size_t queue_index = 0;

  // Initialise the lookup maps.  Initially everything looks traversable and
  // no route has been planned.
  for (size_t y = 0; y < ls::getHeight(); ++y) {
    for (size_t x = 0; x < ls::getWidth(); ++x) {
      closed_nodes_map[x].push_back(false);
      open_nodes_map[x].push_back(0);
      direction_map[x].emplace_back(Vector2i(0, 0));
    }
  }

  // Add start node to the queue - we will check it first
  Node n0(start, 0, 0);
  // Update the priority of the node.
  n0.updatePriority(finish);
  // Push onto the current queue of nodes to check.
  queue[queue_index].push(n0);
  // And update the value in the open_nodes_map.
  open_nodes_map[start.x][start.y] = n0.getPriority();

  // Loop until queue is empty
  while (!queue[queue_index].empty()) {
    auto tmp = queue[queue_index].top();
    n0 = Node(tmp.getPos(), tmp.getLevel(), tmp.getPriority());
    auto pos = n0.getPos();

    // Remove node from open list
    queue[queue_index].pop();
    open_nodes_map[pos.x][pos.y] = 0;
    // Mark on closed node list
    closed_nodes_map[pos.x][pos.y] = true;

    // Check if reached goal
    if (pos == finish) {
      vector<Vector2i> path;
      while (!(pos == start)) {
        // Get the direction
        auto dir = direction_map[pos.x][pos.y];
        // Add the current position to the path
        path.push_back(pos);
        pos += dir;
      }
      // Path is inverted.  Reverse it.
      reverse(begin(path), end(path));
      // Return path.
      return path;
    }
    // Not reached goal.  Check four directions.
    for (auto& dir : directions) {
      auto next = pos + dir;

      // Check if next is valid.
      if (!(next.x < 0 || next.x > ls::getWidth() || next.y < 0 ||
            next.y > ls::getHeight() ||
            ls::getTile(Vector2ul(next.x, next.y)) == LevelSystem::WALL ||
            closed_nodes_map[next.x][next.y])) {
        // Generate new node
        Node m0(next, n0.getLevel(), n0.getPriority());
        m0.nextLevel();
        m0.updatePriority(finish);

        // Check if new node has no priority
        if (open_nodes_map[next.x][next.y] == 0) {
          // Update priority and add to the queue.
          open_nodes_map[next.x][next.y] = m0.getPriority();
          queue[queue_index].push(m0);
          // Put the opposite direction into the direction map.
          // We work backwards.
          direction_map[next.x][next.y] = dir * -1;
        }
        // If it has a priority, check if the priority is better on the new // route.
        else if (open_nodes_map[next.x][next.y] > m0.getPriority()) {
          // Update the priority information
          open_nodes_map[next.x][next.y] = m0.getPriority();
          // Update the direction map with the inverse direction.
          direction_map[next.x][next.y] = dir * -1;
          // Now replace the node via swapping across the lists and ignoring
          // the old one.
          while (queue[queue_index].top().getPos() != next) {
            queue[1 - queue_index].push(queue[queue_index].top());
            queue[queue_index].pop();
          }
          queue[queue_index].pop();
          if (queue[queue_index].size() > queue[1 - queue_index].size())
            queue_index = 1 - queue_index;
          while (!queue[queue_index].empty()) {
            queue[1 - queue_index].push(queue[queue_index].top());
            queue[queue_index].pop();
          }
          queue_index = 1 - queue_index;
          queue[queue_index].push(m0);
        }
      }
    }
  }
  return vector<Vector2i>();
}

```

Now we just need to add a component to use this behaviour.

### `PathfindingComponent`

`PathfindingCompoonent` will use a list of nodes to traverse the level. Every second it will move to the next position in the level until it reaches its destination. To do this it needs to keep track of a path, its index in the path, and the amount of time passed since the last move. We will also require a method to set the path.

```cpp 
//"cmp_path_follow.h"
#pragma once
#include <ecm.h>

class PathfindingComponent : public Component {
protected:
  std::vector<sf::Vector2i> _path;
  size_t _index = 0;
  double _elapsed = 0.0;

public:
  void update(double) override;
  void render() override {}
  void setPath(std::vector<sf::Vector2i>& path);
  explicit PathfindingComponent(Entity* p);
  PathfindingComponent() = delete;
};
```

```cpp 
//"cmp_path_follow.cpp"
#include "cmp_path_follow.h"
#include "../astar.h"
#include <LevelSystem.h>

using namespace sf;
using namespace std;

void PathfindingComponent::update(double dt) {
  _elapsed += dt;
  if (_elapsed >= 0.1) {
    _elapsed = 0.0;
    if (_index < _path.size()) {
      // Could do this in a single line - expanded for clarity
      float new_x = ls::getOffset().x + _path[_index].x * ls::getTileSize();
      float new_y = ls::getOffset().y + _path[_index].y * ls::getTileSize();
      _parent->setPosition(Vector2f(new_x, new_y));
      ++_index;
    }
  }
}

PathfindingComponent::PathfindingComponent(Entity* p) : Component(p) {}

void PathfindingComponent::setPath(std::vector<sf::Vector2i>& path) {
  _index = 0;
  _path = path;
}
```

We now just update the `PathfindingComponent` definition so we create a `PathfindingComponent` on our `entity` and find a path. We will find a path from the character's start position (1,1) to the bottom left corner. The updates are below.

```cpp
//"Updated PathFindingScene"
shared_ptr<PathfindingComponent> ai;

void PathfindingScene::Load()
{
    ls::loadLevelFile("res/pacman.txt", 20.0f);
    ...
    
    // New code from here
    auto path = pathFind(Vector2i(1, 1),
                       Vector2i(ls::getWidth() - 2, ls::getHeight() - 2));
    ai = character->addComponent<PathfindingComponent>();
    ai->setPath(path);
}
```

If you run the application now the character will pathfind to the destination.

![Pathfinding in Action[]{label="fig:screen-path-final"}](screen-path-final){#fig:screen-path-final width="\textwidth"}

## Adding Mouse Input

Let us now modify the update for the `PathfindingScene` to allow the user to click on a tile to change the location to move to. This requires the following stages:

1.  Check if mouse button has been pressed since last frame.
2.  If it has, check if the mouse click is on the map.
3.  If it is, calculate which tile has been clicked.
4.  If tile clicked is not a wall, calculate current character tile.
5.  Use the current character tile and the clicked tile to perform a pathfind.

The code for this is below.

```cpp
//"Using Mouse Clicks to Update Path"
void PathfindingScene::Update(const double& dt) {
  static bool mouse_down = false;
  if (Mouse::isButtonPressed(Mouse::Left) && !mouse_down) {
    auto mouse_pos = Mouse::getPosition(Engine::GetWindow());
    mouse_down = true;
    if (ls::isOnGrid(Vector2f(mouse_pos))) {
      auto relative_pos = mouse_pos - Vector2i(ls::getOffset());
      auto tile_coord = relative_pos / (int)ls::getTileSize();
      if (ls::getTile(Vector2ul(tile_coord)) != ls::WALL) {
        auto char_relative = character->getPosition() - ls::getOffset();
        auto char_tile = Vector2i(char_relative / ls::getTileSize());
        auto path = pathFind(char_tile, tile_coord);
        ai->setPath(path);
      }
    }
  }
  if (mouse_down && !Mouse::isButtonPressed(Mouse::Left)) {
    mouse_down = false;
  }
  Scene::Update(dt);
}
```

You should now be able to click on a new tile to change the path being used.

### Exercise

The key thing to try now is to integrate the physics into our movement. Then, try and implement the arrive steering behaviour with pathfinding to create a smoother movement during pathfinding.