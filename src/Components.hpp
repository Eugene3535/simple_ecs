#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

struct Position
{
    float x = 0.0f;
    float y = 0.0f;
};

struct Velocity
{
    float dx = 0.0f;
    float dy = 0.0f;
};

struct Animation
{
    int frame = 0;
    float dt = 0.0f;
};

#endif // COMPONENTS_HPP
