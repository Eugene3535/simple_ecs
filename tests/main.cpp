#include <cstdio>

#include "Ecs.hpp"

int main()
{
    Ecs ecs;

    uint32_t e1 = ecs.createEntity();
    uint32_t e2 = ecs.createEntity();
    uint32_t e3 = ecs.createEntity();
    printf("Entities has been created: %u, %u, %u;\n", e1, e2, e3);

    if(auto c = ecs.addComponent<int>(e1); c)
    {
        *c = 4;
        printf("Component found with value: %d\n", *c);
    }

    if(auto c = ecs.addComponent<int>(e2); c)
    {
        *c = 8;
        printf("Component found with value: %d\n", *c);
    }

    if(auto c = ecs.getComponent<int>(e2); c)
    {
        *c = 12;
        printf("Component found with value: %d\n", *c);
    }

    ecs.removeComponent<int>(e1);

    if(auto c = ecs.getComponent<int>(e1); c)
    {
        *c = 16;
        printf("Component found with value: %d\n", *c);
    }
    else
    {
        printf("Component not found\n");
    }

    ecs.destroyEntity(e2);

    if(!ecs.exists(e2))
    {
        printf("The entity %u was successfully destroyed\n", e2);
    }

    return 0;
}
