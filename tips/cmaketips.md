---
title: "Cmake Tips"
keywords: Cmake Tips
tags: [cmake]
permalink:  cmake_tips.html
sidebar: home_sidebar
---

# copy_resources target

Take a look here  [Runtime Resources tips and tricks](resources)

# Shortcut Macro for linking libraries

Every project we will need will add to our CMake will have some identical options. Mainly linking with SFML and running our copy_resources target. Duplicated code is bad code. Let's fix that with some additional CMake coolness.

Add this to the end of your CMake script :

```cmake
#### Link Dependencies ####
foreach (exe ${EXECUTABLES})
  #Set working directory to build dir in visual studio
  set_target_properties(${exe} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
  )

  #Add all SFML libs.
  target_link_libraries(${exe}
    #lib_maths # We'll make this later.
    optimized sfml-graphics debug sfml-graphics-d
    optimized sfml-audio debug sfml-audio-d
    optimized sfml-window debug sfml-window-d
    optimized sfml-system debug sfml-system-d
    optimized sfml-graphics debug sfml-graphics-d
    optimized sfml-main
  )

  add_dependencies(${exe} sfml-graphics sfml-audio copy_resources)
endforeach ()
```

Now for each project you would do:

```cmake
## Space invaders
file(GLOB_RECURSE SOURCES 2_space_invaders/*.cpp 2_space_invaders/*.h)
add_executable(2_INVADERS ${SOURCES})
target_include_directories(2_INVADERS SYSTEM PRIVATE ${SFML_INCS})
set(EXECUTABLES ${EXECUTABLES} 2_INVADERS)
```

The `set(EXECUTABLES ${EXECUTABLES} 2_INVADERS)` 
