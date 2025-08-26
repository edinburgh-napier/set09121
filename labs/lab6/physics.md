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

When with picking software off the web, chances are it needs some tweaks. B2D doesn't have a well-built CMake Script, but thanks to the process of open-source software, [Sam fixed it](https://github.com/dooglz/Box2D/commit/e8d2cafa7f1300f5916a2e22f277d998a739e835). 

The fix is still pending in a pull request to the main repo so for now we will use my fork.

### Add the Submodule

We haven't done this since the very beginning when we added SFML, time
to do it again. Open gitbash in the root of your repo

```bash
git submodule add https://github.com/erincatto/box2d.git lib
git submodule init
git submodule update
```
Then, we are going to set box2d on the commit tagged v3.1.1
```bash
cd lib/box2d
git checkout v3.1.1
```

### Amend the CMakeLists

Adding B2D to our build process is pretty easy:

```cmake
# B2D - Box2D phyics library
add_subdirectory("lib/box2d")
set(B2D_INCS "lib/box2d/include")
link_directories("${CMAKE_BINARY_DIR}/lib/box2d/lib")
```

Then we can just link with `Box2D` and include `${B2D_INCS}`.

```cmake
## physics
add_executable(physics ...)
target_include_directories(physics SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(physics Box2D sfml-graphics)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_5_PHYSICS)
```

Remember you'll have to add multiple things to the target_link_libraries line if you want to use Box2D, SMFL, and our libraries we've created!

## A standard physics Engine


A physics System/Engine usually has the following components

- **A World**
  - A data-structure that contains all the physics objects in the "world\". Usually this also has some global parameters such as "Gravity\". Some physics engines allow you to have multiple "worlds\". Think of this like a \"Physics Scene\".
- **An Integrator**
  - This is the algorithm that runs each physics 'Tick' or 'Step', to calculate the acceleration, velocity, and position of all bodies in the world. The More 'ticks', the more accurate the simulation is. We usually don't have any control of the inner workings of this.
- **Physics Bodies**
  - Usually called Rigid Bodies (unless dealing with deformable or fluid things). These are things that have mass, inertia, position, and velocity. The physics Integrator moves these things around based on the rules of physics.
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

So far this works well for an initial scene, but we want interactivity, we want a game. For this we need to feed some game logic *into* the physics world.

We are allowed to manually set the position and velocity of any physics body as a cheap "teleport". Doing this isn't great as it breaks the rules of physics that B2D is trying to stick to. Things don't just teleport in real life. Instead we should use "impulses".

### Impulses

These are momentary forces that are applied to a body for one frame. Think of it as giving a thing a little or nudge, or in some cases, strapping rockets to a box for one frame. This is how we will mostly move things around in the physics world, as it obeys the rules of physics. Heavier objects will need a larger impulse force.

### Cheating Physics {#cheatphysics}

If our game was solely physical bodies moving around realistically, impulses would be all we needed. Unfortunately in the world of games, we tend to need things that don't *quite* follow *all* the rules of physics.

Think of Super Mario, he can jump and fall and collide with things, which obey the rules of physics. However, Mario never rotates. He also jumps very quickly, to a set height, and then falls down rather slowly. He can move at set speed left and right, he never "accelerates" up to that speed.

Figuring out the correct amount of newtons to impulse Mario by when he jumps seems like a complicated step backwards. We just want him to \"jump\" like a video game character. This is the folly of physics engines, they work so hard to give us a near-perfect physical world, only for us to introduce strange limitations and additions to make it feel fun. It can feel at times like the physics system is working against you, a beast to be tamed that really *really* wants to make things go flying off at light-speed (Cite: any Bethesda game).


## Working with Box2D


There are three major factors that we must consider when working with B2D specifically.

1.  B2D has it's own Vector maths classes that we must convert to/from
2.  B2D's world goes upwards. Positive Y is towards the top of the screen.
3.  B2D's world has a 'scale'. We render things in 'pixels'. 
  - A sf::box would be 100 'pixels' wide. How much is this in real world units? 100cm, 100m?
  - Usually I use 1 unit = 1 meter, when working on 3D games.
  -  Box2D has a recommended 30 'units' per 1 'pixel' that feels realistic.


