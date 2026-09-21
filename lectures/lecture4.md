---
title: "Lecture 4 - Introduction to C++"
keywords: Lecture
tags: [Lecture]
permalink:  lecture4.html
summary: lecture4
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 4 - Introduction to C++
### SET09121 - Games Engineering

<br><br>
Leni Le Goff
<br>


School of Computing - Edinburgh Napier University


---

# Recommended Reading:
- Any C++ book really, but C++ Primer is good.
- Online tutorials:
    - https://www.geeksforgeeks.org/cpp/c-plus-plus/
    - http://www.learncpp.com
- C++ references: https://en.cppreference.com


---

# Why use C++

Because it is:
- a multi-paradigm language;
- a "mid-level" language: access to low-level instructions and to higher-level functions;
- a manual memory management language;
- an easily optimisable language;
- one of the fastest language in run-time. 

And based on the foundational language C.

---

# C++ "Hello World"

```cpp
// main.cpp
#include <iostream>
int main()
{
    //print "Hello World" in terminal
    std::cout << "Hello World" << std::endl;
    //executable return 0 upon termination
    return 0;
}
```
- The function `int main()` is the **entry point** of the program. 
- It returns an integer when the program finishes. 
- Returning 0 is the standard for termination without error.

---

# C++ "Hello World" (Cont.)

```cpp
// main.cpp
#include <iostream>
int main()
{
    //print "Hello World" in terminal
    std::cout << "Hello World" << std::endl;
    //executable return 0 upon termination
    return 0;
}
```
- `iostream` input/output standard library
- Input/Ouput in C++ are done through **stream** with the insertion operator `<<` into `std::cout` the **standard output stream**.
- `std::endl` is the endline marker. Additionally to finishing the line, it will flush the buffer. Until the buffer is flushed nothing will be printed. 

---

## C/C++ Basics

---

# Program arguments

```cpp
// main.cpp
#include <iostream>
int main(int argc, char** argv)
{
    if(argc == 1){
        std::cout << "No arguments!" << std::endl;
        return 1;
    }
    for(int i = 1; i <= argc; i++){
        std::cout << "arg " << i << ": " << argv[i] << std::endl;
    }
    return 0;
}
```

- `int argc` is the number of arguments
- `char** argv` is a C-style array of C-style strings.
- `argv[0]` is the name of the function
- `argv[1]` is the first argument

```bash
$> ./my_program arg_1 arg_2
```

---

# Basic Input/Output

```cpp
// main.cpp
#include <iostream>
int main()
{
    std::cout << "Enter a number between 0 and 10" << std::endl;
    int n;
    std::cin >> n; //program stop until getting one input from terminal
    if(n > 10 || n < 0){
        std::cerr << "An error occurred" << std::endl;
        return 1;
    }
    return 0;
}
```

- `std::cout` is the **standard output stream**
- `std::cerr` is the **standard error stream**
- For output streams, use `<<`, called the insertion operator
- `std::cin` is the **standard input stream**
- For input streams, use `>>`, called the extraction operator

---


# Basic data types

```cpp
int a = 1; // 32-bit integer 
float b = 1.0; // 32-bit floating-point number 
double c = 2.0; // 64-bit floating-point number 
char d = 'e'; // character encoded with UTF-32 (32-bit)
bool e = false; // boolean: false or true/ 0 or 1
void f; // the empty type
```

---

# Boolean operations

```cpp
false || true == true; // inclusive or operator
false && true == false; // and operator
!false == true; // negation operator
0 == false; // 0 is equivalent to false
1 == true; // 1 or greater integer is true
nullptr == false;// nullptr is always equivalent to false
```

`nullptr` is an empty instance. 

---

# String


C-style string is a C-style array of chars

```cpp
char c_str[] = "hello"; 
c_str[1] // == e
```

C++-style string is a class from the standard library

```cpp
#include <string>

std::string str = "world";
str[0]; // == w
```

---

# Pointers and References

- A **pointer** is a variable storing an **address** to a value
- A **reference** is a **link** to another variable

```cpp
int main(){
  int a = 0; //a is an integer with value 0
  int &ref = a; //ref is a reference to a
  //references value can be accessed directly
  std::cout << "ref = " << ref << std::endl; 
  ref = 1; // and be modified directly
  std::cout << "a = " << a << std::endl; //modifying ref will also modify a
    
  //ptr is a pointer to the value of a. &a is the address of a
  int *ptr = &a; 
  std::cout << "ptr = " << ptr << std::endl;//the value of ptr is an address
  //to access the value of ptr, it needs to be dereferenced: *ptr
  std::cout << "value of ptr : " << *ptr << std::endl;  
  *ptr = 2; //modifying the value of ref will modify a.
  std::cout << "a = " << a << std::endl;

  return 0;
}
```

```
ref = 0
a = 1
ptr = 0x7ffccaf80574
value of ptr : 1
a = 2
```

---

## C++ I/O - Reading & Writing Files

---

# Reading files

