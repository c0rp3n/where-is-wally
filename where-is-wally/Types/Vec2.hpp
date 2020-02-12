#pragma once


struct Vec2
{
    int x;
    int y;

    Vec2()
    {
        this->x = 0;
        this->y = 0;
    }

    Vec2(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    ~Vec2()
    {
    }

    Vec2 operator+(const Vec2 &vec)
    {
        return Vec2(this->x + vec.x, this->y + vec.y);
    }

    Vec2 operator-(const Vec2 &vec)
    {
        return Vec2(this->x - vec.x, this->y - vec.y);
    }

    Vec2 operator*(const Vec2 &vec)
    {
        return Vec2(this->x * vec.x, this->y * vec.y);
    }

    Vec2 operator/(const Vec2 &vec)
    {
        return Vec2(this->x / vec.x, this->y / vec.y);
    }
};