Converting between SMFL 'screen space' and B2D 'physics world space' requires taking the above 3 factors into account.

### PhysicsScene

First, we need to create a scene like we did in the previous labs.

```cpp
//scenes.hpp
class PhysicsScene : public Scene {
private:
  b2WorldId world_id; //an id to the physic world
public:
  PhysicsScene() = default;
  void update(const float &dt) override;
  void render() override;
  void load()override;
};
```

### Creating the world

For this practical we will using a single main.cpp approach to get the basics of B2D shown.

```cpp
//scenes.cpp
#include "game_parameters.hpp"

using param = Paramters;
void PhysicsScene::load() {
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = b2Vec2({0.0f, param::g});  
    world_id = b2CreateWorld(&world_def);
  ...
}
```

Done, we've just created a world, in 3 lines.

{:class="important"}
You will need to remember to add the correct include statements, or the above and below code will throw errors!

### Creating physics Bodies

I'll give you five functions. The first 3 are conversion helper functions to deal with translating between the two worlds. The `create_physics_box()` is the biggie, inside is all the B2D logic required to add a body to the scene. The last function in an overload of the fourth, which takes in a sf::RectangleShape rather than a position and size.

**Remember you should by now know what you need to include, so don't forget to add them!**

First, we need some handy parameters.
```cpp
//game_parameters.hpp
struct Parameters
{
  ...
  // 1 sfml unit = 30 physics units
  static constexpr float physics_scale = 30.f;
  static constexpr float physics_scale_inv = 1.0f / physics_scale;
  // Magic numbers for accuracy of physics simulation
  static constexpr int velocityIterations = 6;
  static constexpr int positionIterations = 2;
}
```

Then, in new files *b2d_utils.hpp* and *b2d_utils.cpp*, we will declare and implement our helpers function. 

```cpp
//b2d_utils.hpp
namespace box2d{

//Convert from b2Vec2 to a Vector2f
const sf::Vector2f bv2_to_sv2(const b2Vec2& in);
//Convert from Vector2f to a b2Vec2
const b2Vec2 sv2_to_bv2(const sf::Vector2f& in);
//Convert from screenspace.y to physics.y (as they are the other way around)
const sf::Vector2f invert_height(const sf::Vector2f& in);

//Create a Box2D body with a box fixture
b2BodyId create_physics_box(b2WorldId& world_id, const bool dynamic, const sf::Vector2f& position, const sf::Vector2f& size);
b2BodyId create_physics_box(b2WorldId& world_id, const bool dynamic, const std::shared_ptr<sf::RectangleShape>& rs);

}//box2d
```

The helper function are declared in a new namespace. In this way, we are not defining global functions without a context.

```cpp
//b2d_utils.cpp

using param = Parameters
//Convert from b2Vec2 to a Vector2f
const sf::Vector2f box2d::bv2_to_sv2(const b2Vec2& in) {
  return sf::Vector2f(in.x * param::physics_scale, (in.y * param::physics_scale));
}
//Convert from Vector2f to a b2Vec2
const b2Vec2 box2d::sv2_to_bv2(const sf::Vector2f& in) {
  return {in.x * param::physics_scale_inv, in.y * param::physics_scale_inv};
}
//Convert from screenspace.y to physics.y (as they are the other way around)
const sf::Vector2f box2d::invert_height(const sf::Vector2f& in) {
  return sf::Vector2f(in.x, game_height - in.y);
}

//Create a Box2D body with a box fixture
b2BodyId box2d::create_physics_box(b2WorldId& world_id, const bool dynamic, const sf::Vector2f& position, const sf::Vector2f& size) {
  b2BodyDef body_def = b2DefaultBodyDef();
  //Is Dynamic(moving), or static(Stationary)
  body_def.type = dynamic ? b2_dynamicBody : b2_staticBody;
  body_def.position = sv2_to_bv2(position);
  //Create the body
  b2BodyId body_id = b2CreateBody(world_id,&body_def);

  //Create the fixture shape
  b2ShapeDef shape_def = b2DefaultShapeDef();
  shape_def.density = dynamic ? 10.f : 0.f;
  shape_def.material.friction =  dynamic ? 0.8f : 1.f;
  shape_def.material.restitution = 1.0f;
  b2Polygon polygon = b2MakeBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
  b2CreatePolygonShape(body_id,&shape_def,&polygon);

  return body_id;
}

b2BodyId box2d::create_physics_box(b2WorldId& world_id, const bool dynamic, const std::shared_ptr<sf::RectangleShape>& rs){
  return create_physics_box(world_id,dynamic,rs->getPosition(),rs->getSize());
}
```

