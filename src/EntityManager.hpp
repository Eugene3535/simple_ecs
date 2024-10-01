#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "ComponentTable.hpp"
#include "ComponentMap.hpp"

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
    uint32_t frame = 0;
    float dt = 0.016f;
};

class EntityManager
{
public:
    EntityManager() noexcept;

    uint32_t createEntity()                 noexcept;
    void     destroyEntity(uint32_t entity) noexcept;

    template<class T>
    T* addComponent(uint32_t entity) noexcept;

    template<class T>
    T* getComponent(uint32_t entity) noexcept;

    template<class T>
    void removeComponent(uint32_t entity) noexcept;

    std::vector<uint32_t>&       getEntities() noexcept;
    std::vector<ComponentTable>& getEntityTables() noexcept;
    ComponentMap&                getComponentMap() noexcept;

private:
    std::vector<uint32_t>       m_entities;
    std::vector<ComponentTable> m_tables;
    ComponentMap                m_componentMap;
};

 #include "EntityManager.inl"

#endif // !ENTITY_MANAGER_HPP
