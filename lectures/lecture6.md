---
title: "Lecture 6 - OO in C++"
keywords: Lecture
tags: [Lecture]
permalink:  lecture6.html
summary: lecture6
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 6 - Object Orientation in C++
### SET09121 - Games Engineering

<br><br>
Leni Le Goff
<br>


School of Computing. Edinburgh Napier University


---

# Recommended Reading:
## Any C++ book really, but C++ Primer is good.


---

# Goal

- Basic concepts of C++
- Some good practice for C++
- object-orientation in C++

---

## Basics of C++

---

# Declare in Headers, Implement in Code

- This is an idea you might not be as familiar with if you come from a Java and C\# background.
- In C++, declarations should be provided in a header file (.hpp).
- Actual implementation (definition) should be provided in a code file (.cpp).
- Exceptions exist around pre-compiled headers and templates.


```cpp
// A.hpp
class A {
    void work();
    int do_more();
};
```
```cpp
// A.cpp
#include "A.hpp"

void A::work() {
    // Do some work
}
int A::do_more() {
    return 0;  // Do some more work
}
```

---


# RAII

Our First Rule of Good OO in C++ - RAII

*Resource Acquisition Is Initialisation*. 

- It is a rule used in good C++ code. <!-- .element: class="fragment" -->
- When an object is created it allocates or takes ownership of its required resources (via the constructor). <!-- .element: class="fragment" -->
- When an object is destroyed it frees up its allocated and owned resources (via the destructor). <!-- .element: class="fragment" -->
- This ensures that we do not have memory leaks. Resources have their life tied to an object's life. <!-- .element: class="fragment" -->

---

# RAII explained

NOT RAII
```cpp
void main(){
    // Texture is a resource that has to be unloaded, e.g. via an UnloadTexture() function
    Texture mario_texture = load_texture("Mario.jpg"); 
    Mario mario(mario_texture);
    //--- some time later
    close_game();
    //Oh no - we forgot to unload the texture!
}//Mario is destroyed automatically
```

RAII
```cpp
void main(){
    //Mario Loads his texture himself.
    Texture mario_texture = load_texture("Mario.jpg"); 
    Mario mario(mario_texture);
    //--- some time later
    unload_texture(mario_texture);
    close_game();
}//Mario is destroyed automatically

```

---

# Understanding the role of scopes in C++

C++ is built based on the RAII rule. Desallocation of resources are based on scopes.
Scopes are defined with curly bracket `{...}`.
```cpp
int main(){
    int i = 0;// this variable exist in the whole function scope

    {//unamed scope
        int tab[5] = {0,1,2,3}
    }//tab is desallocated here

    for(int n = 0; n < 10; n++){
        //n exists only in the for loop scope
        MyClass A;
    }//A is desallocated here
}//i is desallocated here
```

***DO NOT DECLARE ANYTHING OUT OF SCOPE***

---

## Object-Orientation in C++

---


# Declaring a class in C++

- `class` declarations are simple in C++.
- To declare a `class` in C++ we use the `class` keyword followed by the name of the `class`.
- The declaration of the `class` is anything we put between the curly brackets.
- **Note** - a semi-colon is required at the end of the declaration. This is different to Java and C\#.
- **Note** - by default, class members are declared *private*.
- **Note** - by convention, class names are using *camelCase* and a first letter in upper-case.

```cpp
class MyClass
{
    // Members
};
```

---

# Declaring a struct in C++

- C++ also allows declarations of `struct` types.
- `struct` declarations are also simple in C++.
- To declare a `struct` in C++ we use the `struct` keyword followed by the name of the `struct`.
- A `struct` is then the members declared between the curly brackets.
- **Note** - by default, struct members are declared *public*.
- **Note** - we typically use structs for collections of simple data.
- **Note** - There isn't a widely used convention for struct naming but I will use the same as classes.
```cpp
struct MyStruct
{
    // Members
};
```

---

# Defining Attributes

- Attributes are the values that go along with our objects.
- C++ doesn't define the visibility per attribute (unlike Java & C#).
    - We will look at visibility in a few slides.

```cpp
class MyClass
{
    // Object (instance) values.
    float x; // uninitialised value
    float y = 0.5f; // Initialised value
    const string name; // Constant value
    // Class (static) values.
    static int n;
};
```

---

# Defining Methods

- Same rules apply for methods.
- **Note** Generally, the naming convention for variables, methods and functions is *snake_case*.

```cpp
class MyClass
{
    void do_something()
    {
        // Do something, that may change any member variables of this object.
    }
    // Const methods do not change values of object.
    float get_x() const
    {
        return x;
    }
    // Class (static) methods.
    static int get_n() { return n; }
};
```

---

# Defining Constructors

