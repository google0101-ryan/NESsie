#pragma once

#include <cstdint>

class Mapper
{
public:
    virtual uint8_t read8(uint16_t address) = 0;
    virtual uint8_t read8_ppu(uint16_t address) = 0;
};