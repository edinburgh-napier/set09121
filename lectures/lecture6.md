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
Thomas Methven
<br>
(Original material by Kevin Chalmers and Sam Serrels)

School of Computing. Edinburgh Napier University


---

# Recommended Reading:
## Any C++ book really, but C++ Primer is good.


---

# Goal
## To learn object-orientation in C++


---

## C++ is __NOT__ an object-oriented language.

## Or *C With classes*


**ObjectiveC** is something else entirely, nothing to do with us


---

# Why you Need to Know Object-orientation in C++

- We have defined our games using Formal Elements. <!-- .element: class="fragment" -->
- We have also defined our game as a system. <!-- .element: class="fragment" -->
- We have also taken an entity view of a game. <!-- .element: class="fragment" -->
- All of these elements require us to define objects. <!-- .element: class="fragment" -->
- You are also going to build one of the most complex systems you have undertaken at university. This requires breaking the system down into controllable components. This is what object-orientation is for. <!-- .element: class="fragment" -->
- Also, this is an opportunity to learn object-orientation properly (or at least from one point-of-view). <!-- .element: class="fragment" -->


---

# Basics of Object-orientation in C++

- You'll will see plenty of this in the labs <!-- .element: class="fragment" -->
- Here we are going to talk about what some of that means <!-- .element: class="fragment" -->

---

# Defining a class in C++

- `class` definitions are simple in C++.
- To define a `class` in C++ we use the `class` keyword followed by the name of the `class`.
- A `class` is then the members defined between the curly brackets.
- **Note** - a semi-colon is required at the end of the definition. This is different to Java and C\#.

`class` Definition in C++

```cpp
class my_class
{
    // Members
};
```

---

# Defining a struct in C++

- C++ also allows the definitions of `struct` types.
- `struct` definitions are also simple in C++.
- To define a `struct` in C++ we use the `struct` keyword followed by the name of the `struct`.
- A `struct` is then the members defined between the curly brackets.

`struct` Definition in C++

```cpp
struct my_struct
{
    // Members
};
```

---

# Defining Attributes

- Attributes are the values that go along with our objects.
- C++ doesn't define the visibility per attribute (unlike Java & C#).
    - We will look at visibility in a few slides.

Attributes in C++
```cpp
class my_class
{
    // Object (instance) values.
    float x;
    float y = 0.5f; // Initialised value
    const string name; // Constant value
    // Class (static) values.
    static int n;
};
```

---

# Defining Methods

- Same rules apply for methods.

Methods in C++
```cpp
class my_class
{
    void do_something()
    {
        // Do something.
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
- Constructors are **very** important in C++. They are called in various forms in various places.
    - Understanding object construction in C++ will help you spot a number of bugs.

Constructors in C++

```cpp
class my_class
{
public:
    // Default constructor
    my_class() { }
    // Parameterised constructor
    my_class(float xx, float yy)
    : x(xx), y(yy) // Sets object attributes
    {
    }
};
```

---

# Defining Destructors
- Destructors determine how an object is destroyed when it goes out of scope.
    - This is **very** important. C++ is not garbage collected - you are in control.
- A destructor is called:
    - whenever an object goes out of scope (i.e. defined between curly brackets).
    - When the object is manually deleted
- A destructor looks like a constructor with a tilde ~ in front of it. A class can only have one destructor.


---

# Destructors in C++

```cpp
class my_class
{
public:
    // Default Destructor
    ~my_class()
    {
        // Free up resources.
    }
};
```


---

# RAII

Our First Rule of Good OO in C++ - RAII
- RAII stands for Resource Allocation Is Initialisation. <!-- .element: class="fragment" -->
- It is a rule used in good C++ code. <!-- .element: class="fragment" -->
- When an object is created it allocates or takes ownership of its required resources (via the constructor). <!-- .element: class="fragment" -->
- When an object is destroyed it frees up its allocated and owned resources (via the destructor). <!-- .element: class="fragment" -->
- This ensures that we do not have memory leaks. Resources have their life tied to an object's life. <!-- .element: class="fragment" -->
- Getting into this habit is also good for all programming, even in garbage collected languages. <!-- .element: class="fragment" -->


---

# RAII exaplined

NOT RAII
```cpp
void Main(){
    Texture MarioTexture = LoadTexture("Mario.jpg");
    Mario* mario = new Mario(MarioTexture);
    //--- some time later
    delete mario;
    CloseGame();
    //Oh no - we forgot to unload the texture!
}
```

RAII
```cpp
void Main(){
    //Mario Loads his texture himself
    Mario* mario = new Mario("Mario.jpg");
    //--- some time later
    delete mario; //mario unloads texture in his Destructor.
    CloseGame();
}