- Constructors define how an object is instantiated.
- We can use them to control initialisation of an object
- We can have multiple constructors for a class, using different parameters
- **Note** - always initialise all your variables, either when declaring them or in a constructor
- **Note** - add a print statement in a constructor, and observe all the times they're called!

```cpp
class MyClass
{
public:
    // Default constructor
    MyClass() { }
    // Parameterised constructor
    MyClass(float xx, float yy)
    : x(xx), y(yy) // Sets object attributes
    {
    }
};
```

---

# Defining Destructors
- Destructors determine how an object is destroyed when it goes out of scope.
- A destructor is called:
    - whenever an object goes out of scope (i.e. defined between curly brackets).
    - When the object is manually deleted (e.g. replacing a variable, deleting a pointer)
- A destructor looks like a constructor with a tilde (~) in front of it, and no parameters
- A class can only have one destructor.
- **Note** - add a print statement in a destructor, and observe all the times they're called!

---

# Destructors in C++

```cpp
class MyClass
{
public:
    // Default Destructor
    ~MyClass()
    {
        // Free up resources.
    }
};
```

---

# Scope Protection

- We can specify the visibility of class members via `public`, `private`, and `protected` modifiers.
- The difference between a `struct` and a `class` is just the default visibility. `struct` is `public`, `class` is `private`.
- We define "zones" of visibility in C++ rather than individual values.
- Start with public: most interesting part for users of class

---

```cpp
    class MyClass
    {
        // This value is private.
        int x;
    public:
        // The following are public.
        MyClass() { }
        float n;
    protected:
        // The following are protected.
        string _str;
    private:
        // Private again.
        int _val;
    };
```
**Note:** Often, in C++, private and protected members (attributes and functions) will have an underscore before their name. 

---


## Object-orientation Concepts in C++


---


# Core Object-orientation Concepts

- **Encapsulation**:  bundling of data and functions within an object, providing data protection and promoting modularity through object interaction through a well-defined interface.
- **Abstraction**: the process of hiding implementation details and exposing only the essential characteristics and behaviours of an object, making it easier to understand and use.
- **Inheritance**: the mechanism of creating a new class that reuses, extends, or modifies the attributes and behaviors of an existing class, allowing for a more efficient and organised code structure.
- **Polymorphism**: the ability of an object or function (overloading) to take on multiple forms. With objects, we can use the same interface for different types. With functions, we can have different behaviour depending on passed parameters. It promotes code reusability and flexibility.

---

# Think Back to the Labs

- Have you already used some of these? <!-- .element: class="fragment" -->
- What about the Pong or Space Invaders example? <!-- .element: class="fragment" -->

---

# Core Object-orientation Concepts

- **Encapsulation**
- Abstraction
- Inheritance
- Polymorphism


---

# Encapsulation 

- Hide the data, allow access through a well-defined interface
- A trivial example looks pointless...

```cpp

class Player
{
	public:
		//setter for attribute _name
		void set_name(const std::string& name) 
		{
			_name = name;
		}
		
        //getter for attribute _name
		const std::string& get_name() const
		{
			return _name;
		}
		
	private:
	
		std::string _name;
};
```

---

# Encapsulation 

- Slightly different example, no encapsulation
- What if we have a texture that displays the player's name, e.g. shown above their head?
- What happens if we call ```player.name = "Steve";``` 

```cpp
class Player
{
	public:
		// Name of player
		std::string name;
		// Rectangular texture that contains the name of the player
		//	  useful for overlays, UI, etc
		Texture name_texture;
};
```

---

# Encapsulation 

- We modify the implementation
- The interface remains **unchanged**

```cpp

class Player
{
	public:
		void set_name(const std::string& name) 
		{
			_name = name;
			update_name_texture();
		}
		const std::string& get_name() const
		{
			return _name;
		}
	private:
		void _update_name_texture()
		{
			name_texture = create_texture_from_text(name);
		}
		std::string _name;
		Texture _name_exture;
};
```

---

# Core Object-orientation Concepts

- Encapsulation
- **Abstraction**
- Inheritance
- Polymorphism

---

# Abstraction 

- Hide implementation details
- Hide methods that should not be called from other objects
- We are already doing that here!!!

```cpp

class Player
{
	public:
		void set_name(const std::string& name) 
		{
			_name = name;
			update_name_texture();
		}
		const std::string& get_name() const
		{
			return _name;
		}
	private:
		void _update_name_texture()
		{
			name_texture = create_texture_from_text(name);
		}
		std::string _name;
		Texture _name_exture;
};
```

---

# Core Object-orientation Concepts

- Encapsulation
- Abstraction
- **Inheritance**
- Polymorphism


---

# What is Inheritance?

- Inheritance is the ability to base part of a class behaviour or data on an existing class definition (specification).

- A *base-class* is a *generalisation* of required behaviour (inherited from). 

