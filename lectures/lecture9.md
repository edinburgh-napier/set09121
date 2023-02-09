---
title: "Lecture 9 - C++ Memory"
keywords: Lecture
tags: [Lecture]
permalink:  lecture9.html
summary: lecture9
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 9 - C/C++ Memory and Resources
### SET09121 - Games Engineering

<br><br>
Babis Koniaris
<br>


School of Computing. Edinburgh Napier University



---

# Recommended Reading

- Game Coding Complete, 4th Edition. McShaffry and Graham.
 - Chapter 3 introduces some ideas.
 - Chapter 8 covers resource management.

![image](assets/images/game_coding_book.jpg) <!-- .element width="30%"  -->


---

# Basics of Memory


---

# Different Memory Types

- C++ (and applications in general) have three types of memory.
 - global (static) :   memory where global and static values are stored.
 - stack :   working memory.
 - heap (free-store) :   the rest of memory.
- Each has a different purpose and features.

```cpp
// Allocated in global memory.
int x = 10;

int main(int argc, char **argv)
{
    // Allocated on the stack.
    int y = 20;
    // Allocated on the heap (free store)
    int *z = new int(30);
    
    return 0;
}
```


---

# Scope and Stack 

![image](assets/images/stack-heap.png)


---

# Scope

```cpp
void function(int param_scope){
    // Main scope of the function
    int main_scope = 5;
    {
        // Scope A - can see main scope
        int A_scope = 10;
        {
            // Scope B - can see scope A and main
            int B_scope = 20;
        } 
        // B_scope removed from stack
        {
            // Scope C can see scope A and main, Scope B is no longer valid
            int C_scope = 30;
        } 
        // C_scope removed from stack
    } 
    // A_scope removed from stack
} 
// param_scope and main_scope removed from stack
```


---

# Memory Layout

- Memory is obviously just one big chunk.
- Addressed from `0x00000000` (0) to `0xFFFFFFFF` (4,294,967,295 in 32 bit systems).
- Memory is separated: stack at the top and the heap at the bottom.
- The processor can optimise memory streams to improve performance (and also cache).
- Jumping around the heap can be a major source of performance reduction.

![image](assets/images/mem-layout.png)


---

# Memory Access Times

- The CPU is fastest when accessing adjacent memory.
- If we jump around things slow down - sometimes dramatically.
- Consider a multi-dimensional array:
- The dimensions place memory in the continuous block differently.
- Access time difference between approach A and C can be 100x.
 - i.e. accessing all members using approach A could be 300ns; approach C 30000ns.


```cpp
int matrix[100][100][100];
// A
matrix[0][0][0] = 0;
matrix[0][0][1] = 1; // 4 byte jump.
// B
matrix[0][0][0] = 0;
matrix[0][1][0] = 1; // 400 byte jump.
// C
matrix[0][0][0] = 0;
matrix[1][0][0] = 1; // 40000 byte jump
```


---

# Memory Alignment

- The CPU also reads memory in fixed chunks.
- If a value is not aligned to these chunks, extra reads occur.
- Generally, the C++ compiler will fix this for you, but you can help.

![image](assets/images/mem-align.jpg) <!-- .element width="95%"  -->


---

# Memory Restrictions
You must consider the limits you have in memory.

 - **stack size** - depends on compiler and OS, can be set.
  - Windows is ~1MB.
 - **thread stack size** -  as above, but normally smaller.
 - **main memory** -  commonly ranges from 4GB to 16GB at present.
 - **virtual memory** -  if main memory runs out, the (slow) HDD used.
  - 64bit OS can address ~16.8 million petabytes of memory.
  - If you are using virtual memory you shouldn't be.


---

# Caches

- Different levels of cache replicate memory closer to the CPU to reduce access time.
- If data is in L1 cache can be accessed in about 0.5ns; main memory about 100ns.

![image](assets/images/mem-cache.png) <!-- .element width="95%"  -->


---

# Working with Memory in C


---

# C Memory Management

There are two functions of note

 - **malloc** - allocates space on the heap.
 - **free** - release space allocated.

You need to release everything you allocate or ***memory leaks***.

```cpp
// Declare value
my_data *data;
// Allocate resource on the heap
// Note the casting to the correct type.
// Note we need the number of bytes to allocate
data = (my_data*)malloc(sizeof(my_data));
...
// Do something with the data
...
// Free the resource
// If we don't do this and lose the pointer we get a leak
free(data);
```


---

# Pointer to and Dereference

Let us look at a function that takes a pointer to an `int` as an argument.

`void foo(int *x)`

To pass in a variable we have to get its *address* (a pointer), and pass that, rather than the actual variable
```cpp
int v = 10; //declaration of value
foo(&v); //pass in the address of v to foo
```

- Within the function we need to dereference the pointer to get access to the value.
 - `x` :   within `foo` this is a pointer - *the address of* `v`
 - `*x` :   allows access to the value stored in `x` - *the value of* `v`


