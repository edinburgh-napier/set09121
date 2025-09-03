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

First we need to implement the `PathFindingScene` which will go in *scenes.hpp* and *scenes.cpp*

```cpp
//"scenes.hpp"
...
class PathfindingScene : public Scene
{
private:
    std::shared_ptr<Entity> _character;
public:
    void load() override;
    void unload() override;
    void update(const float &dt) override;
    void render() override;
};
...
```

```cpp
//scenes.cpp
...
void PathfindingScene::load() {
  ls::load_file("resources/levels/pacman.txt", 20.0f);

  _character = make_entity();
  sf::Vector2f start_pos(ls::get_start_position()-{10.f,10.f});
  _character->set_position(start_pos);
  std::shared_ptr<ShapeComponent> s = character->add_component<ShapeComponent>();
  s->set_shape<sf::CircleShape>(10.0f);
  s->get_shape().setFillColor(sf::Color::Red);
}

void PathfindingScene::unload() { Scene::unload(); }

void PathfindingScene::update(const float &dt){
    Scene::update(dt);
}

void PathfindingScene::render(){
    ls::render(Renderer::get_window());
    Scene::render();
}
```

The `load` method just loads a tile map (I will use the one from PacMan) and add a red circle to the screen. Like usual, We then just update the `Scenes` struct and initialise the new scene in *main.cpp*. 

Update `MenuScene::update` so it will select the new `PathFindingScene` when press 2.


![Start of Pathfinding Demo](assets/images/screen-path-start.png)


## Implementing A\*

We discussed the basics of A\* in the lecture. We will define these in the files `astar.h` and `astar.cpp`. The latter is a long definition but the comments and the lecture slides will help you understand what is happening.

```cpp
//"astar.h"
#pragma once

#include <engine.h>
#include <vector>

namespace a_star{
std::vector<sf::Vector2i> path_find(sf::Vector2i start, sf::Vector2i finish);
}
class Node {
private:
  sf::Vector2i _pos;
  int _level;
  int _priority;

public:
  Node() = default;
  Node(const sf::Vector2i& pos, int level, int priority);

  const sf::Vector2i& get_pos() const;

  int get_level() const;

  int get_priority() const;

  unsigned int estimate(const sf::Vector2i& dest) const;

  void update_priority(const sf::Vector2i& dest);

  void next_level();

  // Used for priority ordering
  bool operator<(const Node& other) const;
};
```

```cpp
//astar.cpp
#include "astar.h"
#include <LevelSystem.h>
#include <array>
#include <queue>


std::vector<sf::Vector2i> a_star::path_find(sf:Vector2i start, sf::Vector2i finish) {
  static std::array<sf::Vector2i, 4> directions = {
      Vector2i(1, 0), Vector2i(0, 1), Vector2i(-1, 0), Vector2i(0, -1)};

  // This may not be the most efficient mechanism, but should not be a // problem unless your maps get very big.
  std::vector<std::vector<bool>> closed_nodes_map(ls::getWidth());
  std::vector<std::vector<int>> open_nodes_map(ls::getWidth());
  std::vector<std::vector<sf::Vector2i>> direction_map(ls::getWidth());
  // Queue of nodes to test.  Priority ordered.
  // We need two for when we redirect and copy the path.
  std::priority_queue<Node> queue[2];
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
  return std::vector<sf::Vector2i>();
}

Node::Node(const sf::Vector2i& pos, int level, int priority) : _pos(pos), _level(level), _priority(priority) {}

const sf::Vector2i& Node::get_pos() const { return _pos; }

int Node::get_level() const { return _level; }

int Node::get_priority() const { return _priority; }

unsigned int Node::estimate(const sf::Vector2i& dest) const {
    auto length = [](sf::Vectori v) -> int{
        return std::sqrt(v.x*v.x+v.y+.v.y);
    };
    sf::Vector2i delta = dest - _pos;
    return static_cast<unsigned int>(length(delta));
}

void Node::update_priority(const sf::Vector2i& dest) {
// Heuristic is just Euclidian distance.
// Can be modified.
_priority = _level + estimate(dest) * 10;
}

void Node::next_level() { _level += 10; }

// Used for priority ordering
bool Node::operator<(const Node& other) const {
    return _priority > other._priority;
}

```

Now we just need to add a component to use this behaviour.

### `PathfindingComponent`

`PathfindingCompoonent` will use a list of nodes to traverse the level. Every second it will move to the next position in the level until it reaches its destination. To do this it needs to keep track of a path, its index in the path, and the amount of time passed since the last move. We will also require a method to set the path.

```cpp 
//"ai_cmps.hpp"
#pragma once
#include <ecm.h>

class PathfindingComponent : public Component {
protected:
  std::vector<sf::Vector2i> _path;
  size_t _index = 0;
  double _elapsed = 0.0;

public:
  void update(const float &) override;
  void render() override {}
  void set_path(std::vector<sf::Vector2i>& path);
  explicit PathfindingComponent(Entity* p);
  PathfindingComponent() = delete;
};
```

```cpp 
//"ai_cmps.cpp"
...
void PathfindingComponent::update(double dt) {
  _elapsed += dt;
  if (_elapsed >= 0.1) {
    _elapsed = 0.0;
    if (_index < _path.size()) {
      float new_x = _path[_index].x * param::tile_size;
      float new_y = _path[_index].y * param::tile_size;
      _parent->set_position(sf::Vector2f(new_x, new_y));
      ++_index;
    }
  }
}

PathfindingComponent::PathfindingComponent(Entity* p) : Component(p) {}

void PathfindingComponent::set_path(std::vector<sf::Vector2i>& path) {
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
    std::vector<sf::Vector2i> path = a_star::path_find(sf::Vector2i(1,1),
                        sf::Vector2i(ls::get_width() -2,ls::get_height()-2));
    std::shared_ptr<PathfindingComponent> cmp = _character->add_component<PathfindingComponent>();
    cmp->set_path(path);
}
```

If you run the application now the character will pathfind to the destination.

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
void PathfindingScene::update(const float &dt){
    static bool mouse_down = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouse_down) {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(Renderer::get_window());
        mouse_down = true;
        if (mouse_pos.x > 0 && mouse_pos.x < param::game_width &&
            mouse_pos.y > 0 && mouse_pos.y < param::game_height) {
            // sf::Vector2f relative_pos = mouse_pos; - sf::Vector2i(ls::getOffset());
            sf::Vector2i tile_coord = mouse_pos / static_cast<int>(param::tile_size);
            if (ls::get_tile(sf::Vector2i(tile_coord)) != ls::WALL) {
                sf::Vector2f char_pos = _character->get_position();
                sf::Vector2i char_tile = sf::Vector2i(char_pos / param::tile_size);
                std::vector<sf::Vector2i> path = a_star::path_find(char_tile, tile_coord);
              _pathfinder->set_path(path);
            }
        }
    }
    if (mouse_down && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        mouse_down = false;
    }
    Scene::update(dt);
}
```

You should now be able to click on a new tile to change the path being used.

### Exercise

The key thing to try now is to integrate the physics into our movement. Then, try and implement the arrive steering behaviour with pathfinding to create a smoother movement during pathfinding.

Previous step: [Steering](lab8_1)

Next step: [State Machines](lab9_1)