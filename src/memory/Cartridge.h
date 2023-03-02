#pragma once

#include <cstdint>
#include <string>

namespace Cartridge
{

void Load(std::string name);
void Unload();

uint8_t read8(uint16_t addr);
uint8_t read8_ppu(uint16_t addr);

}