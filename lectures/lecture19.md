---
title: "Lecture19"
keywords: Lecture
tags: [Lecture]
permalink:  lecture19.html
summary: lecture19
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---

<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 19 - Performance Optimisation
### SET09121 - Games Engineering

<br><br>
Babis Koniaris
<br>


School of Computing. Edinburgh Napier University


---

# What is Performance Optimisation?

- Optimisation is about making the best use of a resource.
- Optimisation in software is about making best use of our computer hardware resource(s).
- There are different areas we can optimise for in software, but we will focus on performance.
- Performance is about getting the most work done in the shortest amount of time with our computing resource.
- Therefore, in a game, we are worried about:
    -  producing a frame in a reasonable time (typically 16.6ms) 
    -  performing the most work possible in that time to give a good gameplay experience.
- We are going to look at code level concerns mainly. Turning down update frequencies of systems is another strategy.


---

# Premature Optimisation

 Two famous quotes by Donald Knuth:
- "We should forget about small efficiencies, say about 97% of the time: premature optimization is the root of all evil. Yet we should not pass up our opportunities in that critical 3%."
- "In established engineering disciplines a 12% improvement, easily obtained, is never considered marginal and I believe the same viewpoint should prevail in software engineering."

---

# Premature Optimisation

Basically, Knuth argues that we should not let performance considerations determine the design of our code -  it makes the code more difficult to work with.

I think a good rule for the module is -  get your game working first; then worry about extra features and performance optimisation.

A good approach is to design-build-measure-optimise. 


---

# The 80/20 Rule

- You might have heard of this...
- Pareto Principle (or 80/20 rule) states that 80% of output comes from 20% of input.
- Applied to programming, we can say that 80% of processor time will happen in 20% of our code.
- It does make sense -  loops normally are the biggest area of computation in your application.


 ![image](assets/images/80-20.jpg) <!-- .element width="60%"  -->

---

# What are we interested in?

- There are two areas we can focus on to improve program performance for our games.
- **CPU utilisation**:
 - How well are we using the processor? Is it doing work it doesn't need to?
- **Memory usage**
 - Is memory effectively accessible to the processor? Is the processor waiting too long to do memory operations?
- We will focus on these two areas, looking at best-practice on the CPU and memory usage.
- There are many more techniques and tricks we can use, but normally they come down to these same two areas.




---

# First big trick

Release mode and run without debug

- Some student's don't understand that a program can execute outside Visual Studio.
- More don't realise that Visual Studio hooks into an application.
- Also, debug builds add extra code to interrogate state -  this slows down programs.
- So, don't do it in final builds.


 ![image](assets/images/run-no-debug.JPG)


---

# Second big trick 

Avoid I/O or do it better

- During debugging, we often output values to the console to check behaviour.
- I/O like this is very slow, requiring your program to interact with the OS and present data.
- You should avoid this I/O as far as possible in final builds.
- **IF you must have I/O**, then follow some rules to make things faster:
    - Don't use C++ I/O (`cin` and `cout`) -  these are slow because of error checking.
    - Don't use the C++ end-of-line terminator (`endl`) as this also flushes a stream, which is slow.
    - Do use C-style I/O (from the `cstdio` header) such as `printf`, etc. These are low-level and faster.


---

# Metrics

- Let's define metrics that allow us to talk about performance .
- FPS: Frames-Per-Second. 
    - The key measure most gamers like to talk about. The typical FPS displayed is **average** the number of frames processes per second. 
- Frame Time:  
    - This is actually what we are interested in. How long does it take the game to produce and render a **single** frame? Typically we aim for 16.7ms (60FPS) or 33.3ms (30FPS).
- Speedup
    -  When we make an improvement we need to understand what that improvement is. Speedup is the calculation of the original time against the new time. It is calculated as $S=\frac{original}{new}$.


---

## Step 1 - Only process what you need


---

# Alive Flag

- The first tactic we can use to improve processing is to flag if processing something can be skipped.
- An alive flag is a typical technique to indicate that an object should not be processed.
- This can be extended into other parts of the system:
    - If entity in base for example.
```cpp
if (alive) {
    DoSuperExpensiveOperation();
}
...
if (health == 0) {
    alive = false;
}
```


---

# Object Pool

- Object creation and destruction is very expensive.
- It involves memory allocation, function calls, grabbing bits and pieces, maybe loading content.
- It can also lead to objects being scattered around memory -  expensive to jump around.
- An object pool fixes that (especially when combined with alive flags):
    - Allocate max number of objects required.
    - When a new object is needed grab from allocated pool and set necessary values.
    - When finished, flag as not-alive and give back to pool.


---

# Dirty Flag

