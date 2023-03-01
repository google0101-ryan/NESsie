#pragma once

#include <cstdint>
#include <string>

namespace Cartridge
{

void Load(std::string name);

uint8_t read8(uint16_t addr);

}