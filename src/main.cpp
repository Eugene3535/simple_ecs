#include <iostream>
#include <chrono>

#include "Ecs.hpp"

int main()
{
    const size_t nbEntities = 100000;
    const size_t nbUpdates = 100;

    Ecs ecs;

    for (size_t i = 0; i < nbEntities; ++i)
    {
        int entity = ecs.createEntity();
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

        ecs.moveSystem(dt);
        ecs.animSystem(dt);
    }

    std::chrono::duration<float> dur = std::chrono::system_clock::now() - start;
    std::cout << "time native: " << dur.count() * 1000 << " seconds\n";

    return 0;
}
