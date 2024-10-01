#include "ComponentContainer.hpp"

uint32_t BaseComponentContainer::s_componentCount = 0;

BaseComponentContainer::~BaseComponentContainer() = default;

uint32_t BaseComponentContainer::getComponentCount() noexcept
{
    return s_componentCount;
}