---
title: "Platformer"
keywords: platformer
tags: [platformer]
permalink:  platformer.html
summary: platformer
sidebar: home_sidebar
---

<video class="middle" width="640" height="360" loop autoplay>
  <source src="assets/videos/platformer.webm" type="video/webm">
</video>

Today, we are going to look at the basics of platformer game. It is so far the most complicated genre to implement. In one hand, tuning the physics parameters to have a good game feel of the jump and in the other hand having the right collisions can get tricky. 

## Update the Engine with the physics

The first step is to package what have implemented in the previous lab for the physics and integrate into our engine library.
Similarly, to the other classes in our library, we put all the function related to the physics engine as static members of a class.
```cpp
class Physics {
public:
    static void initialise(); //initialise the box2d world
    static void shutdown(); //close the physics engine
    static void update(const float&);//step the simulation

    static b2WorldId get_world_id();

    static b2ContactEvents get_contact_events();// get all the contact events at the current time step

    //Convert from b2Vec2 to a Vector2f
    static const sf::Vector2f bv2_to_sv2(const b2Vec2& in);
    //Convert from Vector2f to a b2Vec2
    static const b2Vec2 sv2_to_bv2(const sf::Vector2f& in);
    //Convert from screenspace.y to physics.y (as they are the other way around)
    static const sf::Vector2f invert_height(const sf::Vector2f& in,const int &game_height);

    //General parameters related to the physic engine
    static constexpr float physics_scale = 30.0f; //30 pixels = 1 meter
    static constexpr float physics_scale_inv = 1.0f / physics_scale;
    static constexpr int sub_step_count = 4; //specific box2d parameter (see box2d API)
    static constexpr float time_step = 0.017f; //60FPS update
    static constexpr float gravity = -9.8f; //gravity constant 
private:
    static b2WorldId _world_id;
};
```
So, we have `initialise`, `shutdown`, and `update` the routine functions to handle the physics engine. The helper functions to convert sfml vectors to box2d vectors. And, finally, parameters related to the physics engine.

I leave it to you the implementation of those functions. You just have to take what we have implemented in the previous lab and put them in the right places.


## Components
The next step is to implement the components, we will need for this game. Let's create four new files: `graphics_cmp.hpp`, `graphics_cmp.cpp`, `physics_cmp.hpp`, and `physics_cmp.cpp`. As you can guess, *graphics_cmp* will have the components related to what is going to be displyed and *physics_cmp* will have the components related to the physics engine.

In `graphics_cmp.*pp` you are going to put the ShapeComponent we defined in the pacman lab. 

Now, let have a look to the main chunk, the physics components.

### PlatformComponent

A platformer needs platform on which can jump. So, let's define the component that will serve to define the platform behaviour.

```cpp
//physics_cmp.hpp
class PlatformComponent : public Component{
  public:
    PlatformComponent(Entity* p,const sf::Vector2i &tile);
    void update(const float &dt) override;
    void render() override;
    const b2ShapeId &get_shape_id() const;
    ~PlatformComponent() override;

  protected:
    b2BodyId _body_id;
    b2ShapeId _shape_id;
};
```
So, this component will build from a tile size taken from our level system and be a static box. 
```cpp
//physics_cmp.cpp
PlatformComponent::PlatformComponent(Entity *p,const sf::Vector2f &size)
: Component(p){
  b2BodyDef body_def = b2DefaultBodyDef();
  body_def.type = b2_staticBody;
  //Create the body
  _body_id = b2CreateBody(ph::get_world_id(),&body_def);
  //Create the fixture shape
  b2ShapeDef shape_def = b2DefaultShapeDef();
  shape_def.density = _dynamic ? 10.f : 0.f;
  shape_def.material.friction =  _dynamic ? 0.1f : 0.8f;
  shape_def.material.restitution = 0.2f;
  b2Polygon polygon = b2MakeBox(ph::sv2_to_bv2(size).x * 0.5f, ph::sv2_to_bv2(size).y * 0.5f);
  _shape_id = b2CreatePolygonShape(_body_id,&shape_def,&polygon);
}
void PlatformComponent::update(const float &dt){}
void PlatformComponent::render(){}
PlatformComponent::~PlatformComponent(){
  b2DestroyChain(_shape_id);
  _shape_id = b2_nullShapeId;
  b2DestroyBody(_body_id);
  _body_id = b2_nullBodyId;
}
```

### Player Component

For the player, we will first implement a PhysicsComponent class and then implement a PlayerPhysicsComponent class that will inherit from the former. This structure will allow you to define enemies component more easily. 