```cpp
#include <string>
#include <fstream>

int main(int argc, char** argv){
    std::ifstream ifs; //create a reading file stream
    ifs.open(argv[1]); //open the file entered as argument
    if(!ifs.is_open()){// check if the file is properly open
        std::cerr << "Error: file " << argv[1] << " not found" << std::endl;
        return 1;// return 1 to indicate an error occurred
    }
    std::string line;
    while(std::getline(ifs,line)){ //read line by line the file
        std::cout << line << std::endl;
    }
    ifs.close(); //Always close a non-standard stream
}

```

`ifstream` is a stream similar to `std::cin` but with a file as input instead of a terminal

---

# Writing files

```cpp
#include <string>
#include <fstream>

int main(int argc, char** argv){
    std::ofstream ofs;// create a writing file stream
    ofs.open(argv[1]);// open the file entered as argument
    if(!ofs.is_open()){// check if the file is properly open
        std::cerr << "Error: file " << argv[1] << " not found" << std::endl;
        return 1;// return 1 to indicate an error occurred
    }
    for(int i = 0; i = 99; i++){
        ofs << i << ","; //use insertion operator to write in the file
    }
    ofs << 100 << std::endl;
    ofs.close();  //Always close a non-standard stream
}

```

`ofstream` is a stream similar to `std::cout` but with a file as ouput instead of a terminal

---

## Loops and Conditions

---

# Loops
 
For loop
```cpp
for(int i = 0/*initialisation*/;i < 10 /*termination*/;i++/*iteration*/){

}
```
While loop
```cpp
int i = 0;
while(i < 10){//termination condition
    i++;
}
```
Do-While loop
```cpp
int j = 0
do{
    j++;
}while(j<10); //termination condition
```

---

# Conditions

If condition

```cpp
if(/*condition 1*/){

}else if(/*condition 2*/){

}else{

}
```

Switch condition

```cpp
int possibilities = 0;
switch(possibilities){
    case 0:
        break;
    case 1:
        break;
    default:
        break;
}
```

Conditional operator

```cpp
int x = 10, y = 20;
int max = (x > y) ? x : y;

```

---

## Example of Switch conditions.

To handle process states. 

```cpp
int state = 0; // possible states 0, 1, 2
switch(state){
    case(0):
        std::cout << "You are in IDLE state" << std::endl;
        break;
    case(1):
        std::cout << "You are in PROCESSING state" << std::endl;
        break;
    case(2):
        std::cout << "You are in ENDING state" << std::endl;
        break;
    default:
        std::cerr << "ERROR: Unknown state" << std::endl;
        break;
}
```

---

## Example If conditions

To test if coordinates are within the arena boundaries.

```cpp
bool in_arena(int x, int y, int width, int length){
    if(x > length || x < 0){
        return false;
    }
    else if(y > width || y < 0){
        return false;
    }
    else return true;
}
``` 

---

## Scopes, Functions, and Headers

---

# Function 

Functions in C++ are declared and defined with the following syntax: 

```cpp
return_type function_name(arg1_type arg1_name, ...){ Implementation }
```

```cpp
void my_function(int arg1, double arg2){
    // some content
    return; // (optional)
}

bool my_function2(int arg1, double arg2){
    // some content
    return false;
}
```

The naming convention for functions uses *snake_case*.


---

# Declare in Headers, Implement in Source

- This is an idea you might not be as familiar with if you come from a Java and C\# background.
- In C++, declarations should be provided in a header file (**.hpp**, .h, .hh, hxx).
- The actual implementation (definition) should be provided in a source file (**.cpp**, .cc, .cxx).


```cpp
// A.hpp
void my_function(int arg1, double arg2); //function declaration
bool my_function2(int arg1, double arg2);
```

```cpp
// A.cpp
#include "A.hpp"

void my_function(int arg1, double arg2){//function implementation
    // some content
    return;
}

bool my_function2(int arg1, double arg2){
    // some content
    return false;
}
```

---

# Preprocessor instruction

```cpp
// A.hpp
#pragma once  //use this header file only once
#include <iostream> //to include library
#define PI 3.14159 //to define macros
```

`#pragma once` should be at the beginning of every header file. 

More about Macros: https://gcc.gnu.org/onlinedocs/cpp/Macros.html.

---

# Scopes

In C++, scopes are essential because they define variable scopes. Deallocation of resources is based on scopes. Scopes are defined with curly brackets `{...}`.
```cpp
int main(){
    int i = 0;// this variable exists in the whole function scope

    {//unamed scope
        int tab[5] = {0,1,2,3}
    }//tab is deallocated here

    for(int n = 0; n < 10; n++){
        //n exists only in the for loop scope
        MyClass A;
    }//A is deallocated here
}//i is deallocated here
```

***DO NOT DECLARE ANYTHING OUT OF SCOPE***

---

# Namespaces

Namespaces are used to define a context or scope for function, class, struct, enum, or global variables declarations. They are handy to avoid naming conflict.

```cpp
// A.hpp
namespace A{
    void my_function(int arg1, double arg2); //define in namespace A scope
}
bool my_function(int arg1, double arg2); //defined in global scope
```

