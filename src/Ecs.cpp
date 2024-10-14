#include "Ecs.hpp"

Ecs::Ecs() noexcept
{

}

Ecs::~Ecs() noexcept = default;

int Ecs::createEntity() noexcept
{
    int i = m_entities.size();
    m_entities.push_back(i);
    m_tables.emplace_back();

    return i;
}

void Ecs::destroyEntity(int entity) noexcept
{
    removeComponent<Position>(entity);
    removeComponent<Velocity>(entity);
    removeComponent<Animation>(entity);

    m_entities[entity] = m_entities.back();
    std::swap(m_tables[entity], m_tables.back());

    m_entities.pop_back();
    m_tables.pop_back();
}

void Ecs::moveSystem(float dt) noexcept
{
    for (const auto& pair : m_velocities)
    {
        auto& entity_table = m_tables[pair.first];
        auto& velocity = pair.second;
        auto& position = m_positions[entity_table.position].second;

        position.x += velocity.dx;
        position.y += velocity.dy;
    }
}

void Ecs::animSystem(float dt) noexcept
{
    for (auto& pair : m_animations)
    {
        auto& anim = pair.second;
        anim.dt += dt;
        anim.frame = anim.dt;
    }
}
