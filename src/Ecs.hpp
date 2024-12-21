#ifndef ECS_HPP_INCLUDED
#define ECS_HPP_INCLUDED

#include <array>
#include <vector>
#include <memory>
#include <algorithm>
#include <utility>
#include <bitset>

#include "ComponentContainer.hpp"

template<size_t N = 32>
class Ecs final
{
    using ContainerPtr   = std::unique_ptr<BaseComponentContainer>;
    using ComponentTable = std::array<uint32_t, N>;
    using Entity         = std::pair<uint32_t, ComponentTable>;

public:
    Ecs() noexcept;
    ~Ecs() noexcept;

    uint32_t createEntity() noexcept;
    void destroyEntity(uint32_t entity) noexcept;

    template<class T>
    T* addComponent(uint32_t entity) noexcept;

    template<class T>
    T* getComponent(uint32_t entity) noexcept;

    template<class T>
    void removeComponent(uint32_t entity) noexcept;

    template<class T>
    bool hasComponent(uint32_t entity) noexcept;

private:
    template<class T>
    std::vector<T>* getContainer() noexcept;

    std::vector<ContainerPtr>   m_componentContainers;
    std::vector<Entity>         m_entities;
    std::vector<std::bitset<N>> m_bitMasks;
};

#include "Ecs.inl"

#endif // ECS_HPP_INCLUDED
