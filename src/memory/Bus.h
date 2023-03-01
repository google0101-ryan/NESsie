#pragma once

#include <cstdint>
#include <string>

namespace Bus
{

void LoadCartridge(std::string cart_name);

uint8_t read8(uint16_t addr);
uint16_t read16(uint16_t addr);

void write8(uint16_t addr, uint8_t data);

}