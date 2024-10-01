#ifndef COMPONENT_MAP_HPP
#define COMPONENT_MAP_HPP

#include <memory>
#include <unordered_map>

#include "ComponentContainer.hpp"

class ComponentMap
{
public:
    ComponentMap() noexcept;
    ComponentMap(ComponentMap&) = delete;
    ComponentMap(ComponentMap&&) = delete;
    ComponentMap& operator = (ComponentMap&) = delete;
    ComponentMap& operator = (ComponentMap&&) = delete;
    ~ComponentMap();

    template<class T> 
    std::vector<std::pair<uint32_t, T>>* pushContainer() noexcept
    {
        if(auto container = getContainer<T>(); container != nullptr)
        {
            return container;
        }

        auto [iterator, result] = m_components.try_emplace(ComponentContainer<T>::Type);

        if(result)
        {
            iterator->second = std::make_unique<ComponentContainer<T>>();
            auto container = static_cast<ComponentContainer<T>*>(iterator->second.get());

            return &container->components;
        }

        return nullptr;
    }
    
    template<class T> 
    std::vector<std::pair<uint32_t, T>>* getContainer() noexcept
    {
        if(auto iterator = m_components.find(ComponentContainer<T>::Type); iterator != m_components.end())
        {
            auto container = static_cast<ComponentContainer<T>*>(iterator->second.get());

            return &container->components;
        }

        return nullptr;
    }

private:
    std::unordered_map<uint32_t, std::unique_ptr<BaseComponentContainer>> m_components;
};

#endif // !COMPONENT_MAP_HPP