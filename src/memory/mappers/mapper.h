#pragma once

#include <cstdint>

class Mapper
{
public:
    virtual uint8_t read8(uint16_t address) = 0;
};