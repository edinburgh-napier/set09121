---
title: "Tile Engine 2"
keywords: Tile Engine
tags: [TileEngine]
permalink:  tile_engine_2.html
summary: tiles
sidebar: home_sidebar

---
# Writing Libraries

For our next piece of work, we are going to write code that we are going to want to use again in future games. Standard practice would be to use good software engineering to isolate all the logic required for our Level loader code to it's own files and minimize coupling between itself and game code. 

One approach is to build a "helper class" which is completely separate form the code of the game, but included in the build as headers and .cpp's. We could then easily move this code to a new project by just brining along the needed files. This is fairly common practice and most game programmers have a collection of small "helper" classes that they copy over from project to project.


The next step from this approach is to completely separate the code into a library. We are already using several libraries in our project - all the SFML components. Libraries are code that is complied separately to your program and linked in during the link stage. The primary benefit of this is that we don't have to compile libraries again once they are complied (imagine if we had to build SFML every time we needed to build our game code).


In certain situations you can download libraries pre-compiled from the internet. This isn't great for C++ as the compile settings need to be near identical for your application as for the downloaded library. Furthermore - we can't step down into source code when debugging. Providing a library with a well maintained CMakeLists.txt is by far the best way to distribute your code when building libraries for other programmers to use.

## Static vs dynamic linking -- .lib's and .dll's

When we build a library - it will generate a .lib file. Depending on your build settings (Dynamic/Shared linking) it may also produce a .dll. The situation is more complicated but the simple explanation is that with dynamic linking the code for our library lives inside the .dll (.so on linux/mac). When our application starts, it loads the code from the .dll into memory. This means that the .dll has to be somewhere the running program can access. We still need to link with a .lib file, this .lib will be an small file which only describes the .dll. Also, this explains the errors you might have seen where a program can't find a .dll file - be careful not to run into this with your final submission!

With static linking, the .lib is compiled into our executable. Meaning we don't need to bring any dlls along. In this case the compiled .lib file contains all the code and will be significantly larger.

## Setting this up

Finding and setting all the right settings in an IDE to set up building and linking libraries is a nightmare. With CMake it's laughably easier (and cross-platform!).

```cmake
#### level system ####
add_library(tile_level SHARED tile_level_loader/level_system.cpp)
target_include_directories(tile_level INTERFACE tile_level_loader)
target_link_libraries(tile_level sfml-graphics)
```

**This should go in the Add External Dependencies section of your CMake file - keeping this file clean makes it much easier to understand and debug!**

The biggest difference here is the call to "add_library" rather than "add_executable" as we'd do with a main project.

From the CMake we can see that we need to put some code in a "lib_tile_level_loader" folder. Library code is exactly the same as wiring any other C++ file, but we don't need a main() function.


## Level System Code

Let's get started with our header.

```cpp 
//level_system.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include <map>

class LevelSystem {
public:
 enum Tile { EMPTY, START, END, WALL, ENEMY, WAYPOINT };
        
 static void load_level(const std::string&,float tile_size=100.f);
 static void render(sf::RenderWindow &window);
 static sf::Color get_color(Tile t);
 static void set_color(Tile t, sf::Color c);
 //Get Tile at grid coordinate
 static Tile get_tile(sf::Vector2i);
 //Get Screenspace coordinate of tile
 static sf::Vector2f get_tile_position(sf::Vector2i);
 //get the tile at screenspace pos
 static Tile get_tile_at(sf::Vector2f);
 static int get_height();
 static int get_width();
 static sf::Vector2f get_start_position();

protected:
 static std::unique_ptr<Tile[]> _tiles; //Internal array of tiles
 static int _width; //how many tiles wide is level
 static int _height; //how many tile high is level
 static sf::Vector2f _offset; //Screenspace offset of level, when rendered.
 static float _tile_size; //Screenspace size of each tile, when rendered.
 static std::map<Tile, sf::Color> _colors; //color to render each tile type
static sf::Vector2f _start_position; //starting position for the player

 //array of sfml sprites of each tile
 static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites;  
 //generate the _sprites array
 static void build_sprites();

private:
 LevelSystem() = delete;
 ~LevelSystem() = delete;
};
```

