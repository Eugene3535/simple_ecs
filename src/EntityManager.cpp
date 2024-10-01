#include <algorithm>

#include "EntityManager.hpp"

EntityManager::EntityManager() noexcept
{
    m_componentMap.pushContainer<Position>();
    m_componentMap.pushContainer<Velocity>();
    m_componentMap.pushContainer<Animation>();
}

uint32_t EntityManager::createEntity() noexcept
{
    int32_t entity = static_cast<uint32_t>(m_entities.size());
    m_entities.push_back(entity);
    m_tables.emplace_back();

    return entity;
}

void EntityManager::destroyEntity(uint32_t entity) noexcept
{
    removeComponent<Position>(entity);
    removeComponent<Velocity>(entity);
    removeComponent<Animation>(entity);
    
    m_entities[static_cast<size_t>(entity)] = m_entities.back();
    std::swap(m_tables[static_cast<size_t>(entity)], m_tables.back());

    m_entities.pop_back();
    m_tables.pop_back();
}