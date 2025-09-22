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
## To learn object-orientation in C++


---

# Why you Need to Know Object-orientation in C++

- We have defined our games using Formal Elements. <!-- .element: class="fragment" -->
- We have also defined our game as a system. <!-- .element: class="fragment" -->
- Our game system will be entity-based. <!-- .element: class="fragment" -->
- All of these elements require us to define objects. <!-- .element: class="fragment" -->
- You are also going to build one of the most complex systems you have undertaken at university. This requires breaking the system down into controllable components. This is what object-orientation is for. <!-- .element: class="fragment" -->


---

# Basics of Object-orientation in C++

- You'll will see plenty of this in the labs <!-- .element: class="fragment" -->
- Here we are going to talk about what some of that means <!-- .element: class="fragment" -->

---

# Declaring a class in C++

- `class` declarations are simple in C++.
- To declare a `class` in C++ we use the `class` keyword followed by the name of the `class`.
- The declaration of the `class` is anything we put between the curly brackets.
- **Note** - a semi-colon is required at the end of the declaration. This is different to Java and C\#.
- **Note** - by default, class members are declared private.
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
- **Note** - by default, struct members are declared public.
- **Note** - we typically use structs for collections of simple data.
- **Note** - There isn't a widely used convention for struct naming but I will the same as classes.
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
**Note** Generally, in C++, private and protected members (attributes and functions) will have an underscore before their name. 

---

# RAII

Our First Rule of Good OO in C++ - RAII
- RAII stands for Resource Acquisition Is Initialisation. <!-- .element: class="fragment" -->
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
RAII : Mario should clean up after himself!

---

# Understanding the role of scopes in C++

C++ is built based on the RAII rule. Desallocation of resources are based on scopes.
Scopes are defined with embrace bracket `{...}`.
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

---

# Object-orientation Concepts in C++


---

# Core Object-orientation Concepts

- C++ is a multi-paradigm language, and it supports object-orientation well

- There are effectively four main features that define if a language has object-orientation.

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
- Avoid HAS-A relationships:

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

- C++ does not have an interface definition as Java and C#.
    - We can use abstract classes without data
- Multiple-inheritance allows us to define a class as inheriting from more than one base-class.
- It's ok if only one of the classes contains data, otherwise it's a **TERRIBLE** idea

```cpp
    class CarEngine
    {
		int power;
    };
	
	class SteeringWheel
    {
		float radius;
    };

    class TerribleCar : public CarEngine, public SteeringWheel
    {
    };
	
	
```

---

# Multiple-inheritance in C++

- This is ok

```cpp
    class IDrawable
    {
		public:
			virtual void Draw() = 0;
    };
	
	class IPhysicsObject
    {
		public:
			virtual void Update() = 0;
    };
	
	class IEntity : public IDrawable, public IPhysicsObject
    {
    };    
	
	class GameEntity : IEntity
    {
		public:
			void Draw() override { /*...*/}
			void Update() override { /*...*/}
    };
	
	
```


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
- An object can be converted into any of its base types automatically.
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

    // Proper C++ casting -- USE THIS
    bird *b = static_cast<bird*>(s); 

    // Cast outside the inheritance hierarchy. DANGER ZONE.
    dog *d = reinterpret_cast<dog*>(s);

    // Dynamic cast will return nullptr if not possible. 
    dog *d = dynamic_cast<dog*>(s);

    // We can also remove const, but best not to
    const animal *a1;
    animal *a2 = const_cast<animal*>(a1);
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

## Pointers, references, and general guidelines

---

# Pointers

- You need to work with a reference (e.g. `int&`) or a pointer (e.g. `int*`) value to get the polymorphic behaviour.

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

# Pure `virtual` Members

- C# and Java provide an `interface` specifier to indicate a set of methods that a child class **must** implement itself.
- C++ has no such specifier, but it does allow pure virtual methods.
- A pure virtual method is one that is set to `0`.
- If a class has any pure virtual methods no instances can be created of it.


---

# Pure `virtual` Members

```cpp
class A {
public:
    virtual void a() = 0;
};

class B : public A {};

class C : public B {
public:
    void a() override {};
};

// These two will produce compiler errors
A a;
B b;
// This one is OK
C c;
```


---

# Differences for References than Java & C#

- C++ has references (with the & modifier).
- However, C++ references are not the same as Java references.
- C++ references cannot be changed to point to another variable (unlike pointers).
- C++ references cannot be set to `nullptr`