- A *derived-class* is a *specialisation* of this base behaviour (also called subclass)

- Inheritance enables object polymorphism, and is a key feature for OOP code reuse

- IS-A relationship between subclass and base class. A circle is a shape.

---

# Inheritance in C++
```cpp
    class Shape
    {
    };

    class Circle : public Shape 
    {
    };

    class Square : public Shape
    {
    };
```

---

# Be Careful!

- Avoid deep levels of inheritance. 
- Try to identify what should be a *has-a* or an *is-a* relationships:

```cpp

    class CarEngine
    {
		int power;
    };

    class BadCar : public CarEngine
    {
    };
	
	class GoodCar
    {
		CarEngine engine;
    };
```

---

# Multiple-inheritance in C++

- Multiple-inheritance allows us to define a class as inheriting from more than one base-class.
- In practice it is better to avoid MI. It is generally a bad idea and can lead to major issues.
- MI is acceptable when the base classes are *abstract*.

---

# Example of bad Multiple-inheritance

```cpp
// The Deadly Diamond of Death
class A
{ 
public:
    virtual void method_a();
};
class B : public A
{
public:
    void method_a() override;

};
class C : public A
{
    void method_a() override;
};
class D : public B, public C
{
};

int main(){
    D d;
    d.method_a(); // do this call c.method_a() or b.method_a()?
    return 0;
}

```

---

# Abstract Class in C++

- C++ does not have interface like in Java
- Instead C++ allows to define *Abstract Classes*
- A class is abstract if it has *at least* one *pure virtual* method.

```cpp
class Vehicle{
public:
    virtual void drive() = 0;
}
```

**An abstract class cannot be instanciated!**

---

# Core Object-orientation Concepts

- Encapsulation
- Abstraction
- Inheritance
- **Polymorphism**


---

# What is Polymorphism?

- Polymorphism is the ability of our objects to act as different types.
- There are actually three types of polymorphism in computer science. C++ supports all three.
 - **Ad-hoc polymorphism**:   overriding functions with different parameters (we will look at this in the overloading section).
 - **Parametric polymorphism**:   overriding types based on a parameter (e.g. `vector<int>`). We will briefly look at templates at the end of lecture.
 - **Subtyping**: A subclass has compatible interface with its parent class: every method of parent class A can also be called on its derived class B.

---

# Polymorphism in C++


- Polymorphism in C++ occurs whenever we derive classes.
- An object can be converted into any of its base types.
- There are a few caveats which we will look at later. But hopefully you are all familiar with this basic concept.
```cpp
    class Animal{
    public:
        void make_sound() { }
    };

    class Dog : public Animal{};

	// Pass by REFERENCE
    void func(Animal& a) { 
        a.make_sound(); 
    }

    Dog fido;
    func(fido);
```


---

# Examples

Converting (Casting) Between Types in C++
```cpp
    sparrow *s;
    // C-style casting.  DON'T DO THIS!
    bird *b = (bird*)s; 

    // Proper C++ casting -- USE THIS -- checked at compilation
    bird *b = static_cast<bird*>(s); 

    // Dynamic cast will not be checked at compilation. 
    // Evaluated at runtime. return nullptr if not possible. 
    dog *d = dynamic_cast<dog*>(s);
```

---

# Method Overloading

- A basic form of polymorphism is *ad-hoc polymorphism*.
- This means we can have the same method name, but with different parameters and return type.
- Object-orientation generally allows this through method overloading.
- Note that parameters are the real way of distinguishing methods - the return type cannot be different for the same parameters.

```cpp
class A
{
public:
    // No parameters
    void a();
    // Single int parameter
    void a(int x);
    // Different return type requires
    // different parameters.
    int a(float y);
};
```


---

# `virtual` Members

- To mark a method as overridable in a child class we need to state that it is `virtual`.
    - Same as C\#; Java uses `abstract`.
- When a method is `virtual` it means that it can be redefined in child classes
- Child classes can write their own implementation of the function, specifying it using `override`
    - it's not a requirement, but DO IT



---

# `virtual` Members

```cpp
class A
{
public:
    virtual void work()
    {
        std::cout << "Hello" << std::endl;
    }
};

class B : public A
{
public:
	// Compiler will check the
    // override is valid.
    void work() override
    {
        std::cout << "Goodbye" << std::endl;
    }
};

A *a = new B();
// Will print Goodbye
a->work();
```

---

## Pointers and references

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

# Polymorphism subtyping

- You need to work with a reference (e.g. `int&`) or a pointer (e.g. `int*`) value to get the polymorphic behaviour with objects.

