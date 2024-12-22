template<size_t N>
Ecs<N>::Ecs() noexcept = default;

template<size_t N>
Ecs<N>::~Ecs() noexcept = default;

template<class T, size_t N>
constexpr auto make_array(T value) -> std::array<T, N>
{
    std::array<T, N> a{};
    for (auto& x : a)
        x = value;
    return a;
}

template<size_t N>
uint32_t Ecs<N>::createEntity() noexcept
{
    uint32_t i = static_cast<uint32_t>(m_entities.size());
    m_entities.emplace_back(i, make_array<uint32_t, N>(UINT_MAX));

    return i;
}

template<size_t N>
void Ecs<N>::destroyEntity(uint32_t entity) noexcept
{
    auto quick_remove = [](auto& container, size_t index) -> void
    {
        if(index < container.size())
        {
            std::swap(container[index], container.back());
            container.pop_back();
        }
    };

    auto componentCount = static_cast<size_t>(BaseComponentContainer::getComponentCount());
    auto& table         = m_entities[static_cast<size_t>(entity)].second;

    for(size_t componentId = 0; componentId < componentCount; ++componentId)
    {
        if(table[componentId] != UINT32_MAX)
        {
            auto container = m_componentContainers[componentId].get();
            uint32_t idx   = table[componentId];
            uint32_t owner = container->erase(idx);

            m_entities[static_cast<size_t>(owner)].second[componentId] = idx;
        }
    }

    quick_remove(m_entities, static_cast<size_t>(entity));
}

template<size_t N>
bool Ecs<N>::exists(uint32_t entity) const noexcept
{
    auto result = std::find_if(m_entities.begin(), m_entities.end(), [entity] (const std::pair<uint32_t, std::array<uint32_t, N>>& element)
    {
        return element.first == entity;
    });

    return result != m_entities.end();
}

template<size_t N>
template<class T>
T* Ecs<N>::addComponent(uint32_t entity) noexcept
{
    auto& table       = m_entities[static_cast<size_t>(entity)].second;
    auto  componentId = static_cast<size_t>(ComponentContainer<T>::Type);
    auto  container   = getContainer<T>();

    if(table[componentId] == UINT32_MAX)
    {
        uint32_t idx = container->size();
        auto& newComponent = container->emplace_back();
        newComponent.first = entity;
        table[componentId] = idx;

        return &newComponent.second;
    }

    uint32_t idx = table[componentId];

    return &(container->data() + idx)->second;
}

template<size_t N>
template<class T>
T* Ecs<N>::getComponent(uint32_t entity) noexcept
{
    auto& table      = m_entities[static_cast<size_t>(entity)].second;
    auto componentId = static_cast<size_t>(ComponentContainer<T>::Type);

    if(table[componentId] != UINT32_MAX)
    {
        auto container = getContainer<T>();
        auto& table    = m_entities[static_cast<size_t>(entity)].second;
        size_t idx     = static_cast<size_t>(table[componentId]);

        return &(container->data() + idx)->second;
    }

    return nullptr;
}

template<size_t N>
template<class T>
void Ecs<N>::removeComponent(uint32_t entity) noexcept
{
    auto& table      = m_entities[static_cast<size_t>(entity)].second;
    auto componentId = static_cast<size_t>(ComponentContainer<T>::Type);

    if(table[componentId] != UINT32_MAX)
    {
        auto& componentTable = m_entities[static_cast<size_t>(entity)].second;
        size_t index         = static_cast<size_t>(componentTable[componentId]);
        auto container       = static_cast<ComponentContainer<T>*>(m_componentContainers[componentId].get());
        size_t owner         = static_cast<size_t>(container->erase(index));

        componentTable[componentId] = UINT32_MAX;
        m_entities[owner].second[componentId] = index;
    }
}

template<size_t N>
template<class T>
bool Ecs<N>::hasComponent(uint32_t entity) const noexcept
{
    auto componentId = static_cast<size_t>(ComponentContainer<T>::Type);
    const auto& componentTable = m_entities[static_cast<size_t>(entity)].second;

    return componentTable[componentId] != UINT32_MAX;
}

template<size_t N>
template<class T>
std::vector<std::pair<uint32_t, T>>* Ecs<N>::getContainer() noexcept
{
    if(!m_componentContainers.empty())
    {
        if(auto index = static_cast<size_t>(ComponentContainer<T>::Type); index < m_componentContainers.size())
        {
            auto container = static_cast<ComponentContainer<T>*>(m_componentContainers[index].get());

            return &container->components;
        }
    }

    auto containerPtr = static_cast<ComponentContainer<T>*>(m_componentContainers.emplace_back(std::make_unique<ComponentContainer<T>>()).get());

    return &containerPtr->components;
}

