---
title: "Getting Started"
keywords: sample homepage
permalink: index.html
summary: "Hello world!"
sidebar: home_sidebar
toc: false
---
## Welcome to Games Engineering!

The pre-requisite knowledge you require is:

-   a working knowledge of object-oriented programming in a high-level
    language. It an ideal world this is C++ but Java and C\# programmers
    should be able to cope with the material.

-   at least a grasp of mathematical concepts such as trigonometry,
    algebra, and geometry. Linear algebra and matrix mathematics are
    very advantageous.

-   a willingness to spend time solving the problems presented in this
    workbook. Some of the exercises are challenging and require effort.
    There is no avoiding this. However, solving these problems will
    significantly aid your understanding.

-   Git version control basics


### Development Environment
You will be able to work on this module on any operating system that has a modern C++ compiler.
We will be using the CMake build system, so you won't need to use a specific IDE. 
We recommend Visual Studio 2017 on Windows, and Clion on Mac/Linux. This module expects that you follow best practices when it comes to software development:  i.e use version control effectively and properly, maintain a clean codebase, and implement testing procedures(more on this later).
You will also need Git and CMake installed, and added to your PATH.

### SFML
You may be glad to know that for this module, the Rendering of your games projects will be handled for you. We will be making use of the 2D Games/Graphics library SFML [sfml-dev.org](www.sfml-dev.org). This handles many of the low-level grunt-work that goes into making games, allowing us to focus on the high level design. There is also a wide array of tutorials and pre-existing support available around the internet if you run into a SFML-specific problem.