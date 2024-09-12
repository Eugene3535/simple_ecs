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
};

std::vector<int> entities;
std::vector<ComponentTable> tables;

using PositionContainer = std::vector<std::pair<int, Position>>;
using VelocityContainer = std::vector<std::pair<int, Velocity>>;
using AnimationContainer = std::vector<std::pair<int, Animation>>;

PositionContainer positions;
VelocityContainer velocities;
AnimationContainer animations;


int create_entity()
{
    int i = entities.size();
    entities.push_back(i);
    tables.emplace_back();

    return i;
}

template<class T>
T* add_component(int entity)
{
    auto& table = tables[entity];

    if constexpr(std::is_same<T, Position>::value)
    {
        if (table.position == COMPONENT_UNDEFINED)
        {
            table.position = positions.size();
            auto& pair = positions.emplace_back();
            pair.first = entity;

            return &pair.second;
        }
    }
    else if constexpr(std::is_same<T, Velocity>::value)
    {
        if (table.velocity == COMPONENT_UNDEFINED)
        {
            table.velocity = velocities.size();
            auto& pair = velocities.emplace_back();
            pair.first = entity;

            return &pair.second;
        }
    }
    else if constexpr(std::is_same<T, Animation>::value)
    {
        if (table.animation == COMPONENT_UNDEFINED)
        {
            table.animation = animations.size();
            auto& pair = animations.emplace_back();
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

    if constexpr (std::is_same<T, Position>::value)
    {
        if (int pos = table.position; pos != COMPONENT_UNDEFINED)
            return &positions[pos].second;
    }
    else if constexpr (std::is_same<T, Velocity>::value)
    {
        if (int vel = table.velocity; vel != COMPONENT_UNDEFINED)
            return &velocities[vel].second;
    }
    else if constexpr (std::is_same<T, Animation>::value)
    {
        if (int anim = table.animation; anim != COMPONENT_UNDEFINED)
            return &animations[anim].second;
    }

    return nullptr;
}

template<class T>
void remove_component_from_container(int i, T& container)
{
    auto& pos = positions[i];
    auto& back = positions.back();

    if constexpr (std::is_same<T, Position>::value)
    {
        tables[back.first].position = i;
    }
    else if constexpr (std::is_same<T, Velocity>::value)
    {
        tables[back.first].velocity = i;
    }
    else if constexpr (std::is_same<T, Animation>::value)
    {
        tables[back.first].animation = i;
    }
    
    std::swap(pos, back);
    positions.pop_back();
}

template<class T>
void remove_component(int entity)
{
    auto& table = tables[entity];

    if constexpr (std::is_same<T, Position>::value)
    {
        if (int i = table.position; i != COMPONENT_UNDEFINED)
        {
            remove_component_from_container<PositionContainer>(i, positions);
            table.position = COMPONENT_UNDEFINED;
        }
    }
    else if constexpr (std::is_same<T, Velocity>::value)
    {
        if (int i = table.velocity; i != COMPONENT_UNDEFINED)
        {
            remove_component_from_container<VelocityContainer>(i, velocities);
            table.velocity = COMPONENT_UNDEFINED;
        }
    }
    else if constexpr (std::is_same<T, Animation>::value)
    {
        if (int i = table.animation; i != COMPONENT_UNDEFINED)
        {
            remove_component_from_container<AnimationContainer>(i, animations);
            table.animation = COMPONENT_UNDEFINED;
        }
    }
}

void destroy_entity(int entity)
{
    auto& table = tables[entity];

    if (int i = table.position; i != COMPONENT_UNDEFINED)
    {
        remove_component<Position>(entity);
    }

    if (int i = table.velocity; i != COMPONENT_UNDEFINED)
    {
        remove_component<Velocity>(entity);
    }

    if (int i = table.animation; i != COMPONENT_UNDEFINED)
    {
        remove_component<Animation>(entity);
    }

    entities[entity] = entities.back();
    std::swap(tables[entity], tables.back());

    entities.pop_back();
    tables.pop_back();
}

void move_system()
{
    for(auto i : entities)
    {
        auto pos = get_component<Position>(i);
        auto vel = get_component<Velocity>(i);

        if (pos && vel)
        {
            pos->x += vel->x;
            pos->y += vel->y;
        }
    }
}

void anim_system(float dt)
{
    for (auto i : entities)
    {
        auto anim = get_component<Animation>(i);

        if (anim)
        {
            anim->dt += dt;
            anim->frame = anim->dt;
        }
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
    }

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
                std::cout << "removed animation " << i << '\n';
            }
        }

        move_system();
        anim_system(dt);
    }

    std::chrono::duration<float> dur = std::chrono::system_clock::now() - start;
    std::cout << "time native: " << dur.count() * 1000 << " seconds\n";

    return 0;
}