---

# Arrays in C

- Arrays in C can be allocated on the stack or the heap.
- Stack allocated arrays need a known size at compile time.
- Heap allocated arrays can have any size - we just use `malloc`.
- An array is just a pointer to memory where the array starts.
```cpp
    // Stack allocated array
    // Known size at compile time
    int x[10];
    // Heap allocated array
    // Size defined at run time
    int *y = (int*)malloc(10 * sizeof(int));
    // Access is the same
    x[5] = 10;
    y[2] = 20;
    // Free is the same
    free(y);
```

---

# REMEMBER:

**An array is just a pointer to memory where the array starts!**

This is really important, and can lead to all sorts of bugs if you forget!


---

# Multidimensional Arrays in C

- Multi-dimensional arrays can also be stack or heap allocated.
- Multi-dimensional arrays are just an array of pointers.
- Each pointer to array can have a different size.

``` cpp
// On the stack
int x[10][10];
// On the heap
// Array of pointers
int **y = (int**)malloc(10 * sizeof(int*));
// Each array could be of different size
for (size_t n = 0; n < 10; ++n)
    y[n] = (int*)malloc(10 * sizeof(int));
// Have to free each array
for (size_t n = 0; n < 10; ++n)
    free(y[n]);
free(y);
```


---

# Copying and Pointing

- The main reason we have pointers in C is to allow data to be sent around *without duplicating it*.
- For large data objects this is a real problem.
 - Create object of 1MB size.
 - Call function with object - 1MB copy.
- Pointers overcome this problem nicely - a pointer is 4 or 8 bytes (32-bit or 64-bit).
- Pointers also enable data reuse, referencing, and better use of the heap.


---

# Working with Memory in C++


---

# C++ Memory Management

- C++ memory management is a bit easier.
- We don't need to know sizes, cast types, or even initialise separately.
- Two keywords:
 - `new` :   allocates memory on the heap.
 - `delete` :   frees allocated memory.


```cpp
// Allocate a single value
int *x = new int;
// Allocate a single value and initialise
int *y = new int(5);
// Allocate an array
int *z = new int[200];
// Free a value
delete x;
delete y;
// Free an array
delete[] z;
```


---

# Copying and Referencing

- C++ adds a reference type.
- References are like pointers, but have some restrictions.
- Effectively, we can pass-by-reference instead of pointer.
- This means we avoid a copy again - reference is 4 or 8 bytes

```cpp
// A value
int x = 5;
// A pointer
int *y;
// Getting the address of a value
y = &x;
// A reference
// Must reference a value
int &z = x;
// References don't have to be
// dereferenced
*y = 20;
z = 20;
```


---

# Construction and Destruction

- Memory allocation and deallocation in C++ calls constructors and destructors.
- Knowing when and what can be important.
- There are a lot of background functions called in C++ you have to be aware of.


```cpp
my_data do_work(my_data d) {
    // Constructor called for x
    my_data x;
    // ...
    // Move constructor called for x
    return x;
} // Destructor called for d and x

int main(int argc, char **argv) {
    // Constructor called for y
    my_data y;
    // Copy constructor called on y
    // Move assignment operator called on return value
    // Deconstructor called on return value
    my_data z = do_work(y);
    return 0;
} // Destructor called on y and z
```


---

# Arrays in C++


- C++ arrays are similar to C ones.
- There are also other options in C++ though.
- `array` type is statically sized, but acts more like a Java/C\# array. 
- `vector` is dynamically sized (like an array list). 
 - Actually the best option in most cases. Data is automatically resized and on the heap.

```cpp
// Allocate stack array
int x[100];
// Allocate heap array
int y[] = new int[100];
int *z = new int[100];
// Use new array type
array<int, 100> a;
// vector is dynamically sized
// Can set initial size
vector<int> v(100);
```


---

# Multidimensional Arrays in C++
- Basically the same as in C, but can use other array types as well.

```cpp
int x[10][10];
array<array<int, 10>, 10> y;
vector<vector<int>> z(10);
for (size_t n = 0; n < 10; ++n)
    z[n] = vector<int>(10);
```


---

# Smart Pointers

- Due to the pattern of allocation, deallocation, and keeping track of resources many programmers created in-house solutions to these problems.
- This led to many implementations of self-managing pointers - "smart pointer" - that would do the work for the programmer.
- The most popular implementation was seen in the Boost C++ libraries - Boost is known as the missing C++ API.
- Eventually smart pointers were standardised and added to the C++11 standard.
- It is now recommended you use smart pointers and not old (raw) pointers as standard.


---

# `shared_ptr`

- The most common smart pointer is `shared_ptr`.
- This pointer counts the references to the resource.
    - This is done by copy construction, destruction, etc.
- It is the closest to the Java and C\# reference type.

