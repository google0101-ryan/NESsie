#pragma once

#include <src/memory/mappers/mapper.h>

#include <cstdlib>

class NROM : public Mapper
{
private:
    uint8_t* prg_rom;
    uint8_t* chr_rom;
public:
    NROM(uint8_t* data, size_t prg_rom_size, size_t chr_rom_size);

    uint8_t read8(uint16_t addr) override;
    uint8_t read8_ppu(uint16_t addr) override;
};