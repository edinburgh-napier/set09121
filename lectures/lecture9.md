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
Leni Le Goff
<br>


School of Computing. Edinburgh Napier University



---

# Recommended Reading

- Game Coding Complete, 4th Edition. McShaffry and Graham.
 - Chapter 3 introduces some ideas.
 - Chapter 8 covers resource management.

![image](assets/images/game_coding_book.jpg) <!-- .element width="30%"  -->


---

# Memory Layout

![image](assets/images/memory_app.png)

---

# Memory Layout (cont.)

- Memory is obviously just one big chunk.
- Addressed from `0x00000000` (0) to `0xFFFFFFFF` (~4 GigaBytes in 32 bit and ~16 ExaBytes in 64 bit systems).
- Memory is separated: stack at the top and the heap at the bottom.
- Jumping around the heap can be a major source of performance reduction, it is more efficient to use the stack or the static memory.

---

# Memory allocation in C++

Depending on how a variable is declared in C++ they will be allocated in a specific part of the memory

```cpp
int x = 10; // Allocated in the static memory.

int main(int argc, char **argv)
{
    static int z = 45; // Allocated on the static memory
   
    int y = 20; // Allocated on the stack.
    
    int *z = new int(30); // Allocated on the heap
    
    delete z;// the heap is managed manually!

    return 0;
}//memory from the stack freed here.
```

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

<!-- .slide: class="split" -->


# Memory Access Times

- The CPU is the fastest when accessing adjacent memory.
- If we jump around things slow down - sometimes dramatically.
- Consider a multi-dimensional array:
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
![image](assets/images/memory_access.png)


---

# Memory Alignment

- The CPU also reads memory in fixed chunks.
- If a value is not aligned to these chunks, extra reads occur.
- Unless you do Weird Stuff with pointers and allocation, this will not be an issue

![image](assets/images/mem-align.jpg) <!-- .element width="95%"  -->

---

# Caches

- Different levels of cache replicate memory closer to the CPU to reduce access time.
- If data is in L1 cache can be accessed in about 0.5ns; main memory about 100ns.

![image](assets/images/mem-cache.png) <!-- .element width="95%"  -->

---

# Stack faster than Heap

- The stack as a preallocated chunck on the memory. So, all the addresses are allocated at the start.
- Variable on the heap needs an address allocated and deallocated at runtime and this is "slow".
- The stack is organised as a LIFO (last in, first out) data structure, so, allocating and deallocating is just a matter of moving a pointer up and down. 
- Because the CPU read things in fixed chunks, the whole stack can be sent to the CPU caches at once, hence faster.



---

# Memory Restrictions
You must consider the limits you have in memory.

 - **stack size** - depends on compiler and OS, can be set. Generally a few MB
 - **thread stack size** -  as above, but normally smaller.
 - **main memory** -  Depends on the RAM size
 - **virtual memory** -  if main memory runs out, the "slow" HDD or SSD are used.


---

# C++ Memory Management

- To allocate and deallocate memory on the heap:
 - `new` :   allocates memory.
 - `delete` :   frees allocated memory.


```cpp
int x = 0 //allocated on the stack
// Allocate a single value
int *x = new int;
// Allocate a single value and initialise
int *y = new int(5);
// Free a value
delete x;
delete y;
```

---

# Construction and Destruction of objects

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
    // Destructor called on return value
    my_data z = do_work(y);
    return 0;
} // Destructor called on y and z
```

---

## C++ data format


---

# C-style array

- Arrays in C can be allocated on the stack or the heap.
- An array is just a pointer to memory where the array starts.

```cpp
/*array allocated on the stack 
 *and initialised with brace-enclosed initializer list*/
int tab[3] = {1,2,3}; 

int *tab2 = new int[3]; //allocated on the heap
tab2[0] = 4; //equivalent to *tab2 = 4
tab2[1] = 5; //equivalent to *(tab2 + 1) = 5
tab2[2] = 6; //equivalent to *(tab2 + 2) = 6