- Some game data is processed each frame to allow our game to have a dynamic nature.
- However, a lot of data only changes in some circumstances.
    - For example, the player only moves when the user controls them.
- Rather than reprocess certain data every frame, we can use the dirty flag to say that data should be reprocessed that frame.

```cpp
if (player moved) {
    Change position in primary data
    Set dirty flag on primary data
}
...
if (dirty flag is true) {
    Process secondary data (expensive)
    Set dirty flag to false
}
```

---

## Step 2 - Only draw what is visible


---

# Visible Flag

- Rendering to the screen is one of the most expensive processes in games.
    - It's why we have dedicated graphics hardware.
- We can use our flag technique to determine if an object is visible and therefore should be rendered.
- This allows us to hide objects/turn off their rendering when we want.
- It also allows us to add objects that should not be rendered.
    - Remember - what you see when playing a game isn't all that is there.

```cpp
    if (visible)
    {
        Render object (expensive)
    }
```


---

# Spatial Partitioning

- Another question is whether an object is even on screen.
- Spatial partitioning allows us to divide the world up so we only render the parts that are visible.
- Also used for collision detection optimisation.

![image](assets/images/spatial-partition.png) <!-- .element width="80%"  -->


---

# Example - Horizon Zero Dawn

<video class="middle" width="960" height="540" loop autoplay>
  <source src="assets/videos/horizon.mp4" type="video/mp4">
</video>


---

## Step 3 - Think about your memory

---

# Memory

Allocate Your Required Memory First
- We have mentioned this a few times now.
- If you are used to the Java and C\# model of just calling `new` randomly in your code -  stop and think.
- Memory allocation (and subsequent deallocation) is expensive on the free store.
- Try and allocate everything you need at the start of a level or the game. Then it is there and you can access it uniformly.
- Data is also near similar data -  this allows quick processing of blocks during similar operations.


---

# `constexpr` What You Can

- `const` values are typically replaced by the actual value in compiled code.
    - Less lookup time.
- `constexpr` values are replaced, and can be calculated at compile time.
    - So you can produce certain functions that are compile time processed.
- Compile time means the code is not processed during runtime.

```cpp
constexpr int N = 1000;

constexpr int factorial(int n)
{
    return n <= 1 ? 1 : (n * factorial(n - 1));
}

//compiler does this!
constexpr int Nfav = factorial(N); 

```

---

# Memory Alignment and Cache Coherence
- We talked about this during our memory and resource management lectures.
- Memory alignment means that data is aligned in memory, allowing the minimal reads to occur to access the data we need.
- For cache coherency we discussed the difference in processing a multi-dimensional array using different indices, due to memory layout. For example, the first `for` loop below is faster than the second.

```cpp
for (int i=0; i < 32; i++)
    for (int j=0; j < 32; j++)
        total += myArray[i][j];

for (int i=0; i < 32; i++)
    for (int j=0; j < 32; j++)
        total += myArray[j][i];
```


---

# Optimise Multi-dimensional Arrays

- As stated, memory is organised so the second dimension is the one that matches sequential layout.
- When we do need to transition across the first dimension, we need to help the compiler and processor.
- Wasting some memory by having this dimension as a power-of-two helps.
- The processor and compiler can make optimised jumps and do so without a multiply (bitshift operations are fast).

```cpp
    int N[80][25];
    int N_optimised[80][32];
```


---

## Step 4 - Use tools to find slow bits


---

# Finding Hot Paths -  Using Tools

Tools do a good job of finding code that is slowing things down.


![image](assets/images/hot-path.png) <!-- .element width="80%"  -->


---

# Bottlenecks

- The key aim with tools is bottleneck identification.
- Once you find a bit of your code that is impacting performance, you need to identify what, if anything, can be done about it.
- Often, these bottlenecks are loops that are processing lots of data.
- Even a small tweak here can make all the difference.


 ![image](assets/images/bottleneck.jpg)


---

# Algorithmic Analysis

- And this is where algorithmic analysis can come in.
- Abstractly measuring your algorithms, finding more efficient algorithms, and optimising the algorithms you have is important.
- See your Algorithms and Data Structures material for more insight.


 ![image](assets/images/alg-analysis.jpg)


---

## Step 5 - Optimise function calls


---

# Function Calls Cost

- Function calls have a cost associated with them.
- Two things have to happen.
    1.  Set up the parameters on the stack -  copy data.
    2.  Jump to the new code position.
- On return there is a jump back again.


 ![image](assets/images/function-call.png) <!-- .element width="25%"  -->


---

# `inline` Function Calls

- One of the first optimisations we can do for functions is inlining.
- An `inline` function is one we have asked the compiler to replace the function call with the actual code.
- For small functions this is good -  avoid functions.
- For big functions not so much -  larger executables.
- However, it depends on the frequency the function is called.


