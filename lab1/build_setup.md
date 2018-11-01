---
title: "Build Setup"
keywords: build_setup
tags: [build_setup]
permalink:  build_setup.html
summary: build_setup
sidebar: home_sidebar
---
To begin here I am assuming you have a local repo, with the SFML submodule.

### Get Some Code
With a simple text editor, create a **main.cpp** file in the **practical_1** folder, input the following code:
```cpp
#include <SFML/Graphics.hpp>

int main(){
  sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  while (window.isOpen()){
      sf::Event event;
      while (window.pollEvent(event)){
      if (event.type == sf::Event::Closed){
        window.close();
      }
    }
    window.clear();
    window.draw(shape);
    window.display();
  }
  return 0;
}
```

This is the basic "Hello world" for SFML, we will use this to test everything is in-place an working.


## Building code with CMake
Now we need to create our devlopment environment. 

If you were mad you could create makefile and do it like the 1990's hacker. 

If you were naive you may want to open up visual studio, create a new project, and spend 2 hours digigng thourgh build settings. 2005 Called and it want is workflow back.

While C++ doesn't have a standarderised package and build system (i.e, Pythons's pip, Nodes's npm), we have something that's pretty close: **CMake**

CMake allows you to write a **CMakelists.txt file**. In which you specify what your program is, where the source files are, and where any of it's needed dependacies are. From here Cmake will take that config fille and go and make you a prefcet pre-setup visual studio solution. No need to touch configuration options in visual studio.

CMake has many more benefits,but what we care about is:
1. You only need to store CMakelists.txt files in your Repo, __**no huge VSsolutions.sln**__
1. CMakelists.txt play nice with git, you can easily see and track changes
1. CMake doesn't just build Visual Studio solutions, it can build Xcode, Clion, Eclipse, makefiles..etc, this is an important step into writing cross-platform code.

Cmake Downsides:
1. It's yet another new scripting langauge to learn

---
Previous step: [Repo setup](repo_setup)
Next step: [build setup](build_setup)


### Create the CMake script
With a simple text editor, create a **CMakeLists.txt** file in the **root** folder, input the following code:

```CMake
cmake_minimum_required(VERSION 3.11)
# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Games_Engineering)

#### Setup Directories ####
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#### Practical 1 ####
file(GLOB_RECURSE SOURCES practical_1/*.cpp practical_1/*.h)
add_executable(PRACTICAL_1 ${SOURCES})
target_include_directories(PRACTICAL_1 PRIVATE ${SFML_INCS})
target_link_libraries(PRACTICAL_1 sfml-graphics)
```

While that may look foreign, you can generally guess at what every line does. The good news is I'll provide all the CMake code you will need. 

### Creating the Solution, with CMake
If you are unfamiliar with CMake UI; Follow [This guide](https://github.com/edinburgh-napier/aux_guides/blob/master/cmake_guide.pdf)


**Remember to place the build folder *OUTSIDE* of the repo folder. Preferably your desktop, *NOT* your H drive**

{:class="important"}
**NEVER Build from your H drive!** 
Or a memorystick / External HDD
	

The build folder will never contain work you need to save or commit. All code resides in the source directory.

Once configured and generated, you can open the .sln file in the build folder. You should not need to touch any solution or project settings form within Visual Studio.
The solution is set up so you don't have to do much work yourself or even understand Visual Studio settings.