#### Physics Component

The base PhysicsComponent in physics_cmps.cpp is perhaps our most complex component yet. Many of it's function are getters and setters to interface correctly with SFML and Box2d maths, and as an interface to change physics properties.

The constructor does the same logic that we had in the physics practical, creating either a static or dynamic body.

Then there are some new functions such as `get_contacts()`, which we can use to interface gamelogic with collisions. 

```cpp
//physics_cmp.chpp
class PhysicsComponent : public Component {
public:
  PhysicsComponent(Entity* p, bool dyn);

  const b2ShapeId &get_shape_id() const;
  
  void update(const float &dt) override;
  void render() override;
  
  void impulse(const sf::Vector2f& i);
  void teleport(const sf::Vector2f& v);
  void dampen(const sf::Vector2f& s);

  void create_box_shape(const sf::Vector2f& size);
  void create_capsule_shape(const sf::Vector2f& size);
  
  void set_velocity(const sf::Vector2f& v);
  int get_contacts(std::array<b2ContactData,10>& contacts) const;
  const sf::Vector2f get_velocity() const;
  void set_restitution(float r);
  void set_friction(float r);
  void set_mass(float m);

  ~PhysicsComponent() override;
protected:
  b2BodyId _body_id;
  b2ShapeId _shape_id;
  const bool _dynamic;
};
```

#### Movement Gotcha

Take a look at the update():

```cpp
//physics_cmp.cpp
void PhysicsComponent::update(const float &dt) {
    _parent->set_position(ph::invert_height(ph::bv2_to_sv2(b2Body_GetPosition(_body_id)),
                                            param::game_height));
    _parent->set_rotation((180 / M_PI) * b2Rot_GetAngle(b2Body_GetRotation(_body_id)));
}
```

Small thing, M_PI is a constant representing pi which needs to be define somewhere.
I suggest to put it in the *game_parameters.hpp* file.
```cpp
//game_parameters.hpp
#define M_PI 3.14159265358979323846
```
Why not defining it as static constexpr in the `Parameters` structure? You may ask yourself. Well, pi is not a parameter per se. You would not change the value of pi. `#define` is the "old school" C-style way to define `static constexpr` variable. It is a low-level and very efficient way to define constant values.

This is how the physics world is linked to the SFML/Entity world. This happens every frame. There is one huge problem with this:
-   The PhysicsComponent is now in charge of the Entities position.
-   If anything modifies the Entities position (i.e another component), the physics component will overwrite this change with it's own value copied form the physics world.

We *can* still set the position of entities manually, by calling `PhysicsComponent.teleport()`, but we have to *know* that an Entity has a physics object beforehand. 

```cpp
//physics_cmp.cpp
void PhysicsComponent::teleport(const sf::Vector2f& v) {
    b2CosSin cos_sin = b2ComputeCosSin(0.0f);
    b2Rot rot;
    rot.c = cos_sin.cosine;
    rot.s = cos_sin.sine;
    b2Body_SetTransform(_body_id,ph::sv2_to_bv2(ph::invert_height(v,param::game_height)), rot);
}
```

Moving the entity will be done more by using `impulse()`, which will give a impulse to our rigid body like we did in the previous lab.
```cpp
//physics_cmp.cpp
void PhysicsComponent::impulse(const sf::Vector2f& i) {
    b2Vec2 a;
    a.x = i.x;
    a.y = i.y*-1.f;
    b2Body_ApplyLinearImpulseToCenter(_body_id,a, true);
    auto vel = b2Body_GetLinearVelocity(_body_id);
}

void PhysicsComponent::dampen(const sf::Vector2f& i) {
    auto vel = b2Body_GetLinearVelocity(_body_id);
    vel.x *= i.x;
    vel.y *= i.y;
    b2Body_SetLinearVelocity(_body_id,vel);
}
```
The `dampen` fucntion will be used to progressively decrease the velocity of our object if no impulse is inputted. 

### Fixture Shape

