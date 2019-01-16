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


In certain situations you can download libraries pre-compiled from the internet. This isn't great for C++ as the compile settings need to be near identical for your application as for the downloaded library. Furthermore - we can't step down into source code when debugging. Providing a library with a well maintained CmakeLists.txt is by far the best way to distribute your code when building libraries for other programmers to use.

## Static vs dynamic linking -- .lib's and .dll's

When we build a library - it will generate a .lib file. Depending on your build settings (Dynamic/Shared linking) it may also produce a .dll. The situation is more complicated but the simple explanation is that with dynamic linking\" the code for our library lives inside the .dll (.so on linux/mac). When our application starts, it loads the code from the .dll into memory. This means that the .dll has to be somewhere the running program can access. We still need to link with a .lib file, this .lib will be an small file which only describes the .dll.

With static linking, the .lib is compiled into our executable. Meaning we don't need to bring any Dlls along. In this case the compiled .lib file contains all the code and will be significantly larger.

## Setting this up

Finding and setting all the right settings in an IDE to set up building and linking libraries is a nightmare. With CMake it's laughably easier (and cross-platform!).

```cmake
## Tile loader lib
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/" )
target_link_libraries(lib_tile_level_loader sfml-graphics)
```

The biggest difference here is the call to "add_library" rather than "add_executable"

From the CMake we can see that we need to put some code in a "lib_tile_level_loader" folder. Library code is exactly the same as wiring any other c++ file, but we don't need a Main() function.


## Level System Code

Let's get started with our header.

```cpp 
//levelsystem.h
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

#define ls LevelSystem

class LevelSystem {
public:
 enum TILE { EMPTY, START, END, WALL, ENEMY, WAYPOINT };
        
 static void loadLevelFile(const std::string&,float tileSize=100.f);
 static void render(sf::RenderWindow &window);
 static sf::Color getColor(TILE t);
 static void setColor(TILE t, sf::Color c);
 //Get Tile at grid coordinate
 static TILE getTile(sf::Vector2ul);
 //Get Screenspace coordinate of tile
 static sf::Vector2f getTilePosition(sf::Vector2ul);
 //get the tile at screenspace pos
 static TILE getTileAt(sf::Vector2f);
 
protected:
 static std::unique_ptr<TILE[]> _tiles; //Internal array of tiles
 static size_t _width; //how many tiles wide is level
 static size_t _height; //how many tile high is level
 static sf::Vector2f _offset; //Screenspace offset of level, when rendered.
 static float _tileSize; //Screenspace size of each tile, when rendered.
 static std::map<TILE, sf::Color> _colours; //color to render each tile type
 
 //array of sfml sprites of each tile
 static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites;  
 //generate the _sprites array
 static void buildSprites();
    
private:
 LevelSystem() = delete;
 ~LevelSystem() = delete;
};
```

That's quite a lot to begin with. pay attention to the public functions first. This is where we declare what our Library can do. The protected variables are internal state that we need for some calculation later on. The whole LevelSystem is a static class, everything is static so we can access everything within it from anywhere (Downside: we can't inherit from it). I've thrown in a handy \#define macro so we can access everything like \"ls::render()\". Vector2ul will give you an error, this is something that doesn't exist yet, more on this later.

With our Levelsystem declared, let's get to defining it in LevelSystem.cpp

```cpp 
//levelsystem.cpp
#include "LevelSystem.h"
#include <fstream>

using namespace std;
using namespace sf;

std::unique_ptr<LevelSystem::TILE[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;
Vector2f LevelSystem::_offset(0.0f, 30.0f);

float LevelSystem::_tileSize(100.f);
vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

std::map<LevelSystem::TILE, sf::Color> LevelSystem::_colours{ {WALL, Color::White}, {END, Color::Red} };

sf::Color LevelSystem::getColor(LevelSystem::TILE t) {
  auto it = _colours.find(t);
  if (it == _colours.end()) {
    _colours[t] = Color::Transparent;
  }
  return _colours[t];
}

void LevelSystem::setColor(LevelSystem::TILE t, sf::Color c) {
  ...
}

```

We start off by defining all the static member variables declared in the header file. This brings in a C++ data structure that we've not dealt with before, the map. It's statically initialised with two colours, more can be added by the game later. This map is read by the \"getColor\" function which will return a transparent colour if an allocation is not within the map.

You should complete the setColor function. It's super simple, but you may have to look up the c++ docs on the std::map.

Next up, reading in and parsing the text file.

