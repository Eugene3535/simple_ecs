#ifndef COMPONENT_TABLE_HPP
#define COMPONENT_TABLE_HPP

#include <cstdint>
#include <type_traits>

#define UNDEFINED_COMPONENT UINT32_MAX

struct Position;
struct Velocity;
struct Animation;

struct ComponentTable
{
    uint32_t position  = UNDEFINED_COMPONENT;
    uint32_t velocity  = UNDEFINED_COMPONENT;
    uint32_t animation = UNDEFINED_COMPONENT;

    template<class T>
    uint32_t& getComponentIndex() noexcept
    {
        if constexpr(std::is_same_v<T, Position>)
        {
            return position;
        }
        else if constexpr(std::is_same_v<T, Velocity>)
        {
            return velocity;
        }
        else if constexpr(std::is_same_v<T, Animation>)
        {
            return animation;
        }
    }
};

#endif // !COMPONENT_TABLE_HPP