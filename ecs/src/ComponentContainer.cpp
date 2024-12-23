#include "ComponentContainer.hpp"

uint32_t BaseComponentContainer::s_componentCount = 0;

BaseComponentContainer::~BaseComponentContainer() noexcept = default;

uint32_t BaseComponentContainer::erase(uint32_t idx) noexcept
{
    return UINT32_MAX;
}

uint32_t BaseComponentContainer::getComponentCount() noexcept
{
    return s_componentCount;
}