//this will print the address of the first entry
std::cout << tab2 << std::endl; 
//this will print the address of the second entry
std::cout << tab2 + 1 << std::endl; 

delete[] tab2; //memory freed like this
```

---

# REMEMBER:

**An array is just a pointer to memory where the array starts!**

This is really important, and can lead to all sorts of bugs if you forget!

![image](assets/images/c-style_array.png)<!-- .element width="80%"  -->


---

# C++ array and vector

- `std::array` is similar to C-style array in term of performance and memory management
- `std::vector` is dynamically sized array. The data are allocated on the heap but managed automatically.
    - reallocation of memory to increase a vector's size is slow!
- if your data can be handled in fixed size array better to use `std::array`

```cpp
#include <array>
#include <vector>

//allocated on the stack
std::array<double,3> a = {0.1,0.4,0.7}; 

//allocated on the heap
std::vector<char> v = {'a','b','c'}
std::vector<char> v2(100,'v'); //100 times 'v'
```

---

# Multidimensional Arrays in C/C++


```cpp
int x[3][3];
std::array<std::array<int, 10>, 10> y;
std::vector<std::vector<int>> z(10);
for (size_t n = 0; n < 10; ++n)
    z[n] = std::vector<int>(10);
```

---

# Other containers of C++

- There are a lot of data containers in c++ that answers different problems
- `std::map<key,value>` contains key/value pairs with unique key.
```cpp
std::map<int,std::string> data;
data[1] = "first"; //insertion
std::cout << data[1] << std::endl; //access
```
- `std::multimap<key,value>` contains key/value pairs with non-unique key
- `std::queue<value>` first-in,first-out (FIFO) container
- `std::stack<value>` last-in,first-out (LIFO) container

https://en.cppreference.com/w/cpp/container.html

---

## Copy or Not Copy

---

# Copying and Pointing

- The main reason we have pointers in C++ is to allow data to be sent around *without duplicating it*.
- For large data objects this is a real problem.
 - Create object of 1MB size.
 - Call function with object - 1MB copy.
- Pointers overcome this problem nicely - a pointer is 4 or 8 bytes (32-bit or 64-bit).
- Pointers also enable data reuse, referencing, and better use of the heap.

---

# Function and reference

**In the case of function handling large data.**

- Input data to a function should be a constant reference
- Ouput data should be a reference
- Returned value will be generally used for error handling
    - Avoid returning heavy data !

```cpp
int function(const Type1& input, Type2& output);
```

```cpp
int get_value(int key, const std::map<int,std::string> &input, 
              std::string& output){ //passing as constant reference will 
    //check if the key exist in the map. If not return 1;
    if(input.find(key) == std::map<int,std::string>::end())
        return 1;
    //if the key exist
    output = input.at(key);
    return 0;
}
std::map<int,std::string> data = ...
std::string value;
if(get_value(20,data,value) == 1)
    std::cerr << "key = 20 is not in the map" << std::endl;
else
    std::cout << value << std::endl;
```

---

# Class and pointers as link to another data

- If an object of type A need access to the data of another object (type B) but does not need to a copy of it. Then you can use a pointer to this object as a link. 
- In the example below, we use `const B *const` which means a *constant pointer to a constant value of type B*. 
So, the class A can neither modify the value of B nor the pointer to B. 
- You can choose what the class A can modify in B. The value? The pointer?

```cpp
class A{
public:
  A(B* link_to_b) _link_to_b(link_to_b){}
private:
    const B *const _link_to_b; // a link to b to work on.
}

B b; //some object of type B
A(&b); //initialise A with the address of b;

```

---

## Important

**Try to use references and pointers where appropriate to avoid copies!**

---

## Smart Pointers


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
#include <memory>

{
    // Make shared_ptr
    std::shared_ptr<int> ptr = std::make_shared<int>(5);
    // Can derefence as normal
    int n = *ptr;
    // Can get raw pointer - no counting
    int *x = ptr.get();

    {
        std::shared_ptr ptr2;
        ptr2 = ptr; //copy ptr in ptr2, the counter is increased by one.
    }// ptr2 is freed and counter is decrease by one

    // Counter increased by one in call
    do_work(ptr);
    // End of call, counter decreased by one

}//counter decreased, if counter is at 0 the resource of ptr is freed
```


