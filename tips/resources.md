---
title: "Resources, how does my game find them?"
keywords: Runtime Resources
tags: [ssh,keys]
permalink:  resources.html
sidebar: home_sidebar
---

When your game runs, and needs to load some assets from disk. ie:
```cpp
spritesheet.loadFromFile("res/img/invaders_sheet.png")
```
It needs to be able to safely and reliably find those files. There are a few ways to go about this, and I'll discuss them here.

# Where should I put my files anyway?
I believe you should have your runtime assets set up as if the game were installed on a users PC; Inside the game's folder, next to the .exe. 

Our .exe lives inside the build folder, which we don't version control or care about. So we need to keep the assets somewhere safe, and either copy them over to the build folder or change the working directory to the source folder (Yuck. This is what VS does by default), or even hardcode the paths (Don't ever do this)


## Differences from running in VS, and running the .exe

When you complile your game, it build a .exe somewhere. 

We set this in the cmake here `SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")` .

So it should be something like `C:/users/YOU/Desktop/GAMEBUILD/Release/game.exe` .

Release and debug configurations should get their own subfolder.

### Running in VS

Pressing *Debug*, or *start without debugging* doesn't just launch the .exe, it wraps some Visual studio magic around it to catch exceptions, debug memory leaks, and inspect performance. It also by default **Changes the working directory**.
The working directory is the directory the game will look to as it's default path for files.

### Running the .exe yourself

If you go find the exe and click on it, the working directory *should* be the same directory the .exe lives in.
[Note: Sorry Mac users, Apple is... complicated and ever changing so may not do this. Good luck?]


# Ok, So how do I do it?

## Method 1 - The wrong way - Hardcoded paths

You're an absolute madman and your game never needs to work on another PC?
```cpp
spritesheet.loadFromFile("C:/users/YOU/Desktop/GAMEBUILD/Release/res/img/invaders_sheet.png")
```
This will work, and is useful for debugging issues, but don't have this as a permanent solution.

**If you do this for your coursework you WILL lose marks**


## Method 2 - Change Working Dir, copy files from source to build 

```cmake
set_target_properties(2_INVADERS 
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)
```

So when we run our game in VS it will look for file in the build folder, just as if we went and ran the .exe ourselves. **You have to add this for each individual project!**

But we still haven't got our resources in the build folder yet, they are still in the source folder. That means until you complete the next step, this might break. We could build some sort of batch script to do this, but we can tap into some CMake magic to wrap this all up into our build and compile step.

### Copy all resources via Custom CMake Target

CMake can add more than just executables to the solution, it can also add \"custom targets\". We just add below, and add a slightly modified version of our copy script within it. **You only need to do this once per CMake file - put it under the External Dependencies**

```cmake
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} 
  -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
)
```

Then for all of your projects that require things from the res folder, add the following under the set_target_properties command from above:
```cmake
 add_dependencies(2_INVADERS copy_resources)
```

If you need to trigger a copy, right click and build on the "copy_resources" project in VS.

**You will need to reconfigure CMake now**

{:class="important"}
**REMEMBER: DO NOT POINT TO ANYWHERE ON YOUR H DRIVE!!**<br />
If you do this, sometimes the res folder isn't a folder - just a strange blob! 