```cpp 
//levelsystem.cpp
void LevelSystem::loadLevelFile(const std::string& path, float tileSize) {
  _tileSize = tileSize;
  size_t w = 0, h = 0;
  string buffer;

  // Load in file to buffer
  ifstream f(path);
  if (f.good()) {
    f.seekg(0, std::ios::end);
    buffer.resize(f.tellg());
    f.seekg(0);
    f.read(&buffer[0], buffer.size());
    f.close();
  } else {
    throw string("Couldn't open level file: ") + path;
  }

  std::vector<TILE> temp_tiles;
  for (int i = 0; i < buffer.size(); ++i) {
    const char c = buffer[i];
    switch (c) {
    case 'w':
      temp_tiles.push_back(WALL);
      break;
    case 's':
      temp_tiles.push_back(START);
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
      h++; // increment height
      break;
    default:
      cout << c << endl; // Don't know what this tile type is
    }
  }
  if (temp_tiles.size() != (w * h)) {
    throw string("Can't parse level file") + path;
  }
  _tiles = std::make_unique<TILE[]>(w * h);
  _width = w; //set static class vars
  _height = h;
  std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
  cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
  buildSprites();
}
```

If we had many more tile types we would switch out that switch statement for a loop of some kind, but for the limited tile types we need; it will do.

The file handling code at the top is nothing special, we read the whole file into a string then close the open file. This may be a bad move if the level file was larger, but this where we can get away with saying "C++ is fast, it doesn't matter".

Once the level string has been parsed into a vector of tile types, an array is created with the final dimensions. Keeping within a vector could be valid, but we don't ever want to change the size of it, so an array seems a better fit.

Notice that while the level file is 2D, we store it in a 1D storage type. If we know the width of the level we can extrapolate a 2d position from the 1d array easily. We do this as C++ doesn't have a native 2D array type. We could create an array of arrays which would do the job, but makes the functions we need to write later slightly more difficult.

Our level loader library will do more than just parse in a text file, it will also render the level with SFML. To do this we will build a list of sf::shapes for each tile in our array. The colour of this shape will depend on the colour association stored in our map. We only need to build this list of shapes once, so this function is called at the end of loadLevelFile().

```cpp 
//levelsystem.cpp

void LevelSystem::buildSprites() {
  _sprites.clear();
  for (size_t y = 0; y < LevelSystem::getHeight(); ++y) {
    for (size_t x = 0; x < LevelSystem::getWidth(); ++x) {
      auto s = make_unique<sf::RectangleShape>();
      s->setPosition(getTilePosition({x, y}));
      s->setSize(Vector2f(_tileSize, _tileSize));
      s->setFillColor(getColor(getTile({x, y})));
      _sprites.push_back(move(s));
    }
  }
}
```

Note we need yet another function, the getTilePosition().

```cpp 
//levelsystem.cpp
sf::Vector2f LevelSystem::getTilePosition(sf::Vector2ul p) {
  return (Vector2f(p.x, p.y) * _tileSize);
}
```

As we are just doing maths here we don't need to read into the tile array. We could add some validity checks to make sure the requested tile falls within our bounds, but I like my one-liner too much to bother with that.

There will be times when we need to retrieve the actual tile at a position, both screen-space and grid-space. This is where we must convert 2D coordinates to a single index in our tile array.

```cpp 
//levelsystem.cpp
LevelSystem::TILE LevelSystem::getTile(sf::Vector2ul p) {
  if (p.x > _width || p.y > _height) {
    throw string("Tile out of range: ") + to_string(p.x) + "," + to_string(p.y) + ")";
  }
  return _tiles[(p.y * _width) + p.x];
}
```

Most of this function is taken up by a range check (Where we throw an exception, new thing!). The real calculation is in that last line. The secret is to multiply the Y coordinate by the length and add the X. Don't continue on unless you understand why and how this works, it gets more difficult from here on out.

Doing the same, but with a screen-space coordinate is not any different. However as we are dealing with floats now, we must check it's a positive number first, then we can convert to grid-space, and call our above function. Again, don't continue on unless you understand why and how this works.

```cpp 
//levelsystem.cpp
LevelSystem::TILE LevelSystem::getTileAt(Vector2f v) {
  auto a = v - _offset;
  if (a.x < 0 || a.y < 0) {
    throw string("Tile out of range ");
  }
  return getTile(Vector2ul((v - _offset) / (_tileSize)));
}
```

And finally - here lies our Render Function. Nice and Simple.

```cpp 
//levelsystem.cpp
void LevelSystem::render(RenderWindow &window) {
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

You probably could have guessed this addition. Just add the library target name to your link_libraries.

## Using the library

Give it a test, Call some library functions from your lab code.

```cpp
\\main.cpp
#include "LevelSystem.h"

