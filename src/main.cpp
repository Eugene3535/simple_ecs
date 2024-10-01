#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
#include <chrono>
#include <string>

#include "EntityManager.hpp"

void move_system(EntityManager& entityManager)
{
    auto velocities = entityManager.m_componentMap.getContainer<Velocity>();
    auto positions = entityManager.m_componentMap.getContainer<Position>();

    for (auto& pair : *velocities)
    {
        auto& entity_table = entityManager.m_tables[pair.first];
        auto& velocity = pair.second;
        auto& position = (*positions)[entity_table.position].second;

        position.x += velocity.x;
        position.y += velocity.y;
    }
}

void anim_system(EntityManager& entityManager, float dt)
{
    auto animations = entityManager.m_componentMap.getContainer<Animation>();

    for (auto& pair : *animations)
    {
        auto& anim = pair.second;
        anim.dt += dt;
        anim.frame = anim.dt;
    }
}

int main() 
{
    EntityManager entityManager;

    const size_t nbEntities = 100000;
    const size_t nbUpdates = 100;

    for (size_t i = 0; i < nbEntities; ++i)
    {
        uint32_t entity = entityManager.createEntity();
        entityManager.addComponent<Position>(entity);
        entityManager.addComponent<Velocity>(entity);

        if (i % 2 == 0)
            entityManager.addComponent<Animation>(entity);
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
            if (auto anim = entityManager.getComponent<Animation>(i); anim != nullptr)
            {
                entityManager.removeComponent<Animation>(i);
            }
        }

        move_system(entityManager);
        anim_system(entityManager, dt);
    }

    std::chrono::duration<float> dur = std::chrono::system_clock::now() - start;
    std::cout << "time native: " << dur.count() * 1000 << " seconds\n";

    return 0;
}