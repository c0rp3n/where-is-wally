#pragma once

#include <array>
#include <cstdint>

struct Pixel
{
private:
    uint32_t data[3];

public:
    Pixel()
    {
    }

    Pixel(int value)
    {
        this->data[0] = value;
        this->data[1] = value;
        this->data[2] = value;
    }

    Pixel(int red, int green, int blue)
    {
        this->data[0] = red;
        this->data[1] = green;
        this->data[2] = blue;
    }

    uint32_t operator[](int index)
    {
        return this->data[index];
    }

    uint32_t Average()
    {
        return (this->data[0] + this->data[1] + this->data[2]) / 3;
    }
};