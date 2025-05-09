#ifndef ECS_HPP_INCLUDED
#define ECS_HPP_INCLUDED

#include <array>
#include <vector>
#include <span>
#include <memory>
#include <algorithm>

#include "ComponentContainer.hpp"

template<size_t N = 32> // Maximum number of components
class Ecs final
{
    static constexpr uint32_t Undefined = UINT32_MAX;

    using ContainerPtr   = std::unique_ptr<BaseComponentContainer>;
    using ComponentTable = std::array<uint32_t, N>;
    using EntityData     = std::pair<uint32_t, ComponentTable>;

public:
    Ecs() noexcept;
    ~Ecs() noexcept;

    uint32_t createEntity() noexcept;

    void destroyEntity(uint32_t entity) noexcept;

    bool exists(uint32_t entity) const noexcept;

    template<class T>
    T* addComponent(uint32_t entity) noexcept;

    template<class T>
    T* getComponent(uint32_t entity) noexcept;

    template<class T>
    void removeComponent(uint32_t entity) noexcept;

    template<class T>
    bool hasComponent(uint32_t entity) const noexcept;

    template<class T>
    std::span<std::pair<uint32_t, T>> getContainer() noexcept;

private:
    template<class T>
    std::vector<std::pair<uint32_t, T>>* findContainer() noexcept;

    std::array<ContainerPtr, N> m_componentContainers;
    std::vector<EntityData>     m_entities;
};

#include "Ecs.inl"

#endif // ECS_HPP_INCLUDED
