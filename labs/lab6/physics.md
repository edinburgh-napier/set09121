---
title: "Physics"
keywords: Physics
tags: [Physics]
permalink:  physics.html
summary: Physics
sidebar: home_sidebar
---

## Box2D


<video class="middle" width="400" height="300" loop autoplay>
  <source src="assets/videos/physics.mp4" type="video/mp4">
</video>

We will be using the [Box2D](http://box2d.org/) physics engine from here on. B2D is relatively robust and well used. Building a 2d physics engine yourself isn't an impossible task, but we don't have time to cover it in this module, so we will be using something that already exists.

When with picking software off the web, chances are it needs some tweaks. B2D doesn't have a well-built CMake Script, but thanks to the process of open-source software, [I've fixed it](https://github.com/dooglz/Box2D/commit/e8d2cafa7f1300f5916a2e22f277d998a739e835). 

The fix is still pending in a pull request to the main repo so for now we will use my fork.

### Add the Submodule

We haven't done this since the very beginning when we added SFML, time
to do it again. Open gitbash in the root of your repo

```bash
git submodule add https://github.com/dooglz/Box2D.git lib/b2d
git submodule init
git submodule update
```

### Amend the CMakeLists

Adding B2D to our build process is pretty easy:

```cmake
# B2D - Box2D phyics library
add_subdirectory("lib/b2d/Box2D")
#include_directories("lib/b2d/Box2D/")
set(B2D_INCS "lib/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")
```

Then we can just link with `Box2D` and include `${B2D_INCS}`.

```cmake
## physics
file(GLOB_RECURSE SOURCES 5_physics/*.cpp 5_physics/*.h)
add_executable(5_PHYSICS ${SOURCES})
target_include_directories(5_PHYSICS SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(5_PHYSICS Box2D)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_5_PHYSICS)
```

## A standard physics Engine


A physics System/Engine usually has the following components

- **A World**
  - A data-structure that contains all the physics objects in the "world\". Usually this also has some global parameters such as "Gravity\". Some physics engines allow you to have multiple "worlds\". Think of this like a \"physics Scene\".
- **An Integrator**
  - This is the algorithm that runs each physics 'Tick' or 'Step', to calculate the acceleration, velocity, and position of all bodies in the world. The More 'ticks', the more accurate the simulation is. We usually don't have any control of the inner workings of this.
- **Physics Bodies**
  - Usually called Rigid-bodies (unless dealing with deformable or fluid things). These are things that have mass, inertia, position, and velocity. The physics Integrator moves theses things around based on the rules of physics.
- **Colliders**
  - These are the physical 'shapes' of bodies. e.g cubes, circles, polygons. They determine how two bodies collide with each other. A body is just an abstract 'thing' that has mass. Colliders give them shapes and behaviour.
- **Constraints**
  - Connect Bodies together, either permanently, or based on some form of logic (elastic, ropes, springs, hinges, axles).

The typical process of dealing with a physics engine is as follows:

1.  Create the world
2.  Create Bodies and attach colliders to them
3.  Each Update(), step the physics simulation.
4.  Update Entity positions to that of the physics bodies.

As you can see, we keep the physics world separate from the Game world. We leave this all to box2D to manage. After a simulation step, we look at the new positions of all the objects in the physics world and copy the new positions to the 'real' world render objects.

### Interactivity

So far this works well for an initial scene, but we want interactivity, we want a game. For this we need to feed some gamelogic *into* the physics world.

We are allowed to manually set the position and velocity of any physics body as a cheap "teleport". Doing this isn't great as it breaks the rules of physics that B2D is trying to stick to. Things don't just teleport in real life. Instead we should use "impulses".

### Impulses

These are momentary forces that are applied to a body for one frame. Think of it as giving a thing a little or nudge, or in some cases, strapping rockets to a box for one frame. This is how we will mostly move things around in the physics world, as it obeys the rules of physics. Heavier objects will need a larger impulse force.

### Cheating Physics {#cheatphysics}

If our game was solely physical bodies moving around realistically, impulses would be all we needed. Unfortunately in the world of games, we tend to need things that don't *quite* follow *all* the rules of physics.

Think of Super Mario, he can jump and fall and collide with things, which obey the rules of physics. However, Mario never rotates. He also jumps very quickly, to a set height, and then falls down rather slowly. He can move at set speed left and right, he never "accelerates" up to that speed.

Figuring out the correct amount of newtons to impulse Mario by when he jumps seems like a complicated step backwards. We just want him to \"jump\" like a video game character. This is the folly of Physics engines, they work so hard to give us a near-perfect physical world, only for us to introduce strange limitations and additions to make it feel fun. It can feel at times like the physics system is working against you, a beast to be tamed that really *really* wants to make things go flying off at light-speed (Cite: any Bethesda game).


## Working with Box2D


There are three major factors that we must consider when working with B2D specifically.

1.  B2D has it's own Vector maths classes that we must convert to/from
2.  B2D's world goes upwards. Positive Y is towards the top of the screen.
3.  B2D's world has a 'scale'. We render things in 'pixels'. 
  - A sf::box would be 100 'pixels' wide. How much is this in real world units? 100cm, 100m?
  - Usually I use 1 unit = 1 meter, when working on 3D games.
  -  Box2D has a recommended 30 'units' per 1 'pixel' that feels realistic.


Converting between sfml 'screenspace' and b2d 'physics world space' requires taking the above 3 factors into account.

### Creating the world

For this practical we will using a single main.cpp approach to get the basics of B2D shown.

```cpp
//main.cpp
b2World* world;

void init() {
  const b2Vec2 gravity(0.0f, -10.0f);

  // Construct a world, which holds and simulates the physics bodies.
  world = new b2World(gravity);
  ...
}
```

Done, we've just created a world, in 3 lines.

{:class="important"}
You will need to remeber to add the correct include statements, or the above and below code will throw errors!

### Creating physics Bodies

I'll give you five functions. The first 3 are conversion helper functions to deal with translating between the two worlds. The CreatePhysicsBox() is the biggie, inside is all the B2D logic required to add a body to the scene. The last function in an overload of the fourth, which takes in a sf::RectangleShape rather than a position and size.


```cpp
//main.cpp
// 1 sfml unit = 30 physics units
const float physics_scale = 30.0f;
// inverse of physics_scale, useful for calculations
const float physics_scale_inv = 1.0f / physics_scale;
// Magic numbers for accuracy of physics simulation
const int32 velocityIterations = 6;
const int32 positionIterations = 2;

//Convert from b2Vec2 to a Vector2f
inline const Vector2f bv2_to_sv2(const b2Vec2& in) {
  return Vector2f(in.x * physics_scale, (in.y * physics_scale));
}
//Convert from Vector2f to a b2Vec2
inline const b2Vec2 sv2_to_bv2(const Vector2f& in) {
  return b2Vec2(in.x * physics_scale_inv, (in.y * physics_scale_inv));
}
//Convert from Screenspce.y to physics.y
inline const Vector2f invert_height(const Vector2f& in) {
  return Vector2f(in.x, gameHeight - in.y);
}

//Create a Box3d body with a box fixture
b2Body* CreatePhysicsBox(b2World& World, const bool dynamic, const Vector2f& position, const Vector2f& size) {
  b2BodyDef BodyDef;
  //Is Dynamic(moving), or static(Stationary)
  BodyDef.type = dynamic ? b2_dynamicBody : b2_staticBody;
  BodyDef.position = sv2_to_bv2(position);
  //Create the body
  b2Body* body = World.CreateBody(&BodyDef);

  //Create the fixture shape
  b2PolygonShape Shape;
  Shape.SetAsBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
  b2FixtureDef FixtureDef;
  //Fixture properties
  FixtureDef.density = dynamic ? 10.f : 0.f;
  FixtureDef.friction = dynamic ? 0.8f : 1.f;
  FixtureDef.restitution = 1.0;
  FixtureDef.shape = &Shape;
  //Add to body
  body->CreateFixture(&FixtureDef);
  return body;
}

// Create a Box2d body with a box fixture, from a sfml::RectangleShape
b2Body* CreatePhysicsBox(b2World& world, const bool dynamic, const RectangleShape& rs) {
  return CreatePhysicsBox(world, dynamic, rs.getPosition(), rs.getSize());
}

```

Let's put it to use, back to that Init() function.


```cpp
//main.cpp
std::vector<b2Body*> bodies;
std::vector<RectangleShape*> sprites;
...

void init() {
...
  // Create Boxes
  for (int i = 1; i < 11; ++i) {
    // Create SFML shapes for each box
    auto s = new RectangleShape();
    s->setPosition(Vector2f(i * (gameWidth / 12.f), gameHeight * .7f));
    s->setSize(Vector2f(50.0f, 50.0f));
    s->setOrigin(Vector2f(25.0f, 25.0f));
    s->setFillColor(Color::White);
    sprites.push_back(s);
    
    // Create a dynamic physics body for the box
    auto b = CreatePhysicsBox(*world, true, *s);
    // Give the box a spin
    b->ApplyAngularImpulse(5.0f, true);
    bodies.push_back(b);
  }
}
```

So we are creating 10 boxes - both as sfml::RectangleShapes and b2d::bodies, and storing them both in global vectors. Now we just need to keep them in sync. Can you guess what's coming next?
### Updating physics Bodies

This is a two step process, 1: Stepping the physics world, and then copying the data from the bodies to the sf::shapes.


```cpp
//main.cpp
void Update() {
  static sf::Clock clock;
  float dt = clock.restart().asSeconds();
  // Step Physics world by Dt (non-fixed timestep)
  world->Step(dt, velocityIterations, positionIterations);

  for (int i = 0; i < bodies.size(); ++i) {
    // Sync Sprites to physics position
    sprites[i]->setPosition(invert_height(bv2_to_sv2(bodies[i]->GetPosition())));
    // Sync Sprites to physics Rotation
    sprites[i]->setRotation((180 / b2_pi) * bodies[i]->GetAngle());
  }
}
```

{:class="important"}
You now need to ensure that you *render* these boxes so you can test that they do, indeed, move!

### Walls

At the moment our boxes just fall into the abyss. Let's put some walls in. Back to Init() for one last time. We will create 4 walls. The position and size of each will be stored continuously in a vector that we will loop through. I'll let you figure out the full details. In the end it should look like this:

```cpp
//main.cpp

void init() {
 ...
  // Wall Dimensions
  Vector2f walls[] = {
   // Top
   Vector2f(gameWidth * .5f, 5.f), Vector2f(gameWidth, 10.f),
   // Bottom
   Vector2f(gameWidth * .5f, gameHeight - 5.f), Vector2f(gameWidth, 10.f),
   // left
   Vector2f(5.f, gameHeight * .5f), Vector2f(10.f, gameHeight),
   // right
   Vector2f(gameWidth - 5.f, gameHeight * .5f), Vector2f(10.f, gameHeight)
  };

  // Build Walls
  for (int i = 0; i < 7; i += 2) {
    // Create SFML shapes for each wall
    ...
    sprites.push_back(s);
    // Create a static physics body for the wall
   ...
  }
  // Create Boxes
  ...
}
```

{:class="important"}
Don't move on until you have some bouncin' boxes like the gif at the top!

