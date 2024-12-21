#include "ComponentContainer.hpp"

uint32_t BaseComponentContainer::s_componentCount = 0;

BaseComponentContainer::~BaseComponentContainer() noexcept = default;

void BaseComponentContainer::erase(uint32_t idx) noexcept
{

}

uint32_t BaseComponentContainer::getComponentCount() noexcept
{
    return s_componentCount;
}
