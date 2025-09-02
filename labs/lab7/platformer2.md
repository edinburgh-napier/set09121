To solve this issue of *ghost collisions*, we are going to change the PlatformComponent class collision system. For this to work we will need to add some functions in the level system library. 

## The Theory

The main idea is to group contiguous tiles to create one physics body per group. Doing this will increase the performance by reducing the number of rigid bodies and produce cleaner collisions.

So if we look at our first level, we will get the following groups.
![image](assets/images/level_1_grouping.jpg)

After, grouping we need to create a body and a fixture shape for each group. However, a simple box shape won't do here as you can see. a solution would be to subdivide further the groups into rectangle groups. It is a legit solution. Another way to solve this is to chain shapes provided by box2d. Chain shape are perfect to create the static world around the player. It is also more general but a bit more tricky to use. 

Now, have a look at this [page](https://box2d.org/documentation/md_simulation.html#autotoc_md80) of the box2d documentation about chain shape. Try to have a good understanding how they work. In our case, we need to create a list of points that will be the vertices of our chain, they need to be ordered in counter clock wise order, and form a loop. 

The tricky part here is the clockwise sorting. If the shape formed by our group of tile is too complex sorting 2d points in clockwise is difficult. So an extra step will be to form groups that are "simple".
A possible grouping could look like this:
![image](assets/images/level_2_grouping.jpg)



## The implementation

### Step 1: Grouping

Let's go back to our level system library. We will creat two new functions: `get_groups` and `_get_group`. 

```cpp
//level_system.hpp
public:
...
static std::vector<std::vector<sf::Vector2i>> get_groups(Tile type); //get groups of tiles of a specific type.
...
protected:
...
//get one group of type starting at pos and being in a tile_list. 
static void _get_group(Tile type,const sf::Vector2i &pos,
    const std::vector<sf::Vector2i> &tile_list,
    std::vector<sf::Vector2i>& group,bool vert);
```

`get_groups` will use the `_get_group` to build each individual group. Here is the implementation with comments for each step. Take some time to understand it.
```cpp
//level_system.cpp
std::vector<std::vector<sf::Vector2i>> LevelSystem::get_groups(Tile type){
    std::vector<std::vector<sf::Vector2i>> groups;
    std::vector<sf::Vector2i> tile_list = find_tiles(type); //find all the tiles of type
    while(!tile_list.empty()){ //until tile_list is empty continue to form groups
        std::vector<sf::Vector2i> group;
        if(tile_list.size() == 1) //if there is only one tile left just add it in its one group.
            group.push_back(tile_list[0]);
        else
            _get_group(type,tile_list.front(),tile_list,group,true); //form the next group
        groups.push_back(group); //add the new group in our list of groups

        //remove all the tile in the last group created from the tile list.
        for(sf::Vector2i pos: group){
            int i = 0;
            for(; i < tile_list.size(); i++)
                if(tile_list[i] == pos)
                    break;
            tile_list.erase(tile_list.begin() + i);
        }
    }
    return groups;
}
```

This function is fairly simple. The complexity resides in `_get_group`. `_get_group` is a recursive function (a function that calls itself).  The idea is to only consider the neighbors around the tile at pos. 

```cpp
//level_system.cpp
void LevelSystem::_get_group(Tile type,const sf::Vector2i &pos,const std::vector<sf::Vector2i> &tile_list, std::vector<sf::Vector2i> &group,bool vert){
    if(in_group(pos,group)) //if this tile is already in the group stop here
        return;
    group.push_back(pos);// add the current tile in the group.

    //query the type of all neighbors.
    std::vector<Tile> neighbors = {
        in_group({pos.x-1,pos.y-1},tile_list) ? get_tile({pos.x-1,pos.y-1}) : EMPTY, //upper left
        in_group({pos.x,pos.y-1},tile_list) ? get_tile({pos.x,pos.y-1}) : EMPTY, //up
        in_group({pos.x+1,pos.y-1},tile_list) ? get_tile({pos.x+1,pos.y-1}) : EMPTY, //upper right
        in_group({pos.x+1,pos.y},tile_list) ? get_tile({pos.x+1,pos.y}) : EMPTY, //right
        in_group({pos.x+1,pos.y+1},tile_list) ? get_tile({pos.x+1,pos.y+1}) : EMPTY, // lower right
        in_group({pos.x,pos.y+1},tile_list) ? get_tile({pos.x,pos.y+1}) : EMPTY,// down
        in_group({pos.x-1,pos.y+1},tile_list) ? get_tile({pos.x-1,pos.y+1}) : EMPTY,//lower left
        in_group({pos.x-1,pos.y},tile_list) ? get_tile({pos.x-1,pos.y}) : EMPTY // left
    };

    if(neighbors[3] == type) //If the right neighbor is of type then call _get_group with this neighbor as current tile.
        _get_group(type,{pos.x+1,pos.y},tile_list,group,
                   neighbors[3] == neighbors[4] && neighbors[7] == neighbors[6] 
                   && neighbors[3] == neighbors[2] && neighbors[7] == neighbors[0]);
    if(neighbors[7] == type) //same for the left
        _get_group(type,{pos.x-1,pos.y},tile_list,group,
                   neighbors[3] == neighbors[4] && neighbors[7] == neighbors[6] 
                   && neighbors[3] == neighbors[2] && neighbors[7] == neighbors[0]);
    if(!vert) //vert is false don't look at the upper and lower neighboors
        return;
    if(neighbors[3] == neighbors[4] && neighbors[7] == neighbors[6] && neighbors[5] == type) //look down
        _get_group(type,{pos.x,pos.y+1},tile_list,group,true);
    if(neighbors[3] == neighbors[2] && neighbors[7] == neighbors[0] && neighbors[1] == type)//look up
        _get_group(type,{pos.x,pos.y-1},tile_list,group,true);
}
```

The main subtility of this algorithm is the priority given to horizontal grouping over vertical. As it is a platformer, we will tend to group the tiles per line and group the line if their of similar number. This is done by the boolean conditions comparing the neighbors. If you don't get it that's normal. Do some drawing with the different possible configurations of the eight neighbors to understand the conditions.

This one way to do it. You can use my solution or come up with a (better) solution of your own.

### Step 2: Creating the chain shape.

First, we will modify the definition of the `PlatformComponent` class by adding a new method `_create_chain_shape` and replacing the `_shape_id` by a `_chain_id`.

```cpp
//physics_cmp.hpp
class PlatformComponent : public Component{
  ...
  protected:
    ...
    b2ChainId _chain_id;
    void _create_chain_shape(const std::vector<sf::Vector2i> &tile_group);
};
```
`_create_chain_shape` takes as argument a group of tiles and will create the chain shape from it. The function is quite long and complicated. So, take some time to read it through and understand it.
```cpp
//physics_cmp.cpp
void PlatformComponent::_create_chain_shape(const std::vector<sf::Vector2i> &tile_group){
    // --- Step 1: Create the list of points that will be the vertices of the chain.
    std::vector<b2Vec2> points;
    for(int i = 0; i < tile_group.size(); i++){
        const sf::Vector2i &tile = tile_group[i];
        //query the type of all neighbors.
        std::vector<ls::Tile> neighbors = {
            ls::in_group({tile.x-1,tile.y-1},tile_group) ? ls::get_tile({tile.x-1,tile.y-1}) : ls::EMPTY,
            ls::in_group({tile.x,tile.y-1},tile_group) ? ls::get_tile({tile.x,tile.y-1}) : ls::EMPTY,
            ls::in_group({tile.x+1,tile.y-1},tile_group) ? ls::get_tile({tile.x+1,tile.y-1}) : ls::EMPTY,
            ls::in_group({tile.x+1,tile.y},tile_group) ? ls::get_tile({tile.x+1,tile.y}) : ls::EMPTY,
            ls::in_group({tile.x+1,tile.y+1},tile_group) ? ls::get_tile({tile.x+1,tile.y+1}) : ls::EMPTY,
            ls::in_group({tile.x,tile.y+1},tile_group) ? ls::get_tile({tile.x,tile.y+1}) : ls::EMPTY,
            ls::in_group({tile.x-1,tile.y+1},tile_group) ? ls::get_tile({tile.x-1,tile.y+1}) : ls::EMPTY,
            ls::in_group({tile.x-1,tile.y},tile_group) ? ls::get_tile({tile.x-1,tile.y}) : ls::EMPTY
        };
        sf::Vector2f pos = ls::get_tile_position(tile);

        //create the list of points corresponding to the corner of the tile which are a the edge of the group.
        std::vector<sf::Vector2f> pts;
        if(neighbors[0] == ls::EMPTY || neighbors[1] == ls::EMPTY || neighbors[7] == ls::EMPTY)
            pts.push_back(pos);
        if(neighbors[1] == ls::EMPTY || neighbors[2] == ls::EMPTY || neighbors[3] == ls::EMPTY)
            pts.push_back({pos.x+param::tile_size,pos.y});
        if(neighbors[3] == ls::EMPTY || neighbors[4] == ls::EMPTY || neighbors[5] == ls::EMPTY)
            pts.push_back({pos.x+param::tile_size,pos.y+param::tile_size});
        if(neighbors[5] == ls::EMPTY || neighbors[6] == ls::EMPTY || neighbors[7] == ls::EMPTY)
            pts.push_back({pos.x,pos.y+param::tile_size});

        //add these points to the list of points
        for(const sf::Vector2f &pt: pts){
            b2Vec2 point = ph::sv2_to_bv2(ph::invert_height(pt,param::game_height));
            bool already_in = false;
            for(const b2Vec2 &p : points){
                if(p.x == point.x && p.y == point.y){//if this point is already in the list don't add it.
                    already_in = true;
                    break;
                }
            }
            if(!already_in)
                points.push_back(point);
        }
    }

    // --- Step 2: Sorting the list points in counter clockwise order.
    //compute the centroid of the points.
    b2Vec2 centroid = {0,0};
    for(const b2Vec2 pt: points){
        centroid.x += pt.x;
        centroid.y += pt.y;
    }
    centroid.x /= static_cast<float>(points.size());
    centroid.y /= static_cast<float>(points.size());

    //order the list of points in counter clockwise using polar coordinates with the centroid as origin.
    std::sort(points.begin(),points.end(),[&](b2Vec2 a, b2Vec2 b){
        a = {a.x-centroid.x,a.y-centroid.y};
        b = {b.x-centroid.x,b.y-centroid.y};
        float angle1 = std::atan2(a.x,a.y);
        float angle2 = std::atan2(b.x,b.y);
        if(angle1==angle2)
            return std::sqrt(a.x*a.x+a.y*a.y)>std::sqrt(b.x*b.x+b.y*b.y);
        else
            return angle1>angle2;
    });
    points.push_back(points.front());

    // --- Step 3: Create the chain shape. Look at the box2d API for more information.
    b2SurfaceMaterial material = b2DefaultSurfaceMaterial();
    material.friction = _friction;
    material.restitution = _restitution;
    b2ChainDef chain_def = b2DefaultChainDef();
    chain_def.count = points.size();
    chain_def.points = points.data();
    chain_def.isLoop = true;
    chain_def.materials = &material;
    chain_def.materialCount = 1;
    _chain_id = b2CreateChain(_body_id,&chain_def);
    std::vector<b2ShapeId> shape_ids(points.size());
    int nbr_seg = b2Chain_GetSegments(_chain_id,shape_ids.data(),points.size());
    shape_ids.size();
}

```
Now, you just need to add the _create_chain_shape function to the constructor of the PlatformComponent. You will need also to modify the constructor definition to have as argument a list of tiles coordinates corresponding to a group.

## Step 3: Create an entity for each group

Back to our `LevelScene`, let's replace the previous section creating a box shape for each tile by creating one entity per group and a chain shape associated to it.
```cpp
//scenes.cpp
void LevelScene::_load_level(const std::string &file_path){
    ...
    std::vector<std::vector<sf::Vector2i>> wall_groups = ls::get_groups(ls::WALL);
    for (const std::vector<sf::Vector2i> &walls : wall_groups) {
        _walls.push_back(make_entity());
        _walls.back()->add_component<PlatformComponent>(walls);
    }
}
```

Now, with this implementation you should get the intended behaviour. No more ghost collision!

## Phew!

Look at that, you've now going a working game, with multiple levels. Cool, huh? Remember: all of what is here is designed to be modified, broken, fixed, used, altered, changed, tweaked, and generally messed around with. Want a different type of movement for your game? Go add it then! Want new physics objects? Well, you can use Box2D can't ya?

The ball really is in your court from now on: go make something awesome!

## Advanced tasks

- Create a third level with moving enemies. You can derive the PhysicsComponent class to create an EnemyPhysicsComponent. Like we did for the player. For the behaviour of the enemy, just have a look at your favourite platformer and try to reimplement the most basic enemy behaviour.
- Create an ending scene. 
- Tweak the physics parameters to get your own platformer. As written at the very beginning, most of what make a platformer unique is its feeling when moving the player. A lot of it is in the physics parameters: player weight, friction, jumping power, restitution etc...
- Modify the player behaviour to be able to wall jump.
- Create a sprite component to use sprites instead of shapes. You can get some nice free sprites on the internet like [here](https://itch.io/game-assets/free).

Previous step: [Platformer](platformer)

Next step: [Steering](lab8_1_steering)