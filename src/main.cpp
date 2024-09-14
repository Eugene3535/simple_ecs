#include <vector>
#include <iostream>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <chrono>
#include <string>

struct Position
{
    float x = 0, y = 0;
};

struct Velocity
{
    float x = 1, y = 1;
};

struct Animation
{
    int frame = 0;
    float dt = 0.016f;
};

#define COMPONENT_UNDEFINED -1

struct ComponentTable
{
    int position = COMPONENT_UNDEFINED;
    int velocity = COMPONENT_UNDEFINED;
    int animation = COMPONENT_UNDEFINED;
    int name = COMPONENT_UNDEFINED;
};

std::vector<int> entities;
std::vector<ComponentTable> tables;

using PositionContainer = std::vector<std::pair<int, Position>>;
using VelocityContainer = std::vector<std::pair<int, Velocity>>;
using AnimationContainer = std::vector<std::pair<int, Animation>>;
using NameContainer = std::vector<std::pair<int, std::string>>;

PositionContainer positions;
VelocityContainer velocities;
AnimationContainer animations;
NameContainer names;

int create_entity()
{
    int i = entities.size();
    entities.push_back(i);
    tables.emplace_back();

    return i;
}

template<class T, class... Args>
T* add_component(int entity, Args&&... args)
{
    auto& table = tables[entity];

    if constexpr(std::is_same_v<T, Position>)
    {
        if (table.position == COMPONENT_UNDEFINED)
        {
            table.position = positions.size();
            auto& pair = positions.emplace_back();
            pair.first = entity;

            return &pair.second;
        }
    }
    else if constexpr(std::is_same_v<T, Velocity>)
    {
        if (table.velocity == COMPONENT_UNDEFINED)
        {
            table.velocity = velocities.size();
            auto& pair = velocities.emplace_back();
            pair.first = entity;

            return &pair.second;
        }
    }
    else if constexpr(std::is_same_v<T, Animation>)
    {
        if (table.animation == COMPONENT_UNDEFINED)
        {
            table.animation = animations.size();
            auto& pair = animations.emplace_back();
            pair.first = entity;

            return &pair.second;
        }
    }
    else if constexpr(std::is_same_v<T, std::string>)
    {
        if (table.name == COMPONENT_UNDEFINED)
        {
            table.name = names.size();
            auto& pair = names.emplace_back(entity, args...);
            pair.first = entity;

            return &pair.second;
        }
    }

    return nullptr;
}

template<class T>
T* get_component(int entity)
{
    auto& table = tables[entity];

    if constexpr (std::is_same_v<T, Position>)
    {
        if (int pos = table.position; pos != COMPONENT_UNDEFINED)
            return &positions[pos].second;
    }
    else if constexpr (std::is_same_v<T, Velocity>)
    {
        if (int vel = table.velocity; vel != COMPONENT_UNDEFINED)
            return &velocities[vel].second;
    }
    else if constexpr (std::is_same_v<T, Animation>)
    {
        if (int anim = table.animation; anim != COMPONENT_UNDEFINED)
            return &animations[anim].second;
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        if (int name = table.name; name != COMPONENT_UNDEFINED)
            return &names[name].second;
    }

    return nullptr;
}

template<class T>
void remove_component_from_container(int i, T& container)
{
    auto& pos = container[i];
    auto& back = container.back();

    if constexpr (std::is_same_v<T, Position>)
    {
        tables[back.first].position = i;
    }
    else if constexpr (std::is_same_v<T, Velocity>)
    {
        tables[back.first].velocity = i;
    }
    else if constexpr (std::is_same_v<T, Animation>)
    {
        tables[back.first].animation = i;
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        tables[back.first].name = i;
    }
    
    std::swap(pos, back);
    container.pop_back();
}

template<class T>
void remove_component(int entity)
{
    auto& table = tables[entity];

    if constexpr (std::is_same_v<T, Position>)
    {
        if (int i = table.position; i != COMPONENT_UNDEFINED)
        {
            remove_component_from_container<PositionContainer>(i, positions);
            table.position = COMPONENT_UNDEFINED;
        }
    }
    else if constexpr (std::is_same_v<T, Velocity>)
    {
        if (int i = table.velocity; i != COMPONENT_UNDEFINED)
        {
            remove_component_from_container<VelocityContainer>(i, velocities);
            table.velocity = COMPONENT_UNDEFINED;
        }
    }
    else if constexpr (std::is_same_v<T, Animation>)
    {
        if (int i = table.animation; i != COMPONENT_UNDEFINED)
        {
            remove_component_from_container<AnimationContainer>(i, animations);
            table.animation = COMPONENT_UNDEFINED;
        }
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        if (int i = table.name; i != COMPONENT_UNDEFINED)
        {
            remove_component_from_container<NameContainer>(i, names);
            table.name = COMPONENT_UNDEFINED;
        }
    }
}

void destroy_entity(int entity)
{
    remove_component<Position>(entity);
    remove_component<Velocity>(entity);
    remove_component<Animation>(entity);
    remove_component<std::string>(entity);
    
    entities[entity] = entities.back();
    std::swap(tables[entity], tables.back());

    entities.pop_back();
    tables.pop_back();
}

void move_system()
{
    for (auto& pair : velocities)
    {
        auto& entity_table = tables[pair.first];
        auto& velocity = pair.second;
        auto& position = positions[entity_table.position].second;

        position.x += velocity.x;
        position.y += velocity.y;
    }
}

void anim_system(float dt)
{
    for (auto& pair : animations)
    {
        auto& anim = pair.second;
        anim.dt += dt;
        anim.frame = anim.dt;
    }
}

int main() 
{
    const size_t nbEntities = 100000;
    const size_t nbUpdates = 100;

    for (size_t i = 0; i < nbEntities; ++i)
    {
        int entity = create_entity();
        add_component<Position>(entity);
        add_component<Velocity>(entity);

        if (i % 2 == 0)
            add_component<Animation>(entity);

        if (i % 10000 == 0)
            add_component<std::string>(i, "name - " + std::to_string(i));
    }

    for (size_t i = 0; i < nbEntities; ++i)
        if(auto* name = get_component<std::string>(i); name != nullptr)
            std::cout << *name << '\n';

    auto prevTime = std::chrono::system_clock::now();
    auto start = std::chrono::system_clock::now();

    for (size_t i = 0; i < nbUpdates; ++i)
    {
        auto time = std::chrono::system_clock::now();
        auto dt = std::chrono::duration<float>(time - prevTime).count();
        prevTime = time;

        if (i % 2 == 0)
        {
            if (auto anim = get_component<Animation>(i); anim != nullptr)
            {
                remove_component<Animation>(i);
                //std::cout << "removed animation " << i << '\n';
            }
        }

        move_system();
        anim_system(dt);
    }

    std::chrono::duration<float> dur = std::chrono::system_clock::now() - start;
    std::cout << "time native: " << dur.count() * 1000 << " seconds\n";

    return 0;
}