#ifndef COMPONENT_CONTAINER_HPP
#define COMPONENT_CONTAINER_HPP

#include <cstdint>
#include <vector>
#include <utility>

class BaseComponentContainer
{
public:
    virtual ~BaseComponentContainer() noexcept;
    virtual uint32_t erase(uint32_t idx) noexcept;

    static uint32_t getComponentCount() noexcept;

protected:
    template<class T>
    static uint32_t generateComponentType() noexcept
    {
        uint32_t type = s_componentCount;
        ++s_componentCount;

        return type;
    }

private:
    static uint32_t s_componentCount;
};

template<class T>
class ComponentContainer : public BaseComponentContainer
{
    using Component = std::pair<uint32_t, T>;
public:
    uint32_t erase(uint32_t idx) noexcept override
    {
        if(size_t index = static_cast<size_t>(idx); index < components.size())
        {
            auto& backElement = components.back();
            uint32_t owner = backElement.first;
            std::swap(components[index], backElement);
            components.pop_back();

            return owner;
        }

        return UINT32_MAX;
    }

    static const uint32_t Type;

    std::vector<Component> components;
};

template<class T>
const uint32_t ComponentContainer<T>::Type = BaseComponentContainer::generateComponentType<T>();

#endif // COMPONENT_CONTAINER_HPP