We need to give a shape to our rigid body. Generally, good shape for characters for collision is a capsule shape but you could use also a box shape or a circle shape depending of what kind of dynamics you want.
```cpp
void PhysicsComponent::create_capsule_shape(const sf::Vector2f& size,float mass,float friction, float restitution){
    _mass = mass;
  _friction = friction;
  _restitution = restitution;  
  //Create the fixture shape
    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.density = _dynamic ? _mass : 0.f;
    shape_def.material.friction =   _friction;
    shape_def.material.restitution = _restitution;
    b2Vec2 b2_size = ph::sv2_to_bv2(size);
    b2Capsule capsule;
    capsule.center1 = {0,b2_size.y*0.5f-b2_size.x*0.5f};
    capsule.center2 = {0,-b2_size.y*0.5f+b2_size.x*0.5f};
    capsule.radius = b2_size.x*0.5f;
    _shape_id = b2CreateCapsuleShape(_body_id,&shape_def,&capsule);
}
```
I leave it to you to figure the implementation of the `create_box_shape` function. Optionally, you can also implement a `create_circle_shape` function. It could useful later.

### Player Physics Component
Inheriting from PhysicsComponent, the PlayerPhysicsComponent is what drives the player.

```cpp
//physics_cmp.hpp
class PlayerPhysicsComponent : public PhysicsComponent {
public:
  void update(const float &dt) override;
  explicit PlayerPhysicsComponent(Entity* p,const sf::Vector2f& size);
  PlayerPhysicsComponent() = delete;

protected:
  b2Vec2 _size;
  sf::Vector2f _max_velocity;
  bool _grounded;
  float _ground_speed;
  bool is_grounded() const;
};

```
-   A constructor - Set's some relevant physics flags, nothing fancy
-   An update()
-   bool is_grounded() - detect if the player is standing on something.

#### The Player Update

As described in the lectures, players characters usually don't obey the rules of physics. They *look* like they do, but they cheat and bend physics to make the game *feel* responsive and fast.

