template<class T>
T* Ecs::addComponent(int entity) noexcept
{
    auto& table = m_tables[entity];

    if constexpr(std::is_same_v<T, Position>)
    {
        if (table.position == COMPONENT_UNDEFINED)
        {
            table.position = m_positions.size();
            auto& pair = m_positions.emplace_back();
            pair.first = entity;

            return &pair.second;
        }
    }
    else if constexpr(std::is_same_v<T, Velocity>)
    {
        if (table.velocity == COMPONENT_UNDEFINED)
        {
            table.velocity = m_velocities.size();
            auto& pair = m_velocities.emplace_back();
            pair.first = entity;

            return &pair.second;
        }
    }
    else if constexpr(std::is_same_v<T, Animation>)
    {
        if (table.animation == COMPONENT_UNDEFINED)
        {
            table.animation = m_animations.size();
            auto& pair = m_animations.emplace_back();
            pair.first = entity;

            return &pair.second;
        }
    }

    return nullptr;
}

template<class T>
T* Ecs::getComponent(int entity) noexcept
{
    auto& table = m_tables[entity];

    if constexpr (std::is_same_v<T, Position>)
    {
        if (int pos = table.position; pos != COMPONENT_UNDEFINED)
            return &m_positions[pos].second;
    }
    else if constexpr (std::is_same_v<T, Velocity>)
    {
        if (int vel = table.velocity; vel != COMPONENT_UNDEFINED)
            return &m_velocities[vel].second;
    }
    else if constexpr (std::is_same_v<T, Animation>)
    {
        if (int anim = table.animation; anim != COMPONENT_UNDEFINED)
            return &m_animations[anim].second;
    }

    return nullptr;
}

template<class T>
void Ecs::removeComponent(int entity) noexcept
{
    auto& table = m_tables[entity];

    if constexpr (std::is_same_v<T, Position>)
    {
        if (int i = table.position; i != COMPONENT_UNDEFINED)
        {
            removeComponentFromContainer(i, m_positions);
            table.position = COMPONENT_UNDEFINED;
        }
    }
    else if constexpr (std::is_same_v<T, Velocity>)
    {
        if (int i = table.velocity; i != COMPONENT_UNDEFINED)
        {
            removeComponentFromContainer(i, m_velocities);
            table.velocity = COMPONENT_UNDEFINED;
        }
    }
    else if constexpr (std::is_same_v<T, Animation>)
    {
        if (int i = table.animation; i != COMPONENT_UNDEFINED)
        {
            removeComponentFromContainer(i, m_animations);
            table.animation = COMPONENT_UNDEFINED;
        }
    }
}

template<class T>
void Ecs::removeComponentFromContainer(int i, T& container) noexcept
{
    auto& pos = container[i];
    auto& back = container.back();

    if constexpr (std::is_same_v<T, Position>)
    {
        m_tables[back.first].position = i;
    }
    else if constexpr (std::is_same_v<T, Velocity>)
    {
        m_tables[back.first].velocity = i;
    }
    else if constexpr (std::is_same_v<T, Animation>)
    {
        m_tables[back.first].animation = i;
    }

    std::swap(pos, back);
    container.pop_back();
}