```cpp
// Try and pass parameters as references when possible
void work(const int &n) {}

// If needed, or if the parameter might be null, use pointers
void work(const int *n) {}

int n = 5;
int& m = n;
n = 6;
// m is also 6
m = 7;
// n is also 7
A a1;
A& a2 = a1;
a2 = A();
// a1 is also a new A
a2 = nullptr; // Compiler error
```


---

# Smart Pointers

- Allocating raw pointers is discouraged in modern C++ 
    - ... but their use is fine, where appropriate!
- Smart pointers allow automatic memory management, via RAII
	- no more memory leaks!
- Two types:
    `shared_ptr`:   reference counted.
    `unique_ptr`:   has only one owner.

```cpp
// When do we call delete?
int *n1 = new int(5);
// Automatically counts references - like a Java reference, but faster
std::shared_ptr<int> n2 = std::make_shared<int>(5);
// Only one reference will exist. Faster than shared_ptr
std::unique_ptr<int> n3 = std::make_unique<int>(5);
// Can still treat as a standard pointer
int n4 = *n3;
// Now have nullptr, n2 will deconstruct itself
n2 = nullptr;
```


---

# Dereferencing Pointers

- Pointers have to be dereferenced to access their members.
- This means using the `*` operator before the object name.
- As this happens so often, and is tiresome, C++ provides the arrow notation (`->`) as a simplification.


```cpp
class A
{
public:
    void work() { }
};

shared_ptr<A> a = make_shared<A>();
// Calling work by dereferencing
(*a).work();
// Better to use arrow notation
a->work();
```


---

# Construction, Destruction, and Assignment

- C++ gives you a LOT of control over how objects are constructed/copied/assigned/destructed/moved
- Keep your code and data simple, and avoid specialising these behaviours
- In special cases (e.g. when you store unique_ptr objects) you have to override some of them
- Rule of three/five/zero 
	- require destructor/copy constructor/copy assignment (e.g. when storing raw pointers of file handles)
	- require all five (e.g. when storing unique_ptr)
	- only use constructors that don't need special destructors (no special resources)

```cpp
class A {
public:
    ~A() = default;                   // destructor
    A(const A&) = default;            // Copy constructor
    A(A&&) = default;                 // move constructor
    A& operator=(const A&) = default; // assignment operator
    A& operator=(A&&) = default;      // move assignment operator 
};
```


---

# Const-correctness

Define Members as `const` If Possible

- Many method calls do not change the state of an object.
- If this is the case, specify the method as `const`.
- This will allow the compiler to optimise your code, which is good.
- It will also allow the compiler to check you are writing correct code if you do this properly.

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

# Declare in Headers, Implement in Code

- This is an idea you might not be as familiar with if you come from a Java and C\# background.
- In C++, declarations should be provided in a header file (.h).
- Actual implementation (definition) should be provided in a code file (.cpp).
- Exceptions exist around pre-compiled headers and templates.


```cpp
// A.h
class A {
    void work();
    int do_more();
};
```
```cpp
// A.cpp
#include "A.h"

void A::work() {
    // Do some work
}
int A::do_more() {
    return 0;  // Do some more work
}
```


---

# Other Concepts

- A number of additional concepts are worth looking into.
 - **PIMPL**: private implementation or pointer-to-implementation. Useful to hide pointer requirements and allow cheap moving of objects.
 - **templates**: are very powerful in C++. Template metaprogramming is a neat thing if you can wrap your head around it.
 - **virtual destructors**: if you have a base-class, the destructor must be virtual. Otherwise clean-up may not be correct.

---

# Summary


---

# Summary

- You have just learned C++ in an hour. <!-- .element: class="fragment" -->
- This is obviously not possible, and you will need practice in these ideas. I am simply signposting ideas. <!-- .element: class="fragment" -->
- C++ is one of the most complicated languages around (they keep adding features), so get a good working knowledge of what you need and hack it together. <!-- .element: class="fragment" -->
- Key thing today was how to do object-orientation properly. Hopefully you can work around this with your previous Java and C\# knowledge. <!-- .element: class="fragment" -->
- But at the end of the day it is all about practice. <!-- .element: class="fragment" -->


---

# Golden Rules / top tips

1. Keep stuff out of header files. Only the bare minimum!
 - *Forward declare* types in header files (Google it).
 - You don't need to include dog.h if you only ever have a dog pointer.

1. Use unique_ptr or shared_ptr as required
 - Don't even call new. Or delete. Or malloc and free. Ever. Just don't.

1. Use const as much as you can.

1. Put breakpoints in all your constructors/destructors/assignment operators when debugging scope issues. 
 - You might be surprised by when they are called!


