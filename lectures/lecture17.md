---
title: "Lecture17"
keywords: Lecture
tags: [Lecture]
permalink:  lecture17.html
summary: lecture17
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---

<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 17 - Performance Optimisation
### SET09121 - Games Engineering

<br><br>
Leni Le Goff
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

- A debug build is far slower than a release build
- Running with "Debugging" mode on in a build is far costlier than without debugging
- To identify the true performance: build with Release, execute without debugging


```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

 ![image](assets/images/run-no-debug.JPG)<!-- .element width="40%"  -->




---

# Second big trick 

Avoid I/O or do it better

- During debugging, we often output values to the console to check behaviour.
- I/O like this is very slow, requiring your program to interact with the OS and present data.
- You should avoid this I/O as far as possible in final builds.
- When using `cout`, avoid the end-of-line terminator (`endl`), as this also flushes a stream, which is slow.
- `cout` might be slower than `printf` by default, but that's fixable with `std::ios::sync_with_stdio(false);`


---

<!-- .slide: class="leftalign" -->

# Debug-only code

In cmake:

```bash
target_compile_definition(executable_name PUBLIC $<$<CONFIG:Debug>:DEBUG>)
```


The following code will be compiled and executed only in debug. 
```cpp
#ifdef DEBUG
//some code
#endif
```


---

# Metrics

- Let's define metrics that allow us to talk about performance .
- FPS: Frames-Per-Second. 
    - The key measure most gamers like to talk about. The typical FPS displayed is the **average** of the number of frames processed per second. 
- Frame Time:  
    - This is actually what we are interested in. How long does it take the game to produce and render a **single** frame? Typically we aim for 16.7ms (60FPS) or 33.3ms (30FPS).
- Speedup
    -  When we make an improvement we need to understand what that improvement is. Speedup is the calculation of the original time against the new time. It is calculated as $S=\frac{original}{new}$.


---

# Measuring the frame time

```cpp
while(window.isOpen()){
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();
    ...
    // run the code for the current frame
    ...
    #ifdef DEBUG 
    std::cout << "Frame time : " << dt << std::endl;
    #endif
}
```

---

## Step 1 - Only process what you need


---

# Alive Flag

- The first tactic we can use to improve processing is to flag if processing something can be skipped.
- An alive flag is a typical technique to indicate that an object should not be processed.

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

# Basic Object Pool implementation

```cpp
template<typename T, typename... Targs>
class BasicEntityPool{
public:
    BasicEntityPool(int size) : _size(size){
        _entity_pool = std::vector<std::shared_ptr<T>>(size);
        for(std::shared_ptr<T> &entity: _entity_pool)
            entity = std::make_shared<T>();
    }
    std::shared_ptr<T> create(Targs... params){
        for(std::shared_ptr<T> &entity: _entity_pool){
            if(!entity->in_use())
                entity->init(params);
        }
    }
    void update(const double &dt){
        ...
    }
    void render(){
        ...
    }
private:
    int _size = 0;
    std::vector<std::shared_ptr<T>> _entity_pool;
}
```

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
- Memory allocation (and subsequent deallocation) is expensive on the free store.
- Try and allocate everything you need at the start of a level or the game. Then it is there and you can access it uniformly.
- Data should also be near similar data -  this allows quick processing of blocks during similar operations.


---

# `constexpr` What You Can

- `const` is a qualifier used for readability, maintenance and performance
- `constexpr` takes this further: expression is calculated at compile time
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
- Memory alignment means that data is aligned in memory to minimize the reads to access the data that we need.
- For cache coherency we discussed the difference in processing a multi-dimensional array using different indices, due to memory layout. For example, the first `for` loop below is faster than the second.

```cpp
for (int i=0; i < 32; i++)
    for (int j=0; j < 32; j++)
        total += myArray[i][j]; // GOOD! Fast!

for (int i=0; i < 32; i++)
    for (int j=0; j < 32; j++)
        total += myArray[j][i]; // BAD! Slow!
```

---

## Step 4 - Use tools to find slow bits


---

# Finding Hot Paths -  Using Tools

Tools do a good job of finding code that is slowing things down.


![image](assets/images/hot-path.png) <!-- .element width="80%"  -->

[Visual Studio CPU usage](https://learn.microsoft.com/en-us/visualstudio/profiling/beginners-guide-to-performance-profiling?view=vs-2022)
and [Visual Studio Memory usage](https://learn.microsoft.com/en-us/visualstudio/profiling/memory-usage?view=vs-2022) tutorials.

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

## Step 6 - Branching and Loops


---

#  Branching: if vs switch statements 

- A branch has a cost to check and a cost to jump.
- **if statement** for complex conditions and/or few conditions
- **switch statement** when a lot of value to test for a single variable.
- If possible, use a switch statement instead of a lot of nested or consecutives **if/else if statements**

```cpp
    if (var == value1) { /* Do work */ }
	else if (var == value2) { /* Do other work */ }
    else if (var == value3) { /* Do other work */ }
	...
	else { /*fallback*/}
```
```cpp
    switch(var)
	{
		case value1:  /*do work*/ 
			break;
		case value2:  /*do other work*/ 
			break;
        case value3: /*do other work*/ 
			break;
        ...
		default:
			break;
	}
```



---

# `for` Loops

- For loops are one of the most expensive parts of your application due to the number of iterations.
- They are also one of the best places to optimise -  we will look at parallelisation here also.
- One particular point is avoiding doing work that the loop statement can do -  such as the indexer.

```cpp
    // Multiply every iteration
    for (int i = 0; i < 10; ++i)
        std::cout << i * 10 << std::endl;

    // Add every iteration
    for (int i = 0; i < 100; i += 10)
        std::cout << i << std::endl;
```

---

## Step 7 - Use more cores!!!


---

# Just Throw Some Threads at the Problem!?

- A simple solution may be to use more of your hardware resources.
- Multi-core means you can execute code in parallel in different cores at the same time
- There are different techniques: OpenMP, Intel TBB, parallel STL algorithms (C++17), async, threads, etc
    - More on SET10108: Concurrent and Parallel Systems

---

# Cost of Threads

- Threads do have costs: performance, cognitive and maintenance
- They require memory, and switching between threads costs time
- They can easily introduce bugs into your application
- Keeping track of application workflow with threads is harder


---

# Summary
 
- Premature optimisation is the root of all evil, but think of your algorithm choices.
- Use tools to identify bottlenecks. Fix if needed.
- Main performance measure: **Frame Time**
- **Don't release your game compiled in debug !**


---

# Summary (cont.)

Remember the optimisation steps and respect the order:

1. Process and store what is essential (flags, object pool etc..) 
2. Creation and heavy processing needs to be done at the right moment
3. Algorithmic optimisation:
    - function calls
    - for loops and if statement
    - const, static and constexpr
4. Parallelisation

---