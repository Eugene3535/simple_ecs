#include <cstdio>

#include "Ecs.hpp"

int main()
{
    Ecs ecs;

    auto entity = ecs.createEntity();
    entity = ecs.createEntity();
    entity = ecs.createEntity();
    printf("Entity: %u\n", entity);

    if(auto c = ecs.addComponent<int>(entity); c)
    {
        *c = 8;
        printf("sucsess with code: %d\n", *c);
    }

    if(auto c = ecs.addComponent<float>(entity); c)
    {
        *c = 4.5f;
        printf("sucsess with code: %f\n", *c);
    }

    if(auto c = ecs.getComponent<float>(entity); c)
    {
        *c = 0.5f;
        printf("sucsess with code: %f\n", *c);
    }

    ecs.removeComponent<int>(entity);

    if(auto c = ecs.getComponent<int>(entity); c)
    {
        *c = 16;
        printf("sucsess with code: %d\n", *c);
    }
    else
    {
        printf("failed\n");
    }

    ecs.destroyEntity(entity);

    return 0;
}