```cpp
class A {
public:
    virtual void work() { 
        std::cout << "a" << std::endl; 
    }
};

class B : public A {
public:
    void work() override { 
        std::cout << "b" << std::endl;
    }
};

B b;
b.work(); // Prints b
A a1 = static_cast<A>(b);
a1.work(); // Prints a

A& a2 = static_cast<A&>(b);
a2.work(); // Prints b

A* a3 = static_cast<A*>(&b);
a3->work(); // Prints b


```

---

# Pointers of objects

- To access the data of a pointer to an object, the pointer needs to be dereferenced: `(*a).a_method();`.
- As it is something needed a lot, C++ provides a shortcut: 

`a->a_method();`.

```cpp
class A
{
public:
    void work() { }
};

A* a = new A();
// Calling work by dereferencing
(*a).work();
// Better to use arrow notation
a->work();

delete a; //RAII rule always use delete after new.
```

---

# Smart Pointers

- Allocating raw pointers is discouraged in modern C++ 
    - ... but their use is fine, where appropriate!
- Smart pointers allow automatic memory management, via RAII
	- no more needs of using `new` and `delete` operators!
- Two types:

    `shared_ptr`:   reference counted.

    `unique_ptr`:   has only one owner.

```cpp
int main(){
  int *n1 = new int(5); //raw pointer which will need to be deallocated at some point
  // Automatically counts references.
  std::shared_ptr<int> n2 = std::make_shared<int>(5);
  // Only one reference will exist. Faster than shared_ptr
  std::unique_ptr<int> n3 = std::make_unique<int>(5);
  // Can still treat as a standard pointer
  int n4 = *n3;
  delete n1; //the memory allocated to n1 needs to be freed explicitly
  return 0;
} //n2 and n3 are deallocated automatically here.
```


---

## Other import things


---


# Rule of three/five/zero

- C++ class has five special operators
```cpp
class A {
public:
    ~A();                   // destructor
    A(const A&);            // Copy constructor
    A(A&&);                 // move constructor
    A& operator=(const A&); // copy assignment operator
    A& operator=(A&&);      // move assignment operator 
};
```
- **Three:** require destructor/copy constructor/copy assignment, if storing raw pointers
- **Five:** require all five, if some data cannot be copied like a unique_ptr
- **Zero** only use constructors that don't need special destructors (no special resources)

It is recommended to not define them manually if it is not necessary.
https://en.cppreference.com/w/cpp/language/rule_of_three.html


---

# Optimisation using const and constexpr

`const` and `constexpr` keywords define constant variables or modification rights to a object data.

```cpp
const int x = 0; //x is a constant integer
x += 1; //this line will produce an error
int y = x; // this is allowed

constexpr int z = 2;// z is a constant evaluated at compile time


```

Define class methods as `const` if they don't modify the object data.

```cpp
class A {
private:
    int x;
public:
    // Does not modify object state
    int get_x() const {
        return x;
    }
    // Does modify object state
    void set_x(int n) {
        x = n;
    }
};
```

---

# The keyword static 

Variable declared `static` are allocated staticly for the life-time of the program.

```cpp
void func(){
    static int a = 0
    a++;
}
func(); 
//a = 1
func();
//a = 2
```

However, the accessibility of a `static` variable depends of the scope.
```cpp
static int a = 0; //without scope (global): only accessible within this file
void func(){
    static int b = 0; //accessible only within this function
}
```
This variable is allocated staticly only for this file. It will not be accessible outside. 

---

# The keyword static (cont.)

Properly defining global variables:
```cpp 
//header
struct game_parameters{
    static int param1;
    static constexpr int param2 = 0;
}
```

```cpp
//source
int game_parameters::param1 = 2;
```

Class members (vs instance members)
```cpp
class A{
public:
    static int a;
    int b;
}
A::a //a can be accessed without having intantiate an object of Type A
A obj;
obj.b;
```

Static members of a class are shared by all the instance of this class.

---

# Summary

- You have just learned C++ in an hour. <!-- .element: class="fragment" -->
- This is obviously not possible, and you will need practice in these ideas. I am simply signposting ideas. <!-- .element: class="fragment" -->
- C++ is one of the most complicated languages around (they keep adding features), so get a good working knowledge of what you need and hack it together. <!-- .element: class="fragment" -->
- Key thing today was how to do object-orientation properly.  <!-- .element: class="fragment" -->
- But at the end of the day it is all about practice. <!-- .element: class="fragment" -->

====> ***https://en.cppreference.com*** <====<!-- .element: class="fragment" -->

---

# Golden Rules / top tips

1. Keep stuff out of header files. Only the bare minimum!

1. Use unique_ptr or shared_ptr as required: Don't even call new. Or delete. Or malloc and free. Ever. Just don't.

1. Use const as much as you can.

1. Use static variable instead of global variable

1. Put breakpoints in all your constructors/destructors/assignment operators when debugging scope issues. 
You might be surprised by when they are called!