```cpp
    inline int add(int x, int y)
    {
        return x + y;
    }
```


---

# `static` Local Functions

- A `static` function is one that exists within a certain context or
    scope (e.g. class scope).

- If a function is `static` in a C++ code file, the compiler knows it
    can try and optimise it without affecting external code.

- Effectively, rearranging and possible inlining can occur, speeding
    up the program.

```cpp
    static int add(int x, int y)
    {
        return x + y;
    }
```


---

# `virtual` Function Calls

- `virtual` functions have an additional cost.
- A `virtual` function call involves a lookup on the object to determine which function to call.
- Effectively we are double jumping in this instance.


 ![image](assets/images/virtual-function.png)


---

# Exceptions are the Enemy

- If you are from a Java or C\# background you are probably used to using exception calls.
    - `try` and `catch` statements.
- C++ also uses exception statements.
- However, an exception catch is very expensive -  sometimes **thousands of instructions**.
- A better technique is to set a flag that can be tested.
    - This is the standard C model -  using a `get_error` function.

---

# `noexcept` What You Can

- The `noexcept` keyword can be applied to a function to indicate it won't throw an exception.
- This serves two purposes:
    1.  The compiler can optimise the code as it knows no exception to be thrown.
    2.  The function won't throw an exception externally -  allows isolation of an exception.

```cpp
    class my_class
    {
    public:
        void do_work() noexcept
        {
            // Do something
        }
    };
```


---

#  `const` What You Can

- Basically set everything you can to `const`.
- A `const` method is one that will not change the object.
- Therefore the compiler can optimise the code based on access again.

```cpp
    class my_class
    {
    public:
        void do_work() const
        {
            // Do something
        }
    };
```

---

## Step 6 - Use low-level techniques


---

#  Avoid Branching

- A branch (an `if` statement of loop) has a cost to check and a cost to jump.
- The jump cost might be unavoidable.
- However, do you need the `else`? This is a second jump that might be unnecessary.

```cpp
    if (alive)
    {
        // Do work
    }
    ...
    if (what)
    {
        // Do work
    }
    else
    {
        // Do work
    }
```



---

# Write Better `for` Loops

- For loops are one of the most expensive parts of your application due to the number of iterations.
- They are also one of the best places to optimise -  we will look at parallelisation here also.
- One particular point is avoiding doing work that the loop statement can do -  such as the indexer.

```cpp
    // Multiply every iteration
    for (int i = 0; i < 10; ++i)
        cout << i * 10 << endl;

    // Add every iteration
    for (int i = 0; i < 100; i += 10)
        cout << i << endl;
```

---

# Use Bitwise Operators When You Can

- Remembering that the CPU works in binary can be beneficial.
- Certain operations can be done using bitshift, bitwise and, and bitwise or.
- These operations are much faster than a multiply, equality, etc.

```cpp
    x = y * 8;
    x = y << 3;
```



---

# Write Some Assembly

- **FOR THE BRAVE!**
- The compiler will do its best to produce optimised code.
- However, it is not also going to do it as well as some hand-tuned code.
- There are tricks that can be done in assembly that will allow you to gain those few precious cycles each frame.


---

## Step 7 - Use more cores!!!


---

# Just Throw Some Threads at the Problem!?

- A simple solution may be to use more of the hardware.
- Multi-core means running multiples parts of the program at once is an option.
- There are different techniques: OpenMP, parallel execution (C++17), and compiler specific options for `for` loops are easy wins.

```cpp    parallel_for(size_t(0), size, &(size_t i)
    {
        do_work(i);
    });
```


---

# Cost of Threads

- Threads do have a cost.
- Each thread uses CPU time as well as needing about 1MB of memory.
- For the CPU to switch between threads, a number of operations have to happen. This is very expensive.
- It gets worse when the thread switches core.
- If you can, mapping threads to hardware can make your life easier.
- Generally using no more threads than you can physically support is a good rule.


---

# Don't Lock it Down

- Some of you will do Concurrent and Parallel Systems next year!
- This module uses synchronous behaviour to manage concurrency.
- Anything involving locking one thread from progress is very expensive.
- Also, locking means no work being done.
- Aim to have asynchronous tasks which you wait for all to complete -  this will allow best performance.


---

## Summary


---

# Summary

- We've looked at a number of techniques for optimisation, and there are many more.
- Some of these techniques are C and C++ specific, although many can be used across languages.
- Being thoughtful when writing code can be useful.
- But basically, use tools to find issues and try and fix them.
- Parallelisation is a good solution to performance problems... if you do it correctly.
- We have a module in fourth year that examines this.