That's quite a lot to begin with. Let's pay attention to the public functions first: this is where we declare what our library can do. The protected variables are the internal state that we need for some calculations later on. The whole LevelSystem is a static class, everything is static so we can access everything within it from anywhere (Downside: we can't inherit from it).

With our LevelSystem.hpp declared, let's define it in LevelSystem.cpp

```cpp 
//level_system.cpp
#include "LevelSystem.h"
#include <fstream>
#include <iostream>

std::unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
int LevelSystem::_width;
int LevelSystem::_height;
sf::Vector2f LevelSystem::_offset(0.0f, 0.0f);

float LevelSystem::_tile_size(100.f);
std::vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

std::map<LevelSystem::Tile, sf::Color> LevelSystem::_colors{ {WALL, sf::Color::White}, {END, sf::Color::Red} };

int LevelSystem::get_height(){return _height;}
int LevelSystem::get_width(){return _width;}

sf::Color LevelSystem::get_color(LevelSystem::Tile t) {
  auto it = _colors.find(t);
  if (it == _colors.end()) {
    _colors[t] = sf::Color::Transparent;
  }
  return _colors[t];
}

void LevelSystem::set_color(LevelSystem::Tile t, sf::Color c) {
...
}

```

We start off by defining all the static member variables declared in the header file. This brings in a C++ data structure that we've not dealt with before, the map. It's statically initialised with two colours, more can be added by the game later. This map is read by the getColor() function which will return a transparent colour if an allocation is not within the map.

**Important: make sure you use find() for maps if you are searching for an element! If you used *auto it = _colours[t]* then we would CREATE the element if it didn't exist! Bad!**

You should complete the set_color function. It's super simple, but you may have to look up the C++ docs/API on the std::map. Again, you should be looking up the API all the time for what you are using. Oh, also note that again we're using smart pointers throughout!

Next up, reading in and parsing the text file.

```cpp 
//level_system.cpp

void LevelSystem::load_level(const std::string& path, float tile_size) {
  _tile_size = tile_size;
  size_t w = 0, h = 0;
  std::string buffer;

  // Load in file to buffer
  std::ifstream f(path);
  if (f.good()) {
    f.seekg(0, std::ios::end);
    buffer.resize(f.tellg());
    f.seekg(0);
    f.read(&buffer[0], buffer.size());
    f.close();
  } else {
    throw std::string("Couldn't open level file: ") + path;
  }
  int x = 0;

  std::vector<Tile> temp_tiles;
  for (int i = 0; i < buffer.size(); ++i) {
    const char c = buffer[i];
    switch (c) {
    case 'w':
      temp_tiles.push_back(WALL);
      break;
    case 's':
      temp_tiles.push_back(START);
      _start_position = get_tile_position({x,h});
      break;
    case 'e':
      temp_tiles.push_back(END);
      break;
    case ' ':
      temp_tiles.push_back(EMPTY);
      break;
    case '+':
      temp_tiles.push_back(WAYPOINT);
      break;
    case 'n':
      temp_tiles.push_back(ENEMY);
      break;
    case '\n':      // end of line
      if (w == 0) { // if we haven't written width yet
        w = i;      // set width
      }
      x=0;
      h++; // increment height
      break;
    default:
      std::cout << c << std::endl; // Don't know what this tile type is
    }
    x++;
  }
  if (temp_tiles.size() != (w * h)) {
    throw std::string("Can't parse level file") + path;
  }
  _tiles = std::make_unique<Tile[]>(w * h);
  _width = w; //set static class vars
  _height = h;
  std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
  std::cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
  build_sprites();
}
```

If we had many more tile types we would switch out that switch statement for a loop of some kind, but for the limited tile types we need; it will do.

The file handling code at the top is nothing special, we read the whole file into a string then close the open file. This may be a bad move if the level file was larger, but this where we can get away with saying "C++ is fast, it doesn't matter".

Once the level string has been parsed into a vector of tile types, an array is created with the final dimensions. Keeping within a vector could be valid, but we don't ever want to change the size of it, so an array seems a better fit.

Notice that while the level file is 2D, we store it in a 1D storage type. If we know the width of the level we can extrapolate a 2D position from the 1D array easily. We do this as C++ doesn't have a native 2D array type. We could create an array of arrays which would do the job, but makes the functions we need to write later slightly more difficult. Also, 2D arrays in most languages are slower to access/serialise/load anyway.

Also, we store the start position to make it easier and more efficient to query where the player should start.

Our level loader library will do more than just parse in a text file, however, it will also render the level with SFML! To do this we will build a list of sf::shapes for each tile in our array. The colour of this shape will depend on the colour association stored in our map. We only need to build this list of shapes once, so this function is called at the end of load_file().

**You will have to add accessors for height and width at this stage, these will be useful later! You'll need to add them to the header file too**
(This should be pretty easy!)

```cpp 
//level_system.cpp
void LevelSystem::build_sprites() {
  _sprites.clear();
  for (size_t y = 0; y < LevelSystem::get_height(); ++y) {
    for (size_t x = 0; x < LevelSystem::get_width(); ++x) {
      std::unique_ptr<sf::RectangleShape> s = std::make_unique<sf::RectangleShape>();
      s->setPosition(get_tile_position({x, y}));
      s->setSize(sf::Vector2f(_tile_size, _tile_size));
      s->setFillColor(get_color(get_tile({x, y})));
      _sprites.push_back(move(s));
    }
  }
}
```

You need to define getHeight() and getWidth(). we also need yet another function, the getTilePosition().

```cpp 
//level_system.cpp
sf::Vector2f LevelSystem::get_tile_position(sf::Vector2i p) {
  return (sf::Vector2f(p.x, p.y) * _tile_size);
}
```

As we are just doing maths here we don't need to read into the tile array. We could add some validity checks to make sure the requested tile falls within our bounds, but I like my one-liner too much to bother with that.

There will be times when we need to retrieve the actual tile at a position, both screen-space and grid-space. This is where we must convert 2D coordinates to a single index in our tile array.

```cpp 
//level_system.cpp
LevelSystem::Tile LevelSystem::get_tile(sf::Vector2i p) {
  if (p.x > _width || p.y > _height) {
    throw std::string("Tile out of range: ") + std::to_string(p.x) + "," + std::to_string(p.y) + ")";
  }
  return _tiles[(p.y * _width) + p.x];
}
```

Most of this function is taken up by a range check (Where we throw an exception like we have been earlier). The real calculation is in that last line. The secret is to multiply the Y coordinate by the tile map width, and add the X.

**Don't continue on unless you understand why and how this works, it gets more difficult from here on out. Remember to ask for help if you need it!**

Doing the same, but with a screen-space coordinate is not any different. However as we are dealing with floats now, we must check it's a positive number first, then we can convert to grid-space, and call our above function. Again, don't continue on unless you understand why and how this works.

```cpp 
//level_system.cpp
LevelSystem::Tile LevelSystem::get_tile_at(sf::Vector2f v) {
  auto a = v - _offset;
  if (a.x < 0 || a.y < 0) {
    throw std::string("Tile out of range ");
  }
  return get_tile(sf::Vector2i((v - _offset) / (_tile_size)));
}
```

And finally - here lies our Render Function. Nice and Simple.

```cpp 
//level_system.cpp
void LevelSystem::render(sf::RenderWindow &window) {
  for (size_t i = 0; i < _width * _height; ++i) {
    window.draw(*_sprites[i]);
  }
}
```

## Linking our Library

While the library can build by itself, that's rather useless to us. We need to link it into our lab code. Back to CMake:

```cmake
target_link_libraries(... lib_tile_level_loader sfml-graphics)
```

You probably could have guessed this addition. Just add the library target name to your link_libraries, just like we do with SFML. You can add as many as you like, separated by spaces.


## Using the library

Give it a test, Call some library functions from your lab code.

```cpp
//scenes.cpp
#include "level_system.hpp"
using ls = LevelSystem; 
...

void MazeScene::reset() {
    ls::load_level(_file_path);
    for (size_t y = 0; y < ls::get_height(); ++y) {
        for (size_t x = 0; x < ls::get_width(); ++x) {
            std::cout << ls::get_tile({x, y});
        }
        std::cout << std::endl;
    }
    ...
}
...
void MazeScene::render(RenderWindow &window) {
  ls::render(window);
  ...
}
```

That should be all we need to successfully build both our libraries and our game. Give it a go. Build. Run. See if your hard work typing all this has paid off.

**Oh no, did it throw an error about the tile map?** Look at the code for how we test it is all loaded: we only count a line as done when it ends with a new line... can you figure out what is likely missing from your map file?

I know it was lots of work to get here, but you can use loads of what you've just created for your own games!

{:class="important"}
**Your code should compile and your game should run. Congratulations if you've got here! Make sure to show us!**


## Making the Game a Game

### Player Class

Disallow the player from moving into a tile:

Hint:

``` cpp
bool validmove(Vector2f pos) {
  return (ls::getTileAt(pos) != ls::WALL);
}
```
And make the player start from the starting tile.

### Switch to the next maze

```cpp
//scenes.hpp
void MazeScene::update(const float &dt){
    if(/*if the player reached the end tile*/){
        if(_file_path == std::string(param::maze_1)){
            _file_path = param::maze_2;
            reset();
        }
        return;
    }
...
}
```

### Adding the ending screen

For this we need to create a new scene.

```cpp
//scenes.hpp
class EndScene: public Scene{
public:
  EndScene() = default;
  void load() override;
  void render(sf::RenderWindow &window) override;
private:
  sf::Text win_text;
  sf::Font font;
};
```
Where we just need to create a text that will be displayed in the middle of the screen and render it.

Also, we need to add it to our list of available scenes.
```cpp
//scenes.hpp
struct Scenes{
    static std::shared_ptr<Scene> maze;
    static std::shared_ptr<Scene> end;
};
```

I let figure the implementation of the load and render function, we already covered those in previous practicals.

Finally, we need to switch to the ending screen when the player reach the end of the last maze.
```cpp
//scenes.cpp        
unload(); //unload the maze scene
Scenes::end->load(); //load the end scene
gs::set_active_scene(Scenes::end); //switch the active scene to the end scene
        
```

**Don't forget to initialise the shared pointer to the ending scene!! or you will have segmentation fault**

---

Previous step: [Tile Engine 1](tile_engine_1)

Next step: [PacMan 1](pacman1)