See: [Tommy Refenes' on the physics of Super Meat Boy](https://youtu.be/QVpSIdWE0do?t=43m55s)

To make the player *feel* right, we do several things:
-   Dampen X Movement if not moving left or right (essentially apply handbrake to player if no keys are pressed)
-   Apply impulse to move left or right, only if currently going slower than a max velocity.
-   Jump, only if on the ground.
-   Kill all Y velocity at the start of the jump
-   Jump teleports slightly upwards first, then applies an impulse
-   If not on the ground - player has no friction
-   After everything - clamp velocity to a maximum value

```cpp
//physics_cmp.cpp
void PlayerPhysicsComponent::update(const float &dt) {
    const sf::Vector2f pos = _parent->get_position();
    b2Vec2 b2_pos = ph::sv2_to_bv2(ph::invert_height(pos,param::game_height));

        //Teleport to start if we fall off map.
    if (pos.y > ls::get_height() * param::tile_size) {
        teleport(ls::get_start_position());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        // Moving Either Left or Right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            impulse({(dt * _ground_speed), 0});
        } else {
            impulse({-(dt * _ground_speed), 0});
        }
    } else {
        // Dampen X axis movement
        dampen({0.9f, 1.0f});
    }

    // Handle Jump
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        _grounded = is_grounded();
        if (_grounded) {
            set_velocity(sf::Vector2f(get_velocity().x, 0.f));
            teleport(sf::Vector2f(pos.x, pos.y - 2.0f));
            impulse(sf::Vector2f(0, -param::player_jump));
        }
    }
    //Are we in air?
    if (!_grounded) {
        // Check to see if we have landed yet
        _grounded = is_grounded();
        // disable friction while jumping
        set_friction(0.f);
    } else {
        set_friction(_friction);
    }

    // Clamp velocity.
    sf::Vector2f v = get_velocity();
    v.x = copysign(std::min(abs(v.x), _max_velocity.x), v.x);
    v.y = copysign(std::min(abs(v.y), _max_velocity.y), v.y);
    set_velocity(v);

    PhysicsComponent::update(dt);
}
```

Now, we need to implement the is_grounded function:
```cpp
//physics_cmp.cpp
...
int PhysicsComponent::get_contacts(std::array<b2ContactData, 10> &contacts) const {
    int contact_count = b2Body_GetContactData(_body_id,contacts.data(),10);
    return contact_count;
}
...
bool PlayerPhysicsComponent::is_grounded() const {
    std::array<b2ContactData,10> contacts;
    int count = get_contacts(contacts);
    if(count <= 0) // there is no contact so return false
        return false;
    const b2Vec2& pos = b2Body_GetPosition(_body_id);
    const float half_y = _size.y * .5f;
    for (int i = 0; i < count; i++) {
        if(contacts[i].manifold.normal.y == 1) //the contact normal is upward so the player is on a ground.
            return true;
    }

    return false;
}

```
To work, `is_grounded` needs to access all the current contact apply to the rigid body of the player. For that, we implement the `get_contact` function which uses a handy function from box2d, look at the API for more info. So, in `is_grounded`, if there are some contacts, we check if at least of this contact has an upward normal. 

None of these things are an industry standard, it a method that I've adopted after doing this a few times. There are better ways, but this way *works*, but may not work for your game.


## Scenes

For this game, we need at least two scenes: `MenuScene` and `LevelScene`. You could also define one scene per level but I'll show here the solution with one level scene.

```cpp
//scenes.hpp
class LevelScene: public Scene{
public:
    LevelScene() = default;
    void update(const float &dt) override;
    void render() override;
    void load()override;
    void unload() override;
private:
    std::shared_ptr<Entity> _player;
    std::vector<std::shared_ptr<Entity>> _walls;
    void _load_level(const std::string &file_path);
};
```

```cpp
//scenes.cpp
void LevelScene::_load_level(const std::string &file_path){
    //load the level
    ls::load_level(file_path,param::tile_size);

    //create a player
    _player = make_entity();
    _player->set_position(ls::get_start_position());

    std::shared_ptr<ShapeComponent> shape = _player->add_component<ShapeComponent>();
    shape->set_shape<sf::RectangleShape>(sf::Vector2f(param::player_size[0],param::player_size[1]));
    shape->get_shape().setFillColor(sf::Color::Yellow);
    shape->get_shape().setOrigin(sf::Vector2f(param::player_size[0]/2.f,param::player_size[1]/2.f));
    std::shared_ptr<PlayerPhysicsComponent> cmp = _player->add_component<PlayerPhysicsComponent>(sf::Vector2f(param::player_size[0],param::player_size[1]));
    cmp->create_capsule_shape(sf::Vector2f(param::player_size[0],param::player_size[1]),
      param::player_weight,param::player_friction,param::player_restitution);
    
    //create the rigid body for each tiles
    std::vector<sf::Vector2i> walls = ls::find_tiles(ls::WALL);
    for (const sf::Vector2i &w : walls) {
      std::shared_ptr<Entity> e = make_entity();
      e->set_position({ls::get_tile_position(w).x+param::tile_size/2.f,
                      ls::get_tile_position(w).y+param::tile_size/2.f});
      e->add_component<PlatformComponent>(sf::Vector2f(param::tile_size, param::tile_size));
    }

}
```

In the above implementation, we use a new helper function from Scenes: `make_entity` which:
- create an entity
- add it to the EntityManager
- return the pointer to the newly created entity. 

You should add this method to your Scene class in the engine library

The update is nothing new. From the classic update the scene and entities, we add a condition to switch scenes.

```cpp
void LevelScene::update(const float &dt){
    Scene::update(dt);
    _entities.update(dt);
    if(ls::get_tile_at(_player->get_position()) == ls::END){ //if the player reaches the end of the level switch to the next level.
        unload();
        _load_level(param::level_2);
    }
}
```


The `MenuScene` is pretty much the same as in pacman. 

Also, as in pacman we need to define and declare our scenes:
```cpp
//scenes.hpp
struct Scenes{
    static std::shared_ptr<Scene> menu;
    static std::shared_ptr<Scene> level;
};  
```
```cpp
//scenes.cpp
std::shared_ptr<Scene> Scenes::menu;
std::shared_ptr<Scene> Scenes::level;
```

## The Main

The main function is similar to the two previous lab with the only difference that we need to initialise the physics before everything and shut it down after everything. 
```cpp
int main(){
    Physics::initialise();
    Scenes::menu = std::make_shared<MenuScene>();
    Scenes::menu->load();
    Scenes::level = std::make_shared<LevelScene>();
    Scenes::level->load();
    GameSystem::set_active_scene(Scenes::menu);
    GameSystem::start(param::game_width,param::game_height,"Platformer",Physics::time_step,true);//true for physics enabled.
    Physics::shutdown();
    return 0;
}
```

Also, the gamesystem is slightly modified to incorporate the physics update and a flag to indicate if the physics is enabled. I let you find what to modify, there is not much.

## Run The Scene

You shouldn't need to edit anything to get the Menu and first level running. Give it a go and bounce around.

Well, now you should notice some weird collisions when the player is just running on a flat ground. You can load *level_0.txt* to have a good feel of it. There are ghost collision due to the fact that our ground is made of multiple boxes. You could consider it as being a small detail but this kind *small* details can ruin a game. So, next part of the lab is about solving this issue.


Previous step: [Physics](physics)

Next step: [Platformer 2](platformer2)