```
RAII : Mario should clean up after himself!

---

# Object-orientation Concepts in C++


---

# Core Object-orientation Concepts

- As stated, C++ has object-orientated features. There are effectively four features that define a language has having object-orientation.

- These are the key elements of working in an object-oriented style. They should underpin your practice in object-oriented software development.

---

# Core Object-orientation Concepts

- **Encapsulation**: the ability for objects to contain and own resources.
- **Inheritance**: the ability to base behaviour and values on another class specification.
- **Polymorphism**: the ability for a specific object to act like different types. (Common Interfaces) 
- **Overloading**: the ability to overwrite inherited behaviour to specialised local behaviour.

---

# Think Back to the Labs

- Have you already done some of these? <!-- .element: class="fragment" -->
- What about the Pong or Space Invaders example? <!-- .element: class="fragment" -->
- Where have you used these already? <!-- .element: class="fragment" -->

---

# Core Object-orientation Concepts

- **Encapsulation**
- Inheritance
- Polymorphism
- Overloading


---

# Encapsulation 


- Encapsulation means that we can create components from other components (classes have attributes). <!-- .element: class="fragment" -->
- Encapsulation also implies that objects own their resources and control their lifelines. <!-- .element: class="fragment" -->
    - Many formal models work on this view. <!-- .element: class="fragment" -->

---

# Be Careful!

- **It is very simple** to reference across the object boundary, leading to data mutation problems.


```
    class my_class
    {
        string &str;
        my_class(string &s)
        : str(s)
        {
        }
    };

    string s = "hello";
    my_class c1(s);
    my_class c2(s);
    // Both c1 and c2 point to
    // same value.
```


---

# Scope Protection

`public, private` and `protected`

- We can specify the visibility of class members via `public`, `private`, and `protected` modifiers.
- The difference between a `struct` and a `class` is just the default visibility. `struct` is `public`, `class` is `private`.
- We define "zones" of visibility in C++ rather than individual values.


```
    class my_class
    {
        // This value is private.
        int x;
    public:
        // The following are public.
        my_class() { }
        float n;
    protected:
        // The following are protected.
        string str;
    private:
        // Private again.
    };
```


---

# Core Object-orientation Concepts

- Encapsulation
- **Inheritance**
- Polymorphism
- Overloading


---

# What is Inheritance?

- Inheritance is the ability to base part of a class's behaviour on an existing class definition (specification).

- Inheritance is a key reuse feature of object-orientation. 

- A *base-class* is a *generalisation* of required behaviour. A *derived-class* is a *specialisation* of this base behaviour.

- Inheritance is also the object-orientation feature that enables polymorphism.

---

# Be Careful!

- **Warning** - a good practice is to avoid deep levels of inheritance. I personally aim for a base-class to provide an **interface** specification to a collection of derived-classes. I try and have single-level inheritance as far as possible.

---

# Inheritance in C++
```
    class A
    {
    };

    class B : public A
    {
    };

    class C : public A
    {
    };
```


---

# Multiple-inheritance in C++

- C++ does not have an interface definition as Java and C#.
    - We will look at virtual behaviour shortly. 
- We do have multiple-inheritance which provides the same features (more-or-less). 
- Multiple-inheritance allows us to define a class as inheriting from more than one base-class.

```
    class A
    {
    };

    class B
    {
    };

    class C : public A, public B
    {
    };