...

void load() {
  ...
  ls::loadLevelFile("res/maze_2.txt");

  // Print the level to the console
  for (size_t y = 0; y < ls::getHeight(); ++y) {
    for (size_t x = 0; x < ls::getWidth(); ++x) {
      cout << ls::getTile({x, y});
    }
    cout << endl;
  }
}
...
void render(RenderWindow &window) {
  ls::render(window);
  ...
}
```


## Maths Library


Remember that Vector2ul type that doesn't exist? 

The vector maths functionality of SFML is quite lacking when compared to larger libraries like GLM. 
We could bring in GLM and write converter functions to allow it to interface with SFML. 

This would be a good idea if we needed to advanced thing like quaternions, but we don't. 

Instead we will build a small add-on helper library to add in the functions that SFML misses out.

### CMake

Firstly, like we did with our first library, add this to CMake.

``` cmake
# Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" SYSTEM INTERFACE ${SFML_INCS})
```

This is slightly different to the level system library. This time we declare the library as INTERFACE. 

This changes some complex library and linker options that are beyond the scope of explanation here. The simplest explanation is an INTERFACE library target does not directly create build output, though it may have properties set on it and it may be installed, exported and imported. Meaning that in visual studio the library will look like it is part of our main lab code (except it isn't. Magic.)


There are many different way to create and link libraries, CMake allows us to change these options from a central point and not worry about digging through IDE options.


We need to link the maths library, against the levelsystem library. edit the level systems cmake code to include lib_maths


```cmake
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)
```

As this is a static library - and doesn't produce a compiled output, everything will be in a header. 

To extend the functionality of sf::vectors we must first be within the same namespace. From here we can define code as if we were inside the sfml library code itself. Thing get a little strange if we want to change or override functions that already exist, but we don't here as we are only creating new functionality.

We start by creating a new vector type the 'Vector2ul' which will use size_t (i.e the largest unsigned integer type supported on the system) as the internal components. We will use this for the tile array coordinates.

From this we implement the standard vector maths functions that any self respecting game engine would have. Length, Normalization, and Rotation. I've left these incomplete so you wil have to dredge up your vector maths skills to complete them.

Lastly, we override the << stream operator to us do things like "cout << vector". Useful for debugging.

```cpp
//maths.h
#pragma once

#include <SFML/System.hpp>
#include <cmath>
#include <iostream>
#include <vector>

namespace sf {
  //Create a definition for a sf::vector using size_t types
  typedef Vector2<size_t> Vector2ul;
  // Returns the length of a sf::vector
  template <typename T> double length(const Vector2<T> &v) {
    return sqrt(...);
  }
  // return normalized sf::vector
  template <typename T> Vector2<T> normalize(const Vector2<T> &v) {
    Vector2<T> vector;
    double l = length(v);
    if (l != 0) {
      vector.x = ...
      vector.y = ...
    }
    return vector;
  }
  //Allow casting from one sf::vetor internal type to another
  template <typename T, typename U>
  Vector2<T> Vcast(const Vector2<U> &v) {
    return Vector2<T>(static_cast<T>(v.x), static_cast<T>(v.y));
  };
  // Degreess to radians conversion
  static double deg2rad(double degrees) {
    return ...
  }
  //Rotate a sf::vector by an angle(degrees)
  template <typename T>
  Vector2<T> rotate(const Vector2<T> &v, const double degrees) {
    const double theta = deg2rad(degrees);
    const double cs = cos(theta);
    const double sn = sin(theta);
    return {(T)(v.x * cs - v.y * sn), (T)(v.x * sn + v.y * cs)};
  }
  //Allow sf::vectors to be cout'ed
  template <typename T>
  std::ostream &operator<<(std::ostream &os, const Vector2<T> &v) {
     os << '(' << v.x << ',' << v.y << ')';
     return os;
  }
} 
```

That should be all we need to successfully build both our libraries and our game. Give it a go. Build. Run. See if your hard work typing all this has paid off.

{:class="important"}
**Your code should compile and your game should run**


## Making the Game a Game


### Player Class

Disallow the player from moving into a tile:

Hint:

``` cpp
bool validmove(Vector2f pos) {
  return (ls::getTileAt(pos) != ls::WALL);
}
```

### Advanced tasks


- Start the Player from the "Start Tile"
- End the game When the player hit's the end tile
- Time how long it takes to complete the maze
- Show current, previous and best times

---

Previous step: [Tile Engine 1](tile_engine_1)

Next step: [Tile Engine 3](tile_engine_3)