```cpp
// A.cpp
#include "A.hpp"
void A::my_function(int arg1, double arg2){
    // some content
    return;
}
bool ::my_function(int arg1, double arg2){ //explicitly access the global scope
    // some content
    return false;
}
```
Anything declare out of scope or namespace will be in the **global scope**.


---

## Struct and Enum

---

# Structure

- `struct` are generally used for composite types: collections of data.
- `struct` members are declared between the curly brackets.
- There isn't a widely used convention for `struct` naming but I will use *CamelCase*.
- **Important** unlike functions, `struct` declaration finishes with a semi-colon.

```cpp
struct MyStruct
{
   int data_a;
   double data_b;
};
```

```cpp
MyStruct s;
s.data_a = 1;
s.data_b = 0.35;
```
The `MyStruct s` instance contains both data_a and data_b. It contains a collections of variables

---

# Enumeration

- `enum` are used to create customised type which are an enumeration of possibilities such as week days, seasons, or boolean. 
- There isn't a widely used convention for `enum` naming but I will use *CamelCase*
- **Important** `enum` declaration finishes with a semi-colon.

```cpp
enum Colours
{ //each enum member has a correspondance to an integer. 
   RED = 0, // = 0 is optional but it set the translation to integer without ambiguity
   BLUE,
   GREEN,
   YELLOW
};
```

```cpp
Colours colour = GREEN;
colour == GREEN; // true
colour == 2; // also true
```

An `enum` instance contains one data with a limited set of predefined values.

---


## STL Data Containers
(Standard Template Library)


---

# Data container - vector

A vector is a sequence container that encapsulates dynamic size arrays. The size of the vector can be changed after resource allocation. 

```cpp
#include <vector> //include the vector library
std::vector<int> v = {8, 4, 5, 9}; //bracket-initializer
v.push_back(6); //add one value at the end of the vector
v.push_back(9);
v[2] = -1; // replace the third value by -1
v.erase(v.begin());// remove first value
v.erase(v.begin()+3);// remove fourth value
```

---

# Data container - array

An array is a container that encapsulates fixed size arrays. The size of an array cannot change after resource allocation. Therefore, the size of the array has to be specified at declaration.

```cpp
#include <array> //include the array library
std::array<int,5> arr = {1,2,3,4,5}; //initialise like a vector. 
std::cout << arr[0] << std::endl;
int arr[5] = {1,2,3,4,5}; //C-style array
```

Use an array when you know the size and it is not going to change. Using an array prevents constant allocation and deallocation like when using a vector. It is always better to use `std::array` than C-style array. 

---

# Data container - tuple

A tuple is a fixed-size collection of values with heterogeneous types.

```cpp
#include <tuple>
std::tuple<int,std::string,double> param(1,"speed",0.1);
std::cout << "id " << std::get<0>(param) << ", "
          << "name  " << std::get<1>(param) << ", "
          << "value " << std::get<2>(param) << std::endl;
```

Note that you cannot access a element of a tuple via `[]` like for vectors and arrays. Instead, you need to use the function `std::get<>`. It is because of the heterogeneous types.


---

# Data container - map

A map is a sorted associative container that contains **key-value pairs** with **unique keys**. The key type needs to be comparable because the data are sorted using the key. 

```cpp
#include <map>
std::map<std::string,double> parameters = 
    { {"speed",0.2},
     {"strength",1.5},
     {"weight",10} };
std::cout << "speed " << parameters["speed"] << std::endl;
parameters["shield"] = 0.5;
```

Note that the access to an element using `[]` has a logarithmic complexity in the size of the container. Which means it will take at most *log(n)* operations to access a value. *n* being the size of the container.   

---

# Iterate through data containers

```cpp
std::vector<int> v = {8, 4, 5, 9};
for(int i: v){// Ranged-based for. Modern C++ style
    std::cout << i << std::endl;
}
for(int i = 0; i < v.size(); i++){// Classic C-style
    std::cout << i << std::endl;
}
// Erase all element
// Classic C++ style using iterators
for(std::vector<int>::iterator it = v.begin(); it != v.end();){
    it = v.erase(it);
}

// Iterate through the parameters map defined in the previous slide
// With a range-based for the elements are a pair of key, values.
// p.first is the key and p.second is the value
for(std::pair<std::string, double> p: parameters){
    std::cout << p.first << " : " << p.second << std::endl;
}
//Doing the same using iterators.
for (std::map<std::string,int> it = parameters.begin(); 
    it != parameters.end(); 
    it++){
        std::cout << it->first << " : " << it->second << std::endl;
}
```

---

# Summary

- The program **entry point** is *int main(int argc, char** argv)*.
- **Header files** (.hpp) are for declarations and **source files** (.cpp) for implementations.
- Output/Input is done using **streams** with **insertion** (<<) and **extraction** (>>) operators.
- Basic data types: *int*, *float*, *char*, *bool*, and *void*. Additionaly *std::string* for C++ style string
- Boolean operators: `||` for *inclusive or*, `&&` for *and*, and `!` for *negation*
- Pointers (*) and references (&) to manipulate resource addresses. It is the first step towards manually data management
- Data container: *std::vector*, *std::array*, *std::tuple*, and *std::map*.
- Also, the syntax of *for*, *while* and *if*.

---