```cpp
// Make shared_ptr
shared_ptr<int> ptr = make_shared<int>(5);
// Can derefence as normal
int n = *ptr;
// Can get raw pointer - no counting
int *x = ptr.get();
// Counter increased by one in call
do_work(ptr);
// End of call, counter decreased by one
// Set pointer to nullptr; counter decreased by one
ptr = nullptr;
// Allocated resource now freed
```


---

# `unique_ptr`

- `unique_ptr` ensures there is only one owner.
- You cannot copy the pointer, only move it.
- It is faster than `shared_ptr` and you should try and use it as much as possible.


```cpp
// Make unique_ptr
unique_ptr<int> ptr = make_unique<int>(5);
// Can derefence as normal
int n = *ptr;
// Can get raw pointer - no counting
int *x = ptr.get();
// Have to move data into function
do_work(move(ptr));
// If do_work does not do anything to
// store data will be freed.
// ptr is nullptr automatically on move
```


---

# Assignment, Copying, and Moving

- We've hinted at a number of different concepts through this discussion.
- Assignment is whenever you use the `=` to set an object variable.
- Copying is when we create a new object from an existing one.
- Moving is like copying, but we move the already allocated resources to the new object. The original becomes empty.
- This is an important concept to understand in general in C++.
- If you want to work at the lowest level of C++ you really need to recognise these behaviours for optimisation purposes.


---

# Memory in Games


---

# RAII

- Resource Allocation is Initialisation.
- Always give ownership to an allocated resource to an object.
- As long as each resource has one explicit owner, when the owner is removed the resource is freed.
- So...
    - **Do not** give an entity a resource such as a loaded texture or audio clip.
    - **Do** give an entity a pointer or reference to such a resource.
    - Keep track of resources via central pools (we will look at a resource manager soon).
    - Try to allocate those resources when the object is created (we will discuss this soon).


---

# Data Sharing

- Although referencing is efficient to reduce memory usage, it can be more expensive for memory access.
    - The memory adjacency problem.
- It is common to copy data between different contexts to improve efficiency.
- For example having position data in the entity and the physics object.


```cpp
struct world_position {
    vec3 position;
    quat rotation;
};
struct physics {
    vec3 position;
    quat rotation;
    vec3 velocity;
    quat rotation_velocity;
};

Update(){
    trans.position = phys.position;
    trans.rotation = phys.rotation;
```


---

# Resource Management for Games


---

# Game Resource Management

- Games use a lot of resources.
 - Textures can be 100s of MB.
 - 3D model data can be 10s of MB.
 - Sound assets can be 100s of MB.
- We need to avoid loading and unloading these assets all the time.
- We also need to ensure that we only load the assets that are necessary.
- To do this we will use a resource manager.

---

# Resource Manager

- So we need a resource manager in our game.
- Its job:
 - Hide the details of how to load a specific resource.
  - e.g. we just load - we don't need to know the individual calls to load a texture.
 - Manage allocation and deallocation of resources.
   - Data-driven design.
 - Provide a single point to manage all of this.
    - Manager pattern, maybe singleton.
- So we just apply our design pattern thinking to the problem.


---

# Basic Operations

- Our game resource manager needs only a few different operations:
 - `initialise` :   as most of our game engine components will likely have.
 - `load_resource` :   loads and/or retrieves a resource.
 - `unload_resource` :   unloads a loaded resource.
 - `clear_all` :   unloads all loaded resources.
- That is all.
- Depending on your approach:
 - You can have a singleton with typed loads, unloads, and storage
 - You can have a different resource manager for each type.
- Either approach uses basically the same memory.


---

# Storing Resources

- We use lookup tables to store resources.
- We need some kind of key - AAA games will do something fancy. If you've used console commands you have seen this.
- The key is just matched to the actual resource.
- We check that the resource isn't loaded before trying to return it.


```cpp
unordered_map<string, texture> textures;

texture load_resource(const std::string &file){
    if (textures.find(string) != textures.end()){
        return textures.find(string).second();
    }else{
        // Don't care how this works
        texture t = load_texture(file);
        textures[file] = t;
        return t;
    }
}
```

**ALWAYS LOAD YOUR ASSETS AT THE START OF THE GAME/LEVEL!<br />DO NOT DO IT DURING A FRAME!**


---

# Switching Levels

- A resource manager also allows you to manage loading and unloading between levels.
- It works also for the other management components.
- When switching levels:
    - Unload entities.
    - Unload physics resources.
    - Unload assets.
    - Load new assets.
    - Set up new physics.
    - Create new entities.
- You do get better systems but the basic premise is the same.


---

# Summary


---

# Summary

- We've covered a lot of ideas today.
- We looked at how memory works in general.
- We looked at how memory is used in C.
- We looked at how memory is used in C++.
- The key take away is how we apply this to manage game resources.
- You should be able to understand the basic premise of a resource manager, why we need it, and how it operates.