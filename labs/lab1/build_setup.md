---
title: "Build Setup"
keywords: build_setup
tags: [build_setup]
permalink:  build_setup.html
summary: build_setup
sidebar: home_sidebar
---

To begin here I am assuming you have a local repo, with the SFML submodule.

## Get Some Code
With a simple text editor, create a **main.cpp** file in the **practical_1** folder, input the following code:
(I always use Notepad++, but feel free to use one you're used to, as long as it isn't Notepad or Wordpad!)

```cpp
#include <SFML/Graphics.hpp>

int main(){
  sf::RenderWindow window(sf::VideoMode({200, 200}), "SFML works!");
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

This is the basic "Hello world" for SFML, we will use this to test everything is in-place and working.

## Building code with CMake
Now we need to create our development environment. 

If you were mad you could create makefiles and do it like a 1990's hacker. 

If you were naive you may want to open up Visual Studio, create a new project, and spend 2 hours digging thourgh build settings. 2005 called and it want is workflow back.

While C++ doesn't have a standardized package and build system (i.e, Pythons's pip, Nodes's npm), we have something that's pretty close: **CMake**

CMake allows you to write a **CMakelists.txt** file. In this you specify what your program is, where the source files are, and where any of it's needed dependencies are. From here CMake will take that config file and make you a perfect already set up Visual Studio solution. No need to touch configuration options in Visual Studio, hooray!

CMake has many more benefits, but what we care about is:
1. You only need to store CMakelists.txt files in your Repo, __**no huge VSsolutions.sln**__
1. CMakelists.txt play nice with git, you can easily see and track changes
1. CMake doesn't just build Visual Studio solutions, it can build Xcode, Clion, Eclipse, makefiles etc. This is an important step in writing cross-platform code.

CMake Downsides:
1. It's yet another new scripting language to learn


### Create the CMake script
With a simple text editor, create a **CMakeLists.txt** file in the **root** folder (not practical_1) and input the following code:

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

While that may look odd, you can generally guess at what every line does. The good news is we'll provide all the CMake code you will need.
**Having said that, it is useful to understand it, so you can debug it!**

### Installing CMake on your machine
We have CMake in D2 (locally, or via AppsAnywhere), but you'll need to get it for your own machine. You can go get the latest version at [https://cmake.org/](https://cmake.org/). 

**Make sure you have it installed, before continuing!**

### Installing Visual Studio Community 2019 or 2022 on your machine
We have Visual Studio Community in the labs, so if you want to work on your own machine, please download and install it. You can use Code if you like, but I won't be able to help you as much. First things first then, go get it here: [https://visualstudio.microsoft.com/vs/community/](https://visualstudio.microsoft.com/vs/community/)

Next, make sure you've **ACTUALLY** installed the C++ compiler modules. The biggest issue people have with CMake is it failing because it can't find Visual Studio. This is often because you have VS installed via Unity, and it only has C# support. Open the Visual Studio Installer, click **More** against the installation, and pick **Modify** from the list. You should see a bunch of things you can install. From the **Workloads** tick the **Desktop development with C++** option. This should, by default, give you everything you need. Finally, you just need to pick **Modify** from that menu, to get it installing!

{:class="important"}
**Make sure you have installed the C++ stuff before moving on, or you'll just be confused!**

### Creating the Solution, with CMake
If you are unfamiliar with CMake UI; Follow [This guide](https://github.com/edinburgh-napier/aux_guides/blob/master/cmake_guide.pdf)

Make sure you have installed the C++ packages mentioned above, and that you pick Visual Studio 16 2019 or Visual Studio 17 2022, depending on which one is installed on the machine you're working on. You can leave the other settings alone.

{:class="important"}
**Oi! Stop there! Go and actually read the guide before you move on!**

**Remember to place the build folder *OUTSIDE* of the repo folder and *NOT* your H drive**
Remember: your desktop might also be on your H drive!

{:class="important"}
**NEVER Build from your H drive!** 
Or a memorystick / External HDD
	

The build folder will **never** contain work you need to save or commit. All code resides in the source directory.

Once configured and generated (you might have to generate a couple of times until the red goes away), you can open the .sln file in the build folder. You should not need to touch any solution or project settings form within Visual Studio.
The solution is set up so you don't have to do much work yourself or even understand Visual Studio settings.

### Run The solution
CMake should have generated a solution project for you in your build folder, open it.
Practical_1 should be available as a project within it. Compile and run it!

{:class="important"}
You should see a green circle. If you do, congratulations! Your basic setup is working! If not...

One common issue you will run into is when you press the little green arrow in Visual Studio is that it will say "Unable to start program... Access is denied." **Don't worry, it's probably not broken!** This is usually because by default VS puts one of the CMake projects (usually ALL_BUILD) as the Startup Project. Just right-click the project you want to compile and run (at the moment PRACTICAL_1) and select **Set as Startup Project**. The current Startup Project is shown in bold, by the way. Once you've done that, click the little green arrow again!

## Saving your work
You should take this opportunity to commit and push your work. If you know the basics of git, this is nothing new.
```bash
git status
```
Running git status should show you all the files you have modified so far. We need to "Stage" or "add" these files. 

```bash
git add .
```
This is a shorthand to tell git that we want to commit everything.


```bash
git commit -m "SFML hello world working"
```
Now we run the actual commit, which will store the current version of all your ("Staged")files to the local repo. Note that this is only local, you now need to push it up to github.

```bash
git push
```

This is a light-speed gloss over what version control can do for you. If this is new and strange to you, you really should take some time to look through some online git tutorials and guides to get comfortable with what it does and how it works.

## Starting from scratch
If you want to work on another PC, or at home. You obviously don't need to create a new repo.
The steps you need to do are simply:
1. Clone/Pull the repo down from your github
1. Run CMake to generate your build folder

The key here is that you only need to version control your **source** folder.
The build folder, generated by CMake, is full of large junk that Visual Studio needs. Importantly, you don't need to save this or even really care about what's in there! Hooray!
You can re-generate it anytime anywhere using CMake.

Hopefully you can see why this is a much better way of doing things, for sustainability and for working in teams!

---
Previous step: [Repo setup](repo_setup)
Next step: [Pong](pong)
