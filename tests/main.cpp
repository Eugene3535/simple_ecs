#include <cstdio>
#include <chrono>

#include "Ecs.hpp"

#define MAX_COMPONENTS 32
struct Position
{
    float x, y;
};

struct Velocity
{
    float x, y;
};

struct Animation
{
    float dt;
    int frames;
};

void moveSystem(Ecs<MAX_COMPONENTS>& ecs)
{
    auto velocities = ecs.getContainer<Velocity>();

    for (auto& pair : velocities)
    {
        uint32_t entity = pair.first;

        if(ecs.hasComponent<Position>(entity) && ecs.hasComponent<Velocity>(entity))
        {
            auto pos = ecs.getComponent<Position>(entity);
            auto& velocity = pair.second; 

            pos->x += velocity.x;
            pos->y += velocity.y;
        }
    }
}

void animSystem(Ecs<MAX_COMPONENTS>& ecs, float dt)
{
    auto anim = ecs.getContainer<Animation>();

    for(auto& a : anim)
    {
        a.second.dt += dt;

        if(a.second.dt > 1.0f)
        {
            a.second.dt = 0;
            a.second.frames++;
        }
    }
}

int main()
{
    const size_t nbEntities = 1000000;
    const size_t nbUpdates = 100;

    Ecs<MAX_COMPONENTS> ecs;

    for (size_t i = 0; i < nbEntities; ++i)
    {
        uint32_t entity = ecs.createEntity();
        ecs.addComponent<Position>(entity);
        ecs.addComponent<Velocity>(entity);

        if (i % 2 == 0)
            ecs.addComponent<Animation>(entity);
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
            if (auto anim = ecs.getComponent<Animation>(i); anim != nullptr)
            {
                ecs.removeComponent<Animation>(i);
            }
        }

        moveSystem(ecs);
        animSystem(ecs, dt);
    }

    for (uint32_t i = 0; i < nbEntities; ++i)
    {
        ecs.destroyEntity(i);
    }

    std::chrono::duration<float> dur = std::chrono::system_clock::now() - start;
    printf("time native: %f seconds\n", dur.count());

    return 0;
}