```


---

# Core Object-orientation Concepts

- Encapsulation
- Inheritance
- **Polymorphism**
- Overloading


---

# What is Polymorphism?

- Polymorphism is the ability of our objects to act as different types.

- Understanding polymorphism is one of the most important aspects of object-oriented development. Having an object provide a known interface that produces different behaviour is fundamental to software reuse.

---

# What is Polymorphism?

- There are actually three types of polymorphism in computer science. C++ supports all three.
 - **Ad-hoc polymorphism**:   overriding functions with different parameters (we will look at this in the overloading section).
 - **Parametric polymorphism**:   overriding types based on a parameter (e.g. `vector<int>`). We will briefly look at templates at the end of lecture.
 - **Subtyping**:   having a type be derived from other super-types. This is inheritance and the focus of this section.

---

# Polymorphism in C++


- Polymorphism in C++ occurs whenever we derive classes.
- An object can be converted into any of its base types automatically.
- There are a few caveats which we will look at later. But hopefully you are all familiar with this basic concept.
```
    class Animal{
    public:
        void make_sound() { }
    };

    class Dog : public Animal{};

    void func(Animal a) { 
        a.make_sound(); 
    }

    Dog fido;
    func(fido);
```


---

# Examples

Converting (Casting) Between Types in C++
```
    sparrow *s;
    // C-style casting.  DON'T DO THIS!
    bird *b = (bird*)s; 

    // Proper C++ casting
    bird *b = static_cast<bird*>(s); 

    // Cast outside the inheritance hierarchy.
    dog *d = reinterpret_cast<dog*>(s);

    // Dynamic cast will return nullptr if not possible.
    dog *d = dynamic_cast<dog*>(s);

    // We can also remove const, but best not to
    const animal *a1;
    animal *a2 = const_cast<animal*>(a1);
```


---

# Core Object-orientation Concepts

- Encapsulation
- Inheritance
- Polymorphism
- **Overloading**


---

# What is Overloading?

- A key concept of polymorphism is specialisation from a generalised interface.
- What this means is that an object may look like a general type, but will act like a special type.

---

# Overloading Example

- For example:
    - Animal type has a `make_sound` method.
    - Create an object of type Sparrow.
    - Sparrow makes a chirp sound.
    - We treat it like a bird, it still makes a chirp sound.
    - We treat it like an animal, it still makes a chirp sound.
    - If we create a dog, it will always make a bark sound.
- *The specialised objects can overload the generalised behaviour.* <!-- .element: class="fragment" -->

---

# Method Overload

- A basic form of polymorphism is *ad-hoc polymorphism*.
- This means we can have the same method name, but with different parameters and return type.
- Object-orientation generally allows this through method overloading.
- Note that parameters are the real way of distinguishing methods - the return type cannot be different for the same parameters.

```
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
- When a method is `virtual` it means method calls are looked up via a virtual function table. 
    - Called dynamic dispatch.
- Basically it means that instead of looking up a function to call based on type, an object shows where a function is from its own state information.
- Child classes can then overwrite `virtual` ones if they choose.



---

# `virtual` Members

```
class A
{
public:
    virtual void work()
    {
        cout << "Hello" << endl;
    }
};

class B : public A
{
public:
    void work()
    {
        cout << "Goodbye" << endl;
    }
};

A *a = new B();
// Will print Goodbye
a->work();
```


---

# Proper Overloading in C++

- Just overriding behaviour is not best practice in C++.
- You need to indicate what you mean so the compiler you can check you are doing things correctly.
- Two keywords introduced in C++11:
    - `override` marks a method as overriding a parent one.
    - `final`  marks a method as no longer overrideable in further child classes.
- The compiler can create better code if you used correctly.


---

# Proper Overloading in C++

``` 
class A
{
public:
    virtual void work();
};

class B : public A
{
public:
    // Compiler will check the
    // override is valid.
    void work() override;
};

class C : public A
{
public:
    // Compiler will also check
    // that no child classes
    // override.  Compiler can
    // also optimise.
    void work() override final;
};
```


---

## **Danger** - values, references, and pointers

---

# Pointers