Let's put it to use, back to our scene.

```cpp
//scenes.hpp
class PhysicsScene: public Scene{
private:
...
  std::vector<b2BodyId> bodies;
  std::vector<std::shared_ptr<sf::RectangleShape>> sprites;
...
};

struct Scenes{
  static std::shared_ptr<Scene> physics;
};
```


```cpp
//scenes.cpp

...

std::shared_ptr<Scene> Scenes::physics;

void PhysicsScene() {
...
  // Create Boxes
  for (int i = 1; i < 11; ++i) {
    // Create SFML shapes for each box
    std::shared_ptr<sf::RectangleShape> s = std::make_shared<sf::RectangleShape>();
    s->setPosition(sf::Vector2f(i * (game_width / 12.f), game_height * .7f));
    s->setSize(sf::Vector2f(50.0f, 50.0f));
    s->setOrigin(sf::Vector2f(25.0f, 25.0f));
    s->setFillColor(sf::Color::White);
    sprites.push_back(s);
    
    // Create a dynamic physics body for the box
    b2BodyId b = b2::create_physics_box(world_id, true, s);
    // Give the box a spin
    b2Body_ApplyAngularImpulse(b,5.0f, true);
    bodies.push_back(b);
  }
}
```

So we are creating 10 boxes - both as sf::RectangleShapes and box2d bodies identified with b2BodyId, and storing them both in attribute of our scene. Now we just need to keep them in sync. Can you guess what's coming next?
### Updating physics Bodies

This is a two step process, 1: Stepping the physics world, and then copying the data from the bodies to the sf::shapes.


```cpp
//scenes.cpp
void PhysicsScene::update(const float& dt) {

  
  // Step Physics world by time_step
  b2World_Step(world_id,param::time_step,param::sub_step_count);

  for (int i = 0; i < bodies.size(); ++i) {
    // Sync Sprites to physics position
    sprites[i]->setPosition(b2::invert_height(b2::bv2_to_sv2(b2Body_GetPosition(bodies[i]))));
    // Sync Sprites to physics Rotation
    sprites[i]->setRotation((180 / M_PI) * asin(b2Body_GetRotation(bodies[i]).s));
  }
}
```

{:class="important"}
You now need to ensure that you *render* these boxes so you can test that they do, indeed, move!

### Walls

At the moment our boxes just fall into the abyss. Let's put some walls in. Back to init() for one last time. We will create 4 walls. The position and size of each will be stored continuously in a vector that we will loop through. I'll let you figure out the full details. In the end it should look like this:

```cpp
//scenes.cpp
void PhysicsScene::load(){
...
  sf::Vector2f walls[] = {
    // Top
    sf::Vector2f(param::game_width * .5f, 5.f), sf::Vector2f(param::game_width, 10.f),
    // Bottom
    sf::Vector2f(param::game_width * .5f, param::game_height - 5.f), sf::Vector2f(param::game_width, 10.f),
    // left
    sf::Vector2f(5.f, param::game_height * .5f), sf::Vector2f(10.f, param::game_height),
    // right
    sf::Vector2f(param::game_width - 5.f, param::game_height * .5f), sf::Vector2f(10.f, param::game_height)
  };

  // Build Walls
  for (int i = 0; i < 7; i += 2) {
      // Create SFML shapes for each wall
      ...
      sprites.push_back(s);
      // Create a static physics body for the wall
      ...
      bodies.push_back(b);
  }
}
```

A hint: we've defined the vectors in that way so we can store both the central point and size in pairs, hence the slightly different for loop.
Hint 2: the origin is a bit more tricky to get right, but remember you can divide a vector by a number if you need to!

{:class="important"}
Don't move on until you have some bouncin' boxes like the gif at the top!

Previous step: [PacMan 3](pacman3)

Next step: [Platformer](platformer)