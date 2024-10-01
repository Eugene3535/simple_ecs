#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
#include <chrono>
#include <string>

#include "EntityManager.hpp"

class System
{
public:
    System(EntityManager& entityManager) noexcept:
        m_entityManager(entityManager)
    {

    }

    virtual ~System() = default;

    virtual void execute(float delta_time) noexcept
    {
        // empty body
    }

protected:
    EntityManager& m_entityManager;
};

class MovementSystem: public System
{
    using Velocities = std::vector<std::pair<uint32_t, Velocity>>;
    using Positions = std::vector<std::pair<uint32_t, Position>>;

public:
    MovementSystem(EntityManager& entityManager) noexcept:
        System(entityManager),
        m_positions(nullptr),
        m_velocities(nullptr)
    {
        m_velocities = entityManager.getComponentMap().getContainer<Velocity>();
        m_positions = entityManager.getComponentMap().getContainer<Position>();
    }

    void execute(float delta_time) noexcept override
    {
        for (auto& pair : *m_velocities)
        {
            auto& entity_table = m_entityManager.getEntityTables()[pair.first];
            auto& velocity = pair.second;
            auto& position = (*m_positions)[entity_table.position].second;

            position.x += velocity.x;
            position.y += velocity.y;
        }
    }

private:
    Velocities* m_velocities;
    Positions* m_positions;
};

class AnimationSystem: public System
{
    using Animations = std::vector<std::pair<uint32_t, Animation>>;

public:
    AnimationSystem(EntityManager& entityManager) noexcept:
        System(entityManager),
        m_animations(nullptr)
    {
        m_animations = entityManager.getComponentMap().getContainer<Animation>();
    }

    void execute(float delta_time) noexcept override
    {
        for (auto& pair : *m_animations)
        {
            auto& anim = pair.second;
            anim.dt += delta_time;
            anim.frame = anim.dt;
        }
    }

private:
    Animations* m_animations;
};

int main() 
{
    EntityManager entityManager;
    MovementSystem move_system(entityManager);
    AnimationSystem anim_system(entityManager);

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

        move_system.execute(dt);
        anim_system.execute(dt);
    }

    std::chrono::duration<float> dur = std::chrono::system_clock::now() - start;
    std::cout << "time native: " << dur.count() * 1000 << " seconds\n";

    return 0;
}