---

# `unique_ptr`

- `unique_ptr` ensures there is only one owner.
- You cannot copy the pointer, only move it.
- It is faster than `shared_ptr` and you should try and use it as much as possible.


```cpp
#include <memory>

{
    // Make unique_ptr
    std::unique_ptr<int> ptr = std::make_unique<int>(5);
    // Can derefence as normal
    int n = *ptr;
    // Can get raw pointer - no counting
    int *x = ptr.get();

    std::unique_ptr<int> ptr2;
    ptr2 = ptr; //this will produce an error
    ptr.swap(ptr2); //data swap from ptr to ptr2


    // ptr is now nullptr, so no need to free the resource
}// ptr2 is automatically free here
```


---

# Assignment, Copying, and Moving

- **Assignment** is whenever you use the `=` to set an object variable.
- **Copying** is when we create a new object from an existing one.
- **Moving** is like copying, but we move the already allocated resources to the new object. The original becomes empty.
- This is an important concept to understand in general in C++.
- If you want to work at the lowest level of C++ you really need to recognise these behaviours for optimisation purposes.


---

# Memory in Games


---

# RAII

- Resource Acquisition is Initialisation.
    - On object's construction, object acquires resource.
    - On object's destruction, object frees up resource
- But!
    - **Do not** give an entity a resource such as a loaded texture or audio clip.
    - **Do** give an entity a pointer or reference to such a resource.
    - Keep track of resources via central pools (we will look at a resource manager soon).
    - Try to allocate those resources when the object is created (we will discuss this soon).


---

# Data Sharing

- Although referencing is efficient to reduce memory usage, it can be more expensive for memory access.
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



---

# Basic Operations

- Our game resource manager needs only a few different operations:
 - `initialise` :   as most of our game engine components will likely have.
 - `load_resource` :   loads and/or retrieves a resource.
 - `unload_resource` :   unloads a loaded resource.
 - `clear_all` :   unloads all loaded resources.
 - `get_resource` : access a reference/pointer to a specific resource 
- That is all.
- Depending on your approach:
 - You can have a singleton with typed loads, unloads, and storage
 - You can have a different resource manager for each type.


---

# Storing Resources

`TextureManager` class with a lookup table `std::map`. The map will have a key, like a label to identify the texture.
```cpp
class TextureManager{
public:
    //renaming of the std::map for more readable code.
    using TextureMap = std::map<std::string, std::shared_ptr<sf::texture>>;
    ...
    bool load_texture(const std::string& file, const std::string& label){
        TextureMap::iterator it = textures.find(label);
        if (it != textures.end()){//if texture already loaded do nothing
            return true;
        } else {//if texture not loaded load it
            // Don't care how this works
            std::shared_ptr<sf::texture> new_texture = 
                std::make_shared<sf::texture>()
            if(new_texture.loadFromFile(file)){
                textures[label] = new_texture;
                return true;//texture loaded
            }
            return false;//texture not loaded
        }
    }
private:
    TextureMap textures;
};
```

---

# Storing Resources (Cont.)

To access the resouces, we will avoid making copies of the resource and instead making a copy of the `shared_ptr`.

```cpp
bool get_texture(const std::string& label, std::shared_ptr& texture){
    TextureMap::iterator it = textures.find(label);
    if(it != textures.end()){
        return false; //texture not in the storage
    }
    texture = it->second;
    return true;
}
```

---

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


---

# Summary

- We've covered a lot of ideas today.
- We looked at how memory works in general.
- We looked at how memory is used in C++.
- Techniques to avoid copies.
- The key take away is how we apply this to **manage game resources** and **optimise your implementation**
- You should be able to understand the basic premise of a resource manager, why we need it, and how it operates.