- In C++ you do not get polymorphic behaviour from just having a polymorphic type.
- You need to work with a reference (e.g. `int&`) or a pointer (e.g. `int*`) value to get the polymorphic behaviour.
- If you use a value then it will only call the methods defined for that type of the value.

---

# Pointers

```
class A {
public:
    virtual void work() { 
        printf("a"); 
    }
};

class B : public A {
public:
    void work() override { 
        printf("b"); 
    }
};

B b;
b.work(); // Prints b
A a1 = (A)b;
a.work(); // Prints a

A& a2 = (A&)b;
a2.work(); // Prints b

A* a3 = (A*)&b;
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

```
class A {
public:
    virtual void a() = 0;
};

class B : public A {};

class C : public B {
public:
    void a() override final {};
};

// These two will produce compiler errors
A a;
B b;
// This one is OK
C c;
```


---

# How to do Object-orientation Properly in C++


---

# Pointers and References

- As stated, you need to have pointer or references for polymorphic behaviour to work in C++.
- For pure virtual methods, this means that we can only have pointers to objects of their type.
- This can be a stumbling block for new C++ programmers as it is a bit different to what you are used to.


```
class A
{
};

class B : public A
{
};

// This has static type B
B b;
// This has static type A
A a1 = (B)b;
// This has polymorphic type A
A& a2 = (A&)b;
```


---

# Differences for References than Java & C#

- C++ has references (with the & modifier).
- However, C++ references are not the same as Java references.
- C++ references **always** point to the same location.
- C++ references cannot be set to `null` (or similar) except in the case of numbers (as `NULL` is `0`)


```
// Try and pass parameters as references when possible
void work(const int &n) {}

// If needs be use pointers
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
a2 = NULL; // Compiler error
m = NULL; // OK as NULL = 0
```


---

# Smart Pointers

- In modern C++ you are better using smart pointers than raw pointers.
- Raw pointers are now discouraged.
- Smart pointers allow automatic memory allocation, and thus get round all the C++ problems.
- Two types:
    `shared_ptr`:   are reference counted.
    `unique_ptr`:   have only one owner.

```cpp
// When do we call delete?
int *n1 = new int(5);
// Automatically counts references - like a Java reference, but faster
shared_ptr<int> n2 = make_shared<int>(5);
// Only one reference will exist. Faster than shared_ptr
unique_ptr<int> n3 = make_unique<int>(5);
// Can still treat as a standard pointer
int n4 = *n3;
// Now have nullptr, n2 will deconstruct itself
n2 = nullptr;
```


---

# Calling Members to Pointers

- Pointers have to be dereferenced to access their members.
- This means using the `*` operator before the object name.
- As this happens so often, and is tiresome, C++ provides the arrow notation (`->`) as a simplification.


```
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

- C++ allows very fine grained control of a number of behaviours.
- Object copying occurs whenever you call a method, or build an object from an existing one.
- Object assignment occurs whenever you use the `=` operator.
- These new objects will all require a destructor call to clean-up.
- This can lead to numerous unnecessary calls if you are not careful.


```
class A {
public:
    virtual ~A() = default;  // Virtual destructor.  Base class.
    A(const A&) = default;  // Can specify, delete, or use default.
    A(A&&) = default;
    A& operator=(const A&) = default;
    A& operator=(A&&) = default;
};

void work(A a) { };

A a1;
// Calls assign operator
A a2 = a1;
// Calls copy constructor
work(a1);
```


---

# Const

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
 - **templates**: are very powerful in C++. Metatemplate programming is a neat thing if you can wrap your head around it.
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

# Sam's Golden Rules / top tips

1. Keep stuff out of header files. Only the bare minimum!
 - OMG *Forward declare pointers* in header files (Google it).
 - You don't need to include dog.h if you only ever have a dog pointer.

1. When in doubt, use Unique_ptr. Move to shared_ptr if you need it
 - Don't even call New(). Or Delete(). Ever. Just don't.

1. Use Const everywhere.

5. Put breakpoints in all your deconstructors when debugging scope issues. 
 - When they call will surprise you!


