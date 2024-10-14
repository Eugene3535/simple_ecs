#ifndef ECS_HPP_INCLUDED
#define ECS_HPP_INCLUDED

#include <vector>
#include <type_traits>
#include <algorithm>
#include <utility>

#include "Components.hpp"

class Ecs final
{
public:
    Ecs() noexcept;
    ~Ecs() noexcept;

    int  createEntity() noexcept;
    void destroyEntity(int entity) noexcept;

    template<class T>
    T* addComponent(int entity) noexcept;

    template<class T>
    T* getComponent(int entity) noexcept;

    template<class T>
    void removeComponent(int entity) noexcept;

    void moveSystem(float dt) noexcept;
    void animSystem(float dt) noexcept;

private:
    template<class T>
    void removeComponentFromContainer(int i, T& container) noexcept;

    std::vector<int>            m_entities;
    std::vector<ComponentTable> m_tables;

    std::vector<std::pair<int, Position>>  m_positions;
    std::vector<std::pair<int, Velocity>>  m_velocities;
    std::vector<std::pair<int, Animation>> m_animations;
};

#include "Ecs.inl"

#endif // ECS_HPP_INCLUDED
