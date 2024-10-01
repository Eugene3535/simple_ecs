#ifndef COMPONENT_CONTAINER_HPP
#define COMPONENT_CONTAINER_HPP

#include <cstdint>
#include <utility>
#include <vector>

class BaseComponentContainer
{
public:
    virtual ~BaseComponentContainer();

    static uint32_t getComponentCount() noexcept;

protected:
    template<class T> 
    static uint32_t generateComponentType() noexcept
    {
        uint32_t tmp = s_componentCount;
        ++s_componentCount;

        return tmp;
    }

private:
    static uint32_t s_componentCount;
};

template<class T>
class ComponentContainer : public BaseComponentContainer
{
public:
    static const uint32_t Type;

    std::vector<std::pair<uint32_t, T>> components;
};

template<class T>
const uint32_t ComponentContainer<T>::Type = BaseComponentContainer::generateComponentType<T>();

#endif // !COMPONENT_CONTAINER_HPP