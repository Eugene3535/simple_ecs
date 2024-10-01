template<class T>
T* EntityManager::addComponent(uint32_t entity) noexcept
{
    ComponentTable& table = m_tables[static_cast<size_t>(entity)];
    uint32_t& index = table.getComponentIndex<T>();

    if(index == UNDEFINED_COMPONENT)
    {
        auto container = m_componentMap.getContainer<T>();
        index = container->size();
        auto& pair = container->emplace_back();
        pair.first = entity;

        return &pair.second;
    }

    return nullptr;
}

template<class T>
T* EntityManager::getComponent(uint32_t entity) noexcept
{
    ComponentTable& table = m_tables[static_cast<size_t>(entity)];
    uint32_t& index = table.getComponentIndex<T>();

    if(index != UNDEFINED_COMPONENT)
    {
        auto data = m_componentMap.getContainer<T>()->data();

        return &((data + index)->second);
    }

    return nullptr;
}

template<class T>
void EntityManager::removeComponent(uint32_t entity) noexcept
{
    ComponentTable& table = m_tables[static_cast<size_t>(entity)];
    uint32_t& index = table.getComponentIndex<T>();

    if(index != UNDEFINED_COMPONENT)
    {
        std::vector<std::pair<uint32_t, T>>* container = m_componentMap.getContainer<T>();

        std::pair<uint32_t, T>& current_element = (*container)[static_cast<size_t>(index)];
        std::pair<uint32_t, T>& last_element = container->back();

        ComponentTable& last_table = m_tables[last_element.first];
        uint32_t& last_element_index = last_table.getComponentIndex<T>();

        last_element_index = index;

        std::swap(current_element, last_element);
        container->pop_back();

        index = UNDEFINED_COMPONENT;
    }
}