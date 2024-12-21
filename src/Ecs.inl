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
    m_bitMasks.emplace_back();

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

    auto& entityFlags   = m_bitMasks[static_cast<size_t>(entity)];
    auto componentCount = BaseComponentContainer::getComponentCount();
    auto& table         = m_entities[static_cast<size_t>(entity)].second;

    for(uint32_t componentId = 0; componentId < componentCount; ++componentId)
    {
        if(entityFlags.test(componentId))
        {
            auto v = m_componentContainers[static_cast<size_t>(componentId)].get();
            uint32_t idx = table[componentId];

            v->erase(idx);
        }
    }

    quick_remove(m_entities, static_cast<size_t>(entity));
    quick_remove(m_bitMasks, static_cast<size_t>(entity));
}

template<size_t N>
template<class T>
T* Ecs<N>::addComponent(uint32_t entity) noexcept
{
    auto& table       = m_entities[static_cast<size_t>(entity)].second;
    auto  componentId = static_cast<size_t>(ComponentContainer<T>::Type);
    auto& entityFlags = m_bitMasks[static_cast<size_t>(entity)];
    auto  container   = getContainer<T>();

    if(!entityFlags.test(componentId))
    {
        uint32_t idx = container->size();
        auto& newComponent = container->emplace_back();
        table[componentId] = idx;
        entityFlags.set(componentId);

        return &newComponent;
    }

    size_t idx = table[componentId];

    return container->data() + idx;
}

template<size_t N>
template<class T>
T* Ecs<N>::getComponent(uint32_t entity) noexcept
{
    auto componentId  = static_cast<size_t>(ComponentContainer<T>::Type);
    auto& entityFlags = m_bitMasks[static_cast<size_t>(entity)];

    if(entityFlags.test(componentId))
    {
        auto container = getContainer<T>();
        auto& table    = m_entities[static_cast<size_t>(entity)].second;
        size_t idx     = static_cast<size_t>(table[componentId]);

        return container->data() + idx;
    }

    return nullptr;
}

template<size_t N>
template<class T>
void Ecs<N>::removeComponent(uint32_t entity) noexcept
{
    auto componentId  = static_cast<size_t>(ComponentContainer<T>::Type);
    auto& entityFlags = m_bitMasks[static_cast<size_t>(entity)];

    if(entityFlags.test(componentId))
    {
        auto& table  = m_entities[static_cast<size_t>(entity)].second;
        size_t index = static_cast<size_t>(table[componentId]);

        if(auto index = static_cast<size_t>(ComponentContainer<T>::Type); index < m_componentContainers.size())
        {
            auto container = static_cast<ComponentContainer<T>*>(m_componentContainers[index].get());

            container->erase(index);
            entityFlags.reset(componentId);
        }
    }
}

template<size_t N>
template<class T>
bool Ecs<N>::hasComponent(uint32_t entity) noexcept
{
    auto componentId  = static_cast<size_t>(ComponentContainer<T>::Type);
    auto& entityFlags = m_bitMasks[static_cast<size_t>(entity)];

    return entityFlags.test(componentId);
}

template<size_t N>
template<class T>
std::vector<T>* Ecs<N>::getContainer() noexcept
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

