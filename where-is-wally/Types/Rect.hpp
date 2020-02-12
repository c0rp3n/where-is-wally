#pragma once

#include <utility>

#include "Vec2.hpp"

struct Rect
{
public:
    Vec2 position;
    Vec2 size;

    Rect()
    {
    }

    Rect(int x, int y, int sizeX, int sizeY)
    {
        this->position = Vec2::Vec2(x, y);
        this->size = Vec2::Vec2(sizeX, sizeY);
    }

    ~Rect()
    {
    }

    Vec2 TopLeft()
    {
        return this->position + Vec2::Vec2(0, this->size.y);
    }

    Vec2 TopRight()
    {
        return this->position + Vec2::Vec2(this->size.x, this->size.y);
    }

    Vec2 BottomLeft()
    {
        return this->position;
    }

    Vec2 BottomRight()
    {
        return this->position + Vec2(this->size.x, 0);
    }
};
